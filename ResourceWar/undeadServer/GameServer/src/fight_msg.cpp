#ifdef WIN32
#include <winsock2.h>
#include <mswsock.h>
#endif
#include <mysql.h>
#include <string.h>
#include <time.h>
#include "db.h"
#include "gameproc.h"
#include "actor.h"
#include "mail_msg.h"
#include "utils.h"
#include "define.h"
#include "actor_send.h"
#include "script.h"
#include "actor_notify.h"
#include "fight_msg.h"

extern MYSQL *myGame;
// 一个场景中的角色列表
extern Actor *g_actors;
extern int g_maxactornum;

/* 发送战报 */
int fight_msg_send( int actorid, SLK_NetS_FightMsgInfo *pValue )
{
	if ( !pValue )
		return -1;

	MYSQL_RES *res;
	MYSQL_ROW row;
	char szSQL[1024] = { 0 };
	int thistime = (int)time( NULL );
	char bigint_recordid[21];
	lltoa( pValue->m_recordid, bigint_recordid, 10 );
	pValue->m_recvtime = thistime;
	// 插入数据库
	sprintf( szSQL, "INSERT INTO fight_msg ( `actorid`,`title`,`read`,`recvtime`,`posx`,`posy`,`status`,`target_type`,`target_id`,`wood`,`iron`,`food`,`recordid` ) "
		"VALUES ('%d','%s','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%s')",
		actorid, pValue->m_title, pValue->m_read, pValue->m_recvtime, pValue->m_posx, pValue->m_posy, pValue->m_status, pValue->m_target_type, pValue->m_target_id, pValue->m_wood, pValue->m_iron, pValue->m_food, bigint_recordid );
	if ( mysql_query( myGame, szSQL ) )
	{
		printf( "Query failed (%s)\n", mysql_error( myGame ) );
		write_gamelog( "%s", szSQL );
		if ( mysql_ping( myGame ) != 0 )
			db_reconnect_game();
		return -1;
	}

	// 如收件人在线，通知有新的消息
	int actorindex = actor_getindex_withid( actorid );
	if ( actorindex >= 0 )
	{
		sprintf( szSQL, "SELECT LAST_INSERT_ID()" );
		if ( mysql_query( myGame, szSQL ) )
		{
			printf( "Query failed (%s)\n", mysql_error( myGame ) );
			write_gamelog( "%s", szSQL );
			return -1;
		}
		res = mysql_store_result( myGame );
		if ( (row = mysql_fetch_row( res )) )
		{
			pValue->m_msgid = atoll( row[0] );
			netsend_fightmsginfo_S( actorindex, SENDTYPE_ACTOR, pValue );
		}
		mysql_free_result( res );
	}
	return 0;
}

/* 检查是否有未读战报 */
int fight_msg_check_read( int actor_index )
{
	MYSQL_RES	*res;
	MYSQL_ROW	row;
	char	szSQL[1024];
	if ( actor_index < 0 || actor_index >= g_maxactornum )
		return -1;
	sprintf( szSQL, "SELECT COUNT(*) FROM fight_msg WHERE `actorid`='%d' AND `read`=0 AND `recvtime`>'%d' ", g_actors[actor_index].actorid, (int)time( NULL ) - MAX_FIGHTMSG_SAVETIME );
	if ( mysql_query( myGame, szSQL ) )
	{
		printf( "Query failed (%s)\n", mysql_error( myGame ) );
		write_gamelog( "%s", szSQL );
		if ( mysql_ping( myGame ) != 0 )
			db_reconnect_game();
		return -1;
	}
	res = mysql_store_result( myGame );
	if ( (row = mysql_fetch_row( res )) )
	{
		int count = atoi( row[0] );
		if ( count > 0 )
		{
			int value[2] = { 0 };
			value[0] = 10;
			value[1] = count;
			actor_notify_value( actor_index, NOTIFY_MAIL, 2, value, NULL );
		}
	}
	mysql_free_result( res );
	return 0;
}

