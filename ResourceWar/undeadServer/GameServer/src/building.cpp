#ifdef WIN32
#include <winsock2.h>
#include <windows.h>
#endif
#include <mysql.h>
#include <stdio.h>
#include <string.h>
#include "db.h"
#include "define.h"
#include "city.h"
#include "building.h"
#include "actor.h"
#include "troop.h"

extern SConfig g_Config;
extern MYSQL *myGame;
extern MYSQL *myData;

extern Actor *g_actors;
extern int g_maxactornum;

extern City *g_city;
extern int g_city_maxcount;
extern int g_city_maxindex;

extern TroopInfo **g_TroopInfo;

BuildingUpgrade *g_BuildingUpgrade = NULL;
int g_BuildingUpgradeCount = 0;

BuildingLimit *g_BuildingLimit = NULL;
char g_BuildingLimitCount = 0;

BuildingNew *g_BuildingNew = NULL;
int g_BuildingNewCount = 0;

// ������Ϣ��ʼ��
int building_upgrade_init()
{
	MYSQL_RES *res;
	MYSQL_ROW row;
	char szSQL[2048];

	// ��ñ�������Ľ������ͱ��
	sprintf( szSQL, "select max(kind) from building_upgrade" );
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
			g_BuildingUpgradeCount = atoi( row[0] ) + 1;
		else
			g_BuildingUpgradeCount = 0;
	}
	else
	{
		mysql_free_result( res );
		return -1;
	}
	mysql_free_result( res );

	g_BuildingUpgrade = (BuildingUpgrade *)malloc( sizeof(BuildingUpgrade)*g_BuildingUpgradeCount );
	memset( g_BuildingUpgrade, 0, sizeof(BuildingUpgrade)*g_BuildingUpgradeCount );

	// ��ñ�������ĵȼ�
	sprintf( szSQL, "SELECT kind, MAX(level) FROM building_upgrade GROUP BY kind;" );
	if ( mysql_query( myData, szSQL ) )
	{
		printf( "Query failed (%s) [%s](%d)\n", mysql_error( myData ), __FUNCTION__, __LINE__ );
		write_gamelog( "%s", szSQL );
		return -1;
	}
	res = mysql_store_result( myData );
	while ( (row = mysql_fetch_row( res )) )
	{
		short buildingkind = atoi( row[0] );
		if ( buildingkind <= 0 || buildingkind >= g_BuildingUpgradeCount )
		{
			mysql_free_result( res );
			return -1;
		}
		g_BuildingUpgrade[buildingkind].maxlevel = atoi( row[1] );
		g_BuildingUpgrade[buildingkind].info = (BuildingUpgradeConfig *)malloc( sizeof(BuildingUpgradeConfig)*(g_BuildingUpgrade[buildingkind].maxlevel + 1) );
		memset( g_BuildingUpgrade[buildingkind].info, 0, sizeof(BuildingUpgradeConfig)*(g_BuildingUpgrade[buildingkind].maxlevel + 1) );
	}
	mysql_free_result( res );
	
	// ��ȡ����
	sprintf( szSQL, "select * from building_upgrade order by kind asc, level asc;" );
	if ( mysql_query( myData, szSQL ) )
	{
		printf( "Query failed (%s) [%s](%d)\n", mysql_error( myData ), __FUNCTION__, __LINE__ );
		write_gamelog( "%s", szSQL );
		return -1;
	}
	res = mysql_store_result( myData );
	while ( (row = mysql_fetch_row( res )) )
	{
		int index = 0;
		int buildingkind = atoi( row[index++] );
		if ( buildingkind <= 0 || buildingkind >= g_BuildingUpgradeCount )
			continue;

		int level = atoi( row[index++] );
		if ( level < 0 || level > g_BuildingUpgrade[buildingkind].maxlevel )
			continue;

		index++;

		g_BuildingUpgrade[buildingkind].info[level].limit_kind[0] = atoi( row[index++] );
		g_BuildingUpgrade[buildingkind].info[level].limit_level[0] = atoi( row[index++] );
		g_BuildingUpgrade[buildingkind].info[level].limit_kind[1] = atoi( row[index++] );
		g_BuildingUpgrade[buildingkind].info[level].limit_level[1] = atoi( row[index++] );
		g_BuildingUpgrade[buildingkind].info[level].restype[0] = atoi( row[index++] );
		g_BuildingUpgrade[buildingkind].info[level].resvalue[0] = atoi( row[index++] );
		g_BuildingUpgrade[buildingkind].info[level].restype[1] = atoi( row[index++] );
		g_BuildingUpgrade[buildingkind].info[level].resvalue[1] = atoi( row[index++] );
		g_BuildingUpgrade[buildingkind].info[level].restype[2] = atoi( row[index++] );
		g_BuildingUpgrade[buildingkind].info[level].resvalue[2] = atoi(row[index++]);
		g_BuildingUpgrade[buildingkind].info[level].restype[3] = atoi(row[index++]);
		g_BuildingUpgrade[buildingkind].info[level].resvalue[3] = atoi(row[index++]);
		g_BuildingUpgrade[buildingkind].info[level].worker = atoi( row[index++] );
		g_BuildingUpgrade[buildingkind].info[level].sec = atoi( row[index++] );
		g_BuildingUpgrade[buildingkind].info[level].value[0] = atoi( row[index++] );
		g_BuildingUpgrade[buildingkind].info[level].value[1] = atoi( row[index++] );
		g_BuildingUpgrade[buildingkind].info[level].value[2] = atoi( row[index++] );
		g_BuildingUpgrade[buildingkind].info[level].value[3] = atoi( row[index++] );
	}
	mysql_free_result( res );
	
	if ( building_limit_init() < 0 )
		return -1;
	if ( building_new_init() < 0 )
		return -1;
	return 0;
}

