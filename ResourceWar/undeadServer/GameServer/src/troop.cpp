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
#include "troop.h"
#include "hero.h"
#include "army.h"
#include "city.h"

extern SConfig g_Config;
extern MYSQL *myGame;
extern MYSQL *myData;

extern Actor *g_actors;
extern int g_maxactornum;

extern HeroInfo *g_HeroInfo;
extern short g_HeroCount;

extern EnemyArmy *g_EnemyArmy;
extern int g_maxEnemyNum;

TroopInfo **g_TroopInfo = NULL;
short g_TroopCount = 0;
short g_TroopMaxLevel = 0;

TroopCorpsInfo *g_TroopCorpsInfo = NULL;
short g_TroopCorpsCount = 0;

// 兵种初始化
int troop_info_init()
{
	MYSQL_RES *res;
	MYSQL_ROW row;
	char szSQL[2048];

	// 获得表格中最大的兵种类型
	sprintf( szSQL, "select max(corps) from troop" );
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
			g_TroopCount = atoi( row[0] ) + 1;
		else
			g_TroopCount = 0;
	}
	else
	{
		mysql_free_result( res );
		return -1;
	}
	mysql_free_result( res );

	// 获得表格中最大的兵种等级
	sprintf( szSQL, "select max(level) from troop" );
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
			g_TroopMaxLevel = atoi( row[0] ) + 1;
		else
			g_TroopMaxLevel = 0;
	}
	else
	{
		mysql_free_result( res );
		return -1;
	}
	mysql_free_result( res );

	if ( g_TroopCount <= 0 )
		return 0;

	// 分配空间
	g_TroopInfo = (TroopInfo **)malloc( sizeof(TroopInfo)*g_TroopCount );
	memset( g_TroopInfo, 0, sizeof(TroopInfo)*g_TroopCount );

	for ( int tmpi = 0; tmpi < g_TroopCount; tmpi++ )
	{
		g_TroopInfo[tmpi] = (TroopInfo *)malloc( sizeof(TroopInfo)*g_TroopMaxLevel );
		memset( g_TroopInfo[tmpi], 0, sizeof(TroopInfo)*g_TroopMaxLevel );
	}

	sprintf( szSQL, "select * from troop" );
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
		int corps = atoi( row[index++] );
		if ( corps < 0 )
		{
			mysql_free_result( res );
			return -1;
		}
		else if ( corps >= g_TroopCount )
			continue;
		int level = atoi( row[index++] );
		g_TroopInfo[corps][level].corps = corps;
		g_TroopInfo[corps][level].level = level;
		g_TroopInfo[corps][level].corps_type = atoi( row[index++] );
		memcpy( g_TroopInfo[corps][level].name, row[index++], sizeof(char)*NAME_SIZE );
		g_TroopInfo[corps][level].shape = atoi( row[index++] );
		g_TroopInfo[corps][level].hp = atoi( row[index++] );
		g_TroopInfo[corps][level].attack = atoi( row[index++] );
		g_TroopInfo[corps][level].defance = atoi( row[index++] );
		g_TroopInfo[corps][level].hit = atoi( row[index++] );
		g_TroopInfo[corps][level].dodge = atoi( row[index++] );
		g_TroopInfo[corps][level].crit = atoi( row[index++] );
		g_TroopInfo[corps][level].agl = atoi( row[index++] );
		g_TroopInfo[corps][level].crit_harm = atoi( row[index++] );
		g_TroopInfo[corps][level].crit_reduce = atoi( row[index++] );
		g_TroopInfo[corps][level].mobility = atoi( row[index++] );
		g_TroopInfo[corps][level].move_grid = atoi( row[index++] );
		g_TroopInfo[corps][level].attack_order = atoi( row[index++] );
		g_TroopInfo[corps][level].attack_type = atoi( row[index++] );
		g_TroopInfo[corps][level].defance_type = atoi( row[index++] );
		g_TroopInfo[corps][level].carry_res = atoi( row[index++] );
		g_TroopInfo[corps][level].attack_range = atoi( row[index++] );
		g_TroopInfo[corps][level].combat_power = atoi( row[index++] );
		g_TroopInfo[corps][level].upgrade_restype[0] = atoi( row[index++] );
		g_TroopInfo[corps][level].upgrade_resnum[0] = atoi( row[index++] );
		g_TroopInfo[corps][level].upgrade_restype[1] = atoi( row[index++] );
		g_TroopInfo[corps][level].upgrade_resnum[1] = atoi( row[index++] );
		g_TroopInfo[corps][level].upgrade_restype[2] = atoi( row[index++] );
		g_TroopInfo[corps][level].upgrade_resnum[2] = atoi( row[index++] );
		g_TroopInfo[corps][level].upgrade_restype[3] = atoi( row[index++] );
		g_TroopInfo[corps][level].upgrade_resnum[3] = atoi( row[index++] );
		g_TroopInfo[corps][level].sec = atoi( row[index++] );
		g_TroopInfo[corps][level].open_level = atoi( row[index++] );
		g_TroopInfo[corps][level].aclass = atoi( row[index++] );
		g_TroopInfo[corps][level].buildingkind = atoi( row[index++] );
	}
	mysql_free_result( res );
	return 0;
}

