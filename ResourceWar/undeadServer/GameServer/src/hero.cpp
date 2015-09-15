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
#include "army.h"
#include "server_netsend_auto.h"
#include "actor_send.h"
#include "item.h"

extern SConfig g_Config;
extern MYSQL *myGame;
extern MYSQL *myData;

extern Actor *g_actors;
extern int g_maxactornum;

HeroInfo *g_HeroInfo = NULL;
short g_HeroCount = 0;


// 英雄初始化信息
int hero_info_init()
{
	MYSQL_RES *res;
	MYSQL_ROW row;
	char szSQL[2048];

	char corps_text[64] = { 0 };
	char skill_text[64] = { 0 };
	char **corps = NULL;
	char **skill = NULL;
	int corps_count = 0;
	int skill_count = 0;

	// 获得表格中最大的id
	sprintf( szSQL, "select max(kind) from hero" );
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
			g_HeroCount = atoi( row[0] ) + 1;
		else
			g_HeroCount = 0;
	}
	else
	{
		mysql_free_result( res );
		return -1;
	}
	mysql_free_result( res );

	// 分配空间
	g_HeroInfo = (HeroInfo *)malloc( sizeof(HeroInfo)*g_HeroCount );
	memset( g_HeroInfo, 0, sizeof(HeroInfo)*g_HeroCount );

	sprintf( szSQL, "select * from hero" );
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
		int kind = atoi( row[index++] );
		if ( kind < 0 )
		{
			mysql_free_result( res );
			return -1;
		}
		else if ( kind >= g_HeroCount )
			continue;

		memcpy( g_HeroInfo[kind].name, row[index++], sizeof(char)*NAME_SIZE );
		g_HeroInfo[kind].shape = atoi( row[index++] );
		g_HeroInfo[kind].per_attack = atoi( row[index++] );
		g_HeroInfo[kind].per_defance = atoi( row[index++] );
		g_HeroInfo[kind].per_agility = atoi( row[index++] );
		g_HeroInfo[kind].per_hp = atoi( row[index++] );
		g_HeroInfo[kind].growth_base = (float)atof( row[index++] );

		/* 兵种适应性组 */
		memcpy( corps_text, row[index++], 63 );
		corps = u_strcut_ex( corps_text, '|', &corps_count );
		g_HeroInfo[kind].corps = (short *)malloc( sizeof(short)*corps_count );
		memset( g_HeroInfo[kind].corps, 0, sizeof(short)*corps_count );
		for ( int tmpi = 0; tmpi < corps_count; tmpi++ )
		{
			g_HeroInfo[kind].corps[tmpi] = (short)atoi( corps[tmpi] );
		}
		g_HeroInfo[kind].corps_count = (char)corps_count;

		/* 技能组 */
		memcpy( skill_text, row[index++], 63 );
		skill = u_strcut_ex( skill_text, '|', &skill_count );
		g_HeroInfo[kind].skill = (short *)malloc( sizeof(short)*skill_count );
		memset( g_HeroInfo[kind].skill, 0, sizeof(short)*skill_count );
		for ( int tmpi = 0; tmpi < skill_count; tmpi++ )
		{
			g_HeroInfo[kind].skill[tmpi] = (short)atoi( skill[tmpi] );
		}
		g_HeroInfo[kind].skill_count = (char)skill_count;
	}
	mysql_free_result( res );
	return 0;
}

/* 英雄获取属性 */
HeroAttr *actor_hero_getattr( int actor_index, int offset )
{
	if ( actor_index < 0 || actor_index >= g_maxactornum )
		return NULL;
	if ( offset < 0 || offset >= HERO_MAXCOUNT )
		return NULL;
	return &g_actors[actor_index].hero[offset];
}