// ������Ϣ��ʼ��
int building_limit_init()
{
	MYSQL_RES *res;
	MYSQL_ROW row;
	char szSQL[2048];

	// ��ñ��������id
	sprintf( szSQL, "select max(kind) from building_limit" );
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
			g_BuildingLimitCount = atoi( row[0] ) + 1;
		else
			g_BuildingLimitCount = 0;
	}
	else
	{
		mysql_free_result( res );
		return -1;
	}
	mysql_free_result( res );
	
	if ( g_BuildingUpgradeCount != g_BuildingLimitCount )
	{
		printf( "g_BuildingUpgradeCount != g_BuildingLimitCount \n" );
		//return -1;
	}

	// ����ռ�
	g_BuildingLimit = (BuildingLimit *)malloc( sizeof(BuildingLimit)*g_BuildingLimitCount );
	memset( g_BuildingLimit, 0, sizeof(BuildingLimit)*g_BuildingLimitCount );

	sprintf( szSQL, "select * from building_limit" );
	if ( mysql_query( myData, szSQL ) )
	{
		printf( "Query failed (%s) [%s](%d)\n", mysql_error( myData ), __FUNCTION__, __LINE__ );
		write_gamelog( "%s", szSQL );
		return -1;
	}

	res = mysql_store_result( myData );
	while ( (row = mysql_fetch_row( res )) )
	{
		short buildingkind = atoi( row[0] );
		if ( buildingkind < 0 )
		{
			mysql_free_result( res );
			return -1;
		}
		else if ( buildingkind >= g_BuildingLimitCount )
			continue;

		for ( int tmpi = 0; tmpi < 1; tmpi++ )
		{
			g_BuildingLimit[buildingkind].maxnum[tmpi] = atoi( row[tmpi * 2 + 1] );
			g_BuildingLimit[buildingkind].maxlevel[tmpi] = atoi( row[tmpi * 2 + 2] );
		}
	}
	mysql_free_result( res );
	return 0;
}

