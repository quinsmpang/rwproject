//== Copyright 2005-2008,Xuling(许凌),Linkye Corporation,All rights reserved.==
//
// 模块   :NetClient.cpp
// 作者   :Shunet
// 日期   :2005年10月3日
// 说明   :客户端的网络层, 网络层提供发送包的函数, 并且需要回调应用层提供的接收
//         处理的过程, 另外回调断开连接时的断开处理过程
//
//=============================================================================
#ifndef WIN32
//#include <strings.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#endif

#include "NetClient.h"
#include "utils.h"
#include "thread.h"

//#define _USEGPK
#ifdef _USEGPK
// 引用GPK客户端头文件
#include <GPKitClt.h>
// 包含GPK客户端静态库
#pragma comment(lib, "GPKitClt.lib")
// 使用GPK命名空间
using namespace SGPK;

IGPKCltDynCode *g_pCltDynCode[1000];
PUCHAR g_DynCode[1000];
#endif

struct sockaddr_in clientaddr;

LPCALLBACKPROCESSPACK g_pCallbackProcessPack = NULL;
LPCALLBACKDISCONNECTED g_pCallbackDisConnected = NULL;

int send_client_ver( int sock_index );
int recv_client_ver( int sock_index );

void SetCallbackProcessPack( LPCALLBACKPROCESSPACK pCallbackProcessPack )
{
	g_pCallbackProcessPack = pCallbackProcessPack;
}
void SetCallbackDisConnected( LPCALLBACKDISCONNECTED pCallbackDisConnected )
{
	g_pCallbackDisConnected = pCallbackDisConnected;
}

// 需要一个队列的结构,用来接收零碎的包,主逻辑会访问这个队列,接收到客户缓冲,接收函数也会写这个队列

// 队列缓冲区
SBuf *read_queue = NULL;
int g_nReadQueueHead;
int g_nReadQueueTail;

SBuf *write_queue = NULL;
int g_nWriteQueueHead;
int g_nWriteQueueTail;

Mthr	*g_pthr_recv=NULL;
Mthr	*g_pthr_gamemain=NULL;
Mthr	*g_pthr_send=NULL;
ThrMux	*g_read_mmux;
ThrMux	*g_write_mmux;

char g_bExit = 0;			// 是否需要退出系统

int g_sockset[1000];
int g_sendkey[1000];
int g_recvkey[1000];
char g_xor[1000];

int g_socknum = 0;

short netcode_getrecvkey( int sock_index )
{
	return (((g_recvkey[sock_index] = g_recvkey[sock_index] * 214013L + 2531011L) >> 16) & 0xffff );
}
short netcode_getsendkey( int sock_index )
{
	return (((g_sendkey[sock_index] = g_sendkey[sock_index] * 214013L + 2531011L) >> 16) & 0xffff );
}

short netcode_lookrecvkey( int sock_index )
{
	return (short)(( ( g_recvkey[sock_index] * 214013L + 2531011L) >> 16) & 0xffff );
}

short netcode_looksendkey( int sock_index )
{
	return (short)(( ( g_sendkey[sock_index] * 214013L + 2531011L) >> 16) & 0xffff );
}

void netcode_encode( int sock_index, char *dst, const char *src, int size )
{
	int tmpi;
	for( tmpi = 0; tmpi < size; tmpi += 2 )
	{
		if( size - 1 == tmpi )
			*((unsigned char *)(dst+tmpi)) = *((unsigned char *)(src+tmpi)) ^ (unsigned char)netcode_getsendkey( sock_index );
		else
			*((unsigned short *)(dst+tmpi)) = *((unsigned short *)(src+tmpi)) ^ (unsigned short)netcode_getsendkey( sock_index );
	}
}

