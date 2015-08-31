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
#include "city.h"
#include "building.h"
#include "army.h"
#include "area.h"
#include "actor.h"
#include "robot.h"
#include "global.h"
#include "mapunit.h"
#include "actor_send.h"
#include "actor_notify.h"
#include "server_netsend_auto.h"

extern SConfig g_Config;
extern MYSQL *myGame;
extern MYSQL *myData;

extern MapUnit *g_mapunit;
extern int g_mapunit_maxcount;

extern Actor *g_actors;
extern int g_maxactornum;

extern Army *g_Army;
extern int g_maxArmyNum;

extern BuildingUpgrade *g_BuildingUpgrade;
extern int g_BuildingUpgradeCount;

extern BuildingLimit *g_BuildingLimit;
extern char g_BuildingLimitCount;

extern BuildingNew *g_BuildingNew;
extern int g_BuildingNewCount;

City *g_city = NULL;
int g_city_maxcount = 0;
int g_city_maxindex = -1;

char city_allinited = 0;
// ������������ȡ���гǳ���Ϣ���ڴ�
int city_load()
{
	// ��ǰ����ÿռ�
	city_allinited = 0;
	g_city_maxcount = g_Config.max_citycount;
	g_city = (City*)malloc( sizeof(City)* g_city_maxcount );
	memset( g_city, 0, sizeof(City)* g_city_maxcount );
	printf( "City  maxcount=%d  memory=%0.2fMB\n", g_city_maxcount, (sizeof(City)* g_city_maxcount) / 1024.0 / 1024.0 );

	// �����ݿ�
	MYSQL_RES *res;
	MYSQL_ROW row;
	char szSQL[2048];
	char reconnect_flag = 0;
	int city_index = 0;
RE_CITY_LOAD:
	sprintf( szSQL, "select * from city" );
	if ( mysql_query( myGame, szSQL ) )
	{
		printf( "Query failed (%s)\n", mysql_error( myGame ) );
		write_gamelog( "%s", szSQL );
		if ( reconnect_flag )
			return -1;
		if ( mysql_ping( myGame ) != 0 )
		{
			db_reconnect_game();
			reconnect_flag = 1;
			goto RE_CITY_LOAD;
		}
		return -1;
	}
	res = mysql_store_result( myGame );
	while ( (row = mysql_fetch_row( res )) )
	{
		int index = 0;
		if ( city_index >= g_city_maxcount )
		{
			printf( "warning!!!warning!!!  city_index >= g_city_maxcount \n" );
			return -1;
		}
		g_city[city_index].cityid = atoi( row[index++] );
		g_city[city_index].laird_type = atoi( row[index++] );
		g_city[city_index].laird_actorid = atoi( row[index++] );
		memcpy( g_city[city_index].laird_name, row[index++], sizeof(char)*NAME_SIZE );
		g_city[city_index].laird_shape = atoi( row[index++] );
		g_city[city_index].laird_level = atoi( row[index++] );
		g_city[city_index].laird_lastlogin = atoi( row[index++] );
		g_city[city_index].match_cityid = atoi( row[index++] );
		g_city[city_index].aclass = atoi( row[index++] );
		g_city[city_index].skin = atoi( row[index++] );
		g_city[city_index].clubid = atoi( row[index++] );
		g_city[city_index].posx = atoi( row[index++] );
		g_city[city_index].posy = atoi( row[index++] );
		g_city[city_index].wood = atoi( row[index++] );
		g_city[city_index].food = atoi( row[index++] );
		g_city[city_index].iron = atoi( row[index++] );
		g_city[city_index].mithril = atoi( row[index++] );
		g_city[city_index].gold = atoi( row[index++] );
		for ( int tmpi = 0; tmpi < CityCorpsMax; tmpi++ )
		{
			memcpy( g_city[city_index].corps_num[tmpi], row[index], sizeof(int)*CityCorpsLevelMax );
			memcpy(g_city[city_index].corps_wound[tmpi], row[CityCorpsMax + index], sizeof(int)*CityCorpsLevelMax);
			index++;
		}

		g_city[city_index].actor_index = -1;
		// ��ȡ����
		building_load( &g_city[city_index] );
		// ���¼�����ʱ����
		city_reset( &g_city[city_index] );
		// ��ӵ���ͼ��ʾ��Ԫ
		if ( g_city[city_index].laird_type != CityLairdType_Match )
		{
			mapunit_add( MAPUNIT_TYPE_CITY, city_index );
		}
		city_index++;
		g_city_maxindex = city_index;
	}
	mysql_free_result( res );
	// ���������˳ǳ�
	robot_city_create();
	// ���гǳس�ʼ�����
	city_allinited = 1;
	return 0;
}