// ������ʼ������Ϣ��ʼ��
int building_new_init()
{
	MYSQL_RES *res;
	MYSQL_ROW row;
	char szSQL[2048];

	// ��ñ��������id
	sprintf( szSQL, "select max(id) from building_new" );
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
			g_BuildingNewCount = atoi( row[0] ) + 1;
		else
			g_BuildingNewCount = 0;
	}
	else
	{
		mysql_free_result( res );
		return -1;
	}
	mysql_free_result( res );

	// ����ռ�
	g_BuildingNew = (BuildingNew *)malloc( sizeof(BuildingNew)*g_BuildingNewCount );
	memset( g_BuildingNew, 0, sizeof(BuildingNew)*g_BuildingNewCount );

	sprintf( szSQL, "select * from building_new" );
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
		else if ( id >= g_BuildingNewCount )
			continue;

		g_BuildingNew[id].buildingindex = atoi( row[1] );
		g_BuildingNew[id].buildingkind = atoi( row[2] );
		g_BuildingNew[id].level = atoi( row[4] );
	}
	mysql_free_result( res );
	return 0;
}

// �ǳؽ�����Ϣ��ȡ
int building_load( City *pCity )
{
	MYSQL_RES		*res;
	MYSQL_ROW		row;
	char	szSQL[8192];
	if ( !pCity )
		return -1;

	sprintf( szSQL, "select * from city_building where cityid='%d'", pCity->cityid );
	if ( mysql_query( myGame, szSQL ) )
	{
		printf( "Query failed (%s)\n", mysql_error( myGame ) );
		write_gamelog( "%s", szSQL );
		if ( mysql_ping( myGame ) != 0 )
			db_reconnect_game();
		return -1;
	}
	res = mysql_store_result( myGame );

	while ( (row = mysql_fetch_row( res )) )
	{
		int index = 0;
		index++;
		short offset = atoi( row[index++] );
		pCity->building[offset].buildingkind = atoi( row[index++] );
		pCity->building[offset].level = atoi( row[index++] );
		pCity->building[offset].state = atoi( row[index++] );
		pCity->building[offset].begintime = atoi( row[index++] );
		pCity->building[offset].value[0] = atoi( row[index++] );
		pCity->building[offset].value[1] = atoi( row[index++] );
		pCity->building[offset].value[2] = atoi( row[index++] );
		pCity->building[offset].value[3] = atoi( row[index++] );
	}
	mysql_free_result( res );
	return 0;
}

// �ǳؽ�����Ϣ����
int building_save( City *pCity, FILE *fp )
{
	for ( int offset = 0; offset < CityBuildingMax; offset++ )
	{
		if ( pCity->building[offset].buildingkind <= 0 )
			continue;
		building_single_save( pCity->cityid, offset, &pCity->building[offset], fp );
	}
	return -1;
}

// һ���ǳؽ�����Ϣ����
int building_single_save( int cityid, short offset, Building *pBuilding, FILE *fp )
{
	char szSQL[2048];
	char reconnect_flag = 0;
RE_BUILDING_REPLACE:
	sprintf( szSQL, "REPLACE INTO city_building (`cityid`,`offset`,`buildingkind`,`level`,`state`,`begintime`,`value1`,`value2`,`value3`,`value4`) "
		"Values('%d','%d','%d','%d','%d','%d','%d','%d','%d','%d')",
		cityid, offset, pBuilding->buildingkind, pBuilding->level, pBuilding->state, pBuilding->begintime,
		pBuilding->value[0], pBuilding->value[1], pBuilding->value[2], pBuilding->value[3] );
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
			goto RE_BUILDING_REPLACE;
		}
	}
	return -1;
}

