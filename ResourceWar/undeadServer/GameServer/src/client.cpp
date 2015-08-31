#include "client.h"
#include "login.h"

#define HEARTBEAT_OUTTIME	18			// 2分钟
#define ENTERGAME_WAITTIME	30			// 5分钟

int g_max_connection = 100;

extern SOCKET_SET *g_sockset;
extern int g_net_time;
extern int g_max_connection;
static int s_max_recv_count = 0;

void client_setstat( int client_index, char stat )
{
	if( client_index >= 0 && client_index < g_max_connection )
		g_sockset[client_index].m_stat = stat;
}

char client_getstat( int client_index )
{
	if( client_index >= 0 && client_index < g_max_connection )
		return g_sockset[client_index].m_stat;
	return CLIENT_STAT_NULL;
}

void client_setwait( int client_index, char iswait )
{
	if( client_index >= 0 && client_index < g_max_connection )
		g_sockset[client_index].m_iswait = iswait;
}

char client_getwait( int client_index )
{
	if( client_index >= 0 && client_index < g_max_connection )
		return g_sockset[client_index].m_iswait;
	return 0;
}

// 0完全不同 1.一个高位相同 2.两个高位相同 3.三个高位相同 4完全相同
char cleint_ipcomp( int client_index, char *ip )
{
	int tmpi;
	int curflag = 0;
	if( ip == NULL )
		return 0;
	if( client_index >= 0 && client_index < g_max_connection )
	{
		for( tmpi = 0; tmpi < 16; tmpi++ )
		{
			if( ip[tmpi] != g_sockset[client_index].m_ip[tmpi] )
				return curflag;
			if( ip[tmpi] == 0 )
				return curflag+1;
			if( ip[tmpi] == '.' )
				curflag++;
		}
	}
	return 0;
}

char *client_getiptoken( int client_index, char *ptr )
{
	if( client_index >= 0 && client_index < g_max_connection )
	{
		if( ptr && g_sockset[client_index].m_expires_in > 0 )
		{
			if( g_sockset[client_index].m_expires_in < (int)time(NULL) )
			{
				//user_login( client_index, g_sockset[client_index].m_refresh_token, g_sockset[client_index].m_username, "" );
				return NULL;
			}
			sprintf( ptr, "%s#%s#%s", g_sockset[client_index].m_username, g_sockset[client_index].m_access_token, g_sockset[client_index].m_ip );
			return ptr;
		}
		else
			return g_sockset[client_index].m_ip;
	}
	return NULL;
}

char *client_getip( int client_index )
{
	if( client_index >= 0 && client_index < g_max_connection )
		return g_sockset[client_index].m_ip;
	return NULL;
}

char *client_getdeviceid( int client_index )
{
	if( client_index >= 0 && client_index < g_max_connection )
		return g_sockset[client_index].m_deviceid;
	return NULL;
}

void client_setuserinfo( int client_index, SClientInfo *pClientInfo )
{
	if( client_index >= 0 && client_index < g_max_connection && pClientInfo )
	{
		if( pClientInfo->userid > 0 )
			g_sockset[client_index].m_userid = pClientInfo->userid;
		if( pClientInfo->username[0] )
			memcpy( g_sockset[client_index].m_username, pClientInfo->username, MAX_USERNAME_LENGTH );
		g_sockset[client_index].m_username[MAX_USERNAME_LENGTH] = 0;
		g_sockset[client_index].m_usertype = pClientInfo->usertype;
		g_sockset[client_index].m_expires_in = pClientInfo->expires_in;
		memcpy( g_sockset[client_index].m_access_token, pClientInfo->access_token, 65 );
		memcpy( g_sockset[client_index].m_refresh_token, pClientInfo->refresh_token, 65 );
		memcpy( g_sockset[client_index].m_deviceid, pClientInfo->deviceid, 65 );
	}
}

i64 client_getuserinfo( int client_index, SClientInfo *pClientInfo )
{
	if( client_index >= 0 && client_index < g_max_connection )
	{
		if( pClientInfo )
		{
			pClientInfo->userid = g_sockset[client_index].m_userid;
			memcpy( pClientInfo->username, g_sockset[client_index].m_username, MAX_USERNAME_LENGTH );
			pClientInfo->username[MAX_USERNAME_LENGTH] = 0;
			pClientInfo->usertype = g_sockset[client_index].m_usertype;
			pClientInfo->expires_in = g_sockset[client_index].m_expires_in;
			memcpy( pClientInfo->access_token, g_sockset[client_index].m_access_token, 65 );
			memcpy( pClientInfo->refresh_token, g_sockset[client_index].m_refresh_token, 65 );
		}
		return g_sockset[client_index].m_userid;
	}
	return -1;
}

i64 client_getuserid( int client_index )
{
	if( client_index >= 0 && client_index < g_max_connection )
	{
		return g_sockset[client_index].m_userid;
	}
	return -1;
}

int client_get_exec_code( int client_index )
{
	if( client_index >= 0 && client_index < g_max_connection )
	{
		return g_sockset[client_index].m_exec_code;
	}
	return -1;
}

short client_getusertype( int client_index )
{
	if( client_index >= 0 && client_index < g_max_connection )
	{
		return g_sockset[client_index].m_usertype;
	}
	return -1;
}

char *client_getusername( int client_index )
{
	if( client_index >= 0 && client_index < g_max_connection )
	{
		return g_sockset[client_index].m_username;
	}
	return NULL;
}

int client_getplatid( int client_index )
{
	if( client_index >= 0 && client_index < g_max_connection )
	{
		return g_sockset[client_index].m_platid;
	}
	return -1;
}

int client_getlanguage( int client_index )
{
	if ( client_index >= 0 && client_index < g_max_connection )
	{
		return g_sockset[client_index].m_language;
	}
	return 1;
}

int client_getinvcode( int client_index )
{
	if( client_index >= 0 && client_index < g_max_connection )
	{
		return g_sockset[client_index].m_invcode;
	}
	return -1;
}

int client_getclientoptype( int client_index )
{
	if( client_index >= 0 && client_index < g_max_connection )
	{
		return g_sockset[client_index].m_optype;
	}
	return -1;
}

int client_get_maxrecvcount()
{
	return s_max_recv_count;
}

// 退出标志
char client_isexited( int client_index )
{
	if( client_index >= 0 && client_index < g_max_connection && g_sockset[client_index].m_fd == EXITED_SOCKET )
		return 1;
	return 0;
}

// 每个客户端的处理, 每10秒一次
int client_logic()
{
	int client_index;
	g_net_time++;
	for( client_index = 0; client_index < g_max_connection; client_index++ )
	{
		if( g_sockset[client_index].m_fd != INVALID_SOCKET )
		{
#ifndef _DEBUG
			++g_sockset[client_index].m_idle_time;
#endif
//			if( g_sockset[client_index].m_idle_time >= HEARTBEAT_OUTTIME ||
			if( g_sockset[client_index].m_idle_time >= 90 ||	// 先定15分钟，客户端有心跳协议后再改
				( g_sockset[client_index].m_stat < CLIENT_STAT_ENTERGAME && g_net_time - g_sockset[client_index].m_enter_time > ENTERGAME_WAITTIME ) )
			{
				write_netlog( "message:close client(m_stat:%d),fd=%d", g_sockset[client_index].m_stat, g_sockset[client_index].m_fd );
				close_client( client_index );
			}
		}
		if( g_sockset[client_index].m_recv_count > s_max_recv_count )
			s_max_recv_count = g_sockset[client_index].m_recv_count;
		g_sockset[client_index].m_recv_count = 0;
	}
	return 0;
}