int troop_corps_info_init()
{
	MYSQL_RES *res;
	MYSQL_ROW row;
	char szSQL[2048];

	// 获得表格中最大的兵种类型
	sprintf( szSQL, "select max(corps) from troop_corps" );
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
			g_TroopCorpsCount = atoi( row[0] ) + 1;
		else
			g_TroopCorpsCount = 0;
	}
	else
	{
		mysql_free_result( res );
		return -1;
	}
	mysql_free_result( res );

	g_TroopCorpsInfo = (TroopCorpsInfo *)malloc( sizeof(TroopCorpsInfo)*g_TroopCorpsCount );
	memset( g_TroopCorpsInfo, 0, sizeof(TroopCorpsInfo)*g_TroopCorpsCount );

	sprintf( szSQL, "select * from troop_corps" );
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
		int corps = atoi( row[index++] );
		if ( corps < 0 )
		{
			mysql_free_result( res );
			return -1;
		}
		else if ( corps >= g_TroopCorpsCount )
			continue;
		index++;
		g_TroopCorpsInfo[corps].corps = corps;
		g_TroopCorpsInfo[corps].sol_modulus = (float)atof( row[index++] );
		g_TroopCorpsInfo[corps].hp_modulus = (float)atof( row[index++] );
		g_TroopCorpsInfo[corps].atk_modulus = (float)atof( row[index++] );
		g_TroopCorpsInfo[corps].def_modulus = (float)atof( row[index++] );
		g_TroopCorpsInfo[corps].crit_modulus = (float)atof( row[index++] );
		g_TroopCorpsInfo[corps].critharm_modulus = (float)atof( row[index++] );
		g_TroopCorpsInfo[corps].hit_modulus = (float)atof( row[index++] );
		g_TroopCorpsInfo[corps].agl_modulus = (float)atof( row[index++] );
		g_TroopCorpsInfo[corps].dodge_modulus = (float)atof( row[index++] );
		g_TroopCorpsInfo[corps].critreduce_modulus = (float)atof( row[index++] );
	}
	mysql_free_result( res );
	return 0;
}

int troop_get_entry_position(short corps, char level, char isenemy)
{
	char corps_type = g_TroopInfo[corps][level].corps_type;
	int position = 0;
	switch (corps_type)
	{
	case 1:
		position = 4;
		break;
	case 2:
		position = 3;
		break;
	case 3:
		position = 0;
		break;
	case 4:
		position = 2;
		break;
	case 5:
		position = 1;
		break;
	default:
		position = 4;
		break;
	}
	if (isenemy)
	{
		position = 21 - position;
	}
	return position;
}

char troop_get_attack_range(short corps, char level)
{
	return g_TroopInfo[corps][level].attack_range;
}

char troop_get_move_range(short corps, char level)
{
	return (char)g_TroopInfo[corps][level].move_grid;
}

int troop_get_hp(short corps, char level)
{
	return g_TroopInfo[corps][level].hp;
}

int troop_get_attacktype(short corps, char level)
{
	return g_TroopInfo[corps][level].attack_type;
}

int troop_get_defancetype(short corps, char level)
{
	return g_TroopInfo[corps][level].defance_type;
}

int troop_get_attackaction(short corps, char level)
{
	switch (g_TroopInfo[corps][level].corps_type)
	{
	case 1:	// 步兵
		return 1;
		break;
	case 2:	// 骑兵
		return 1;
		break;
	case 3:	// 飞兵
		return 1;
		break;
	case 4:	// 法师
		return 3;
		break;
	case 5:	// 弓箭
		return 2;
		break;
	}
	return 1;
}