// �������رգ����гǳ���Ϣ�浽���ݿ�
int city_save( FILE *fp )
{
	for ( int city_index = 0; city_index < g_city_maxcount; city_index++ )
	{
		printf( "city_save %d/%d\r", city_index + 1, g_city_maxcount );
		if ( g_city[city_index].cityid <= 0 )
			continue;
		city_single_save( &g_city[city_index], fp );
	}
	return 0;
}
int city_single_save( City *pCity, FILE *fp )
{
	if ( pCity == NULL )
		return -1;
	char szSQL[2048];
	char reconnect_flag = 0;
	char szText_corps_num[CityCorpsMax][4 * 80] = { 0 };
	char szText_corps_wound[CityCorpsMax][4 * 80] = { 0 };
	for ( int tmpi = 0; tmpi < CityCorpsMax; tmpi++ )
	{
		db_escape((const char *)pCity->corps_num[tmpi], szText_corps_num[tmpi], sizeof(int)*CityCorpsLevelMax);
		db_escape((const char *)pCity->corps_wound[tmpi], szText_corps_wound[tmpi], sizeof(int)*CityCorpsLevelMax);
	}
RE_CITY_REPLACE:
	sprintf( szSQL, "REPLACE INTO city (`cityid`,`laird_type`,`laird_actorid`,`laird_name`,`laird_shape`,`laird_level`,`laird_lastlogin`,`match_cityid`,`aclass`,`skin`,`clubid`,`posx`,`posy`,`wood`,`food`,`iron`,`mithril`,`gold`, "
		"`corps_num1`, `corps_num2`, `corps_num3`, `corps_num4`, `corps_num5`, "
		"`corps_wound1`, `corps_wound2`, `corps_wound3`, `corps_wound4`, `corps_wound5` ) "
		"Values('%d','%d','%d','%s','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d',"
		"'%s','%s','%s','%s','%s',"
		"'%s','%s','%s','%s','%s')",
		pCity->cityid, pCity->laird_type, pCity->laird_actorid, pCity->laird_name, pCity->laird_shape, pCity->laird_level, pCity->laird_lastlogin, pCity->match_cityid, pCity->aclass, pCity->skin,
		pCity->clubid, pCity->posx, pCity->posy, pCity->wood, pCity->food, pCity->iron, pCity->mithril, pCity->gold,
		szText_corps_num[0], szText_corps_num[1], szText_corps_num[2], szText_corps_num[3], szText_corps_num[4],
		szText_corps_wound[0], szText_corps_wound[1], szText_corps_wound[2], szText_corps_wound[3], szText_corps_wound[4] );
	if ( fp )
	{
		fprintf( fp, "%s;\n", szSQL );
	}
	else if ( mysql_query( myGame, szSQL ) )
	{
		printf( "Query failed (%s)\n", mysql_error( myGame ) );
		write_gamelog( "%s", szSQL );
		if ( reconnect_flag )
			return -1;
		if ( mysql_ping( myGame ) != 0 )
		{
			db_reconnect_game();
			reconnect_flag = 1;
			goto RE_CITY_REPLACE;
		}
		return -1;
	}
	// ���潨��
	building_save( pCity, fp );
	return 0;
}

