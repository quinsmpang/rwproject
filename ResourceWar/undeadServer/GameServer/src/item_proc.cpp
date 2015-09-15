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
#include "actor_send.h"
#include "server_netsend_auto.h"
#include "hero.h"

extern MYSQL *myData;
extern MYSQL *myGame;

extern Actor *g_actors;
extern int g_maxactornum;

extern ItemKind *g_itemkind;
extern int g_itemkindnum;

//-----------------------------------------------------------------------------
// 函数说明: 道具的使用
// 参数    : actor_index - 
//           hero_index	- 英雄index
//           target_index	- 目标index
//-----------------------------------------------------------------------------
int item_use( int actor_index, short itemindex, short itemnum, int hero_index, int target_index )
{
	Actor *pActor = NULL;
	HeroAttr *pHeroAttr = NULL;
	int item_kind = 0;
	short item_type = 0;
	short true_usenum = 0;
	short tmp_ability = 0;
	int eff_result = 0;

	if ( itemindex < 0 || itemindex >= MAX_ACTOR_ITEMNUM )
		return -1;
	if ( actor_index < 0 || actor_index >= g_maxactornum )
		return -1;
	if ( target_index < 0 || target_index >= g_maxactornum )
	{
		target_index = actor_index;
	}

	// 检查物品itemkind
	item_kind = g_actors[actor_index].item[itemindex].m_kind;
	if ( g_actors[actor_index].item[itemindex].m_kind <= 0 )
		return -1;

	// 获取物品类型
	item_type = item_gettype( item_kind );

	// 获取被使用物品的角色的属性
	if ( hero_index < 0 || hero_index >= HERO_MAXCOUNT )
	{ // 主角
		pActor = &g_actors[target_index];
	}
	else 
	{ // 英雄
		pHeroAttr = &g_actors[target_index].hero[hero_index];
	}

	if ( itemnum == 0 ) // 全用
		true_usenum = g_actors[actor_index].item[itemindex].m_num;
	else if ( itemnum > 0 )
		true_usenum = itemnum;
	else
		return -1;

	// 使用其它物品，物品真实使用数量
	if ( g_actors[actor_index].item[itemindex].m_num < true_usenum )
	{
		true_usenum = g_actors[actor_index].item[itemindex].m_num;
	}
	if ( true_usenum <= 0 )
		return -1;


	// 普通物品根据类型进行不同的使用方式
	if ( item_type == ITEM_TYPE_NORMAL || item_type == ITEM_TYPE_GEM || item_type == ITEM_TYPE_MATERIAL || item_type == ITEM_TYPE_STUFF )
	{ // 普通使用类道具
		int ability1 = item_get_base_ability( g_actors[actor_index].item[itemindex].m_kind, 0 );
		int value1 = item_get_base_value( g_actors[actor_index].item[itemindex].m_kind, 0 );
	}
	else
		return item_sendnotuse( actor_index, itemindex, -2 );

	// 减去数量
	g_actors[actor_index].item[itemindex].m_num -= true_usenum;

	// 记录物品使用日志
	wlog( 0, LOGOP_ITEMLOST, PATH_ITEMUSE, g_actors[actor_index].item[itemindex].m_kind, true_usenum, g_actors[actor_index].item[itemindex].m_itemid, g_actors[actor_index].actorid, g_actors[target_index].actorid );

	// 如果数量为0，那么就删掉
	if ( g_actors[actor_index].item[itemindex].m_num <= 0 )
	{
		item_deletebox( actor_index, itemindex, 0 );
		memset( &g_actors[actor_index].item[itemindex], 0, sizeof(Item) );
	}

	// 发送使用物品结果
	SLK_NetS_ItemUse Value = {};
	Value.m_itemoffset = itemindex;
	Value.m_usenum = true_usenum;
	Value.m_effres = eff_result;
	netsend_itemuse_S( actor_index, SENDTYPE_ACTOR, &Value );

	return eff_result;
}