/* 战报列表 */
int fight_msg_loadlist( int actor_index, i64 maxid )
{
	if ( actor_index < 0 || actor_index >= g_maxactornum )
		return -1;
	MYSQL_RES	*res;
	MYSQL_ROW	row;
	char	szSQL[1024];
	SLK_NetS_FightMsgList info = { 0 };
	char bigint[21];
	lltoa( maxid, bigint, 10 );
	sprintf( szSQL, "SELECT `id`, `read`, `recvtime`, `status`, `target_type`, `title` FROM fight_msg WHERE id>'%s' AND actorid='%d' AND recvtime>'%d' LIMIT 10", bigint, g_actors[actor_index].actorid, (int)time( NULL ) - MAX_FIGHTMSG_SAVETIME );
	if ( mysql_query( myGame, szSQL ) )
	{
		printf( "Query failed (%s)\n", mysql_error( myGame ) );
		write_gamelog( "%s", szSQL );
		if ( mysql_ping( myGame ) != 0 )
			db_reconnect_game();
		return -1;
	}
	res = mysql_store_result( myGame );
	while ( (row = mysql_fetch_row( res )) )
	{
		// 邮件简单信息列表
		info.m_list[info.m_count].m_msgid = atoll( row[0] );
		info.m_list[info.m_count].m_read = (char)atoi( row[1] );
		info.m_list[info.m_count].m_recvtime = atoi( row[2] );
		info.m_list[info.m_count].m_status = atoi( row[3] );
		info.m_list[info.m_count].m_target_type = atoi( row[4] );
		memcpy( info.m_list[info.m_count].m_title, row[5], 32 );
		info.m_list[info.m_count].m_title_length=strlen(info.m_list[info.m_count].m_title);
		info.m_count++;
		if ( info.m_count >= 10 )
			break;
	}
	mysql_free_result( res );
	netsend_fightmsglist_S( actor_index, SENDTYPE_ACTOR, &info );
	return 0;
}

/* 战报详情 */
int fight_msg_read( int actor_index, i64 msgid )
{
	if ( actor_index < 0 || actor_index >= g_maxactornum )
		return -1;
	MYSQL_RES	*res;
	MYSQL_ROW	row;
	char	szSQL[1024];
	char bigint[21];
	lltoa( msgid, bigint, 10 );
	sprintf( szSQL, "SELECT posx, posy, target_type, target_id, target_posx, target_posy, wood, iron, food, recordid FROM fight_msg WHERE id='%s'", bigint );
	if ( mysql_query( myGame, szSQL ) )
	{
		printf( "Query failed (%s)\n", mysql_error( myGame ) );
		write_gamelog( "%s", szSQL );
		if ( mysql_ping( myGame ) != 0 )
			db_reconnect_game();
		return -1;
	}
	res = mysql_store_result( myGame );
	if ( (row = mysql_fetch_row( res )) )
	{
		// 发送详细信息
		int index = 0;
		SLK_NetS_FightMsgInfo info = { 0 };
		info.m_msgid = msgid;
		info.m_posx = (short)atoi( row[index++] );
		info.m_posy = (short)atoi( row[index++] );
		info.m_target_type = (char)atoi( row[index++] );
		info.m_target_id = atoi( row[index++] );
		info.m_target_posx = atoi( row[index++] );
		info.m_target_posy = atoi( row[index++] );
		info.m_wood = atoi( row[index++] );
		info.m_iron = atoi( row[index++] );
		info.m_food = atoi( row[index++] );
		info.m_recordid = atoi( row[index++] );
		info.m_read = 1;
		netsend_fightmsginfo_S( actor_index, SENDTYPE_ACTOR, &info );
	}
	mysql_free_result( res );

	// 设置已读状态
	sprintf( szSQL, "UPDATE fight_msg SET read='%d' WHERE id='%s'", 1, bigint );
	if ( mysql_query( myGame, szSQL ) )
	{
		printf( "Query failed (%s)\n", mysql_error( myGame ) );
		write_gamelog( "%s", szSQL );
		if ( mysql_ping( myGame ) != 0 )
			db_reconnect_game();
		return -1;
	}
	return 0;
}

/* 删除战报 */
int fight_msg_delete( int actor_index, i64 msgid )
{
	char szSQL[1024];
	char bigint[21];
	lltoa( msgid, bigint, 10 );
	sprintf( szSQL, "DELETE FROM fight_msg WHERE id='%s'", bigint );
	if ( mysql_query( myGame, szSQL ) )
	{
		printf( "Query failed (%s) [%s](%d)\n", mysql_error( myGame ), __FUNCTION__, __LINE__ );
		write_gamelog( "%s", szSQL );
		if ( mysql_ping( myGame ) != 0 )
			db_reconnect_game();
		return -1;
	}
	return 0;
}

/* 战报过期检查 */
int fight_msg_overdue()
{
	char szSQL[2048] = { 0 };
	// 删除过期的邮件
	sprintf( szSQL, "DELETE FROM fight_msg WHERE recvtime < %d", (int)time( NULL ) - MAX_FIGHTMSG_SAVETIME );
	if ( mysql_query( myGame, szSQL ) )
	{
		printf( "Query failed (%s) [%s](%d)\n", mysql_error( myGame ), __FUNCTION__, __LINE__ );
		write_gamelog( "%s", szSQL );
		if ( mysql_ping( myGame ) != 0 )
			db_reconnect_game();
		return -1;
	}
	return 0;
}