// ����һ������
int building_create( City *pCity, short buildingindex, short buildingkind )
{
	if ( pCity == NULL )
		return -1;
	if ( buildingindex < 0 || buildingindex >= CityBuildingMax )
		return -1;
	if ( buildingkind >= g_BuildingUpgradeCount )
		return -1;
	// ���buildingindex�Ƿ�Ϸ�
	if ( pCity->building[buildingindex].buildingkind > 0 )
		return -2;
	Building *pBuilding = &pCity->building[buildingindex];
	int buildingnum = building_getnum( pCity, buildingkind );
	//if ( buildingnum >= g_BuildingLimit[buildingkind].maxnum[pCity->towncenter_level] )
	//{// �������������
	//	return -1;
	//}


	// ��ӵ���������
	if ( city_handle_queue_add( pCity, buildingindex ) >= 0 )
	{
		pBuilding->buildingkind = (unsigned char)buildingkind;
		pBuilding->level = 0;
		pBuilding->state = BUILDING_STATE_CREATE_ING;
		pBuilding->begintime = (int)time( NULL );
	}
	else
	{
		return -3;
	}

	return buildingindex;
}

// ����һ������
int building_upgrade( City *pCity, short buildingindex )
{
	if ( pCity == NULL )
		return -1;
	if ( buildingindex < 0 || buildingindex >= CityBuildingMax )
		return -1;
	Building *pBuilding = &pCity->building[buildingindex];
	if ( pBuilding->level < 0 || pBuilding->level >= g_BuildingUpgrade[pBuilding->buildingkind].maxlevel )
		return -1;
	if ( pBuilding->state != BUILDING_STATE_NORMAL )
		return -1;
	// ������Ϣ
	BuildingUpgradeConfig *config = &g_BuildingUpgrade[pBuilding->buildingkind].info[pBuilding->level];
	//config->limit_kind
		
	// ��ӵ���������
	if ( city_handle_queue_add( pCity, buildingindex ) >= 0 )
	{
		// ����״̬
		pBuilding->state = BUILDING_STATE_UPGRADE_ING;
		pBuilding->begintime = (int)time( NULL );
	}
	else
	{
		return -3;
	}
	
	city_building_buildtimeinfo( pCity->actor_index, buildingindex, pBuilding );
	return 0;
}

// �������
int building_instantly_finish( City *pCity, short buildingindex )
{
	if ( pCity == NULL )
		return -1;
	if ( buildingindex < 0 || buildingindex >= CityBuildingMax )
		return -1;
	Building *pBuilding = &pCity->building[buildingindex];
	if ( pBuilding->level < 0 || pBuilding->level >= g_BuildingUpgrade[pBuilding->buildingkind].maxlevel )
		return -1;
	// ������Ϣ
	BuildingUpgradeConfig *config = &g_BuildingUpgrade[pBuilding->buildingkind].info[pBuilding->level];

	// ������ɻ���

	// ���
	building_finish( pCity, buildingindex );
	return 0;
}

// ɾ��һ������
int building_delete( City *pCity, short buildingindex )
{
	if ( pCity == NULL )
		return -1;
	if ( buildingindex < 0 || buildingindex >= CityBuildingMax )
		return -1;
	Building *pBuilding = &pCity->building[buildingindex];
	if ( pBuilding->level < 0 || pBuilding->level > g_BuildingUpgrade[pBuilding->buildingkind].maxlevel )
		return -1;

	// ��ӵ���������
	if ( city_handle_queue_add( pCity, buildingindex ) < 0 )
		return -3;

	// ������Ϣ
	BuildingUpgradeConfig *config = &g_BuildingUpgrade[pBuilding->buildingkind].info[pBuilding->level];
	if ( pBuilding->level == 0 && pBuilding->state == BUILDING_STATE_CREATE_ING )
	{ // �������ڽ����У������ˣ�����һЩ��Դ���ǳأ���ɾ�������δ����õĽ���
		memset( pBuilding, 0, sizeof(Building) );
		city_building_baseinfo( pCity->actor_index, buildingindex, pBuilding );
	}
	else if ( pBuilding->state == BUILDING_STATE_UPGRADE_ING )
	{ // ���������У�ȡ������, ����һЩ��Դ���ǳ�
		pBuilding->state = BUILDING_STATE_NORMAL;
		pBuilding->begintime = 0;
		city_building_buildtimeinfo( pCity->actor_index, buildingindex, pBuilding );
	}
	else if ( pBuilding->state == BUILDING_STATE_NORMAL )
	{ // һЩ�ɲ���Ľ���
		pBuilding->state = BUILDING_STATE_DELETE_ING;
		pBuilding->begintime = (int)time( NULL );
		city_building_buildtimeinfo( pCity->actor_index, buildingindex, pBuilding );
	}
	else
	{
		city_handle_queue_del( pCity, buildingindex );
	}

	return 0;
}