//-----------------------------------------------------------------------------
// 函数说明: 装备单件装备并且如果有前一个装备卸载下前一个装备
// 参数    : actor_index - 
//           hero_index	- 英雄
//           item_index	- 源道具位置
//-----------------------------------------------------------------------------
int item_equipup( int actor_index, short hero_index, short itemindex )
{
	Item *pEquip = NULL;
	Item *pItem = NULL;
	Item tmpItem;
	short equip_target = -1;
	int eff_result = 0;
	char is_send = FALSE;
	short attrlevel = 0;
	// 合法性检查
	if ( actor_index < 0 || actor_index >= g_maxactornum )
		return -1;
	// 源道具不在背包里就返回
	if ( itemindex >= 0 && itemindex < MAX_ACTOR_ITEMNUM )
		pItem = &g_actors[actor_index].item[itemindex];
	else
		return -1;

	// 装备类型决定装备索引位置
	equip_target = item_gettype( pItem->m_kind ) - 1;
	if ( equip_target < 0 || equip_target >= MAX_ACTOR_EQUIPNUM )
		return -1;

	if ( hero_index >= 0 && hero_index < HERO_MAXCOUNT )
	{ // 给英雄穿
		if ( g_actors[actor_index].hero[hero_index].m_herokind <= 0 )
		{
			return -1;
		}
		attrlevel = g_actors[actor_index].hero[hero_index].m_level;
		equip_target += hero_index*MAX_ACTOR_EQUIPNUM;
		pEquip = &g_actors[actor_index].hero_equip[equip_target];
	}
	else
	{ // 给主角穿
		attrlevel = g_actors[actor_index].level;
		pEquip = &g_actors[actor_index].equip[equip_target];
	}

	// 等级检查
	short item_level = item_getlevel( pItem->m_kind );
	if ( attrlevel < item_level )
	{
		// 等级不够装备
		return -1;
	}

	// 交换道具栏和装备栏的道具
	memcpy( &tmpItem, pItem, sizeof(Item) );
	memcpy( pItem, pEquip, sizeof(Item) );
	memcpy( pEquip, &tmpItem, sizeof(Item) );
	
	// 遍历所有的属性检查是否有信息改变
	//eff_result = equip_change_value( pitem, &g_actors[actor_index].equip[equip_target], g_actors[actor_index].pattr[member], 1 );

	//if ( (eff_result & EFF_LIFE_CHANGED) || (eff_result & EFF_MAGIC_CHANGED) || (eff_result & EFF_STAT_DRUNK) )
	//{
	//	actor_changelife( member, actor_index );
	//}
	//if ( eff_result & EFF_ALL_CHANGED )
	//{
	//	attr_calc( g_actors[actor_index].pattr[member], 0, NULL, NULL );
	//	actors_getinfo( actor_index, member );
	//}
	//else if ( eff_result & EFF_ATTR_CHANGED )
	//{
	//	actors_modifyinfo( member, actor_index );
	//}
	//if ( eff_result & EFF_SULT_CHANGED )
	//{
	//	equip_info_all( actor_index, member );
	//}

	SLK_NetS_ItemUse Value = {};
	Value.m_itemoffset = itemindex;
	Value.m_usenum = equip_target;
	Value.m_effres = 0;
	netsend_itemuse_S( actor_index, SENDTYPE_ACTOR, &Value );

	//if ( is_send )
		//item_info( actor_index, EQUIP_OFFSETBASE + equip_target );

	return 0;
}

