#ifdef WIN32
#include <winsock2.h>
#include <windows.h>
#endif
#include <mysql.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include "db.h"
#include "actor.h"
#include "gameproc.h"
#include "item.h"
#include "actor_notify.h"

extern MYSQL *myData;
extern MYSQL *myGame;

extern Actor *g_actors;
extern int g_maxactornum;

extern ItemKind *g_itemkind;
extern int g_itemkindnum;

// 新装备初始化
int equip_new( Item *pitem, int itemkind )
{
	int tmpi;
	int ability_count = 0;
	pitem->m_color_level = 0;
	for ( tmpi = 0; tmpi < ITEM_ABILITY_NUM; tmpi++ )
	{
		// 基础属性
		if ( g_itemkind[itemkind].m_ability[tmpi] != 0 )
		{
			pitem->m_ability[ability_count] = g_itemkind[itemkind].m_ability[tmpi];
			pitem->m_value[ability_count] = g_itemkind[itemkind].m_value_min[tmpi];
			if ( pitem->m_value[ability_count] <= 0 )
				pitem->m_value[ability_count] = 1;
			ability_count++;
		}
	}
	return 0;
}

// 装备创建
int equip_create( int actor_index, int itemkind, int item_offset )
{
	Item *pitem = NULL;
	Actor *pactor = &g_actors[actor_index];
	pitem = item_getptr( actor_index, item_offset );
	if ( pitem == NULL )
		return -1;
	equip_new( pitem, itemkind );
	return 0;
}
