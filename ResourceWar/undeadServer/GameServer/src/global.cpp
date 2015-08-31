#ifdef WIN32
#include <winsock2.h>
#include <windows.h>
#endif
#include <mysql.h>
#include <stdio.h>
#include <string.h>
#include "db.h"
#include "define.h"
#include "global.h"

extern MYSQL *myGame;
extern MYSQL *myData;

Global global;

int global_init()
{
	MYSQL_RES *res;
	MYSQL_ROW row;
	char szSQL[2048];
	sprintf( szSQL, "select id, value from global" );
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
		float value = (float)atof( row[1] );
		if ( id == 1 )
			global.damage_para1 = value;
		else if ( id == 2 )
			global.damage_para2 = value;
		else if ( id == 3 )
			global.damage_para3 = value;
		else if ( id == 4 )
			global.damage_para4 = value;
		else if ( id == 5 )
			global.dodge_para1 = value;
		else if ( id == 6 )
			global.dodge_para2 = value;
		else if ( id == 7 )
			global.dodge_para3 = value;
		else if ( id == 8 )
			global.dodge_para4 = value;
		else if ( id == 9 )
			global.crit_para1 = value;
		else if ( id == 10 )
			global.crit_para2 = value;
		else if ( id == 11 )
			global.crit_para3 = value;
		else if ( id == 12 )
			global.crit_para4 = value;
		else if ( id == 13 )
			global.robot_city_count = atoi( row[1] );
	}
	mysql_free_result( res );
	return 0;
}