// �ƶ�һ������
int building_move( City *pCity, short buildingindex, unsigned char posx, unsigned char posy )
{
	if ( pCity == NULL )
		return -1;
	if ( buildingindex < 0 || buildingindex >= CityBuildingMax )
		return -1;
	Building *pBuilding = &pCity->building[buildingindex];
	return 0;
}

// �ɼ�
int building_gather( struct _city *pCity, short buildingindex )
{
	if ( pCity == NULL )
		return -1;
	if ( buildingindex < 0 || buildingindex >= CityBuildingMax )
		return -1;
	Building *pBuilding = &pCity->building[buildingindex];
	// �������������Ҫ����ʱ����
	switch ( pBuilding->buildingkind )
	{

	case BUILDING_WoodFactory:		//	ľ�ĳ�/������
		city_change_res( pCity, CityRes_Wood, pBuilding->value[0], PATH_BUILDING_GATHER );
		pBuilding->value[0] = 0;
		break;
	case BUILDING_Farm:				//	ũ��/����
		city_change_res( pCity, CityRes_Food, pBuilding->value[0], PATH_BUILDING_GATHER );
		pBuilding->value[0] = 0;
		break;
	case BUILDING_IronFactory:		//	����/������
		city_change_res( pCity, CityRes_Iron, pBuilding->value[0], PATH_BUILDING_GATHER );
		pBuilding->value[0] = 0;
		break;
	case BUILDING_Mithril:			//	����Ѩ/���
		city_change_res( pCity, CityRes_Mithril, pBuilding->value[0], PATH_BUILDING_GATHER );
		pBuilding->value[0] = 0;
		break;
	default:
		return -1;
	}
	city_building_abilityinfo( pCity->actor_index, buildingindex, pBuilding );
	return 0;
}

// ����������������ʱ��
int building_checktime( City *pCity, short buildingindex )
{
	int timeout = 0;
	if ( pCity == NULL )
		return 0;
	if ( buildingindex < 0 || buildingindex >= CityBuildingMax )
		return 0;
	Building *pBuilding = &pCity->building[buildingindex];
	if ( pBuilding->level < 0 || pBuilding->level > g_BuildingUpgrade[pBuilding->buildingkind].maxlevel )
		return 0;

	if ( pBuilding->state != BUILDING_STATE_CREATE_ING && pBuilding->state != BUILDING_STATE_DELETE_ING && pBuilding->state != BUILDING_STATE_UPGRADE_ING )
		return 0;

	BuildingUpgradeConfig *config = &g_BuildingUpgrade[pBuilding->buildingkind].info[pBuilding->level];
	if ( config == NULL )
		return 0;
	if ( (int)time( NULL ) >= pBuilding->begintime + config->sec )
	{ // ʱ���ѵ�
		timeout = 1;
	}
	return timeout;
}

