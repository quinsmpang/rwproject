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
#include "global.h"
#include "actor_send.h"
#include "server_netsend_auto.h"
#include "troop.h"

extern SConfig g_Config;
extern MYSQL *myGame;
extern MYSQL *myData;

extern Actor *g_actors;
extern int g_maxactornum;

extern BuildingUpgrade *g_BuildingUpgrade;
extern int g_BuildingUpgradeCount;

extern BuildingLimit *g_BuildingLimit;
extern char g_BuildingLimitCount;

extern BuildingNew *g_BuildingNew;
extern int g_BuildingNewCount;

extern City *g_city;
extern int g_city_maxcount;
extern int g_city_maxindex;


// 玩家城市信息
int city_info( int actor_index )
{
	ACTOR_CHECK_INDEX( actor_index );
	CITY_CHECK_INDEX( g_actors[actor_index].city_index );
	city_resinfo( actor_index, &g_city[g_actors[actor_index].city_index] );
	city_buildinglist( actor_index, &g_city[g_actors[actor_index].city_index] );
	city_building_abilitylist( actor_index, &g_city[g_actors[actor_index].city_index] );
	city_building_buildtimeinfolist( actor_index, &g_city[g_actors[actor_index].city_index] );
	city_troop_sendinfolist( actor_index, &g_city[g_actors[actor_index].city_index] );
	return 0;
}

// 城市基本信息
void city_resinfo( int actor_index, City *pCity )
{
	if ( pCity == NULL )
		return;
	SLK_NetS_CityResInfo info = { 0 };
	info.m_wood = pCity->wood;
	info.m_food = pCity->food;
	info.m_iron = pCity->iron;
	info.m_mithril = pCity->mithril;
	info.m_gold = pCity->gold;
	info.m_towncenter_level = pCity->building[0].level;
	info.m_enter_worldmap = pCity->match_cityid > 0 ? 0 : 1;
	netsend_cityresinfo_S( actor_index, SENDTYPE_ACTOR, &info );
}

// 城市建筑基本信息
void city_buildinglist( int actor_index, City *pCity )
{
	if ( pCity == NULL )
		return;
	SLK_NetS_CityBuildingList info = { 0 };
	for ( unsigned char tmpi = 0; tmpi < CityBuildingMax; tmpi++ )
	{
		Building *pBuilding = &pCity->building[tmpi];
		if ( pBuilding->buildingkind <= 0 || pBuilding->buildingkind >= g_BuildingUpgradeCount )
			continue;
		info.m_list[info.m_count].m_offset = tmpi;
		info.m_list[info.m_count].m_kind = pBuilding->buildingkind;
		info.m_list[info.m_count].m_level = pBuilding->level;
		info.m_count++;
	}
	netsend_citybuildinglist_S( actor_index, SENDTYPE_ACTOR, &info );
}

// 城市建筑升级或建造时间信息
void city_building_buildtimeinfo( int actor_index, short buildingindex, Building *pBuilding )
{
	if ( pBuilding == NULL )
		return;

	SLK_NetS_BuildTimeInfo info = { 0 };
	info.m_offset = (unsigned char)buildingindex;
	info.m_state = pBuilding->state;
	if ( pBuilding->state == BUILDING_STATE_NORMAL )
		info.m_time = 0;
	else
		info.m_time = pBuilding->begintime + building_getneedtime( pBuilding );
	netsend_buildtimeinfo_S( actor_index, SENDTYPE_ACTOR, &info );
}

// 城市建筑升级或建造时间信息
void city_building_buildtimeinfolist( int actor_index, City *pCity )
{
	if ( pCity == NULL )
		return;
	SLK_NetS_BuildTimeInfoList info = { 0 };
	info.m_servertime = (int)time( NULL );
	for ( unsigned char tmpi = 0; tmpi < CityBuildingMax; tmpi++ )
	{
		Building *pBuilding = &pCity->building[tmpi];
		if ( pBuilding->buildingkind <= 0 || pBuilding->buildingkind >= g_BuildingUpgradeCount )
			continue;
		if ( pBuilding->begintime <= 0 )
			continue;

		info.m_timeinfo[info.m_count].m_offset = tmpi;
		info.m_timeinfo[info.m_count].m_state = pBuilding->state;
		info.m_timeinfo[info.m_count].m_time = pBuilding->begintime + building_getneedtime(pBuilding);
		info.m_count++;
	}

	netsend_buildtimeinfolist_S( actor_index, SENDTYPE_ACTOR, &info );
}

