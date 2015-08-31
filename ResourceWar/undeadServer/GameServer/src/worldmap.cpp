#ifdef WIN32
#include <winsock2.h>
#include <windows.h>
#endif
#include <mysql.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "db.h"
#include "define.h"
#include "actor.h"
#include "hero.h"
#include "fight.h"
#include "server_netsend_auto.h"
#include "actor_send.h"
#include "worldmap.h"
#include "global.h"
#include "city.h"
#include "mapunit.h"

extern Global global;

extern SConfig g_Config;
extern MYSQL *myGame;
extern MYSQL *myData;

extern Actor *g_actors;
extern int g_maxactornum;

extern City *g_city;
extern int g_city_maxcount;
extern int g_city_maxindex;

Map g_map;

//-----------------------------------------------------------------------------
// worldmap_init
// 函数说明: 世界地图初始化
//-----------------------------------------------------------------------------
int worldmap_init()
{
	g_map.m_nMaxWidth = 120*2;
	g_map.m_nMaxHeight = 120*2;

	g_map.m_nAreaXNum = (g_map.m_nMaxWidth - 1) / AREA_WIDTH + 1;
	g_map.m_nAreaYNum = (g_map.m_nMaxHeight - 1) / AREA_HEIGHT + 1;

	g_map.m_aArea = (Area*)malloc(sizeof(Area)*(g_map.m_nAreaXNum*g_map.m_nAreaYNum));
	memset(g_map.m_aArea, 0, sizeof(Area)*(g_map.m_nAreaXNum*g_map.m_nAreaYNum));
	//printf( "Area  maxcount=%d  memory=%0.2fMB\n", g_map.m_nAreaXNum*g_map.m_nAreaYNum, (sizeof(Area)*(g_map.m_nAreaXNum*g_map.m_nAreaYNum)) / 1024.0 / 1024.0 );

	for (int tmpi = 0; tmpi < g_map.m_nAreaXNum*g_map.m_nAreaYNum; tmpi++)
	{
		g_map.m_aArea[tmpi].unit_head = -1;
		g_map.m_aArea[tmpi].unit_tail = -1;
	}

	return 0;
}

void worldmap_logic()
{
	for (int tmpi = 0; tmpi < g_map.m_nAreaXNum*g_map.m_nAreaYNum; tmpi++)
	{
		area_logic(tmpi);
	}
}

// 世界地图配置信息
void worldmap_sendinfo( int actor_index )
{
	SLK_NetS_WorldMapInfo info = { 0 };
	info.m_map_width = g_map.m_nMaxWidth;
	info.m_map_height = g_map.m_nMaxHeight;
	info.m_area_width = AREA_WIDTH;
	info.m_area_height = AREA_HEIGHT;
	info.m_map_area_xnum = g_map.m_nAreaXNum;
	info.m_map_area_ynum = g_map.m_nAreaYNum;
	info.m_my_city_unit_index = mapunit_getindex( MAPUNIT_TYPE_CITY, g_actors[actor_index].city_index );
	if ( g_actors[actor_index].city_index >= 0 && g_actors[actor_index].city_index < g_city_maxcount )
	{
		info.m_my_city_posx = g_city[g_actors[actor_index].city_index].posx;
		info.m_my_city_posy = g_city[g_actors[actor_index].city_index].posy;
	}
	netsend_worldmapinfo_S( actor_index, SENDTYPE_ACTOR, &info );
}

// 野外地图配置信息
void outsidemap_sendinfo( int actor_index )
{
	SLK_NetS_OutsideMapInfo info = { 0 };
	info.m_map_width = 10;
	info.m_map_height = 10;
	info.m_my_city_posx = info.m_map_width-2;
	info.m_my_city_posy = info.m_map_height-2;
	info.m_match_city_posx = 1;
	info.m_match_city_posy = 1;
	netsend_outsidemapinfo_S( actor_index, SENDTYPE_ACTOR, &info );
}