void netcode_decode( int sock_index, char *dst, const char *src, int size )
{
	int tmpi;
	for( tmpi = 0; tmpi < size; tmpi += 2 )
	{
		if( size - 1 == tmpi )
			*((unsigned char *)(dst+tmpi)) = *((unsigned char *)(src+tmpi)) ^ (unsigned char)netcode_getrecvkey( sock_index );
		else
			*((unsigned short *)(dst+tmpi)) = *((unsigned short *)(src+tmpi)) ^ (unsigned short)netcode_getrecvkey( sock_index );
	}
}

short netcode_decode_size( int sock_index, short datasize )
{
	return datasize ^ netcode_lookrecvkey( sock_index );
}

#include "Command.h"
extern SLK_Player g_Player[1000];
int restart( int sock_index )
{
	int exec_code;
	if( g_sockset[sock_index] != 0 && g_Player[sock_index].isconnected!=0 )
		return -1;

	while( 1 )
	{
		exec_code = g_Player[sock_index].exec_code+1;
		g_Player[sock_index].isconnected = 0;
		memset( &g_Player[sock_index], 0, sizeof(SLK_Player) );
		g_Player[sock_index].exec_code = exec_code;
		g_Player[sock_index].stat = STAT_NONE;

//		Sleep( 1000+rand()%2000 );

		g_sockset[sock_index] = (int)socket(AF_INET, SOCK_STREAM, 0);

		if(connect(g_sockset[sock_index],(struct sockaddr *)&clientaddr,sizeof(clientaddr)) < 0)
		{
			continue;
		}
		if( send_client_ver( sock_index ) < 0 )
			return -2;
		if( recv_client_ver( sock_index ) < 0 )
			return -2;

//		printf( "restart:%d\n", sock_index);
		g_Player[sock_index].isconnected = 1;
		return 0;
	}
	return 0;
}

int net_closeclient( int sock_index, char r = 0 )
{
	SOCKET s = g_sockset[sock_index];
	shutdown(s,2);
	closesocket(s);
	if( g_pCallbackDisConnected )
		g_pCallbackDisConnected( (int)s );
	g_sockset[sock_index] = 0;
	if( r )
		return restart( sock_index );
	return 0;
}
int max_read_list_count = 0;
int max_write_list_count = 0;
// 这个函数应该是将接收数据,然后放置到队列
// 该函数可做的优化(废除tmpbuf)
int readtoqueue( char *pBuf, int readsize, int sock_index, int exec_code )
{
	int read_list_count;
	int write_list_count;
	int queue_tail;
	if ( readsize == 0 )
	{
		return -1;
    }
	// 锁住读队列缓冲
	mmux_lock( g_read_mmux );
	if( g_nReadQueueHead <= g_nReadQueueTail )
		read_list_count = g_nReadQueueTail-g_nReadQueueHead;
	else
		read_list_count = MAX_QUEUE_NUM-g_nReadQueueHead+g_nReadQueueTail;
	if( read_list_count > max_read_list_count )
		max_read_list_count = read_list_count;

	if( g_nWriteQueueHead <= g_nWriteQueueTail )
		write_list_count = g_nWriteQueueTail-g_nWriteQueueHead;
	else
		write_list_count = MAX_WRITE_QUEUE_NUM-g_nWriteQueueHead+g_nWriteQueueTail;
	if( write_list_count > max_write_list_count )
		max_write_list_count = write_list_count;
	
	queue_tail = g_nReadQueueTail + 1;
	if( queue_tail >= MAX_QUEUE_NUM )
	{
		queue_tail = 0;
	}
	if( g_nReadQueueHead == queue_tail )
	{
		// 队列已经满了, 断开这个客户
		mmux_unlock( g_read_mmux );
		net_closeclient( sock_index );
		mcond_broadcast( g_pthr_gamemain );
		return -1;
	}

#ifndef NONECODE
	// 这里解密客户端的数据,将明文复制到 read_queue[g_nReadQueueTail].buf
	netcode_decode( sock_index, read_queue[g_nReadQueueTail].buf, pBuf, readsize );
#else
	memcpy( read_queue[g_nReadQueueTail].buf, pBuf, readsize );
#endif
#ifdef _USEGPK
	g_pCltDynCode[sock_index]->Decode((unsigned char*)(read_queue[g_nReadQueueTail].buf+sizeof(short)), readsize-sizeof(short) );
#endif

	read_queue[g_nReadQueueTail].buf_size = readsize;
	read_queue[g_nReadQueueTail].index = sock_index;
	read_queue[g_nReadQueueTail].exec_code = exec_code;

	g_nReadQueueTail = queue_tail;

	mmux_unlock( g_read_mmux );

	// 通知接收线程开始工作
	mcond_broadcast( g_pthr_gamemain );
	return 0;
}

