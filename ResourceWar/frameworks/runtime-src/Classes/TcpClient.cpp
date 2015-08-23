#ifndef WIN32
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#endif
#include "TcpClient.h"

TcpClient::TcpClient()
{
	m_socket = -1;
	m_read_mmux = nullptr;
	m_pthr_recv = nullptr;
	m_write_mmux = nullptr;
	m_pthr_send = nullptr;
	m_pthr_readqueue = nullptr;
	m_recv_synch_mutex = nullptr;
	m_bStartReady = false;
	m_bRecvExit = false;
	m_bExit = false;
}

TcpClient::~TcpClient()
{
}

// 连接服务器
void TcpClient::AsynConnect( const char *host, unsigned short port )
{
#ifdef WIN32
	WORD tSocketVerNo;
	WSADATA tVerData;

	tSocketVerNo = MAKEWORD( 2, 0 );
	WSAStartup( tSocketVerNo, &tVerData );
#endif

	struct sockaddr_in clientaddr;
	memset( &clientaddr, 0, sizeof(clientaddr) );
	clientaddr.sin_family = AF_INET;
	clientaddr.sin_addr.s_addr = inet_addr( host );
	clientaddr.sin_port = htons( port );

	// 创建套接字
	m_socket = (int)socket( AF_INET, SOCK_STREAM, 0 );
	if ( m_socket < 0 )
	{
		if ( m_Delegate )
			m_Delegate->OnDisconnected();
		return;
	}

	// 连接服务器
	int value = 32 * 1024;
	setsockopt( m_socket, SOL_SOCKET, SO_RCVBUF, (char*)&value, sizeof(int) );
	if ( connect( m_socket, (struct sockaddr *)&clientaddr, sizeof(clientaddr) ) < 0 )
	{
		CloseSocket();
		if ( m_Delegate )
			m_Delegate->onConnectFail();
		return;
	}

	value = 1;
	setsockopt( m_socket, SOL_SOCKET, SO_RCVTIMEO, (char*)&value, sizeof(int) );
	m_bRecvExit = false;

	// 连接成功后，启动TCP客户端
	this->Start();
	if ( m_Delegate )
		m_Delegate->onConnect();
}

// 启动
void TcpClient::Start()
{
	m_nReadQueueHead = 0;
	m_nReadQueueTail = 0;
	m_nWriteQueueHead = 0;
	m_nWriteQueueTail = 0;

	m_read_mmux = mmux_open();
	m_write_mmux = mmux_open();
	m_recv_synch_mutex = mmux_open();

	m_pthr_readqueue = mthr_open();
	m_pthr_recv = mthr_open();
	m_pthr_send = mthr_open();

	// 初始化条件变量，有需要才唤醒的线程，读线程不需要，因为要一直工作
	mcond_init( m_pthr_readqueue );
	mcond_init( m_pthr_send );

	// 创建主逻辑线程
	mthr_create( m_pthr_readqueue, (void *)ThreadReadQueueProc, this );

	// 创建接收线程
	mthr_create( m_pthr_recv, (void *)ThreadRecvProc, this );

	// 创建发送线程
	mthr_create( m_pthr_send, (void *)ThreadSendProc, this );

	// 启动就绪
	m_bStartReady = true;
}

void TcpClient::Destroy()
{
	m_bExit = true;
	CloseSocket();
	if ( m_recv_synch_mutex == nullptr )
		return;
	mmux_lock( m_recv_synch_mutex );
	m_bRecvExit = true;
	mmux_unlock( m_recv_synch_mutex );

	// 等待接收线程结束
	mthr_join( m_pthr_recv );

	mmux_lock( m_read_mmux );
	memset( m_ReadQueue, 0, sizeof(SBuf)*MAX_QUEUE_NUM );
	m_nReadQueueHead = 0;
	m_nReadQueueTail = 0;
	mmux_unlock( m_read_mmux );

	mmux_lock( m_write_mmux );
	memset( m_WriteQueue, 0, sizeof(SBuf)*MAX_QUEUE_NUM );
	m_nWriteQueueHead = 0;
	m_nWriteQueueTail = 0;
	mmux_unlock( m_write_mmux );

	mcond_broadcast( m_pthr_readqueue );
	mcond_broadcast( m_pthr_send );
}

void TcpClient::CloseSocket()
{
	m_bStartReady = false;
	if ( m_socket < 0 )
		return;
	shutdown( m_socket, 2 );
#ifdef WIN32
	closesocket( m_socket );
#else
	close( m_socket );
#endif
	if ( m_Delegate )
		m_Delegate->onClose();
	m_socket = -1;
}

int TcpClient::ThreadReadQueueProc( void *arg )
{
	TcpClient *client = static_cast<TcpClient*>(arg);
	return client->ReadQueueProc();
}

