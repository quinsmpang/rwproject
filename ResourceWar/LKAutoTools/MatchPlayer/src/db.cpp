#include <stdio.h>
#ifdef WIN32
#include <winsock2.h>
#include <mswsock.h>
#endif
#include <mysql.h>
#include "db.h"

MYSQL *myData;

int db_init( char *host, char *user, char *pass, char *database )
{
	if( (myData = mysql_init((MYSQL*) 0)) == NULL )
	{
		printf("Failed to init mysql\n");
		return -1;
	}
	if( !mysql_real_connect( myData,host,user,pass,database,0,NULL,0 ) )
	{
		printf("Failed to connect to data database: Error: %s\n", mysql_error(myData));
		return -1;
	}
	return 0;
}

int db_closedata()
{
	mysql_close( myData ) ;
	return 0;
}

char *db_escape( const char *pStr, char *pBuf, int size )
{
	mysql_real_escape_string( myData, pBuf, pStr, (unsigned long)(size<=0?strlen(pStr):size) );
	return pBuf;
}