/* 玩家获取一个英雄 */
int actor_gethero( int actor_index, int herokind, char path )
{
	ACTOR_CHECK_INDEX( actor_index );
	if ( herokind <= 0 || herokind >= g_HeroCount )
		return -1;
	for ( int offset = 0; offset < HERO_MAXCOUNT; offset++ )
	{
		if ( g_actors[actor_index].hero[offset].m_herokind <= 0 )
		{
			g_actors[actor_index].hero[offset].m_actorid = g_actors[actor_index].actorid;
			g_actors[actor_index].hero[offset].m_offset = offset;
			g_actors[actor_index].hero[offset].m_herokind = herokind;
			memcpy( g_actors[actor_index].hero[offset].m_name, g_HeroInfo[herokind].name, sizeof(char)*NAME_SIZE );
			g_actors[actor_index].hero[offset].m_shape = g_HeroInfo[herokind].shape;
			g_actors[actor_index].hero[offset].m_level = 1;
			g_actors[actor_index].hero[offset].m_experience = 0;

			// 技能这里以创建时候的为准，存数据库
			for ( int skillindex = 0; skillindex < g_HeroInfo[herokind].skill_count; skillindex++ )
			{
				g_actors[actor_index].hero[offset].m_skill[skillindex] = g_HeroInfo[herokind].skill[skillindex];
			}

			// 保存到数据库
			actor_hero_save_auto( &g_actors[actor_index].hero[offset], "actor_hero", NULL );

			// 记录LOG
			wlog( 0, LOGOP_HERO, path, herokind, 0, 0, g_actors[actor_index].actorid, 0 );
			break;
		}
	}
	return 0;
}


//发送给客户端 英雄列表
int actor_hero_list( int actor_index )
{
	if ( actor_index < 0 || actor_index >= g_maxactornum )
		return -1;
	SLK_NetS_HeroList info = { 0 };
	info.m_count = 0;
	for ( int tmpi = 0; tmpi < HERO_MAXCOUNT; tmpi++ )
	{
		if ( g_actors[actor_index].hero[tmpi].m_offset >= 0 && g_actors[actor_index].hero[tmpi].m_herokind > 0 )
		{
			info.m_list[info.m_count].m_herokind = g_actors[actor_index].hero[tmpi].m_herokind;
			info.m_list[info.m_count].m_level = g_actors[actor_index].hero[tmpi].m_level;
			info.m_list[info.m_count].m_official = g_actors[actor_index].hero[tmpi].m_official;
			info.m_list[info.m_count].m_offset = g_actors[actor_index].hero[tmpi].m_offset;
			info.m_count = info.m_count + 1;
		}
	}
	netsend_heroinfolist_S( actor_index, SENDTYPE_ACTOR, &info );

	return 0;
}

//发现详细的英雄信息
int actor_hero_info( int actor_index, int offset )
{
	if ( actor_index < 0 || actor_index >= g_maxactornum )
		return -1;

	if ( offset < 0 || offset >= HERO_MAXCOUNT )
	{
		return -1;
	}

	if ( g_actors[actor_index].hero[offset].m_offset < 0 || g_actors[actor_index].hero[offset].m_herokind <= 0 )
	{
		return -1;
	}

	SLK_NetS_HeroInfo info = { 0 };
	info.m_agility = g_actors[actor_index].hero[offset].m_agility;
	info.m_attack = g_actors[actor_index].hero[offset].m_attack;
	info.m_defance = g_actors[actor_index].hero[offset].m_defance;

	//获取这个英雄装备的道具
	int beginindex = 0;
	int max_itemnum = 0;
	beginindex = offset*MAX_ACTOR_EQUIPNUM;
	max_itemnum = MAX_ACTOR_EQUIPNUM*HERO_MAXCOUNT;
	Item* pEquip = g_actors[actor_index].hero_equip;
	int equipIndex = 0;
	for ( int tmpi = beginindex; tmpi < beginindex + MAX_ACTOR_EQUIPNUM; tmpi++ )
	{
		if ( pEquip[tmpi].m_kind <= 0 || pEquip[tmpi].m_num <= 0 )
			continue;
		if ( equipIndex >= 6 )
		{
			break;
		}
		info.m_equip[equipIndex] = (short)pEquip[tmpi].m_itemid;
	}

	info.m_experience = g_actors[actor_index].hero[offset].m_experience;
	info.m_hp = g_actors[actor_index].hero[offset].m_hp;

	strcpy( info.m_name, g_actors[actor_index].hero[offset].m_name );

	info.m_offset = g_actors[actor_index].hero[offset].m_offset;

	for ( int tmpi = 0; tmpi < HERO_SKILL_MAX; tmpi++ )
	{
		info.m_skill[tmpi] = g_actors[actor_index].hero[offset].m_skill[tmpi];
	}

	netsend_heroinfo_S( actor_index, SENDTYPE_ACTOR, &info );

	return 0;
}