int TcpClient::ThreadRecvProc( void *arg )
{
	TcpClient *client = static_cast<TcpClient*>(arg);
	return client->RecvProc();
}

int TcpClient::ThreadSendProc( void *arg )
{
	TcpClient *client = static_cast<TcpClient*>(arg);
	return client->SendProc();
}

// 读工作线程
int TcpClient::ReadQueueProc()
{
	int readsize;
	int truesize;
	char tmpbuf[MAX_PACKAGE_SIZE];
	while ( m_Delegate == NULL )
	{
#ifdef WIN32
		Sleep( 1000 );
#else
		sleep( 1 );
#endif
	}

	while ( !m_bExit )
	{
		readsize = 0;
		// 锁住队列缓冲
		mmux_lock( m_read_mmux );
		while ( m_nReadQueueTail == m_nReadQueueHead )
		{
			mcond_wait( m_pthr_readqueue, m_read_mmux );
			if ( m_bExit )
			{
				mmux_unlock( m_read_mmux );
				return 0;
			}
		}

		// 将数据从接收队列中取出来, 然后就可以自己工作了
		memcpy( tmpbuf, m_ReadQueue[m_nReadQueueHead].buf, m_ReadQueue[m_nReadQueueHead].buf_size );
		truesize = m_ReadQueue[m_nReadQueueHead].buf_size;
		m_nReadQueueHead++;
		if ( m_nReadQueueHead >= MAX_QUEUE_NUM )
		{
			m_nReadQueueHead = 0;
		}
		mmux_unlock( m_read_mmux );
		m_Delegate->onMessage( tmpbuf + sizeof(short), truesize - sizeof(short) );
	}
	return 0;
}

// 发送线程
int TcpClient::SendProc()
{
	int writesize;
	int writecount;
	char tmpbuf[MAX_BUF_SIZE];
	char *buf;
	while ( !m_bExit )
	{
		writesize = 0;
		// 锁住队列缓冲
		mmux_lock( m_write_mmux );
		while ( m_nWriteQueueTail == m_nWriteQueueHead )
		{
			mcond_wait( m_pthr_send, m_write_mmux );
			if ( m_bExit )
			{
				mmux_unlock( m_write_mmux );
				return 0;
			}
		}
		// 从队列中取出一项
		memcpy( tmpbuf, m_WriteQueue[m_nWriteQueueHead].buf, m_WriteQueue[m_nWriteQueueHead].buf_size );
		writesize = m_WriteQueue[m_nWriteQueueHead].buf_size;
		m_nWriteQueueHead++;
		if ( m_nWriteQueueHead >= MAX_QUEUE_NUM )
		{
			m_nWriteQueueHead = 0;
		}
		buf = tmpbuf;
		mmux_unlock( m_write_mmux );

		// 发送
		while ( 1 )
		{
			writecount = send( m_socket, buf, writesize, 0 );
			if ( writecount <= 0 )
			{
				break;
			}
			else
			{
				writesize -= writecount;
				if ( writesize <= 0 )
				{
					break;
				}
				buf += writecount;
			}
		}
	}
	return 0;
}

// 接收线程
int TcpClient::RecvProc()
{
	int readsize;
	char tmpbuf[MAX_BUF_SIZE];
	int nProcBytes = 0;
	int procsize = 0;

	while ( !m_bExit )
	{
		mmux_lock( m_recv_synch_mutex );
		if ( m_bRecvExit )
		{
			m_bRecvExit = false;
			mmux_unlock( m_recv_synch_mutex );
			return -1;
		}
		if ( m_socket < 0 )
		{
			mmux_unlock( m_recv_synch_mutex );
#ifdef WIN32
			Sleep( 1000 );
#else
			sleep( 1 );
#endif
			continue;
		}
		// 从sock中读数据到缓冲区
		if ( (readsize = recv( m_socket, tmpbuf + nProcBytes, MAX_PACKAGE_SIZE - nProcBytes, 0 )) < 0 )
		{
#ifdef WIN32
			if ( WSAETIMEDOUT == WSAGetLastError() )
			{
				mmux_unlock( m_recv_synch_mutex );
				continue;
			}
			// 连接断开了(连接重置了)
			write_netlog( "socket recv error, recv_proc errno = %d", WSAGetLastError() );
			CloseSocket();
			mmux_unlock( m_recv_synch_mutex );
			Sleep( 100 );
#else
			CloseSocket();
			mmux_unlock( m_recv_synch_mutex );
			sleep( 1 );
#endif
			continue;
		}
		else if ( readsize == 0 )
		{
			// 客户端断开
			write_netlog( "socket closed read == 0" );
			CloseSocket();
			mmux_unlock( m_recv_synch_mutex );
#ifdef WIN32
			Sleep( 100 );
#else
			sleep( 1 );
#endif
			continue;
		}

		nProcBytes += readsize;
		procsize = RecvPackage( tmpbuf, nProcBytes );
		if ( procsize < 0 )
		{
			procsize = readsize;
			write_netlog( "socket closed procsize < 0 " );
			CloseSocket();
			mmux_unlock( m_recv_synch_mutex );
#ifdef WIN32
			Sleep( 100 );
#else
			sleep( 1 );
#endif
			continue;
		}
		if ( procsize > 0 )
		{
			nProcBytes -= procsize;
			memmove( tmpbuf, tmpbuf + procsize, nProcBytes );
		}
		mmux_unlock( m_recv_synch_mutex );
	}
	return 0;
}

