#ifdef WIN32
#include <winsock2.h>
#include <mswsock.h>
#endif
#include <stdio.h>
#include <string.h>

#include <mysql.h>
#include "netcmd_config.h"
#include "utils.h"

extern MYSQL *myData;

// 任务目标的分组信息
SNetcmdConfig *g_netccmd = NULL;
SNetcmdConfig *g_netscmd = NULL;
int g_netccmd_count = 0;
int g_netscmd_count = 0;

int netcmd_reset()
{
	if( g_netccmd )
		free( g_netccmd );
	g_netccmd = NULL;
	if( g_netscmd )
		free( g_netscmd );
	g_netscmd = NULL;

	netcmd_init();
	return 0;
}

int netcmd_init()
{
	MYSQL_RES		*res;
	MYSQL_ROW		row;
	char	szSQL[1024];

	int tmpi;

	g_netccmd_count = 0;

	sprintf( szSQL, "select count(*) from netccmd where unuse=0" );
	if( mysql_query( myData, szSQL ) )
	{
		printf( "Query failed (%s)\n", mysql_error(myData) );
		write_gamelog( "%s", szSQL );
		return -1;
	}
	res = mysql_store_result( myData );
	if( ( row = mysql_fetch_row( res ) ) )
	{
		if( row[0] )
			g_netccmd_count = atoi( row[0] );
		else
			g_netccmd_count = 0;
	}
	else
	{
		g_netccmd_count = 0;
		mysql_free_result( res );
		return -1;
	}
	mysql_free_result( res );

	g_netccmd = (SNetcmdConfig *)malloc( sizeof(SNetcmdConfig)*g_netccmd_count );
	memset( g_netccmd, 0, sizeof(SNetcmdConfig)*g_netccmd_count );

	// 确定每个目标组的个数
	sprintf( szSQL, "select cmdid,cmdstr,structid,target_type,context from netccmd where unuse=0" );
	if( mysql_query( myData, szSQL ) )
	{
		printf( "Query failed (%s)\n", mysql_error(myData) );
		write_gamelog( "%s", szSQL );
		return -1;
	}
	res = mysql_store_result( myData );
	tmpi = 0;
	while( ( row = mysql_fetch_row( res ) ) )
	{
		g_netccmd[tmpi].m_cmdid = atoi( row[0] );
		strncpy( g_netccmd[tmpi].m_cmdstr, row[1], 32 );
		g_netccmd[tmpi].m_structid = atoi( row[2] );
		g_netccmd[tmpi].m_target_type = atoi( row[3] );
#ifndef _NETPROCESSAUTO
		g_netccmd[tmpi].m_target_type = 0;
#endif
		strncpy( g_netccmd[tmpi].m_context, row[4], 128 );
		tmpi++;
	}
	mysql_free_result( res );
///////////////////////////////////////////////////////////////////
	g_netscmd_count = 0;

	sprintf( szSQL, "select count(*) from netscmd where unuse=0" );
	if( mysql_query( myData, szSQL ) )
	{
		printf( "Query failed (%s)\n", mysql_error(myData) );
		write_gamelog( "%s", szSQL );
		return -1;
	}
	res = mysql_store_result( myData );
	if( ( row = mysql_fetch_row( res ) ) )
	{
		if( row[0] )
			g_netscmd_count = atoi( row[0] );
		else
			g_netscmd_count = 0;
	}
	else
	{
		g_netscmd_count = 0;
		mysql_free_result( res );
		return -1;
	}
	mysql_free_result( res );

	g_netscmd = (SNetcmdConfig *)malloc( sizeof(SNetcmdConfig)*g_netscmd_count );
	memset( g_netscmd, 0, sizeof(SNetcmdConfig)*g_netscmd_count );

	// 确定每个目标组的个数
	sprintf( szSQL, "select cmdid,cmdstr,structid,target_type,context from netscmd where unuse=0" );
	if( mysql_query( myData, szSQL ) )
	{
		printf( "Query failed (%s)\n", mysql_error(myData) );
		write_gamelog( "%s", szSQL );
		return -1;
	}
	res = mysql_store_result( myData );
	tmpi = 0;
	while( ( row = mysql_fetch_row( res ) ) )
	{
		g_netscmd[tmpi].m_cmdid = atoi( row[0] );
		strncpy( g_netscmd[tmpi].m_cmdstr, row[1], 32 );
		g_netscmd[tmpi].m_structid = atoi( row[2] );
		g_netscmd[tmpi].m_target_type = atoi( row[3] );
//#ifndef _NETPROCESSAUTO
//		g_netscmd[tmpi].m_target_type = 0;
//#endif
		strncpy( g_netscmd[tmpi].m_context, row[4], 128 );
		tmpi++;
	}
	mysql_free_result( res );

	return 0;
}

int netcmd_create()
{
	return 0;
}
