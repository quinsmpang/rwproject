#ifdef WIN32
#include <winsock2.h>
#include <windows.h>
#endif
#include <mysql.h>
#include <stdio.h>
#include <string.h>
#include "db.h"
#include "define.h"
#include "plat.h"

extern SConfig g_Config;
extern MYSQL *myGame;
extern MYSQL *myData;

PlatInfo *g_PlatInfo = NULL;
int g_platnum = 0;

// 平台信息初始化
int plat_init()
{
	MYSQL_RES *res;
	MYSQL_ROW row;
	char szSQL[2048];
	int platid;

	// 获得表格中最大的id
	sprintf( szSQL, "select max(platid) from info_plat" );
	if ( mysql_query( myData, szSQL ) )
	{
		printf( "Query failed (%s) [%s](%d)\n", mysql_error( myData ), __FUNCTION__, __LINE__ );
		write_gamelog( "%s", szSQL );
		return -1;
	}
	res = mysql_store_result( myData );
	if ( (row = mysql_fetch_row( res )) )
	{
		if ( row[0] )
			g_platnum = atoi( row[0] ) + 1;
		else
			g_platnum = 0;
	}
	else
	{
		mysql_free_result( res );
		return -1;
	}
	mysql_free_result( res );

	// 分配空间
	g_PlatInfo = (PlatInfo *)malloc( sizeof(PlatInfo)*g_platnum );
	memset( g_PlatInfo, 0, sizeof(PlatInfo)*g_platnum );

	sprintf( szSQL, "select platid,userhost,userport,allow,allowpay from info_plat" );
	if ( mysql_query( myData, szSQL ) )
	{
		printf( "Query failed (%s) [%s](%d)\n", mysql_error( myData ), __FUNCTION__, __LINE__ );
		write_gamelog( "%s", szSQL );
		return -1;
	}

	res = mysql_store_result( myData );
	while ( (row = mysql_fetch_row( res )) )
	{
		platid = atoi( row[0] );
		if ( platid < 0 )
		{
			mysql_free_result( res );
			return -1;
		}
		else if ( platid >= g_platnum )
			continue;

		g_PlatInfo[platid].m_platid = platid;
		strcpy( g_PlatInfo[platid].m_userhost, row[1] );
		g_PlatInfo[platid].m_userport = atoi( row[2] );
		g_PlatInfo[platid].m_allow = atoi( row[3] );
		g_PlatInfo[platid].m_allowpay = atoi( row[4] );
	}
	mysql_free_result( res );
	return 0;
}

// 平台信息重读
void plat_reload()
{
	db_opendata();
	if ( g_PlatInfo != NULL )
	{
		free( g_PlatInfo );
		g_PlatInfo = NULL;
	}
	plat_init();
	db_closedata();
}


// ---------------------------------- 语言 ---------------------------------------
Localize *g_localize = NULL;
int g_localize_textcount = 0;

int localize_init()
{
	MYSQL_RES *res;
	MYSQL_ROW row;
	char szSQL[2048];
	mysql_query( myData, "set names utf8;" );
	// 获得表格中最大的id
	sprintf( szSQL, "select max(id) from localize" );
	if ( mysql_query( myData, szSQL ) )
	{
		printf( "Query failed (%s) [%s](%d)\n", mysql_error( myData ), __FUNCTION__, __LINE__ );
		write_gamelog( "%s", szSQL );
		return -1;
	}
	res = mysql_store_result( myData );
	if ( (row = mysql_fetch_row( res )) )
	{
		if ( row[0] )
			g_localize_textcount = atoi( row[0] ) + 1;
		else
			g_localize_textcount = 0;
	}
	else
	{
		mysql_free_result( res );
		return -1;
	}
	mysql_free_result( res );

	// 分配空间
	g_localize = (Localize *)malloc( sizeof(Localize)*g_localize_textcount );
	memset( g_localize, 0, sizeof(Localize)*g_localize_textcount );

	sprintf( szSQL, "select * from localize" );
	if ( mysql_query( myData, szSQL ) )
	{
		printf( "Query failed (%s) [%s](%d)\n", mysql_error( myData ), __FUNCTION__, __LINE__ );
		write_gamelog( "%s", szSQL );
		return -1;
	}

	res = mysql_store_result( myData );
	while ( (row = mysql_fetch_row( res )) )
	{
		int id = atoi( row[0] );
		if ( id < 0 )
		{
			mysql_free_result( res );
			return -1;
		}
		else if ( id >= g_localize_textcount )
			continue;

		for ( int tmpi = 0; tmpi < LANGUAGE_MAX; tmpi++ )
		{
			memcpy( g_localize[id].text[tmpi], row[tmpi + 1], sizeof(char)*LOCALIZE_TEXTMAXLEN );
		}
	}
	mysql_free_result( res );
	return 0;
}
char *localize_gettext( short language, int textid )
{
	if ( language < 0 || language >= LANGUAGE_MAX )
		return "";
	if ( textid <= 0 || textid >= g_localize_textcount )
		return "";
	return g_localize[textid].text[language];
}
