#include "Game.h"
#include "Message.h"
#include "NetClient.h"
#include "NetProcess.h"
#include "Command.h"

extern char	g_nRecMode;
extern SLK_Player g_Player[1000];

void SendNow()
{
}

int SendToBuf( const char *buf, int size, int sock_index )
{
	sendtoqueue( buf, size, sock_index, g_Player[sock_index].exec_code );
	return 0;
}

extern int g_send_num;
int rawsend( char *data, int size, short cmd, int sock_index )
{
	char sendbuf[2048];
	char bakbuf[2048];
	*((short *)bakbuf) = size+2;
	*((short *)(bakbuf+2)) = cmd;
	memcpy( bakbuf+4, data, size );
#ifndef NONECODE
	netcode_encode( sock_index, sendbuf, bakbuf, size+4 );
#else
	memcpy( sendbuf, bakbuf, size+4 );
#endif
	SendToBuf( sendbuf, size+4, sock_index );
	g_send_num++;
	return 0;
}

int Net_HeartBeat( int sock_index )
{
	char buf[2];
	int bufsize = 0;
	rawsend( buf, bufsize, CMDC_HEARTBEAT, sock_index );
	return 0;
}

// 需要告诉服务器的事情

// 输入你的用户名和密码, 连接和用户就关联上了
// 用户名的长度为3-18,密码长度为6-16
// || CMDC_LOGIN | szUserName[19] | szPassWord[17] ||
int Net_Login( char *szUserName, char *szPassWord, int sock_index )
{
	int userlen,passlen;
	char buf[1024];
	int bufsize = 0;

	userlen = (int)strlen(szUserName);
	passlen = (int)strlen(szPassWord);
	if( userlen < 1 || userlen > MAX_USERNAME_LENGTH || passlen < 1 || passlen > MAX_PASSWORD_LENGTH )
		return -1;

	memcpy( (buf+bufsize), szUserName, MAX_USERNAME_LENGTH+1 ); bufsize += MAX_USERNAME_LENGTH+1;
	memcpy( (buf+bufsize), szPassWord, MAX_PASSWORD_LENGTH+1 ); bufsize += MAX_PASSWORD_LENGTH+1;
	
	*(short *)(buf+bufsize) = 0; bufsize += sizeof(short);
	*(short *)(buf+bufsize) = 0; bufsize += sizeof(short);
	rawsend( buf, bufsize, CMDC_LOGIN, sock_index );
	return 0;
}

// 进入游戏, 使用这个账户的那个角色呢?
// || CMDC_ADDACTOR | actorid[int] ||
int Net_EnterGame( int actorid, int sock_index )
{
	char buf[1024];
	int bufsize = 0;

	*(int *)(buf+bufsize) = actorid; bufsize += sizeof(int);

	// 发送之前的包要打成这样的格式 |short(包长度)|data(变长的内容)
	rawsend( buf, bufsize, CMDC_ENTERGAME, sock_index );

	return 0;
}

// 列角色
// || CMDC_LIST ||
int Net_List( int sock_index )
{
	// 发送之前的包要打成这样的格式 |short(包长度)|data(变长的内容)
	rawsend( NULL, 0, CMDC_LIST, sock_index );
	return 0;
}

// 创建角色
// || CMDC_CREATE ||
int Net_Create( char aclass, char *name, int sock_index )
{
	char buf[1024];
	int bufsize = 0;

	*(char *)(buf + bufsize) = aclass; bufsize += sizeof(char);
	memcpy( (buf+bufsize), name, NAME_SIZE*sizeof(char) ); bufsize += NAME_SIZE*sizeof(char);

	// 发送之前的包要打成这样的格式 |short(包长度)|data(变长的内容)
	rawsend( buf, bufsize, CMDC_CREATE, sock_index );

	return 0;
}

// 删除角色
// || CMDC_DELETE ||
int Net_Delete( char nOffset, int sock_index )
{
	char buf[1024];
	int bufsize = 0;

	*(char *)(buf+bufsize) = nOffset; bufsize += sizeof(char);

	// 发送之前的包要打成这样的格式 |short(包长度)|data(变长的内容)
	rawsend( buf, bufsize, CMDC_DELETE, sock_index );

	return 0;
}

// 离开游戏, 没什么好说的
// || CMDC_DELACTOR ||
int Net_LeaveGame( int sock_index )
{
	rawsend( NULL, 0, CMDC_LEAVEGAME, sock_index );
	return 0;
}

