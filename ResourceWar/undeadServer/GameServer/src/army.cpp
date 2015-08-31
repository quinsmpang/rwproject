#ifdef WIN32
#include <winsock2.h>
#include <windows.h>
#endif
#include <mysql.h>
#include <stdio.h>
#include <string.h>
#include "db.h"
#include "define.h"
#include "building.h"
#include "actor.h"
#include "hero.h"
#include "troop.h"
#include "army.h"
#include "area.h"
#include "actor_send.h"
#include "server_netsend_auto.h"

extern SConfig g_Config;
extern MYSQL *myGame;
extern MYSQL *myData;

extern Actor *g_actors;
extern int g_maxactornum;

extern HeroInfo *g_HeroInfo;
extern short g_HeroCount;

extern TroopInfo **g_TroopInfo;
extern short g_TroopCount;
extern short g_TroopMaxLevel;

extern TroopCorpsInfo *g_TroopCorpsInfo;
extern short g_TroopCorpsCount;

Army *g_Army = NULL;
int g_maxArmyNum = 0;

EnemyArmy *g_EnemyArmy = NULL;
int g_maxEnemyNum = 0;

//-----------------------------------------------------------------------------
// army_init
// 函数说明: 军队初始化
//-----------------------------------------------------------------------------
int army_init()
{
	MYSQL_RES		*res;
	MYSQL_ROW		row;
	char	szSQL[1024];
	int enemyid;

	// 部队
	g_maxArmyNum = g_Config.max_citycount * 1 + 1024;
	g_Army = (Army *)malloc(sizeof(Army)*g_maxArmyNum);
	memset(g_Army, 0, sizeof(Army)*g_maxArmyNum);
	printf( "Army  maxcount=%d  memory=%0.2fMB\n", g_maxArmyNum, (sizeof(Army)*g_maxArmyNum) / 1024.0 / 1024.0 );

	// 根据战斗表中的最大ID值确定内存的开销
	sprintf(szSQL, "select max(enemyid) from enemy");
	if (mysql_query(myData, szSQL))
	{
		printf("Query failed (%s) [%s](%d)\n", mysql_error(myData), __FUNCTION__, __LINE__);
		write_gamelog("%s", szSQL);
		return -1;
	}
	res = mysql_store_result(myData);
	if ((row = mysql_fetch_row(res)))
	{
		// 战斗信息表中，g_fightinfo_count只是表明了存储元素的个数
		if (row[0])
			g_maxEnemyNum = atoi(row[0]) + 1;
		else
			g_maxEnemyNum = 0;
	}
	else
	{
		mysql_free_result(res);
		return -1;
	}
	mysql_free_result(res);
	// 分配战斗信息数据的内存
	g_EnemyArmy = (EnemyArmy *)malloc(sizeof(EnemyArmy)*g_maxEnemyNum);
	memset(g_EnemyArmy, 0, sizeof(EnemyArmy)*g_maxEnemyNum);

	// 读取战斗信息的数据
	sprintf(szSQL, "select enemyid,herokind1,herolevel1,corps1,level1,number1,herokind2,herolevel2,corps2,level2,number2,herokind3,herolevel3,corps3,level3,number3,\
				   herokind4,herolevel4,corps4,level4,number4,herokind5,herolevel5,corps5,level5,number5 from enemy");
	if (mysql_query(myData, szSQL))
	{
		printf("Query failed (%s) [%s](%d)\n", mysql_error(myData), __FUNCTION__, __LINE__);
		write_gamelog("%s", szSQL);
		return -1;
	}
	res = mysql_store_result(myData);

	while ((row = mysql_fetch_row(res)))
	{
		int currow;
		enemyid = atoi(row[0]);
		if (enemyid <= 0 || enemyid >= g_maxEnemyNum)
		{
			printf("enemyid = %d, not corrected. continue...\n", enemyid);
			continue;
		}
		currow = 1;
		g_EnemyArmy[enemyid].enemyid = enemyid;
		g_EnemyArmy[enemyid].troops_count = 0;
		for (int tmpi = 0; tmpi < 5; tmpi++)
		{
			g_EnemyArmy[enemyid].herokind[tmpi] = atoi(row[currow++]);
			g_EnemyArmy[enemyid].herolevel[tmpi] = atoi(row[currow++]);
			g_EnemyArmy[enemyid].corps[tmpi] = atoi(row[currow++]);
			g_EnemyArmy[enemyid].corpslevel[tmpi] = atoi(row[currow++]);
			g_EnemyArmy[enemyid].number[tmpi] = atoi(row[currow++]);
			if (g_EnemyArmy[enemyid].corps[tmpi] <= 0)
				break;
			g_EnemyArmy[enemyid].troops_count++;
		}
	}
	mysql_free_result(res);
	return 0;
}

int army_getfreeindex()
{
	for (int tmpi = 1;/* 1开始 */ tmpi < g_maxArmyNum; tmpi++)
	{
		if (g_Army[tmpi].isnpc == 0 && g_Army[tmpi].actorid == 0)
		{
			return tmpi;
		}
	}
	return -1;
}

