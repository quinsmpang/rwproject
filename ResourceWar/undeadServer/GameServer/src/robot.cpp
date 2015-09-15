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
#include "city.h"
#include "building.h"
#include "actor.h"
#include "robot.h"
#include "worldmap.h"
#include "mapunit.h"

extern SConfig g_Config;
extern MYSQL *myGame;
extern MYSQL *myData;

extern Global global;

extern Map g_map;

extern Actor *g_actors;
extern int g_maxactornum;

extern City *g_city;
extern int g_city_maxcount;
extern int g_city_maxindex;

// 创建机器人城池
int robot_city_create()
{
	// 读数据库
	MYSQL_RES *res;
	MYSQL_ROW row;
	char szSQL[2048];
	int count = 0;
	sprintf( szSQL, "select count(*) from city where laird_actorid<%d", MINACTORID );
	if ( mysql_query( myGame, szSQL ) )
	{
		printf( "Query failed (%s)\n", mysql_error( myGame ) );
		write_gamelog( "%s", szSQL );
		return -1;
	}
	res = mysql_store_result( myGame );
	if ( (row = mysql_fetch_row( res )) )
	{
		// 有机器人就返回
		count = atoi( row[0] );
		if ( count > 0 )
			return -1;
	}
	mysql_free_result( res );
	
	// 创建机器人城池
	for ( int tmpi = 0; tmpi < global.robot_city_count; tmpi++ )
	{
		/* 给这个玩家创建一个城池 */
		City city = { 0 };
		city.laird_type = CityLairdType_Robot;
		city.laird_actorid = tmpi+1;
		sprintf( city.laird_name, "robot%d", city.laird_actorid );
		city.laird_shape = 0;
		city.aclass = 0;
		city.posx = rand( ) % (g_map.m_nMaxWidth-8);
		city.posy = rand( ) % (g_map.m_nMaxHeight-8);
		int city_index = city_new( &city );
		mapunit_add( MAPUNIT_TYPE_CITY, city_index );
		printf( "robot city create cityid:%d laird_actorid:%d\n", g_city[city_index].cityid, city.laird_actorid );
	}
	return 0;
}