//-----------------------------------------------------------------------------
// 函数说明: 放置道具到格子( 卸下装备或者交换道具格 )
// 参数    : actor_index - 
//           item_resource	- 源位置
//           item_target	- 目标位置
//-----------------------------------------------------------------------------
int item_put( int actor_index, short item_resource, short item_target )
{
	Item tmpItem;
	int eff_result = 0;
	short item_type;

	short res_num = -1;
	short target_num = -1;
	char needdown = 1;
	int member = -1;

	Item * pSrc = NULL;
	Item * pDes = NULL;

	if ( actor_index < 0 || actor_index >= g_maxactornum )
		return -1;
	if ( item_target < 0 || item_target >= MAX_ACTOR_ITEMNUM )
	{
		actor_notify( actor_index, NOTIFY_TEXTTABLE, "510" ); // 背包已满
		return -1;
	}

	// 检查背包格子数量
	int max_itemnum = MAX_DEFAULT_ITEMNUM + g_actors[actor_index].itemext;
	if ( max_itemnum > MAX_ACTOR_ITEMNUM )
		max_itemnum = MAX_ACTOR_ITEMNUM;

	// 源位置是装备栏，那么这里面进行的操作就是卸下装备
	if ( item_resource >= EQUIP_OFFSETBASE && item_resource <= HEROEQUIP_OFFSETBASE + MAX_ACTOR_EQUIPNUM*HERO_MAXCOUNT )
	{
		// 目标只能是背包
		if ( item_target >= 0 && item_target < max_itemnum )
		{
			// 源-装备栏的装备
			pSrc = item_getptr( actor_index, item_resource );
			if ( pSrc == NULL )
				goto SENDITEMPUT;

			// 在确定一下是不是装备
			item_type = item_gettype( pSrc->m_kind );
			if ( item_type < ITEM_TYPE_EQUIP1 || item_type > ITEM_TYPE_EQUIP10 )
				goto SENDITEMPUT;

			// 目标-背包里的道具
			pDes = &g_actors[actor_index].item[item_target];

			// 如果目标位置有道具，不能进行交换，也就是说卸下不能顶替
			if ( pDes->m_kind > 0 )
				goto SENDITEMPUT;

			
			//// 遍历所有的属性
			//eff_result = equip_change_value( pSrc, pDes, g_actors[actor_index].pattr[member], needdown );

			//if ( (eff_result & EFF_LIFE_CHANGED) || (eff_result & EFF_MAGIC_CHANGED) || (eff_result & EFF_STAT_DRUNK) )
			//	actor_changelife( -1, actor_index );
			//if ( eff_result & EFF_ALL_CHANGED )
			//{
			//	attr_calc( g_actors[actor_index].pattr[member], 0, NULL, NULL );
			//	actors_getinfo( actor_index, member );
			//}
			//else if ( eff_result & EFF_ATTR_CHANGED )
			//	actors_modifyinfo( member, actor_index );
			//else if ( eff_result & EFF_STAT_CAR )
			//	actor_car_sendequip( actor_index, member );
			

			memcpy( &tmpItem, pSrc, sizeof(Item) );
			memcpy( pSrc, pDes, sizeof(Item) );
			memcpy( pDes, &tmpItem, sizeof(Item) );

			res_num = pSrc->m_num;
			target_num = pDes->m_num;
		}
		else
		{
			actor_notify( actor_index, NOTIFY_TEXTTABLE, "510" ); // 背包已满
			return -1;
		}

	}
	// 源物品是 背包物品
	else if ( item_resource >= 0 && item_resource < MAX_ACTOR_ITEMNUM && g_actors[actor_index].item[item_resource].m_kind > 0 )
	{
		pSrc = &g_actors[actor_index].item[item_resource]; // 源是背包物品

		if ( item_target >= 0 && item_target < max_itemnum )
			pDes = &g_actors[actor_index].item[item_target]; // 目标是背包物品

		if ( pSrc->m_kind != pDes->m_kind )
		{ 
			// 不是同一种物品，就交换
			memcpy( &tmpItem, pSrc, sizeof(Item) );
			memcpy( pSrc, pDes, sizeof(Item) );
			memcpy( pDes, &tmpItem, sizeof(Item) );

			res_num = pSrc->m_num;
			target_num = pDes->m_num;
		}
		else
		{
			// 如果是同样的物品,则添加堆叠的功能
			int diff = 0;
			int kind = pSrc->m_kind;
			if ( g_itemkind[kind].m_overlap <= 1 )
			{// 当overlap <= 1 时，表示不能堆叠，对物品进行简单的交换即可。
				memcpy( &tmpItem, pSrc, sizeof(Item) );
				memcpy( pSrc, pDes, sizeof(Item) );
				memcpy( pDes, &tmpItem, sizeof(Item) );

				res_num = pSrc->m_num;
				target_num = pDes->m_num;
			}
			else
			{
				kind = pSrc->m_kind;
				diff = g_itemkind[kind].m_overlap - pDes->m_num; // 差额
				if ( diff < pSrc->m_num )
				{ // 原物品够支付目标物品的差额
					res_num = pSrc->m_num - diff;
					target_num = pDes->m_num + diff;
					pSrc->m_num = res_num;
					pDes->m_num = target_num;
				}
				else
				{ // 要删掉一个物品了
					diff = pSrc->m_num;
					res_num = pSrc->m_num - diff;
					target_num = pDes->m_num + diff;
					pSrc->m_num = res_num;
					pDes->m_num = target_num;
					item_deletebox( actor_index, item_resource, 0 );
				}
			}
		}
	}
	else
	{
		res_num = -1;
		target_num = -1;
	}

SENDITEMPUT:
	SLK_NetS_ItemPut Value = {};
	Value.m_res_offset = item_resource;
	Value.m_res_num = res_num;
	Value.m_target_offset = item_target;
	Value.m_target_num = target_num;
	netsend_itemput_S( actor_index, SENDTYPE_ACTOR, &Value );

	//if ( eff_result & EFF_SULT_CHANGED )
	//	equip_info_all( actor_index, member );

	return 0;
}