// ����һ��������Ϣ�������ز����id
int city_insert( City *pCity )
{
	if ( pCity == NULL )
		return -1;
	char szSQL[2048];
	char reconnect_flag = 0;
RE_CITY_INSERT:
	sprintf( szSQL, "INSERT INTO `city` SET `laird_type`='%d',`laird_actorid`='%d',`laird_name`='%s',`laird_shape`='%d',`aclass`='%d',`posx`='%d',`posy`='%d' ",
		pCity->laird_type, pCity->laird_actorid, pCity->laird_name, pCity->laird_shape, pCity->aclass, pCity->posx, pCity->posy );
	if ( mysql_query( myGame, szSQL ) )
	{
		printf( "Query failed (%s)\n", mysql_error( myGame ) );
		write_gamelog( "%s", szSQL );
		if ( reconnect_flag )
			return -1;
		if ( mysql_ping( myGame ) != 0 )
		{
			db_reconnect_game();
			reconnect_flag = 1;
			goto RE_CITY_INSERT;
		}
		return -1;
	}
	return (int)mysql_insert_id( myGame );
}

// ���г���ÿ����߼� , һ���Ǽ�齨���Ľ�����������������
void city_logic_sec()
{
	if ( !city_allinited )
		return;
	//DWORD b = timeGetTime();
	for ( int tmpi = 0; tmpi < g_city_maxindex/*ע�⣺ʹ������λ�ã�Ϊ��Ч��*/; tmpi++ )
	{
		if ( g_city[tmpi].cityid <= 0 )
			continue;
		// Ϊ�˱���Ч�ʣ�ֱ����ָ��Ѱַ�����ٱ�������
		for ( short handleindex = 0; handleindex < CityHandleQueueMax; handleindex++ )
		{
			short buildingindex = g_city[tmpi].handle_queue[handleindex];
			if ( buildingindex >= 0 )
			{
				if ( building_checktime( &g_city[tmpi], buildingindex ) )
				{ // ʱ�䵽�����
					building_finish( &g_city[tmpi], buildingindex );
				}
			}
		}

		// �������ܣ���������Ƽ�����
		for ( int buildingindex = 0; buildingindex < CityBuildingMax; buildingindex++ )
		{
			Building *pBuilding = &g_city[tmpi].building[buildingindex];
			switch ( pBuilding->buildingkind )
			{
			case BUILDING_Barracks:			// ��Ӫ
			case BUILDING_Stable:			// ���
			case BUILDING_ShootingRange:	// �г�
			case BUILDING_Tower:			// ��¥
			case BUILDING_Magician:			// ��ʦӪ��
				if ( building_troop_checktime( &g_city[tmpi], buildingindex ) )
				{ // ʱ�䵽�����
					building_troop_ontrainfinish( g_city[tmpi].actor_index, buildingindex );
				}
				break;
			default:
				break;
			}
		}
	}
	//DWORD e = timeGetTime();
	//printf( "sec logic:%d\n", e - b );
}

// ���г���ÿ���ӵ��߼�,��ҪΪ��������Դ
void city_logic_min()
{
	//DWORD b = timeGetTime();
	for ( int tmpi = 0; tmpi < g_city_maxindex/*ע�⣺ʹ������λ�ã�Ϊ��Ч��*/; tmpi++ )
	{
		if ( g_city[tmpi].cityid <= 0 )
			continue;
		City *pCity = &g_city[tmpi];
		// �������н���
		for ( int buildingindex = 0; buildingindex < CityBuildingMax; buildingindex++ )
		{
			Building *pBuilding = &pCity->building[buildingindex];
			if ( pBuilding->buildingkind <= 0 || pBuilding->buildingkind >= g_BuildingUpgradeCount )
				continue;
			if ( pBuilding->begintime > 0 )
				continue;
			switch ( pBuilding->buildingkind )
			{
			case BUILDING_WoodFactory:	//	ľ�ĳ�/������
			case BUILDING_Farm:			//	ũ��/����
			case BUILDING_IronFactory:	//	����/������
			case BUILDING_Mithril:		//	����Ѩ/���
			{
				BuildingUpgradeConfig *pBuildingConfig = &g_BuildingUpgrade[pBuilding->buildingkind].info[pBuilding->level];
				if ( pBuildingConfig == NULL )
					break;
				pBuilding->value[0] += pBuildingConfig->value[0];
				if ( pBuilding->value[0] > pBuildingConfig->value[1] )
					pBuilding->value[0] = pBuildingConfig->value[1];
			}
				break;
			default:
				break;
			}
		}

		// ����Ҫ����ʳ
		for ( int corps = 0; corps < CityCorpsMax; corps++ )
		{
			for ( int level = 0; level < CityCorpsLevelMax; level++ )
			{
				//pCity->corps_num[corps][level] += 1;	// ������5�ֱ�ÿ��10��
			}
		}
	}
	//DWORD e = timeGetTime();
	//printf( "min logic:%d\n", e - b );
}