// 登出游戏
// || CMDC_LOGOUT ||
int Net_Logout( int sock_index )
{
	//rawsend( NULL, 0, CMDC_LOGOUT, sock_index );
	return 0;
}

// 角色移动位置
// || CMDC_MOVE | posx[short] | posy[short] ||
int Net_Move( int posx, int posy, int sock_index )
{
	char buf[1024];
	int bufsize = 0;

	*(short *)(buf+bufsize) = posx; bufsize += sizeof(short);
	*(short *)(buf+bufsize) = posy; bufsize += sizeof(short);

	rawsend( buf, bufsize, CMDC_MOVE, sock_index );

	return 0;
}

// || CMDC_FIGHT | act_pos[char] | act_type[char] | act_value[short] | pet_pos[char] | pet_type[char] | pet_value[short] ||
//int Net_Fight( char act_pos, char act_type, short act_value, char pet_pos, char pet_type, short pet_value )
int Net_Fight( SLK_Act *pAct, int sock_index )
{
	int tmpi;
	char buf[1024];
	int bufsize = 0;

	for( tmpi = 0; tmpi < 7; tmpi++ )
	{
		*(char *)(buf+bufsize) = pAct[tmpi].m_nPos; bufsize += sizeof(char);
		*(char *)(buf+bufsize) = pAct[tmpi].m_nType; bufsize += sizeof(char);
		*(short *)(buf+bufsize) = pAct[tmpi].m_nValue; bufsize += sizeof(short);
	}

	rawsend( buf, bufsize, CMDC_FIGHT, sock_index );

	return 0;
}

// 角色说话
// || CMDC_TALK | msgsize[short] | pText[msgsize] ||
int Net_Talk( const char *pText, char nType, int sock_index )
{
	char buf[1024];
	int bufsize = 0;
	int nTextLen = 0;
	int msgsize;

	nTextLen = (int)strlen( pText );
	if( nTextLen <= 0 || nTextLen >= 128 )
		return -1;

	msgsize = ( nTextLen + 1 )*sizeof(char);

	*(char *)(buf+bufsize) = nType; bufsize += sizeof(char);
	*(short *)(buf+bufsize) = msgsize; bufsize += sizeof(short);
	memcpy( (buf+bufsize), pText, msgsize ); bufsize += msgsize;

	rawsend( buf, bufsize, CMDC_TALK, sock_index );

	return 0;
}

int Net_GMCommand( short nCmd, int value1, int value2, int value3, int value4, char *pMsg, int sock_index )
{
	char buf[1024];
	int bufsize = 0;
	int msglen;

	*(short *)(buf+bufsize) = nCmd; bufsize += sizeof(short);
	*(int *)(buf+bufsize) = value1; bufsize += sizeof(int);
	*(int *)(buf+bufsize) = value2; bufsize += sizeof(int);
	*(int *)(buf+bufsize) = value3; bufsize += sizeof(int);
	*(int *)(buf+bufsize) = value4; bufsize += sizeof(int);
	if( pMsg == NULL )
	{
		*(short *)(buf+bufsize) = 0; bufsize += sizeof(short);
	}
	else
	{
		msglen = (int)strlen(pMsg)+1;
		*(short *)(buf+bufsize) = msglen; bufsize += sizeof(short);
		memcpy( buf+bufsize, pMsg, msglen ); bufsize += msglen;
	}

	rawsend( buf, bufsize, CMDC_GMCOMMAND, sock_index );

	return 0;
}


int Net_TouchNpc( char nType, char nSelected, int nActorID, int sock_index )
{
	char buf[1024];
	int bufsize = 0;

	*(char *)(buf+bufsize) = nType; bufsize += sizeof(char);
	*(char *)(buf+bufsize) = nSelected; bufsize += sizeof(char);
	*(int *)(buf+bufsize) = nActorID; bufsize += sizeof(int);

	rawsend( buf, bufsize, CMDC_TOUCHNPC, sock_index );

	return 0;
}

int Net_ItemUse( short nItemIndex, short nItemNum, char nPetIndex, int sock_index )
{
	char buf[1024];
	int bufsize = 0;

	*(short *)(buf+bufsize) = nItemIndex; bufsize += sizeof(short);
	*(short *)(buf+bufsize) = nItemNum; bufsize += sizeof(short);
	*(char *)(buf+bufsize) = nPetIndex; bufsize += sizeof(char);
	*(int *)(buf+bufsize) = 0; bufsize += sizeof(int);

	rawsend( buf, bufsize, CMDC_ITEMUSE, sock_index );

	return 0;
}