// �������죬������ɾ���������
int building_finish( City *pCity, short buildingindex )
{
	if ( pCity == NULL )
		return -1;
	if ( buildingindex < 0 || buildingindex >= CityBuildingMax )
		return -1;
	Building *pBuilding = &pCity->building[buildingindex];
	if ( pBuilding->level < 0 || pBuilding->level >= g_BuildingUpgrade[pBuilding->buildingkind].maxlevel )
		return -1;
	BuildingUpgradeConfig *config = &g_BuildingUpgrade[pBuilding->buildingkind].info[pBuilding->level];
	short buildingkind = pBuilding->buildingkind;
	if ( pBuilding->state == BUILDING_STATE_CREATE_ING )
	{ // �������
		pBuilding->level = 1;
		pBuilding->begintime = 0;
		pBuilding->state = BUILDING_STATE_NORMAL;
	}
	else if( pBuilding->state == BUILDING_STATE_UPGRADE_ING )
	{ // �������
		pBuilding->level += 1;
		pBuilding->begintime = 0;
		pBuilding->state = BUILDING_STATE_NORMAL;
	}
	else if ( pBuilding->state == BUILDING_STATE_DELETE_ING )
	{ // ɾ�����
		memset( pBuilding, 0, sizeof(Building) );
	}
	// ɾ����������
	city_handle_queue_del( pCity, buildingindex );
	city_reset( pCity );
	city_building_buildtimeinfo( pCity->actor_index, buildingindex, pBuilding );
	city_building_baseinfo( pCity->actor_index, buildingindex, pBuilding );
	return 0;
}

// �������������
int building_getnum( City *pCity, short buildingkind )
{
	if ( pCity == NULL )
		return -1;
	int num = 0;
	for ( int tmpi = 0; tmpi < CityBuildingMax; tmpi++ )
	{
		if ( pCity->building[tmpi].buildingkind == buildingkind )
			num++;
	}
	return num;
}

// ��ȡһ����������
int building_getemptyindex( City *pCity )
{
	if ( pCity == NULL )
		return -1;
	int buildingindex = -1;
	for ( int tmpi = 0; tmpi < CityBuildingMax; tmpi++ )
	{
		if ( pCity->building[tmpi].buildingkind == 0 )
		{
			buildingindex = tmpi;
			break;
		}
	}
	return buildingindex;
}

// ��ȡ��������ʱ��
int building_getneedtime( Building *pBuilding )
{
	switch ( pBuilding->state )
	{
	case BUILDING_STATE_CREATE_ING:
	case BUILDING_STATE_UPGRADE_ING:
	{
		BuildingUpgradeConfig *config = &g_BuildingUpgrade[pBuilding->buildingkind].info[pBuilding->level];
		if ( config != NULL )
			return config->sec;
	}
		break;
	case BUILDING_STATE_TROOPTRAIN_ING:
	{
		TroopInfo *config = &g_TroopInfo[pBuilding->value[0]][pBuilding->value[1]];
		if ( config != NULL )
			return config->sec * pBuilding->value[2];
	}
		break;
	case BUILDING_STATE_TROOPUP_ING:
	{
		if( pBuilding->value[1] >= CityCorpsLevelMax )
			return 0;
		TroopInfo *config = &g_TroopInfo[pBuilding->value[0]][pBuilding->value[1]+1];
		if ( config != NULL )
			return config->sec * pBuilding->value[2];
	}
		break;
	case BUILDING_STATE_TROOPHEAL_ING:
		break;
	}

	return 0;
}

/************************************************************************/
/* ��ҵı�����Ϣ                                                         */
/************************************************************************/

// ���ʱ����
int building_troop_checktime( struct _city *pCity, short buildingindex )
{
	int timeout = 0;
	if ( pCity == NULL )
		return 0;
	if ( buildingindex < 0 || buildingindex >= CityBuildingMax )
		return 0;
	Building *pBuilding = &pCity->building[buildingindex];
	if ( pBuilding->value[0] <= 0 )
		return 0;

	if ( pBuilding->state != BUILDING_STATE_TROOPTRAIN_ING && pBuilding->state != BUILDING_STATE_TROOPUP_ING && pBuilding->state != BUILDING_STATE_TROOPHEAL_ING )
		return 0;

	if ( pBuilding->level < 0 || pBuilding->level > g_BuildingUpgrade[pBuilding->buildingkind].maxlevel )
		return 0;
	if ( (int)time( NULL ) >= pBuilding->begintime + building_getneedtime( pBuilding ) )
	{ // ʱ���ѵ�
		timeout = 1;
	}
	return timeout;
}