//-----------------------------------------------------------------------------
// 函数说明: 丢弃道具
// 参数    : actor_index - 
//           itemindex	- 位置
//           dropcount	- 数量
//-----------------------------------------------------------------------------
int item_drop( int actor_index, short itemindex, short dropcount )
{
	Item * pitem = NULL;
	int itemtype;
	int price;

	if ( actor_index < 0 || actor_index >= g_maxactornum )
		return -1;

	if ( itemindex >= 0 && itemindex < MAX_ACTOR_ITEMNUM )
		pitem = &g_actors[actor_index].item[itemindex];
	else
		return -1;

	itemtype = item_gettype( pitem->m_kind );
	// 任务道具不可丢弃
	//if ( itemtype == ITEM_TYPE_QUEST )
	//{
	//	actor_notify( actor_index, NOTIFY_TEXTTABLE, "511" ); // 此类物品不可丢弃
	//	return -1;
	//}

	if ( dropcount > pitem->m_num )
	{
		dropcount = pitem->m_num;
	}
	price =  item_getprice( pitem->m_kind );
	if ( price <= 0 )
		price = 1;
	price *= dropcount;

	// 记录log
	wlog( 0, LOGOP_ITEMLOST, PATH_SELL, pitem->m_kind, pitem->m_num, pitem->m_itemid, g_actors[actor_index].actorid, dropcount );

	pitem->m_num -= dropcount;
	// 如果数量为0，那么就删掉
	if ( pitem->m_num <= 0 )
	{
		item_deletebox( actor_index, itemindex, 0 );
		memset( &g_actors[actor_index].item[itemindex], 0, sizeof(Item) );
	}
	// 发送角色失去物品
	item_sendlost( actor_index, -1, itemindex, dropcount, PATH_SELL );
	return 0;
}

// 整理背包使用，判断顺序
static unsigned int item_get_order( int item_type, short item_level )
{
	unsigned int rt = 0;
	int order_rule[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8 };
	int order_count;
	int tmpi;

	if ( item_type < 0 )
		return 0xffffffff; // 如果没有,是个空格子,则返回最大数

	order_count = sizeof(order_rule) / sizeof(order_rule[0]);
	for ( tmpi = 0; tmpi < order_count; tmpi++ )
	{
		if ( item_type == order_rule[tmpi] )
		{
			rt = tmpi;
			break;
		}
	}
	if ( tmpi >= order_count )
	{
		rt = order_count + 1 + item_type;
	}
	return (rt << 16) | item_level;
}

// 背包道具合并
static int item_find_nextkind( Item *pItem, int itemkind, int curindex, int max_itemnum )
{
	if ( pItem == NULL || curindex >= max_itemnum || curindex < 0 )
		return -1;
	for ( int tmpi = curindex; tmpi < max_itemnum; tmpi++ )
	{
		if ( pItem[tmpi].m_num > 0 && pItem[tmpi].m_kind == itemkind )
		{
			return tmpi;
		}
	}
	return -1;
}

