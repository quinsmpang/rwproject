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
#include "server_netsend_auto.h"
#include "actor_notify.h"

extern MYSQL *myGame;
// 一个场景中的角色列表
extern Actor *g_actors;
extern int g_maxactornum;

/* 发送邮件 */
int mail_send( char type, int from_actorid, char *from_name, int to_actorid, char *title, char *content, char *attach )
{
	MYSQL_RES *res;
	MYSQL_ROW row;
	char szSQL[4096] = {0};
	char szTitle[MAIL_TITLE_MAXSIZE * 2] = {0};
	char szContent[MAIL_CONTENT_MAXSIZE * 2] = {0};
	char szAttach[MAIL_ATTACH_MAXSIZE * 2] = {0};
	int thistime = (int)time( NULL );
	int deltime = 0;
	int has_attach = 0;
	if ( attach == NULL || attach[0] == 0 )
	{ // 没有附件的
		has_attach = 0;
		deltime = thistime + MAX_MAIL_SAVETIME;
	}
	else
	{ // 有附件的
		has_attach = 1;
		deltime = thistime + MAX_MAIL_SAVETIME * 2;
	}

	if ( title )
		db_escape( title, szTitle, 0 );
	if ( content )
		db_escape( content, szContent, 0 );
	if ( attach )
		db_escape( attach, szAttach, 0 );

	// 插入数据库
	sprintf( szSQL, "INSERT INTO mail ( type, from_actorid, from_name, to_actorid, title, content, attach, recvtime, deltime ) VALUES ('%d','%d','%s','%d','%s,'%s,'%s','%d','%d')",
		type, from_actorid, from_name, to_actorid, szTitle, szContent, szAttach, thistime, deltime );
	if ( mysql_query( myGame, szSQL ) )
	{
		printf( "Query failed (%s)\n", mysql_error(myGame) );
		write_gamelog( "%s", szSQL );
		if( mysql_ping(myGame) != 0 )
			db_reconnect_game();
		return -1;
	}

	// 如收件人在线，通知有新的消息
	int to_actorindex = actor_getindex_withid( to_actorid );
	if ( to_actorindex >= 0 )
	{
		sprintf( szSQL, "SELECT LAST_INSERT_ID()" );
		if( mysql_query( myGame, szSQL ) )
		{
			printf( "Query failed (%s)\n", mysql_error(myGame) );
			write_gamelog( "%s", szSQL );
			return -1;
		}
		res = mysql_store_result( myGame );
		if( ( row = mysql_fetch_row( res ) ) )
		{
			SLK_NetS_MailSimpleInfo info = {0};
			info.m_mailid = atoll( row[0] );
			info.m_type = type;

			info.m_from_name_length = strlen( from_name );
			memcpy( info.m_from_name, from_name, sizeof(char)*info.m_from_name_length );

			info.m_title_length = strlen( title );
			memcpy( info.m_title, title, sizeof(char)*info.m_title_length );

			/*info.m_simple_content_length = strlen( content );
			if ( info.m_simple_content_length >= 32 )
			{
				memcpy( info.m_simple_content, content, sizeof(char)*24 );
				strcat( info.m_simple_content, "......" );
				info.m_simple_content_length = strlen( info.m_simple_content );
			}*/

			info.m_read = 0;
			info.m_attach_isget = 0;
			info.m_recvtime = thistime;
			info.m_has_attach = has_attach;
			info.m_deltime = deltime;
			netsend_mailsimpleinfo_S( to_actorindex, SENDTYPE_ACTOR, &info );
		}
		mysql_free_result( res );
	}
	return 0;
}

/* 检查是否有未读邮件 */
int mail_check_read( int actor_index )
{
	MYSQL_RES	*res;
	MYSQL_ROW	row;
	char	szSQL[1024];
	if ( actor_index < 0 || actor_index >= g_maxactornum )
		return -1;
	sprintf( szSQL, "select count(*) from mail where `to_actorid`='%d' and `read`=0 and `deltime`>'%d' ", g_actors[actor_index].actorid, (int)time( NULL ) );
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
			value[0] = 1;
			value[1] = count;
			actor_notify_value( actor_index, NOTIFY_MAIL, 2, value, NULL );
		}
	}
	mysql_free_result( res );
	return 0;
}