int check_packeg( char *pack_data, int packlen )
{
	short cmd;
	int allsize;
	int tmpsize;
	short datasize;
	char *ptr;
	if( pack_data == NULL || packlen < 0 || packlen >= 2048 )
		return -1;
	ptr = pack_data;
	// || CMDS_COMMAND | datasize[short] | ... ||
	allsize = 0;
	while(1)
	{
		tmpsize = 0;
		if( allsize >= packlen )
			break;
		cmd = *((short *)ptr); ptr+=sizeof(short);tmpsize+=sizeof(short);
		datasize = *((short *)ptr); ptr+=sizeof(short);tmpsize+=sizeof(short);
		allsize += tmpsize;
		if( datasize < 0 )
			return -1;
		tmpsize = datasize;
		ptr += tmpsize;
		allsize += tmpsize;
		break;
	}
	return 0;
}

// 辅助函数, 解析数据包, 放置到接收队列, 返回已经处理完的字节数
// 对于网络底层, 是简单的包结构: len[short]|data[len]
int recv_package( char *buf, int size, int sock_index )
{
	char *ptr;
	int package_size;
	int proc_size;
	int remain_size;
	if( size > MAX_BUF_SIZE )
		return -1;

	proc_size = 0;
	remain_size = size;
	ptr = buf;

	while(1)
	{
		if( remain_size < (int)sizeof(short) )
			return proc_size;
#ifndef NONECODE
		// 如果包长度也加密, 这里的包长度也需要解密
		package_size = netcode_decode_size( sock_index, *( (short *)ptr ) );	// package_size 包括自己，所以至少必须大于sizeof(short)
#else
		package_size = *( (short *)ptr );	// package_size 包括自己，所以至少必须大于sizeof(short)
#endif
		// 如果包长度也加密, 这里的包长度也需要解密
		package_size += sizeof(short);
		if( package_size < (int)sizeof(short) || package_size >= MAX_PACKAGE_SIZE )
		{
			return -1;
		}
		if( remain_size < package_size )
			return proc_size;
		readtoqueue( ptr, package_size, sock_index, g_Player[sock_index].exec_code );
		check_packeg(ptr, package_size);
		proc_size += package_size;
		remain_size -= package_size;
		ptr += package_size;
	}
	return -1;
}

int game_main()
{
	int readsize;
	int truesize;
	int sock_index;
	int exec_code;
	char tmpbuf[MAX_PACKAGE_SIZE];

	while( g_pCallbackProcessPack == NULL )
	{
		Sleep(100);
	}

	while( !g_bExit )
	{
		readsize = 0;
		// 锁住队列缓冲
		mmux_lock( g_read_mmux );
		while( g_nReadQueueTail == g_nReadQueueHead )
		{
			mcond_wait( g_pthr_gamemain, g_read_mmux );
			if( g_bExit )
			{
				mmux_unlock( g_read_mmux );
				return 0;
			}
		}

		// 将数据从接收队列中取出来, 然后就可以自己工作了
		memcpy( tmpbuf, read_queue[g_nReadQueueHead].buf, read_queue[g_nReadQueueHead].buf_size );
		sock_index = read_queue[g_nReadQueueHead].index;
		exec_code = read_queue[g_nReadQueueHead].exec_code;
		truesize = read_queue[g_nReadQueueHead].buf_size;
		g_nReadQueueHead++;
		if( g_nReadQueueHead >= MAX_QUEUE_NUM )
		{
			g_nReadQueueHead = 0;
		}
		mmux_unlock( g_read_mmux );
		if( g_Player[sock_index].exec_code == exec_code )
			g_pCallbackProcessPack( tmpbuf+sizeof(short), truesize-sizeof(short), sock_index );
	}
	return 0;
}

