#include <stdio.h>
#include <string.h>
#ifdef WIN32
#include <winsock2.h>
#include <windows.h>
#else
#include <strings.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <sys/resource.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#endif
#include <mysql.h>
#include <time.h>
#include "db.h"
#include "actor.h"
#include "gameproc.h"
#include "define.h"

extern int g_tick;

MYSQL *myData;
MYSQL *myGame;
MYSQL *myLog;

extern SConfig g_Config;

int db_init()
{
	if( (myData = mysql_init((MYSQL*) 0)) == NULL )
	{
		printf("Failed to init mysql\n");
		return -1;
	}
	if( !mysql_real_connect( myData,g_Config.datadb_host,g_Config.datadb_user,g_Config.datadb_pass,g_Config.datadb_database,0,NULL,0 ) )
	{
		printf("Failed to connect to data database: Error: %s\n", mysql_error(myData));
		return -1;
	}
	mysql_set_character_set( myData, "utf8" );

	if( (myGame = mysql_init((MYSQL*) 0)) == NULL )
	{
		printf("Failed to init mysql\n");
		return -1;
	}
	if( !mysql_real_connect( myGame,g_Config.gamedb_host,g_Config.gamedb_user,g_Config.gamedb_pass,g_Config.gamedb_database,0,NULL,0 ) )
	{
		printf("Failed to connect to game database: Error: %s\n", mysql_error(myGame));
		return -1;
	}
	mysql_set_character_set( myGame, "utf8" );

	if( (myLog = mysql_init((MYSQL*) 0)) == NULL )
	{
		printf("Failed to init mysql\n");
		return -1;
	}
	if( !mysql_real_connect( myLog,g_Config.logdb_host,g_Config.logdb_user,g_Config.logdb_pass,g_Config.logdb_database,0,NULL,0 ) )
	{
		printf("Failed to connect to log database: Error: %s\n", mysql_error(myLog));
		return -1;
	}
	mysql_set_character_set( myLog, "utf8" );

	return 0;
}

int db_reconnect_game()
{
	MYSQL myMem;
	memcpy( &myMem, myGame, sizeof(MYSQL) );
	myGame = mysql_init((MYSQL*) 0);
	if( !mysql_real_connect( myGame,g_Config.gamedb_host,g_Config.gamedb_user,g_Config.gamedb_pass,g_Config.gamedb_database,0,NULL,0 ) )
	{
		printf("Failed to connect to game database: Error: %s\n", mysql_error(myGame));
		memcpy( myGame, &myMem, sizeof(MYSQL) );
		return -1;
	}
	mysql_set_character_set( myGame, "utf8" );
	return 0;
}

int db_reconnect_log()
{
	MYSQL myMem;
	memcpy( &myMem, myLog, sizeof(MYSQL) );
	myLog = mysql_init((MYSQL*) 0);
	if( !mysql_real_connect( myLog,g_Config.logdb_host,g_Config.logdb_user,g_Config.logdb_pass,g_Config.logdb_database,0,NULL,0 ) )
	{
		printf("Failed to connect to log database: Error: %s\n", mysql_error(myLog));
		memcpy( myLog, &myMem, sizeof(MYSQL) );
		return -1;
	}
	mysql_set_character_set( myLog, "utf8" );
	return 0;
}
int db_opendata()
{
	if( (myData = mysql_init((MYSQL*) 0)) == NULL )
	{
		printf("Failed to init mysql\n");
		return -1;
	}
	if( !mysql_real_connect( myData,g_Config.datadb_host,g_Config.datadb_user,g_Config.datadb_pass,g_Config.datadb_database,0,NULL,0 ) )
	{
		printf("Failed to connect to data database: Error: %s\n", mysql_error(myData));
		return -1;
	}
	mysql_set_character_set( myData, "utf8" );
	return 0;
}

int db_closedata()
{
	mysql_close( myData ) ;
	return 0;
}

int db_closegame()
{
	mysql_close( myGame );
	mysql_close( myLog );
	return 0;
}

char *db_escape( const char *pStr, char *pBuf, int size )
{
//	mysql_real_escape_string( myData, pBuf, pStr, (unsigned long)(size<=0?strlen(pStr)+1:size) );
	mysql_real_escape_string( myGame, pBuf, pStr, (unsigned long)(size<=0?strlen(pStr):size) );
	return pBuf;
}

