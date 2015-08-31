#ifdef WIN32
#include <winsock2.h>
#include <windows.h>
#endif
#include <mysql.h>
#include <stdio.h>
#include <string.h>
#include "db.h"
#include "define.h"
#include "worldmap.h"
#include "city.h"
#include "army.h"
#include "actor.h"
#include "utils.h"
#include "system.h"
#include "actor_send.h"
#include "server_netsend_auto.h"
#include "mapunit.h"

extern SConfig g_Config;
extern MYSQL *myGame;
extern MYSQL *myData;

extern Map g_map;
extern Actor *g_actors;
extern int g_maxactornum;

extern City *g_city;
extern int g_city_maxcount;
extern int g_city_maxindex;

extern Army *g_Army;
extern int g_maxArmyNum;

MapUnit *g_mapunit = NULL;
int g_mapunit_maxcount = 0;

int mapunit_init()
{
	g_mapunit_maxcount = g_Config.max_citycount;
	g_mapunit = (MapUnit *)malloc( sizeof(MapUnit)*g_mapunit_maxcount );
	memset( g_mapunit, 0, sizeof(MapUnit)*g_mapunit_maxcount );
	printf( "MapUnit  maxcount=%d  memory=%0.2fMB\n", g_mapunit_maxcount, (sizeof(MapUnit)*g_mapunit_maxcount) / 1024.0 / 1024.0 );
	return 0;
}

// ��ȡ��ʾ��Ԫ��Ӧ��������Ϣ
int mapunit_getattr( int unit_index, SLK_NetS_AddMapUnit *pAttr )
{
	if ( unit_index < 0 || unit_index >= g_mapunit_maxcount )
		return -1;
	MapUnit *pMapUnit = &g_mapunit[unit_index];
	pAttr->m_unit_index = unit_index;
	pAttr->m_type = pMapUnit->type;
	switch ( pMapUnit->type )
	{
	case MAPUNIT_TYPE_CITY:
		{
			  if ( pMapUnit->index < 0 || pMapUnit->index >= g_city_maxcount )
				  break;
			  City *pCity = &g_city[pMapUnit->index];
			  pAttr->m_posx = pCity->posx;
			  pAttr->m_posy = pCity->posy;
			  pAttr->m_tposx = 0;
			  pAttr->m_tposy = 0;
			  pAttr->m_lastway = 0;
			  pAttr->m_shape = pCity->skin;
			  pAttr->m_level = pCity->laird_level;
			  pAttr->m_stat = 0;
			  pAttr->m_namelen = strlen( pCity->laird_name );
			  memcpy( pAttr->m_name, pCity->laird_name, sizeof(char)*NAME_SIZE );
			  pAttr->m_underfire_count = 0;
			  for ( int tmpi = 0; tmpi < CityUnderFire; tmpi++ )
			  { // �������ĵĲ����б�
				  if ( pCity->underfire[tmpi].army_index <= 0 )
					  continue;
				  pAttr->m_underfire_frompos[pAttr->m_underfire_count].m_unit_index = g_Army[pCity->underfire[tmpi].army_index].unit_index;
				  pAttr->m_underfire_frompos[pAttr->m_underfire_count].m_posx = g_Army[pCity->underfire[tmpi].army_index].from_posx;
				  pAttr->m_underfire_frompos[pAttr->m_underfire_count].m_posy = g_Army[pCity->underfire[tmpi].army_index].from_posy;
				  pAttr->m_underfire_count++;
			  }
			  break;
		}
	case MAPUNIT_TYPE_ARMY:
		{
			  if ( pMapUnit->index < 0 || pMapUnit->index >= g_maxArmyNum )
				  break;
			  Army *pArmy = &g_Army[pMapUnit->index];
			  pAttr->m_posx = pArmy->posx;
			  pAttr->m_posy = pArmy->posy;
			  pAttr->m_tposx = pArmy->target_posx;
			  pAttr->m_tposy = pArmy->target_posy;
			  //army_get_targetpos( pMapUnit->index, &pAttr->m_tposx, &pAttr->m_tposy );
			  pAttr->m_lastway = pArmy->lastway;
			  pAttr->m_shape = (unsigned char)pArmy->shape;
			  pAttr->m_level = 0;
			  pAttr->m_stat = pArmy->stat;
			  pAttr->m_namelen = strlen( pArmy->armyname );
			  memcpy( pAttr->m_name, pArmy->armyname, sizeof(char)*NAME_SIZE );
			  pAttr->m_underfire_count = 0;
			  break;
		}
	default:
		break;
	}
	return 0;
}

// ��ȡһ�����е�����
int mapunit_getfreeindex()
{
	for ( int tmpi = 0; tmpi < g_mapunit_maxcount; tmpi++ )
	{
		if ( g_mapunit[tmpi].type == 0 )
		{
			return tmpi;
		}
	}
	return -1;
}

// �������ͺͶ�Ӧ����ȷ����Ԫ����
int mapunit_getindex( char type, int index )
{
	for( int tmpi = 0; tmpi < g_mapunit_maxcount; tmpi++ )
	{
		if ( g_mapunit[tmpi].type == type && g_mapunit[tmpi].index == index )
		{
			return tmpi;
		}
	}
	return -1;
}