int item_packeg_in( int actor_index )
{
	if ( actor_index < 0 || actor_index >= g_maxactornum )
		return -1;
	Item *pItem = g_actors[actor_index].item;

	int max_itemnum = MAX_DEFAULT_ITEMNUM + g_actors[actor_index].itemext;

	if ( max_itemnum > MAX_ACTOR_ITEMNUM )
		max_itemnum = MAX_ACTOR_ITEMNUM;

	int overlap;
	int curindex;
	int nextindex;
	char haschange;
	int value[2];
	for ( int tmpi = 0; tmpi < max_itemnum; tmpi++ )
	{
		if ( pItem[tmpi].m_num <= 0 || pItem[tmpi].m_kind <= 0 )
			continue;
		overlap = item_get_overlap( pItem[tmpi].m_kind );
		if ( overlap <= 1 || pItem[tmpi].m_num >= overlap )
			continue;
		curindex = tmpi + 1;
		haschange = 0;
		while ( curindex < max_itemnum )
		{
			nextindex = item_find_nextkind( pItem, pItem[tmpi].m_kind, curindex, max_itemnum );
			if ( nextindex < curindex || nextindex >= max_itemnum )
				break;
			if ( pItem[nextindex].m_num + pItem[tmpi].m_num >= overlap )
			{
				haschange = 1;
				pItem[nextindex].m_num = pItem[nextindex].m_num + pItem[tmpi].m_num - overlap;
				pItem[tmpi].m_num = overlap;
				if ( pItem[nextindex].m_num <= 0 )
				{
					item_deletebox( actor_index, nextindex, 0 );
					pItem[nextindex].m_kind = 0;
				}
				value[0] = nextindex;
				value[1] = pItem[nextindex].m_num;
				actor_notify_value( actor_index, NOTIFY_ITEMNUM, 2, value, NULL );
			}
			else
			{
				pItem[tmpi].m_num += pItem[nextindex].m_num;
				haschange = 1;

				item_deletebox( actor_index, nextindex, 0 );
				pItem[nextindex].m_kind = 0;
				pItem[nextindex].m_num = 0;

				value[0] = nextindex;
				value[1] = pItem[nextindex].m_num;
				actor_notify_value( actor_index, NOTIFY_ITEMNUM, 2, value, NULL );
			}
			curindex = nextindex + 1;
		}
		if ( haschange )
		{
			value[0] = tmpi;
			value[1] = pItem[tmpi].m_num;
			actor_notify_value( actor_index, NOTIFY_ITEMNUM, 2, value, NULL );
		}
	}
	return 0;
}

//-----------------------------------------------------------------------------
// 函数说明: 装备整理
// 参数    : actor_index - 
//           itemindex	- 位置
//           dropcount	- 数量
//-----------------------------------------------------------------------------
int item_settle( int actor_index )
{
	Item tmp_item = { 0 };
	int new_index[MAX_ACTOR_ITEMNUM] = { 0 };
	int tmp_index = 0;
	int tmpi, tmpj;
	Item *pItemBase = NULL;

	if ( actor_index < 0 || actor_index >= g_maxactornum )
		return -1;

	pItemBase = g_actors[actor_index].item;

	item_packeg_in( actor_index );

	int max_itemnum = MAX_DEFAULT_ITEMNUM + g_actors[actor_index].itemext;
	if ( max_itemnum > MAX_ACTOR_ITEMNUM )
		max_itemnum = MAX_ACTOR_ITEMNUM;

	for ( tmpi = 0; tmpi < max_itemnum; tmpi++ )
	{
		new_index[tmpi] = tmpi;
	}

	for ( tmpi = 0; tmpi < max_itemnum; tmpi++ )
	{
		for ( tmpj = 0; tmpj < max_itemnum - 1; tmpj++ )
		{
			int item_kind_1 = 0, item_kind_2 = 0;
			int item_type_1 = -1, item_type_2 = -1;
			short item_level_1 = 0, item_level_2 = 0;
			unsigned order_1 = 0, order_2 = 0;

			item_kind_1 = pItemBase[tmpj].m_kind;
			if ( item_kind_1 > 0 )
			{
				item_type_1 = item_gettype( item_kind_1 );
				item_level_1 = item_getlevel( item_kind_1 );
			}
			item_kind_2 = pItemBase[tmpj + 1].m_kind;
			if ( item_kind_2 > 0 )
			{
				item_type_2 = item_gettype( item_kind_2 );
				item_level_2 = item_getlevel( item_kind_2 );
			}
			order_1 = item_get_order( item_type_1, item_level_1 );
			order_2 = item_get_order( item_type_2, item_level_2 );
			if ( (order_1 > order_2) ||
				(order_1 == order_2 && item_kind_1 > item_kind_2) )
			{ // 要调换位置了
				tmp_item = pItemBase[tmpj];
				pItemBase[tmpj] = pItemBase[tmpj + 1];
				pItemBase[tmpj + 1] = tmp_item;

				tmp_index = new_index[tmpj];
				new_index[tmpj] = new_index[tmpj + 1];
				new_index[tmpj + 1] = tmp_index;
			}
		}
	}

	SLK_NetS_ItemSettle Value = {};
	Value.m_itemnum = max_itemnum;
	for ( tmpi = 0; tmpi < max_itemnum; tmpi++ )
	{
		Value.m_itemoffset[tmpi] = new_index[tmpi];
	}
	netsend_itemsettle_S( actor_index, SENDTYPE_ACTOR, &Value );

	return 0;
}