int army_getindex_withid(int actorid, char isnpc )
{
	if (actorid <= 0 || actorid >= g_maxArmyNum )
		return -1;
	for (int tmpi = 0; tmpi < g_maxArmyNum; tmpi++)
	{
		if (g_Army[tmpi].actorid == actorid && g_Army[tmpi].isnpc == isnpc )
		{
			return tmpi;
		}
	}
	return -1;
}

int army_create( int actorid, char isnpc, short shape, char *pname )
{
	int army_index = army_getfreeindex();
	if (army_index < 0)
		return -1;

	memset(&g_Army[army_index], 0, sizeof(Army));
	g_Army[army_index].actorid = actorid;
	g_Army[army_index].isnpc = isnpc;
	g_Army[army_index].shape = shape;
	g_Army[army_index].unit_index = -1;
	strcpy(g_Army[army_index].armyname, pname);
	return army_index;
}

int army_addtroop( int army_index, TroopUnit *pTroopUnit )
{
	if (g_Army[army_index].troops_count >= MAX_TROOPS_COUNT)
		return -1;
	memcpy(&g_Army[army_index].troops[g_Army[army_index].troops_count], pTroopUnit, sizeof(TroopUnit));
	g_Army[army_index].troops_count++;
	return 0;
}

int army_addenemy(int army_index, int enemyid)
{
	TroopUnit troopUnit;
	int herokind;
	short corps;
	char level;
	if (enemyid <= 0 || enemyid >= g_maxEnemyNum)
		return -1;

	for (int tmpi = 0; tmpi < g_EnemyArmy[enemyid].troops_count; tmpi++)
	{
		memset(&troopUnit, 0, sizeof(TroopUnit));
		herokind = g_EnemyArmy[enemyid].herokind[tmpi];
		troopUnit.hero_kind = herokind;
		strcpy(troopUnit.hero_name, g_HeroInfo[herokind].name);
		troopUnit.hero_shape = g_HeroInfo[herokind].shape;
		troopUnit.hero_level = g_EnemyArmy[enemyid].herolevel[tmpi];
		memcpy(troopUnit.hero_skill, g_HeroInfo[herokind].skill, sizeof(short)* 4);

		corps = g_EnemyArmy[enemyid].corps[tmpi];
		level = g_EnemyArmy[enemyid].corpslevel[tmpi];
		troopUnit.corps = corps;
		troopUnit.level = level;
		troopUnit.attack = g_TroopInfo[corps][level].attack;
		troopUnit.defance = g_TroopInfo[corps][level].defance;
		troopUnit.hit = g_TroopInfo[corps][level].hit;
		troopUnit.dodge = g_TroopInfo[corps][level].dodge;
		troopUnit.crit = g_TroopInfo[corps][level].crit;
		troopUnit.agl = g_TroopInfo[corps][level].agl;
		troopUnit.crit_harm = g_TroopInfo[corps][level].crit_harm;
		troopUnit.crit_reduce = g_TroopInfo[corps][level].crit_reduce;

		troopUnit.curnumber = troopUnit.maxnumber = g_EnemyArmy[enemyid].number[tmpi];
		troopUnit.fightnumber = troopUnit.curnumber;

		army_addtroop(army_index, &troopUnit);
	}

	return 0;
}

void army_alllogic()
{
	for ( int army_index = 0; army_index < g_maxArmyNum; army_index++ )
	{
		if ( g_Army[army_index].actorid <= 0 )
			continue;
		army_logic( army_index );
	}
}

void army_logic( int army_index )
{
	if ( g_Army[army_index].stat == ARMY_STAT_WALK )
	{ // 部队移动
		g_Army[army_index].walk_len += 1;
		if ( g_Army[army_index].walk_len >= 10 )
		{
			g_Army[army_index].walk_len = 0;
			g_Army[army_index].walk_distance += 1;
			g_Army[army_index].posx = ((g_Army[army_index].target_posx - g_Army[army_index].from_posx) * g_Army[army_index].walk_distance) / g_Army[army_index].walk_total_distance + g_Army[army_index].from_posx;
			g_Army[army_index].posy = ((g_Army[army_index].target_posy - g_Army[army_index].from_posy) * g_Army[army_index].walk_distance) / g_Army[army_index].walk_total_distance + g_Army[army_index].from_posy;
			mapunit_area_change( g_Army[army_index].unit_index, g_Army[army_index].posx, g_Army[army_index].posy, 0 );
		}
	}
}

// 创建部队时候创建的行军路线
int army_marchroute( int actor_index, int from_unit_index, short from_posx, short from_posy, int to_unit_index, short to_posx, short to_posy )
{
	SLK_NetS_MarchRoute info = { 0 };
	info.m_from_unit_index = from_unit_index;
	info.m_from_posx = from_posx;
	info.m_from_posy = from_posy;
	info.m_to_unit_index = to_unit_index;
	info.m_to_posx = to_posx;
	info.m_to_posy = to_posy;
	netsend_marchroute_S( actor_index, SENDTYPE_AREA, &info );
	return 0;
}