// 资源改变
void city_res_changeinfo( int actor_index, char restype, int resvalue, char path )
{
	if ( actor_index < 0 || actor_index >= g_maxactornum )
		return;
	SLK_NetS_CityResChangeInfo info = { 0 };
	info.m_restype = restype;
	info.m_resvalue = resvalue;
	info.m_path = path;
	netsend_cityreschangeinfo_S( actor_index, SENDTYPE_ACTOR, &info );
}

// 发送一个建筑的信息
void city_building_baseinfo( int actor_index, short buildingindex, Building *pBuilding )
{
	if ( actor_index < 0 || actor_index >= g_maxactornum )
		return;
	if ( buildingindex < 0 || buildingindex >= CityBuildingMax )
		return;
	if ( pBuilding == NULL )
		return;
	SLK_NetS_BuildingBaseInfo info = { 0 };
	info.m_offset = (unsigned char)buildingindex;
	info.m_kind = pBuilding->buildingkind;
	info.m_level = pBuilding->level;
	netsend_buildingbaseinfo_S( actor_index, SENDTYPE_ACTOR, &info );
}

// 发送一个建筑的属性信息
void city_building_abilityinfo( int actor_index, short buildingindex, Building *pBuilding )
{
	if ( actor_index < 0 || actor_index >= g_maxactornum )
		return;
	if ( buildingindex < 0 || buildingindex >= CityBuildingMax )
		return;
	if ( pBuilding == NULL )
		return;
	SLK_NetS_BuildingAbilityInfo info = { 0 };
	info.m_offset = (unsigned char)buildingindex;
	for ( int tmpi = 0; tmpi < BUILDING_INFO_VALUE_MAX; tmpi++ )
	{
		info.m_value[tmpi] = pBuilding->value[tmpi];
	}
	netsend_buildingabilityinfo_S( actor_index, SENDTYPE_ACTOR, &info );
}

// 发送一个建筑的属性信息
void city_building_abilitylist( int actor_index, City *pCity )
{
	if ( actor_index < 0 || actor_index >= g_maxactornum )
		return;
	if ( pCity == NULL )
		return;
	SLK_NetS_BuildingAbilityList list = { 0 };
	for ( unsigned char tmpi = 0; tmpi < CityBuildingMax; tmpi++ )
	{
		Building *pBuilding = &pCity->building[tmpi];
		if ( pBuilding->buildingkind > 0 && pBuilding->value[0] != 0 )
		{
			list.m_list[list.m_count].m_offset = tmpi;
			for ( int index = 0; index < BUILDING_INFO_VALUE_MAX; index++ )
			{
				list.m_list[list.m_count].m_value[index] = pBuilding->value[index];
			}
			list.m_count++;
		}
	}
	netsend_buildingabilitylist_S( actor_index, SENDTYPE_ACTOR, &list );
}

// 发送城市兵种信息列表 
void city_troop_sendinfolist( int actor_index, City* pCity )
{
	if ( actor_index < 0 || actor_index >= g_maxactornum )
		return;
	if ( pCity == NULL )
		return;
	SLK_NetS_TroopInfoList list = { 0 };
	for ( int tmpi = 0; tmpi < CityCorpsMax; tmpi++ )
	{
		for( int tmpj = 0; tmpj < CityCorpsLevelMax; tmpj++ )
		{
			if ( pCity->corps_num[tmpi][tmpj] > 0 || pCity->corps_wound[tmpi][tmpj] > 0 )
			{
				list.m_troop_list[list.m_count].m_corpstype = tmpi + 1;
				list.m_troop_list[list.m_count].m_level = tmpj + 1;
				list.m_troop_list[list.m_count].m_have = pCity->corps_num[tmpi][tmpj];
				list.m_troop_list[list.m_count].m_wound = pCity->corps_wound[tmpi][tmpj];
				list.m_count++;
			}
		}
	}
	netsend_troopinfolist_S( actor_index, SENDTYPE_ACTOR, &list );
}

// 发送单个兵种信息更新
void city_troop_sendinfo( int actor_index, City* pCity, int corps_type, int level )
{
	if ( actor_index < 0 || actor_index >= g_maxactornum )
		return;
	if ( pCity == NULL )
		return;
	SLK_NetS_TroopInfo info = { 0 };

	info.m_corpstype = corps_type;
	info.m_level = level;
	info.m_have = pCity->corps_num[corps_type-1][level-1];
	info.m_wound = pCity->corps_wound[corps_type-1][level-1];

	netsend_troopinfo_S( actor_index, SENDTYPE_ACTOR, &info );
}