// 这里的发送,实际上是将数据放置到写缓冲队列中,并且置sock的epoll状态为写
int sendtoqueue( const char *buf, int bufsize, int sock_index, int exec_code )
{
	int queue_tail;

	if( buf == NULL || bufsize <= 0 )
		return -1;

	// 锁住队列缓冲
	mmux_lock( g_write_mmux );
	
	queue_tail = g_nWriteQueueTail + 1;
	if( queue_tail >= MAX_WRITE_QUEUE_NUM )
	{
		queue_tail = 0;
	}
	if( g_nWriteQueueHead == queue_tail )
	{
		// 队列已经满了, 断开这个客户
		mmux_unlock( g_write_mmux );
		net_closeclient( sock_index );
		write_netlog("write queue full");
		return -1;
	}

	memcpy( write_queue[g_nWriteQueueTail].buf, buf, bufsize );
	write_queue[g_nWriteQueueTail].buf_size = bufsize;
	write_queue[g_nWriteQueueTail].index = sock_index;
	write_queue[g_nWriteQueueTail].exec_code = exec_code;
#ifdef _USEGPK
	g_pCltDynCode[sock_index]->Encode((unsigned char*)(write_queue[g_nWriteQueueTail].buf+sizeof(short)), bufsize-sizeof(short));
#endif

	g_nWriteQueueTail = queue_tail;

	mmux_unlock( g_write_mmux );
	mcond_broadcast( g_pthr_send );
	return 0;
}
extern int g_send_count;
// 这里访问队列完成写的操作
int send_proc()
{
	int writesize;
	int writecount;
	int sock_index;
	int exec_code;
	int queue_tail;
	char tmpbuf[MAX_BUF_SIZE];
	while( !g_bExit )
	{
		writesize = 0;
		// 锁住队列缓冲
		mmux_lock( g_write_mmux );
		while( g_nWriteQueueTail == g_nWriteQueueHead )
		{
			mcond_wait( g_pthr_send, g_write_mmux );
			if( g_bExit )
			{
				mmux_unlock( g_write_mmux );
				return 0;
			}
		}
		// 从队列中取出一项
		memcpy( tmpbuf, write_queue[g_nWriteQueueHead].buf, write_queue[g_nWriteQueueHead].buf_size );
		writesize = write_queue[g_nWriteQueueHead].buf_size;
		sock_index = write_queue[g_nWriteQueueHead].index;
		exec_code = write_queue[g_nWriteQueueHead].exec_code;
		g_nWriteQueueHead++;
		if( g_nWriteQueueHead >= MAX_WRITE_QUEUE_NUM )
		{
			g_nWriteQueueHead = 0;
		}
		if( g_Player[sock_index].exec_code != exec_code )
		{
			mmux_unlock( g_write_mmux );
			continue;
		}
		writecount = send( g_sockset[sock_index], tmpbuf, writesize, 0 );
		if( writecount < 0 )
		{
			mmux_unlock( g_write_mmux );
			continue;
		}
		g_send_count++;
		if( writecount < writesize ) // 没有写完再插入到队列的尾部
		{
			queue_tail = g_nWriteQueueTail + 1;
			if( queue_tail >= MAX_WRITE_QUEUE_NUM )
			{
				queue_tail = 0;
			}
			if( g_nWriteQueueHead == queue_tail )
			{
				// 队列已经满了, 断开这个客户
				mmux_unlock( g_write_mmux );
				net_closeclient( sock_index );
				continue;
			}
		
			memcpy( write_queue[g_nWriteQueueTail].buf, tmpbuf + writecount, writesize - writecount );
			write_queue[g_nWriteQueueTail].buf_size = writesize - writecount;
		
			g_nWriteQueueTail = queue_tail;
		}
		mmux_unlock( g_write_mmux );
		
	}
	return 0;
}