// ����ѵ��, op : 1-ѵ����2-����
int building_troop_train( int actor_index, int buildingindex, char op, short corps, char level, int num )
{
	if ( actor_index < 0 || actor_index > g_maxactornum )
		return -1;

	TroopInfo* config = &g_TroopInfo[corps][level];
	if ( config == NULL )
		return -1;

	int index = config->corps_type - 1;
	City*	city = city_getptr( actor_index );
	Building* pBuilding = &city->building[buildingindex];

	// ״̬����
	if( pBuilding->state != BUILDING_STATE_NORMAL )
		return -1;

	// ���岻��
	if( config->aclass != g_actors[actor_index].aclass )
		return -1;

	// �������಻�� 
	if( pBuilding->buildingkind != config->buildingkind )
		return -1;

	switch ( op )
	{
	case 1:	// ѵ��
		// �����Դ
		if ( false )
		{
			return -1;
		}

		pBuilding->state = BUILDING_STATE_TROOPTRAIN_ING;
		pBuilding->value[3] = 1;

		break;

	case 2:	// ����
		if ( level >= 10 )
		{
			return -1;
		}

		if ( city->corps_num[index][level-1] - num < 0 )
		{
			return -1;
		}

		// �����Դ
		if ( false )
		{
			return -1;
		}

		city->corps_num[index][level - 1] -= num;
		city_troop_sendinfo( actor_index, city, config->corps_type, config->level );

		pBuilding->state = BUILDING_STATE_TROOPUP_ING;
		pBuilding->value[3] = 2;
		break;
	}

	pBuilding->begintime = (int)time( NULL );
	pBuilding->value[0] = corps;
	pBuilding->value[1] = level;
	pBuilding->value[2] = num;
	city_building_abilityinfo( actor_index, buildingindex, pBuilding );
	city_building_buildtimeinfo( actor_index, buildingindex, pBuilding );

	return 0;
}

// ����
int building_troop_wound( int actor_index, short corps, char level, int num )
{
	if ( actor_index < 0 || actor_index > g_maxactornum )
		return -1;

	TroopInfo* config = &g_TroopInfo[corps][level];
	if ( config == NULL )
		return -1;

	int index = config->corps_type - 1;
	City*	city = city_getptr( actor_index );

	if ( city->corps_num[index][level-1] - num < 0 )
	{
		return -1;
	}

	city->corps_num[index][level-1] -= num;
	city->corps_wound[index][level-1] += num;

	city_troop_sendinfo( actor_index, city, corps, level );

	return 0;
}

// ���������
int building_troop_abandon( int actor_index, short corps, char level, int num )
{
	if ( actor_index < 0 || actor_index > g_maxactornum )
		return -1;

	TroopInfo* config = &g_TroopInfo[corps][level];
	if( config == NULL )
		return -1;

	int index = config->corps_type - 1;
	City*	city = city_getptr( actor_index );

	if ( city->corps_num[index][level-1] - num < 0 )
	{
		return -1;
	}

	city->corps_num[index][level-1] -= num;

	city_troop_sendinfo( actor_index, city, config->corps_type, level );

	return 0;
}


// ȡ��ѵ��
int building_troop_traincancel( int actor_index, int buildingindex )
{
	if ( actor_index < 0 || actor_index > g_maxactornum )
		return -1;

	City*	city = city_getptr( actor_index );
	Building* pBuilding = &city->building[buildingindex];
	TroopInfo* config = &g_TroopInfo[pBuilding->value[0]][pBuilding->value[1]];
	if ( config == NULL )
		return -1;

	int index = config->corps_type - 1;
	// ������Դ
	switch ( pBuilding->state )
	{
	case BUILDING_STATE_TROOPTRAIN_ED:	// ѵ��

		break;
	case BUILDING_STATE_TROOPUP_ED:	// ����
		city->corps_num[index][config->level-1] += pBuilding->value[3];
		city_troop_sendinfo( actor_index, city, config->corps_type, config->level );
		break;
	default:
		return -1;
	}

	// ����״̬
	pBuilding->state = BUILDING_STATE_NORMAL;
	pBuilding->begintime = 0;
	pBuilding->value[0] = 0;
	pBuilding->value[1] = 0;
	pBuilding->value[2] = 0;
	pBuilding->value[3] = 0;
	city_building_abilityinfo( actor_index, buildingindex, pBuilding );
	city_building_buildtimeinfo( actor_index, buildingindex, pBuilding );
	return 0;
}