// ÿ�춨ʱ�ݻ�һЩ�ǳ�
void city_logic_destroy()
{
	for ( int tmpi = 0; tmpi < g_city_maxindex/*ע�⣺ʹ������λ�ã�Ϊ��Ч��*/; tmpi++ )
	{
		if ( g_city[tmpi].cityid <= 0 )
			continue;
		if ( g_city[tmpi].actor_index >= 0 )
			continue;
		if ( g_city[tmpi].laird_actorid < MINACTORID )
			continue;
		if ( g_city[tmpi].laird_level <= 15 && g_city[tmpi].laird_type == CityLairdType_Player )
		{
			if ( (int)time( NULL ) - g_city[tmpi].laird_lastlogin > 86400 * 10 )
			{
				// ɾ�������Ķ��ֳ���
				if ( g_city[tmpi].match_cityid > 0 )
				{
					g_city[tmpi].match_cityindex = city_getindex_withcityid( g_city[tmpi].match_cityid );
					if ( g_city[tmpi].match_cityindex >= 0 && g_city[tmpi].match_cityindex < g_city_maxcount )
						city_del( &g_city[g_city[tmpi].match_cityindex], g_city[tmpi].match_cityindex );
				}
				// ɾ�����ݿ���ڴ�
				city_del( &g_city[tmpi], tmpi );
			}
		}
	}
}

// �������id�ҵ��ǳ�����
int city_getindex_withactorid( int actorid )
{
	int city_index = -1;
	for ( int tmpi = 0; tmpi < g_city_maxindex/*ע�⣺ʹ������λ�ã�Ϊ��Ч��*/; tmpi++ )
	{
		if ( g_city[tmpi].laird_actorid == actorid && g_city[tmpi].laird_type == CityLairdType_Player )
		{
			city_index = tmpi;
			break;
		}
	}
	return city_index;
}

// ���ݳǳ�id�ҵ��ǳ�����
int city_getindex_withcityid( int cityid )
{
	int city_index = -1;
	for ( int tmpi = 0; tmpi < g_city_maxindex/*ע�⣺ʹ������λ�ã�Ϊ��Ч��*/; tmpi++ )
	{
		if ( g_city[tmpi].cityid == cityid )
		{
			city_index = tmpi;
			break;
		}
	}
	return city_index;
}

// ����һ���³���
int city_new( City *pCity )
{
	if ( pCity == NULL )
		return -1;
	int city_index = -1;
	for ( city_index = 0; city_index < g_city_maxcount; city_index++ )
	{
		if ( g_city[city_index].cityid <= 0 )
		{
			memcpy( &g_city[city_index], pCity, sizeof(City) );
			g_city[city_index].cityid = city_insert( &g_city[city_index] );
			g_city[city_index].actor_index = -1;
			city_handle_queue_clear( &g_city[city_index] );
			break;
		}
	}

	// û�����ɹ�
	if ( city_index >= g_city_maxcount )
		return -1;

	// Ϊ�˱�����Ч�ʣ�������������
	if ( city_index >= g_city_maxindex )
	{
		g_city_maxindex = city_index + 1;
	}

	// ��������
	for ( int tmpi = 0; tmpi < g_BuildingNewCount; tmpi++ )
	{
		if ( g_BuildingNew[tmpi].buildingkind <= 0 )
			continue;
		int buildingindex = building_create( &g_city[city_index], g_BuildingNew[tmpi].buildingindex, g_BuildingNew[tmpi].buildingkind );
		if ( buildingindex >= 0 && buildingindex < CityBuildingMax )
		{ // ���ղŴ����õĽ����������
			building_finish( &g_city[city_index], buildingindex );
		}

	}
	city_reset( &g_city[city_index] );
	return city_index;
}

