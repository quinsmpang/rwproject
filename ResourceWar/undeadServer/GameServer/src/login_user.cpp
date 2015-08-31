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


// �û����͵�½���󵽵�½����
int user_login( int client_index, const char *szUserName, const char *szPassWord, char *szDeviceID )
{
	int authid;
	int queue_tail;
	char tmpstr[2048];

	authid = server_getautuid( client_index );
	if( authid < 0 )
		return -1;
	// ��סд���л���
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
		// ֪ͨ��¼�̵߳øϽ��ɻ�, �����˰�
		mcond_broadcast( g_pthr_login );
		return -1;
	}
	// ����Ҫ����Ŀͻ����õ���¼����
	login_queue[g_nLoginQueueTail].client_index = client_index;
	login_queue[g_nLoginQueueTail].authid = authid;
	login_queue[g_nLoginQueueTail].command = USERCMDC_LOGIN;
	sprintf( login_queue[g_nLoginQueueTail].data, "&v1=%s&v2=%s&v3=%d&v4=%s&v5=%s", szUserName, HttpString( szPassWord, tmpstr ), client_getclientoptype( client_index ), client_getip( client_index ), szDeviceID );
	g_nLoginQueueTail = queue_tail;
	mmux_unlock( g_login_mmux );
	// ֪ͨ��¼�̸߳ɻ�
	mcond_broadcast( g_pthr_login );
	return 0;
}

// �û����������ص�½���
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

	// �����û���
	Value.m_username_length = (int)strlen( pClientInfo->username ) + 1;
	strcpy( Value.m_username, pClientInfo->username );

	// ��½ʧ��
	if ( pClientInfo->result < 0  )
	{
		netsend_login_S( client_index, SENDTYPE_ACTOR, &Value );
		return -1;
	}

	// ��½�ɹ�
	Value.m_token_length = 65;
	memcpy( Value.m_token, pClientInfo->access_token, Value.m_token_length*sizeof(char) );
	netsend_login_S( client_index, SENDTYPE_ACTOR, &Value );

	// ����Ƿ��н�ɫ
	SLK_NetS_List ListValue = {0};
	actor_getlist( client_getplatid( client_index ), client_getuserid( client_index ), &ListValue );
	netsend_list_S( client_index, SENDTYPE_ACTOR, &ListValue );
	return 0;
}

// �û������˺�
int user_lock( int client_index, int authid, i64 player_userid, int lockmin )
{
	char *ptr;
	int queue_tail;

	if( client_index < 0 )
		player_userid = actor_getoffline_userid( authid );

	if( player_userid < 0 )
		return -1;

	// ��סд���л���
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
		// ֪ͨ��¼�̵߳øϽ��ɻ�, �����˰�
		mcond_broadcast( g_pthr_login );
		return -1;
	}
	// ����Ҫ����Ŀͻ����õ���¼����
	login_queue[g_nLoginQueueTail].client_index = client_index;
	login_queue[g_nLoginQueueTail].authid = authid;
	login_queue[g_nLoginQueueTail].command = USERCMDC_LOCKUSER;
	ptr = login_queue[g_nLoginQueueTail].data;

	char tmpBuf[MAX_PATH];
	sprintf( login_queue[g_nLoginQueueTail].data, "&v1=%s&v2=%d", lltoa(player_userid,tmpBuf,10), lockmin );

	g_nLoginQueueTail = queue_tail;
	mmux_unlock( g_login_mmux );
	// ֪ͨ��¼�̸߳ɻ�
	mcond_broadcast( g_pthr_login );
	return 0;
}


// �����Ŷ���Ϣ����ʱ����
int user_queue_logic()
{
	int tmpi;

	// ��סд���л���
	mmux_lock( g_login_mmux );
	
	// ����Ҫ����Ŀͻ����õ���¼����
	for ( tmpi = g_nLoginQueueHead; tmpi < g_nLoginQueueTail; tmpi++ )
	{
		if ( login_queue[tmpi].client_index >= 0 )
		{
			int value[1] = { 0 };
			value[0] = tmpi - g_nLoginQueueHead;
			//actor_notify_value( login_queue[tmpi].client_index, NOTIFY_LOGIN_QUEUE, 1, value, 0, NULL );
		}
	}
	
	// �������л���
	mmux_unlock( g_login_mmux );
	return 0;
}

// ��յ�½����
void login_queue_clear()
{
	// ��סд���л���
	mmux_lock( g_login_mmux );

	g_nLoginQueueHead = 0;
	g_nLoginQueueTail = 0;
	memset( login_queue, 0, sizeof(SLoginQueue)*MAX_LOGINQUEUENUM );
	// �������л���
	mmux_unlock( g_login_mmux );
}
