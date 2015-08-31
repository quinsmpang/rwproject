#include <stdio.h>
#ifdef WIN32
#include <winsock2.h>
#include <mswsock.h>
#endif
#include <mysql.h>
#include "db.h"

#define DB_HOST	"192.168.16.201"
#define DB_USER	"cy"
#define DB_PASS	"cy"
#define DB_DATABASE	"nxdata"

MYSQL *myData;

int db_init()
{
	if( (myData = mysql_init((MYSQL*) 0)) == NULL )
	{
		printf("Failed to init mysql\n");
		return -1;
	}
	if( !mysql_real_connect( myData,DB_HOST,DB_USER,DB_PASS,DB_DATABASE,0,NULL,0 ) )
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