// �������ѵ��
int building_troop_trainfinish( int actor_index, int buildingindex )
{
	int token = 0;
	if ( actor_index < 0 || actor_index > g_maxactornum )
		return -1;

	// ��ȡ����
	City*	city = city_getptr( actor_index );
	Building* pBuilding = &city->building[buildingindex];

	switch ( pBuilding->state )
	{
	case BUILDING_STATE_TROOPTRAIN_ING:	// ѵ��
		// �ȿ�Ǯ������
		//token = troop_get_tofinishtoken(op, corps, level, g_actors[actor_index].troop[index].m_training );
		if ( token > g_actors[actor_index].token )
		{
			return -1;
		}

		break;
	case BUILDING_STATE_TROOPUP_ING:	// ����
		// �ȿ�Ǯ������
		//token = troop_get_tofinishtoken(op, corps, level, g_actors[actor_index].troop[index].m_training);
		if ( token > g_actors[actor_index].token )
		{
			return -1;
		}
		break;

	default:
		return -1;
	}

	building_troop_ontrainfinish( actor_index, buildingindex );
	return 0;
}

// ���ѵ��
int building_troop_ontrainfinish( int actor_index, int buildingindex )
{
	if ( actor_index < 0 || actor_index > g_maxactornum )
		return -1;

	// ��ȡ����
	City*	city = city_getptr( actor_index );
	Building* pBuilding = &city->building[buildingindex];

	switch ( pBuilding->state )
	{
	case BUILDING_STATE_TROOPTRAIN_ING:	// ѵ��
		pBuilding->state = BUILDING_STATE_TROOPTRAIN_ED;
		break;
	case BUILDING_STATE_TROOPUP_ING:	// ����
		pBuilding->state = BUILDING_STATE_TROOPUP_ED;
		break;

	default:
		return -1;
	}

	// ����ʱֱ�ӻ�ȡ��
	building_troop_ontrainget( actor_index, buildingindex );
	return 0;
}

// ��ɲ���
int building_troop_ontrainget( int actor_index, int buildingindex )
{
	int index = 0;
	if ( actor_index < 0 || actor_index > g_maxactornum )
		return -1;

	// ��ȡ����
	City*	city = city_getptr( actor_index );
	Building* pBuilding = &city->building[buildingindex];
	TroopInfo* config = &g_TroopInfo[pBuilding->value[0]][pBuilding->value[1]];
	if ( config == NULL )
		return -1;
	index = config->corps_type - 1;

	switch ( pBuilding->state )
	{
	case BUILDING_STATE_TROOPTRAIN_ED:	// ѵ��
		city->corps_num[index][pBuilding->value[1] - 1] += pBuilding->value[2];
		city_troop_sendinfo( actor_index, city, config->corps_type, pBuilding->value[1] );
		break;
	case BUILDING_STATE_TROOPUP_ED:	// ����
		city->corps_num[index][pBuilding->value[1]] += pBuilding->value[2];
		city_troop_sendinfo( actor_index, city, config->corps_type, pBuilding->value[1] + 1 );
		break;

	default:
		return -1;
	}

	// ����״̬
	pBuilding->state = BUILDING_STATE_NORMAL;
	pBuilding->begintime = 0;
	pBuilding->value[0] = 0;
	pBuilding->value[1] = 0;
	pBuilding->value[2] = 0;
	pBuilding->value[3] = 0;
	city_building_abilityinfo( actor_index, buildingindex, pBuilding );
	city_building_buildtimeinfo( actor_index, buildingindex, pBuilding );

	return 0;
}