// 辅助函数, 解析数据包, 放置到接收队列, 返回已经处理完的字节数
// 对于网络底层, 是简单的包结构: len[short]|data[len]
int TcpClient::RecvPackage( char *buf, int size )
{
	char *ptr;
	int package_size;
	int proc_size;
	int remain_size;
	ptr = buf;
	proc_size = 0;
	remain_size = size;

	while ( 1 )
	{
		if ( remain_size < (int)sizeof(short) )
		{
			return proc_size;
		}
#ifndef NONECODE
		// 如果包长度也加密, 这里的包长度也需要解密
#ifdef SCRIPTCODE
		package_size = decode_size( *((short *)ptr) );	// package_size 包括自己，所以至少必须大于sizeof(short)
#else
		package_size = netcode_decode_size( *((short *)ptr) );	// package_size 包括自己，所以至少必须大于sizeof(short)
#endif
#else
		package_size = *((short *)ptr);	// package_size 包括自己，所以至少必须大于sizeof(short)
#endif
		// 如果包长度也加密, 这里的包长度也需要解密
		package_size += sizeof(short);
		if ( package_size < (int)sizeof(short) || package_size >= MAX_PACKAGE_SIZE )
		{
			return -1;
		}
		if ( remain_size < package_size )
		{
			return proc_size;
		}
		ReadToQueue( ptr, package_size );
		proc_size += package_size;
		remain_size -= package_size;
		ptr += package_size;
	}
	return -1;
}

// 这个函数应该是将接收数据,然后放置到队列
// 该函数可做的优化(废除tmpbuf)
int TcpClient::ReadToQueue( char *pBuf, int readsize )
{
	int queue_tail;
	if ( readsize == 0 )
	{
		return -1;
	}
	
	// 锁住读队列缓冲
	mmux_lock( m_read_mmux );
	queue_tail = m_nReadQueueTail + 1;
	if ( queue_tail >= MAX_QUEUE_NUM )
	{
		queue_tail = 0;
	}
	if ( m_nReadQueueHead == queue_tail )
	{
		// 队列已经满了, 断开这个客户
		mmux_unlock( m_read_mmux );
		write_netlog( "read queue full!" );
		CloseSocket();
		return -1;
	}

#ifndef NONECODE
	// 这里解密客户端的数据,将明文复制到 read_queue[g_nReadQueueTail].buf
#ifdef SCRIPTCODE
	decode_data( read_queue[g_nReadQueueTail].buf, pBuf, readsize );
#else
	netcode_decode( read_queue[g_nReadQueueTail].buf, pBuf, readsize );
#endif
#else
	memcpy( m_ReadQueue[m_nReadQueueTail].buf, pBuf, readsize );
#endif
	m_ReadQueue[m_nReadQueueTail].buf_size = readsize;

	m_nReadQueueTail = queue_tail;

	mmux_unlock( m_read_mmux );

	// 通知接收线程开始工作
	mcond_broadcast( m_pthr_readqueue );
	return 0;
}

// 这里的发送,实际上是将数据放置到写缓冲队列中,并且置sock的epoll状态为写
int TcpClient::SendToQueue( const char *buf, int bufsize )
{
	int queue_tail;
	if ( buf == NULL || bufsize <= 0 )
		return -1;

	// 锁住队列缓冲
	mmux_lock( m_write_mmux );

	queue_tail = m_nWriteQueueTail + 1;
	if ( queue_tail >= MAX_QUEUE_NUM )
	{
		queue_tail = 0;
	}
	if ( m_nWriteQueueHead == queue_tail )
	{
		// 队列已经满了, 断开这个客户
		mmux_unlock( m_write_mmux );
		write_netlog( "write queue full" );
		CloseSocket();
		return -1;
	}

	memcpy( m_WriteQueue[m_nWriteQueueTail].buf, buf, bufsize );
	m_WriteQueue[m_nWriteQueueTail].buf_size = bufsize;
	m_nWriteQueueTail = queue_tail;

	mmux_unlock( m_write_mmux );
	mcond_broadcast( m_pthr_send );
	return 0;
}
