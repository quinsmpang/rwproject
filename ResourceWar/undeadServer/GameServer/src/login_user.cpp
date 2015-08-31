#ifdef WIN32
#include <winsock2.h>
#include <windows.h>
#endif
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <mysql.h>
#include "netfunc.h"
#include "login.h"
#include "actor.h"
#include "gameproc.h"
#include "define.h"
#include "netserver.h"
#include "actor_send.h"
#include "http.h"
#include "server_netsend_auto.h"

extern SConfig g_Config;
extern int g_tick;

extern MYSQL *myGame;

extern SLoginQueue *login_queue;
extern int g_nLoginQueueHead;
extern int g_nLoginQueueTail;

extern Mthr	*g_pthr_login;
extern ThrMux	*g_login_mmux;

extern int g_maxactorid;
extern Actor *g_actors;
extern int g_maxactornum;
extern char g_bServerIsInit;

extern MYSQL *myData;


// 用户发送登陆请求到登陆队列
int user_login( int client_index, const char *szUserName, const char *szPassWord, char *szDeviceID )
{
	int authid;
	int queue_tail;
	char tmpstr[2048];

	authid = server_getautuid( client_index );
	if( authid < 0 )
		return -1;
	// 锁住写队列缓冲
	mmux_lock( g_login_mmux );
	queue_tail = g_nLoginQueueTail + 1;
	if( queue_tail >= MAX_LOGINQUEUENUM )
	{
		queue_tail = 0;
	}
	if( g_nLoginQueueHead == queue_tail )
	{
		write_netlog("login queue full");
		mmux_unlock( g_login_mmux );
		// 通知登录线程得赶紧干活, 都满了啊
		mcond_broadcast( g_pthr_login );
		return -1;
	}
	// 将需要处理的客户放置到登录队列
	login_queue[g_nLoginQueueTail].client_index = client_index;
	login_queue[g_nLoginQueueTail].authid = authid;
	login_queue[g_nLoginQueueTail].command = USERCMDC_LOGIN;
	sprintf( login_queue[g_nLoginQueueTail].data, "&v1=%s&v2=%s&v3=%d&v4=%s&v5=%s", szUserName, HttpString( szPassWord, tmpstr ), client_getclientoptype( client_index ), client_getip( client_index ), szDeviceID );
	g_nLoginQueueTail = queue_tail;
	mmux_unlock( g_login_mmux );
	// 通知登录线程干活
	mcond_broadcast( g_pthr_login );
	return 0;
}

// 用户服务器返回登陆结果
int user_logined( int client_index, int authid, SClientInfo *pClientInfo )
{
	char client_stat;
	SLK_NetS_Login Value = {0};

	if ( pClientInfo == NULL )
		return -1;

	if ( server_getautuid( client_index ) != authid )
		return -1;

	client_stat = client_getstat( client_index );
	if ( client_stat < 0 || client_stat > CLIENT_STAT_LOGIN )
		return -1;

	if ( pClientInfo->result >= 0 )
	{
		client_setstat( client_index, CLIENT_STAT_LOGIN );
		client_setuserinfo( client_index, pClientInfo );
	}

	Value.m_result = pClientInfo->result;

	// 返回用户名
	Value.m_username_length = (int)strlen( pClientInfo->username ) + 1;
	strcpy( Value.m_username, pClientInfo->username );

	// 登陆失败
	if ( pClientInfo->result < 0  )
	{
		netsend_login_S( client_index, SENDTYPE_ACTOR, &Value );
		return -1;
	}

	// 登陆成功
	Value.m_token_length = 65;
	memcpy( Value.m_token, pClientInfo->access_token, Value.m_token_length*sizeof(char) );
	netsend_login_S( client_index, SENDTYPE_ACTOR, &Value );

	// 检查是否有角色
	SLK_NetS_List ListValue = {0};
	actor_getlist( client_getplatid( client_index ), client_getuserid( client_index ), &ListValue );
	netsend_list_S( client_index, SENDTYPE_ACTOR, &ListValue );
	return 0;
}

// 用户锁定账号
int user_lock( int client_index, int authid, i64 player_userid, int lockmin )
{
	char *ptr;
	int queue_tail;

	if( client_index < 0 )
		player_userid = actor_getoffline_userid( authid );

	if( player_userid < 0 )
		return -1;

	// 锁住写队列缓冲
	mmux_lock( g_login_mmux );
	queue_tail = g_nLoginQueueTail + 1;
	if( queue_tail >= MAX_LOGINQUEUENUM )
	{
		queue_tail = 0;
	}
	if( g_nLoginQueueHead == queue_tail )
	{
		write_netlog("login queue full");
		mmux_unlock( g_login_mmux );
		// 通知登录线程得赶紧干活, 都满了啊
		mcond_broadcast( g_pthr_login );
		return -1;
	}
	// 将需要处理的客户放置到登录队列
	login_queue[g_nLoginQueueTail].client_index = client_index;
	login_queue[g_nLoginQueueTail].authid = authid;
	login_queue[g_nLoginQueueTail].command = USERCMDC_LOCKUSER;
	ptr = login_queue[g_nLoginQueueTail].data;

	char tmpBuf[MAX_PATH];
	sprintf( login_queue[g_nLoginQueueTail].data, "&v1=%s&v2=%d", lltoa(player_userid,tmpBuf,10), lockmin );

	g_nLoginQueueTail = queue_tail;
	mmux_unlock( g_login_mmux );
	// 通知登录线程干活
	mcond_broadcast( g_pthr_login );
	return 0;
}


// 发送排队信息，暂时无用
int user_queue_logic()
{
	int tmpi;

	// 锁住写队列缓冲
	mmux_lock( g_login_mmux );
	
	// 将需要处理的客户放置到登录队列
	for ( tmpi = g_nLoginQueueHead; tmpi < g_nLoginQueueTail; tmpi++ )
	{
		if ( login_queue[tmpi].client_index >= 0 )
		{
			int value[1] = { 0 };
			value[0] = tmpi - g_nLoginQueueHead;
			//actor_notify_value( login_queue[tmpi].client_index, NOTIFY_LOGIN_QUEUE, 1, value, 0, NULL );
		}
	}
	
	// 解锁队列缓冲
	mmux_unlock( g_login_mmux );
	return 0;
}

// 清空登陆队列
void login_queue_clear()
{
	// 锁住写队列缓冲
	mmux_lock( g_login_mmux );

	g_nLoginQueueHead = 0;
	g_nLoginQueueTail = 0;
	memset( login_queue, 0, sizeof(SLoginQueue)*MAX_LOGINQUEUENUM );
	// 解锁队列缓冲
	mmux_unlock( g_login_mmux );
}