// ɾ��һ���ǳ�
void city_del( City *pCity, int city_index )
{
	if ( pCity == NULL )
		return;
	// ���ݿ�ɾ��
	char szSQL[2048];
	char reconnect_flag = 0;
RE_CITY_DEL:
	sprintf( szSQL, "DELETE FROM `city` WHERE `cityid`=%d;" );
	if ( mysql_query( myGame, szSQL ) )
	{
		printf( "Query failed (%s)\n", mysql_error( myGame ) );
		write_gamelog( "%s", szSQL );
		if ( reconnect_flag )
			return;
		if ( mysql_ping( myGame ) != 0 )
		{
			db_reconnect_game();
			reconnect_flag = 1;
			goto RE_CITY_DEL;
		}
		return;
	}

	sprintf( szSQL, "DELETE FROM `city_building` WHERE `cityid`=%d" );
	if ( mysql_query( myGame, szSQL ) )
	{
		printf( "Query failed (%s)\n", mysql_error( myGame ) );
		write_gamelog( "%s", szSQL );
	}
	// ����ʾ��Ԫ�������
	if ( city_index >= 0 )
	{
		mapunit_del( MAPUNIT_TYPE_CITY, city_index, area_getindex( pCity->posx, pCity->posy ) );
	}
	// �ڴ���ɾ��
	memset( pCity, 0, sizeof(City) );
	pCity->actor_index = -1;
	pCity->match_cityindex = -1;
}

// ���¼��������ʱ����
void city_reset( City *pCity )
{
	if ( pCity == NULL )
		return;
	// �������ʱ����

	// ��ղ�������
	city_handle_queue_clear( pCity );

	// �������н���
	for ( int tmpi = 0; tmpi < CityBuildingMax; tmpi++ )
	{
		Building *pBuilding = &pCity->building[tmpi];
		if ( pBuilding->buildingkind <= 0 || pBuilding->buildingkind >= g_BuildingUpgradeCount )
			continue;
		if ( pBuilding->level > g_BuildingUpgrade[pBuilding->buildingkind].maxlevel )
			pBuilding->level = g_BuildingUpgrade[pBuilding->buildingkind].maxlevel;
		BuildingUpgradeConfig *pBuildingConfig = &g_BuildingUpgrade[pBuilding->buildingkind].info[pBuilding->level];
		if ( pBuildingConfig == NULL )
			continue;
		if ( pBuilding->begintime > 0 && ( pBuilding->state == BUILDING_STATE_CREATE_ING || pBuilding->state == BUILDING_STATE_UPGRADE_ING ) )
		{
			// ��ӵ���������
			city_handle_queue_add( pCity, tmpi );
		}

		// �������������Ҫ����ʱ����
		switch ( pBuilding->buildingkind )
		{
		case BUILDING_TownCenter:
			break;
		default:
			break;
		}
	}
}

// ��ȡ�����ҵĳǳ�����
City *city_getptr( int actor_index )
{
	if ( actor_index < 0 || actor_index >= g_maxactornum )
		return NULL;
	int city_index = g_actors[actor_index].city_index;
	if ( city_index < 0 || city_index >= g_city_maxcount )
		return NULL;
	return &g_city[city_index];
}