int client_init()
{
#ifdef WIN32
	WORD tSocketVerNo;
	WSADATA tVerData;

	tSocketVerNo = MAKEWORD(2, 0);
	WSAStartup(tSocketVerNo, &tVerData);
#endif

	g_nReadQueueHead = 0;
	g_nReadQueueTail = 0;
	g_nWriteQueueHead = 0;
	g_nWriteQueueTail = 0;
	read_queue = (SBuf *)malloc( sizeof(SBuf)*MAX_QUEUE_NUM );
	write_queue = (SBuf *)malloc( sizeof(SBuf)*MAX_WRITE_QUEUE_NUM );

	memset( g_sockset, 0, sizeof(int)*1000 );
#ifdef _USEGPK
	memset( g_pCltDynCode, 0, sizeof(IGPKCltDynCode *)*1000 );
	memset( g_DynCode, 0, sizeof(PUCHAR)*1000 );
#endif
	g_read_mmux = mmux_open();
	g_write_mmux = mmux_open();
	g_pthr_gamemain = mthr_open();
	g_pthr_recv = mthr_open();
	g_pthr_send = mthr_open();
	mcond_init( g_pthr_gamemain );
	mcond_init( g_pthr_send );

	return 0;
}

int client_destroy()
{
	g_bExit = 1;
	for( int tmpi = 0; tmpi < g_socknum; tmpi++ )
		net_closeclient( tmpi );
	mcond_broadcast( g_pthr_gamemain );
	mcond_broadcast( g_pthr_send );

	mthr_join( g_pthr_recv );
	mthr_close( g_pthr_recv );

	mthr_join( g_pthr_send );
	mthr_close( g_pthr_send );

	mthr_join( g_pthr_gamemain );
	mthr_close( g_pthr_gamemain );

	mmux_close( g_read_mmux );
	mmux_close( g_write_mmux );

	mcond_destroy( g_pthr_gamemain );
	mcond_destroy( g_pthr_send );
	return 0;
}

int recv_proc( int index )
{
	int readsize;
	char tmpbuf[MAX_BUF_SIZE];
	int sock_index = index;
	int nProcBytes = 0;
	int procsize = 0;
	int leftsize = 0;

	while( !g_bExit )
	{
		if( g_socknum <= 0 || g_sockset[sock_index] <= 0 )
		{
			restart( sock_index );
			Sleep(1000);
			continue;
		}
		// 从sock中读数据到缓冲区
		if ( ( readsize = recv( g_sockset[sock_index], tmpbuf+nProcBytes, MAX_BUF_SIZE-1-nProcBytes, 0 ) ) < 0)
		{
			// 连接断开了(连接重置了)
			net_closeclient( sock_index, 1 );
			continue;
		}
		else if ( readsize == 0 )
		{
			// 客户端断开
			net_closeclient( sock_index, 1 );
			continue;
		}

		nProcBytes += readsize;
		procsize = recv_package( tmpbuf, nProcBytes, sock_index );
		if( procsize < 0 )
		{
			procsize = readsize;
			printf("error\n");
			net_closeclient( sock_index, 1 );
			continue;
		}
		if( procsize > 0 )
		{
			nProcBytes -= procsize;
			memmove( tmpbuf, tmpbuf+procsize, nProcBytes );
		}

	}
	return 0;
}

int net_setver( unsigned int sendkey, unsigned int recvkey, int sock_index )
{
	g_sendkey[sock_index] = sendkey;
	g_recvkey[sock_index] = recvkey;
	g_xor[sock_index] = (char)recvkey| 0x80;
	return 0;
}

