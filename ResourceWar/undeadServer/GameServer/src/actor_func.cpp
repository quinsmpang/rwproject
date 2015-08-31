#ifdef WIN32
#include <winsock2.h>
#include <windows.h>
#endif
#include <mysql.h>
#include "db.h"
#include "actor.h"
#include "actor_send.h"
#include "server_netsend_auto.h"

extern MYSQL *myGame;
extern Actor *g_actors;
extern int g_maxactornum;

//-----------------------------------------------------------------------------
// actor_getoffline_userid
// 函数说明: 根据索引获取用户id
//-----------------------------------------------------------------------------
i64 actor_getoffline_userid( int actorid )
{
	MYSQL_RES		*res;
	MYSQL_ROW		row;
	char	szSQL[1024];
	i64 userid = -1;

	sprintf( szSQL, "select userid from actor_list where actorid='%d'", actorid );
	if ( mysql_query( myGame, szSQL ) )
	{
		printf( "Query failed (%s) [%s](%d)\n", mysql_error( myGame ), __FUNCTION__, __LINE__ );
		write_gamelog( "%s", szSQL );
		if ( mysql_ping( myGame ) != 0 )
			db_reconnect_game();
		return -1;
	}
	res = mysql_store_result( myGame );

	if ( (row = mysql_fetch_row( res )) )
	{
		userid = atoll( row[0] );
	}
	mysql_free_result( res );
	return userid;
}

//-----------------------------------------------------------------------------
// actor_getindex_withid
// 函数说明: 根据角色ID返回角色索引
//-----------------------------------------------------------------------------
int actor_getindex_withid( int actorid )
{
	int tmpi;

	if ( actorid <= 0 )
		return -1;
	for ( tmpi = 0; tmpi < g_maxactornum; tmpi++ )
	{
		if ( g_actors[tmpi].actorid == actorid )
			return tmpi;
	}
	return -1;
}

//-----------------------------------------------------------------------------
// actor_getlist
// 函数说明: 返回他的角色列表
//-----------------------------------------------------------------------------
int actor_getlist( int platid, i64 userid, SLK_NetS_List *pValue )
{
	MYSQL_RES		*res;
	MYSQL_ROW		row;
	char	szSQL[1024];
	char reconnect_flag = 0;
	int thistime = (int)time( NULL );
	int locktime;
	char szUserID[21];
	char num = 0;
	int index;
	pValue->m_actor_num = 0;
	lltoa( userid, szUserID, 10 );
RE_QUERY:
	sprintf( szSQL, "select actorid,name,aclass,level,lock_time,delete_stoptime from actor_list where userid='%s' and platid='%d' and offset >= 0 ", szUserID, platid );
	if ( mysql_query( myGame, szSQL ) )
	{
		printf( "Query failed (%s) [%s](%d)\n", mysql_error( myGame ), __FUNCTION__, __LINE__ );
		write_gamelog( "%s", szSQL );
		if ( reconnect_flag )
			return -1;
		if ( mysql_ping( myGame ) != 0 )
		{
			db_reconnect_game();
			reconnect_flag = 1;
			goto RE_QUERY;
		}
		return -1;
	}
	res = mysql_store_result( myGame );

	while ( (row = mysql_fetch_row( res )) )
	{
		index = 0;
		pValue->m_listinfo[pValue->m_actor_num].m_actorid = atoi( row[index++] );
		memcpy( pValue->m_listinfo[pValue->m_actor_num].m_name, row[index++], NAME_SIZE*sizeof(char) );
		pValue->m_listinfo[pValue->m_actor_num].m_aclass = atoi( row[index++] );
		pValue->m_listinfo[pValue->m_actor_num].m_level = atoi( row[index++] );
		locktime = atoi( row[index++] );
		pValue->m_listinfo[pValue->m_actor_num].m_delete_stoptime = atoi( row[index++] );
		if ( pValue->m_listinfo[pValue->m_actor_num].m_delete_stoptime > 0 && thistime >= pValue->m_listinfo[pValue->m_actor_num].m_delete_stoptime )
		{
			// 如果时间到了,则删除这个角色,并不发送
			if ( actor_delete( userid, pValue->m_listinfo[pValue->m_actor_num].m_actorid ) >= 0 ) // 删除这个角色
			{
				continue;
			}
		}
		if ( locktime > thistime )
			pValue->m_listinfo[pValue->m_actor_num].m_lockstat = 1;
		else
			pValue->m_listinfo[pValue->m_actor_num].m_lockstat = 0;
		pValue->m_actor_num++;
		if ( pValue->m_actor_num >= 8 )
		{
			break;
		}
	}
	mysql_free_result( res );
	return 0;
}

//-----------------------------------------------------------------------------
// actor_verifyid
// 函数说明: 验证一下这个角色id是不是属于登陆的这个用户
//-----------------------------------------------------------------------------
int actor_verifyid( i64 userid, int actorid )
{
	MYSQL_RES *res;
	MYSQL_ROW row;
	char szSQL[1024];
	int verify = 0;
	char szUserID[21] = {0};
	lltoa( userid, szUserID, 10 );
	sprintf( szSQL, "select actorid from actor_list where actorid='%d' and userid='%s'", actorid, szUserID );
	if ( mysql_query( myGame, szSQL ) )
	{
		printf( "Query failed (%s) [%s](%d)\n", mysql_error( myGame ), __FUNCTION__, __LINE__ );
		write_gamelog( "%s", szSQL );
		if ( mysql_ping( myGame ) != 0 )
			db_reconnect_game();
		return -1;
	}
	res = mysql_store_result( myGame );
	if ( (row = mysql_fetch_row( res )) )
	{
		verify = 0;
	}
	else
	{
		verify = -1;
	}
	mysql_free_result( res );
	return verify;
}

//-----------------------------------------------------------------------------
// actor_getinfo
// 函数说明: 角色信息
//-----------------------------------------------------------------------------
int actor_getinfo( int actor_index )
{
	SLK_NetS_ActorInfo info = { 0 };
	info.m_actorid = g_actors[actor_index].actorid;
	memcpy( info.m_name, g_actors[actor_index].name, sizeof(char)*NAME_SIZE );
	info.m_aclass = g_actors[actor_index].aclass;
	info.m_shape = g_actors[actor_index].shape;
	info.m_level = g_actors[actor_index].level;
	info.m_experience = g_actors[actor_index].experience;
	info.m_experience_max = 0;
	info.m_leadership = g_actors[actor_index].leadership;
	info.m_token = g_actors[actor_index].token;
	info.m_viplevel = g_actors[actor_index].vip;
	netsend_actorinfo_S( actor_index, SENDTYPE_ACTOR, &info );
	return 0;
}