// ����Ҫ��ʾ�ĳǳػ������ӵ���ʾ��Ԫ
int mapunit_add( char type, int index )
{
	int unit_index = mapunit_getfreeindex();
	if ( unit_index < 0 )
		return -1;
	memset( &g_mapunit[unit_index], 0, sizeof(MapUnit) );
	g_mapunit[unit_index].type = type;
	g_mapunit[unit_index].index = index;
	g_mapunit[unit_index].lastadd_areaindex = -1;
	g_mapunit[unit_index].pre_index = -1;
	g_mapunit[unit_index].next_index = -1;
	SLK_NetS_AddMapUnit info = { 0 };
	mapunit_getattr( unit_index, &info );
	mapunit_enterworld( unit_index, info.m_posx, info.m_posy );
	return unit_index;
}

// ���Ѿ������ĵ�ͼ��Ԫɾ����
int mapunit_del( char type, int index, int area_index )
{
	int unit_index = mapunit_getindex( type, index );
	if ( unit_index < 0 )
		return -1;
	area_delmapunit( unit_index, area_index );
	memset( &g_mapunit[unit_index], 0, sizeof(MapUnit) );
	return 0;
}

// ��ʾ��Ԫ���������ͼ�������������У���֪ͨ����
int mapunit_enterworld( int unit_index, short posx, short posy )
{
	int area_index = area_getindex( posx, posy );
	if ( area_addmapunit( unit_index, area_index ) < 0 )
	{
		write_gamelog( "AreaAdd Error unit_index:%d", unit_index );
		return -1;
	}

	// ��֯���ݰ����͵����򻺳�
	char tmpbuf[2048] = {0};
	int tmpsize = 0;
	mapunit_enterinfo( unit_index, tmpbuf, &tmpsize );
	area_sendmsg( area_index, tmpsize, tmpbuf );
	return 0;
}

// ��֯һ����Ԫ����������������ݰ�
int mapunit_enterinfo( int unit_index, char *databuf, int *psize )
{
	char tmpbuf[2048];
	int allsize = 0;
	int sizeleft = 2048;
	int cursize = 0;
	SLK_NetS_AddMapUnit info = { 0 };
	mapunit_getattr( unit_index, &info );
	if ( (cursize = netsend_addmapunit_S( tmpbuf, sizeleft, &info )) == 0 )
		return -1;
	sizeleft -= cursize;
	allsize += cursize;

	if ( *psize + allsize > 2040 )
	{
		return -1;
	}
	memcpy( databuf + (*psize), tmpbuf, allsize );
	*psize += allsize;
	return 0;
}

// ��֯һ����Ԫ�����뿪��������ݰ�
int mapunit_leaveinfo( int unit_index, char *databuf, int *psize )
{
	char tmpbuf[2048];
	int allsize = 0;
	int sizeleft = 2048;
	int cursize = 0;
	if ( unit_index >= 0 && g_mapunit[unit_index].hide )
		return 0;
	SLK_NetS_DelMapUnit info = { 0 };
	info.m_unit_index = unit_index;
	if ( (cursize = netsend_delmapunit_S( tmpbuf, sizeleft, &info )) == 0 )
		return -1;
	sizeleft -= cursize;
	allsize += cursize;

	if ( *psize + allsize > 2040 )
	{
		return -1;
	}
	memcpy( databuf + (*psize), tmpbuf, allsize );
	*psize += allsize;

	return 0;
}


// ����һ����Ľ����¼�
int mapunit_enterarea( int unit_index, int area_index )
{
	char msg[2048] = {0};
	int size = 0;
	if ( unit_index < 0 || unit_index >= g_mapunit_maxcount )
		return -1;

	// ��֯���ݰ�
	mapunit_enterinfo( unit_index, msg + sizeof(short), &size );

	// ��Ϣ���͵�����
	if ( size > 0 )
	{
		*(unsigned short *)msg = size;
		area_send( area_index, size + sizeof(short), msg );
	}
	return 0;
}

// ����һ������뿪�¼�
int mapunit_leavearea( int unit_index, int area_index )
{
	char msg[2048] = {0};
	int size = 0;
	if ( unit_index < 0 || unit_index >= g_mapunit_maxcount )
		return -1;

	// ��֯���ݰ�
	mapunit_leaveinfo( unit_index, msg + sizeof(short), &size );

	// ��Ϣ���͵�����
	if ( size > 0 )
	{
		*(unsigned short *)msg = size;
		area_send( area_index, size + sizeof(short), msg );
	}

	return 0;
}


// ����������Ϣ
int mapunit_resetinfo( int unit_index, char *databuf, int *psize )
{
	char tmpbuf[2048];
	int tmpsize;
	int leftsize = 2048;
	SLK_NetS_AddMapUnit info = { 0 };
	mapunit_getattr( unit_index, &info );

	SLK_NetS_MapUnitCorrdinate Value = {};
	Value.m_unit_index = unit_index;
	Value.m_posx = info.m_posx;
	Value.m_posy = info.m_posy;
	tmpsize = netsend_mapunitcorrdinate_S( tmpbuf, leftsize, &Value );

	memcpy(databuf + *psize, tmpbuf, tmpsize);
	*psize += tmpsize;
	return 0;
}

// ����һ����ĸ�����Ϣ�¼�
int mapunit_resetarea( int unit_index, int area_index )
{
	char msg[2048] = {0};
	int size = 0;

	if ( unit_index < 0 || unit_index >= g_mapunit_maxcount )
		return -1;

	// ��֯���ݰ�
	mapunit_resetinfo( unit_index, msg + sizeof(short), &size );

	// ��Ϣ���͵�����
	if ( size > 0 )
	{
		*(unsigned short *)msg = size;
		area_send( area_index, size + sizeof(short), msg );
	}
	return 0;
}