// 接收服务器发送的版本信息
int recv_client_ver( int sock_index )
{
	int totlesize;
	int readsize;
	char tmpbuf[16];

	totlesize = 0;
	while( totlesize < 16 )
	{
		// 从sock中读数据到缓冲区
		if ( ( readsize = recv( g_sockset[sock_index], tmpbuf+totlesize, 16-totlesize, 0 ) ) < 0)
		{
			// 连接断开了(连接重置了)
			net_closeclient( sock_index );
			return -1;
		}
		else if ( readsize == 0 )
		{
			// 客户端断开
			net_closeclient( sock_index );
			return -1;
		}
		totlesize += readsize;
	}
	net_setver( *((unsigned int *)tmpbuf+1), *((unsigned int *)tmpbuf+2), sock_index );
	return 0;
}

// 连接上先就发送自己的版本信息
int send_client_ver( int sock_index )
{
	int totlesize;
	int writesize;
	char tmpbuf[16];
	char *ptr;
	int bufsize;

	ptr = tmpbuf;
	bufsize = 0;
	*(unsigned int *)( ptr+bufsize ) = 0; bufsize += sizeof(unsigned int);
	*(unsigned int *)( ptr+bufsize ) = 0; bufsize += sizeof(unsigned int);
	*(unsigned int *)( ptr+bufsize ) = 0; bufsize += sizeof(unsigned int);
	*(unsigned int *)(ptr + bufsize) = ((3 & 0xffff) | (0 << 16)); bufsize += sizeof(unsigned int);

	totlesize = 0;
	while( totlesize < 16 )
	{
		// 从sock中读数据到缓冲区
		if ( ( writesize = send( g_sockset[sock_index], tmpbuf+totlesize, 16-totlesize, 0 ) ) < 0)
		{
			// 连接断开了(连接重置了)
			net_closeclient( sock_index );
			return -1;
		}
		else if ( writesize == 0 )
		{
			// 客户端断开
			net_closeclient( sock_index );
			return -1;
		}
		totlesize += writesize;
	}
	return 0;
}

int client_start( unsigned int nIP, unsigned short port, int socknum )
{

	// 创建发送线程
	mthr_create(g_pthr_send,(void *)send_proc,0);
	// 创建主逻辑线程
	mthr_create(g_pthr_gamemain,(void *)game_main,0);

//	for( int tmpi = 0; tmpi < socknum; tmpi++ )
//	{
//#ifdef _USEGPK
//		g_pCltDynCode[tmpi] = GPKStart(NULL, "73");
//		if ( !g_pCltDynCode[tmpi] )
//		{
//			printf( "GPKStart: (%d) ERROR     \r", tmpi+1);
//			return -1;
//		}
//		printf( "GPKStart: (%d)      \r", tmpi+1);
//#endif
//	}

	memset(&clientaddr, 0, sizeof(clientaddr));
	clientaddr.sin_family = AF_INET;
	clientaddr.sin_addr.s_addr = nIP;
	clientaddr.sin_port=htons(port);

	for( int tmpi = 0; tmpi < socknum; tmpi++ )
	{

		g_sockset[tmpi] = (int)socket(AF_INET, SOCK_STREAM, 0);
		g_Player[tmpi].exec_code++;

		if(connect(g_sockset[tmpi],(struct sockaddr *)&clientaddr,sizeof(clientaddr)) < 0)
			return -1;

		if( send_client_ver( tmpi ) < 0 )
			return -2;
		if( recv_client_ver( tmpi ) < 0 )
			return -2;
		if( mthr_create(g_pthr_recv,(void *)recv_proc, (void *)tmpi ) < 0 )
			printf("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
		printf( "Create succeed: (%d)      \r", tmpi+1);
	}

	g_socknum = socknum;
	return 0;
}