// ��ȡ�����ҳ������ֵĳǳ�����
City *city_match_getptr( int actor_index )
{
	if ( actor_index < 0 || actor_index >= g_maxactornum )
		return NULL;

	int city_index = g_actors[actor_index].city_index;
	if ( city_index < 0 || city_index >= g_city_maxcount )
		return NULL;
	if ( g_city[city_index].match_cityid <= 0 )
		return NULL;
	int match_cityindex = g_city[city_index].match_cityindex;
	if ( match_cityindex < 0 || match_cityindex >= g_city_maxcount )
	{
		match_cityindex = city_getindex_withcityid( g_city[city_index].match_cityid );
		g_city[city_index].match_cityindex = match_cityindex;
	}
	if ( match_cityindex < 0 || match_cityindex >= g_city_maxcount )
		return NULL;

	return &g_city[match_cityindex];
}

// սʤ���ֳǳ�
void city_beat_match_city( int city_index )
{
	if ( city_index < 0 || city_index >= g_city_maxcount )
		return;
	City *pCity = &g_city[city_index];
	int match_cityindex = pCity->match_cityindex;
	if ( match_cityindex < 0 || match_cityindex >= g_city_maxcount )
	{
		match_cityindex = city_getindex_withcityid( pCity->match_cityid );
		pCity->match_cityindex = match_cityindex;
	}
	if ( match_cityindex < 0 || match_cityindex >= g_city_maxcount )
		return;
	city_del( &g_city[pCity->match_cityindex], pCity->match_cityindex );
	pCity->match_cityindex = -1;
	pCity->match_cityid = 0;
	mapunit_add( MAPUNIT_TYPE_CITY, city_index );
}

// ������Դ�ı�
void city_change_res( City *pCity, CityRes res, int value, char path )
{
	if ( pCity == NULL )
		return;
	int *num = NULL;
	switch ( res )
	{
	case CityRes_Wood:
		num = &pCity->wood;
		break;
	case CityRes_Food:
		num = &pCity->food;
		break;
	case CityRes_Iron:
		num = &pCity->iron;
		break;
	case CityRes_Mithril:
		num = &pCity->mithril;
		break;
	case CityRes_Gold:
		num = &pCity->gold;
		break;
	default:
		break;
	}
	*num += value;
	if ( *num < 0 )
		*num = 0;
	else if ( *num > 0x7fffffff )
		*num = 0x7fffffff;
	wlog( 0, LOGOP_CITYRES, path, res, value, *num, pCity->laird_actorid, pCity->cityid );
	city_res_changeinfo( pCity->actor_index, res, *num, path );
}

// ���еı������б�����
static int underfire_freeindex( City *pCity )
{
	if ( pCity == NULL )
		return -1;
	for ( int tmpi = 0; tmpi < CityUnderFire; tmpi++ )
	{
		if ( pCity->underfire[tmpi].army_index <= 0 )
			return tmpi;
	}
	return -1;
}
// �ѱ��趨Ϊ����Ŀ��
void city_underfire( City *pCity, int army_index )
{
	if ( pCity == NULL )
		return;
	if ( army_index < 0 || army_index >= g_maxArmyNum )
		return;
	int index = underfire_freeindex( pCity );
	if ( index < 0 )
	{ // �Ѿ��ﵽ��������������
		//ͨ���ó��еĵ�·����
		return;
	}
	pCity->underfire[index].army_index = army_index;
	// ֪ͨ�ó���
}