/* 读取邮件列表 */
int mail_loadlist( int actor_index, i64 maxmailid )
{
	if ( actor_index < 0 || actor_index >= g_maxactornum )
		return -1;
	MYSQL_RES	*res;
	MYSQL_ROW	row;
	char	szSQL[1024];
	SLK_NetS_MailList info = { 0 };
	char bigint[21];
	lltoa( maxmailid, bigint, 10 );
	sprintf( szSQL, "SELECT * FROM mail WHERE id>'%s' AND to_actorid='%d' AND deltime>'%d' GROUP BY `type` ", bigint, g_actors[actor_index].actorid, (int)time( NULL ) );
	if( mysql_query( myGame, szSQL ) )
	{
		printf( "Query failed (%s)\n", mysql_error(myGame) );
		write_gamelog( "%s", szSQL );
		if( mysql_ping(myGame) != 0 )
			db_reconnect_game();
		return -1;
	}
	res = mysql_store_result( myGame );
	while( ( row = mysql_fetch_row( res ) ) )
	{
		// 邮件简单信息列表
		info.m_list[info.m_count].m_mailid = (unsigned int)atoi( row[0] );
		info.m_list[info.m_count].m_type = atoi( row[1] );

		info.m_list[info.m_count].m_from_name_length = strlen( row[3] );
		memcpy( info.m_list[info.m_count].m_from_name, row[3], sizeof(char)*info.m_list[info.m_count].m_from_name_length );

		info.m_list[info.m_count].m_title_length = strlen( row[5] );
		memcpy( info.m_list[info.m_count].m_title, row[5], sizeof(char)*info.m_list[info.m_count].m_title_length );

		info.m_list[info.m_count].m_simple_content_length = strlen( row[6] );
		if ( info.m_list[info.m_count].m_simple_content_length >= 32 )
		{
			memcpy( info.m_list[info.m_count].m_simple_content, row[6], sizeof(char)* 24 );
			strcat( info.m_list[info.m_count].m_simple_content, "......" );
			info.m_list[info.m_count].m_simple_content_length = strlen( info.m_list[info.m_count].m_simple_content );
		}
		if ( strlen( row[7] ) > 1 )
			info.m_list[info.m_count].m_has_attach = 1;
		else
			info.m_list[info.m_count].m_has_attach = 0;
		info.m_list[info.m_count].m_attach_isget = (char)atoi( row[8] );
		info.m_list[info.m_count].m_recvtime = (int)atoi( row[9] );
		info.m_list[info.m_count].m_deltime = (int)atoi( row[10] );
		info.m_list[info.m_count].m_read = (char)atoi( row[11] );
		info.m_count++;
		if ( info.m_count >= 10 )
			break;
	}
	mysql_free_result( res );
	netsend_maillist_S( actor_index, SENDTYPE_ACTOR, &info );
	return 0;
}

/* 读取详细内容 */
int mail_read_content( int actor_index, i64 mailid )
{
	if ( actor_index < 0 || actor_index >= g_maxactornum )
		return -1;
	MYSQL_RES	*res;
	MYSQL_ROW	row;
	char	szSQL[1024];
	char bigint[21];
	lltoa( mailid, bigint, 10 );
	sprintf( szSQL, "select content, attach from mail where id='%s'", bigint );
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
		SLK_NetS_MailFullInfo info = { 0 };
		info.m_mailid = mailid;

		info.m_content_length = strlen( row[0] );
		memcpy( info.m_content, row[0], sizeof(char)*info.m_content_length );

		info.m_attach_length = strlen( row[1] );
		memcpy( info.m_attach, row[1], sizeof(char)*info.m_attach_length );

		info.m_read = 1;

		netsend_mailfullinfo_S( actor_index, SENDTYPE_ACTOR, &info );
	}
	mysql_free_result( res );

	// 设置已读状态
	sprintf( szSQL, "update mail set read='%d' where id='%s'", 1, bigint );
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

/* 领取附件 */
int mail_read_attach( int actor_index, i64 mailid )
{
	if ( actor_index < 0 || actor_index >= g_maxactornum )
		return -1;
	MYSQL_RES	*res;
	MYSQL_ROW	row;
	char	szSQL[1024];
	char attach[MAIL_ATTACH_MAXSIZE] = { 0 };
	char bigint[21];
	lltoa( mailid, bigint, 10 );
	sprintf( szSQL, "select attach from mail where id='%s'", bigint );
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
		memcpy( attach, row[0], MAIL_ATTACH_MAXSIZE );

	}
	mysql_free_result( res );

	// 设置已领取附件状态
	sprintf( szSQL, "update mail set attach_isget='%d' where id='%s'", 1, bigint );
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

/* 删除 */
int mail_delete( int actor_index, i64 mailid )
{
	char szSQL[1024];
	char bigint[21];
	lltoa( mailid, bigint, 10 );
	sprintf( szSQL, "DELETE FROM `mail` WHERE id='%s'", bigint );
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

/* 邮件过期检查 */
int mail_overdue()
{
	char szSQL[2048] = { 0 };
	int thistime = (int)time( NULL );

	// 删除过期的邮件
	sprintf( szSQL, "DELETE FROM `mail` WHERE deltime < %d", thistime );
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

/* 邮件过期检查每天早上2点检查 */
void mail_overdue_everyday()
{
	//time_t t;
	//struct tm *newtime;
	//time( &t );
	//newtime = localtime( &t );
	//if ( newtime->tm_hour == 4 )
	//{
		mail_overdue();
	/*}*/
}