// ����
void city_battle( int actor_index, City *pCity, SLK_NetC_CityBattleInfo *info )
{
	if ( pCity == NULL )
		return;
	City *pTargetCity = NULL;
	int from_unit_index = -1;
	short from_posx = pCity->posx;
	short from_posy = pCity->posy;
	int actorid = g_actors[actor_index].actorid;
	if ( info->m_to_unit_index >= 0 && info->m_to_unit_index < g_mapunit_maxcount )
	{ // ��������ʵ��	
		if ( g_mapunit[info->m_to_unit_index].type == MAPUNIT_TYPE_CITY )
		{
			pTargetCity = &g_city[g_mapunit[info->m_to_unit_index].index];
			info->m_to_posx = pTargetCity->posx;
			info->m_to_posy = pTargetCity->posy;
		}
	}
	else
	{ // פ���յ�

	}

	// ��������
	int army_index = army_create( 1, 0, 0, "army" );
	g_Army[army_index].actorid = actorid;
	g_Army[army_index].stat = ARMY_STAT_WALK;
	g_Army[army_index].stat_time = (int)time( NULL );
	g_Army[army_index].from_posx = from_posx;
	g_Army[army_index].from_posy = from_posy;
	g_Army[army_index].posx = from_posx;
	g_Army[army_index].posy = from_posy;
	g_Army[army_index].target_posx = info->m_to_posx;
	g_Army[army_index].target_posy = info->m_to_posy;
	g_Army[army_index].walk_total_distance = (int)sqrt( pow( (float)(info->m_to_posx - from_posx), 2 ) + pow( (float)(info->m_to_posy - from_posy), 2 ) );

	// ֪ͨ��֧�����������ĳ���
	if ( pTargetCity )
	{
		city_underfire( pTargetCity, army_index );
	}

	// ��ӵ���ͼ��ʾ��Ԫ
	from_unit_index = mapunit_add( MAPUNIT_TYPE_ARMY, army_index );
	g_Army[army_index].unit_index = from_unit_index;

	// ֪ͨ������������һֻ����
	area_clearmsg( area_getindex( g_Army[army_index].from_posx, g_Army[army_index].from_posx ) );

	// ���о�·��֪ͨ��Ŀ�������������Լ�����Ϊ�Լ����ܲ��ڲ�������������
	army_marchroute( actor_index, from_unit_index, from_posx, from_posy, info->m_to_unit_index, info->m_to_posx, info->m_to_posy );

	// ��¼�³������Ӱ�����ʿ�����ͺ�����
	for ( int tmpi = 0; tmpi < info->m_troop_count; tmpi++ )
	{
		//��ʱ�ȼ�¼������Ϣ 
		g_Army[army_index].troops[tmpi].corps = info->m_troop_list[tmpi].m_corps;
		g_Army[army_index].troops[tmpi].level = info->m_troop_list[tmpi].m_level;
		g_Army[army_index].troops[tmpi].maxnumber = info->m_troop_list[tmpi].m_count;
	}
	// ��¼�������Ӱ�����Ӣ������
	for ( int tmpi = 0; tmpi < info->m_hero_count; tmpi++ )
	{
		g_Army[army_index].heroes_offset[tmpi] = info->m_hero_list[tmpi];
	}
}

// Ǩ��
int city_move( int actor_index, short posx, short posy )
{
	ACTOR_CHECK_INDEX( actor_index );
	CITY_CHECK_INDEX( g_actors[actor_index].city_index );

	// ���������Ƿ����Ǩ��

	// ��ɾ�������
	City *pCity = &g_city[g_actors[actor_index].city_index];
	pCity->posx = posx;
	pCity->posy = posy;
	mapunit_area_change( mapunit_getindex( MAPUNIT_TYPE_CITY, g_actors[actor_index].city_index ), posx, posy, 1 );
	return 0;
}

// ��������
int city_handle_queue_add( City *pCity, short buildingindex )
{
	short handleindex = 0;
	for ( handleindex = 0; handleindex < CityHandleQueueMax; handleindex++ )
	{
		if ( pCity->handle_queue[handleindex] == -1 )
		{
			pCity->handle_queue[handleindex] = buildingindex;
			break;
		}
	}
	if ( handleindex >= CityHandleQueueMax )
		return -1;
	return handleindex;
}

void city_handle_queue_del( City *pCity, short buildingindex )
{
	short handleindex = 0;
	for ( handleindex = 0; handleindex < CityHandleQueueMax; handleindex++ )
	{
		if ( pCity->handle_queue[handleindex] == buildingindex )
		{
			pCity->handle_queue[handleindex] = -1;
			break;
		}
	}
}

void city_handle_queue_clear( City *pCity )
{
	short handleindex = 0;
	for ( handleindex = 0; handleindex < CityHandleQueueMax; handleindex++ )
	{
		pCity->handle_queue[handleindex] = -1;
	}
}
