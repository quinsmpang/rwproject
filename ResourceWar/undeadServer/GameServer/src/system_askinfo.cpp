#include <stdio.h>
#include <time.h>
#include "system_askinfo.h"
#include "actor.h"
#include "gameproc.h"
#include "utils.h"
#include "db.h"
#include "client.h"
#include "building.h"
#include "city.h"
#include "area.h"
#include "worldmap.h"
#include "mapunit.h"
#include "army.h"
#include "actor_notify.h"
#include "server_structsend_auto.h"
#include "system.h"
#include "actor_send.h"
#include "item_send.h"
#include "item.h"
#include "hero.h"

extern Actor *g_actors;
extern int g_maxactornum;
extern SConfig g_Config;

extern City *g_city;
extern int g_city_maxcount;
extern int g_city_maxindex;

extern Army *g_Army;
extern int g_maxArmyNum;

extern MapUnit *g_mapunit;
extern int g_mapunit_maxcount;

// 客户端脚本直接发来的消息
int system_askinfo( int actor_index, int msgid, char *pstr, int *pvalue )
{
	if ( actor_index < 0 || actor_index >= g_maxactornum )
		return -1;
	switch ( msgid )
	{
	case ASKINFO_NORMAL:
		break;
	case ASKINFO_BUILDING: // 建筑操作
		if ( pvalue[0] == 0 )
		{// 建造
			short buildingindex = (short)pvalue[1];
			City *pCity = city_getptr( actor_index );
			int ret = building_create( pCity, buildingindex, (short)pvalue[2] );
			if ( ret == -1 )
				city_building_baseinfo( actor_index, buildingindex, &pCity->building[buildingindex] );
			else if ( ret == -2 )
				city_info( actor_index );
			else if ( ret == -3 )
				actor_notify( actor_index, NOTIFY_TEXTTABLE, "601" ); // 只能同时建造或升级2个建筑
			else
			{
				city_building_baseinfo( actor_index, buildingindex, &pCity->building[buildingindex] );
				city_building_buildtimeinfo( actor_index, buildingindex, &pCity->building[buildingindex] );
			}
		}
		else if ( pvalue[0] == 1 )
		{// 升级
			int ret = building_upgrade( city_getptr( actor_index ), (short)pvalue[1] );
			if ( ret == -3 )
				actor_notify( actor_index, NOTIFY_TEXTTABLE, "601" ); // 只能同时建造或升级2个建筑
		}
		else if ( pvalue[0] == 2 )
		{// 删除
			int ret = building_delete( city_getptr( actor_index ), (short)pvalue[1] );
			if ( ret == -3 )
				actor_notify( actor_index, NOTIFY_TEXTTABLE, "601" ); // 只能同时建造或升级2个建筑
		}
		else if ( pvalue[0] == 3 )
		{// 移动
			building_move( city_getptr( actor_index ), (short)pvalue[1], (unsigned char)pvalue[2], (unsigned char)pvalue[3] );
		}
		else if ( pvalue[0] == 4 )
		{ // 采集收取
			building_gather( city_getptr( actor_index ), (short)pvalue[1] );
		}
		else if ( pvalue[0] == 6 )
		{ // 立即完成
			building_instantly_finish( city_getptr( actor_index ), (short)pvalue[1] );
		}
		else if ( pvalue[0] == 7 )
		{ // 建筑属性列表
			city_building_abilitylist( actor_index, city_getptr( actor_index ) );
		}
		break;
	case ASKINFO_WORLDMAP: // 世界地图操作
		if ( pvalue[0] == 1 )
		{
			worldmap_sendinfo( actor_index );
			//if ( city_match_getptr( actor_index ) == NULL )
			//	worldmap_sendinfo( actor_index );
			//else
			//	outsidemap_sendinfo( actor_index );
		}
		else if ( pvalue[0] == 2 )
		{ // 侦查
		}
		else if ( pvalue[0] == 3 )
		{ // 攻击
			//已修改为单独命令
			//city_battle(actor_index, city_getptr(actor_index), pvalue[1], pvalue[2], pvalue[3]);
		}
		else if ( pvalue[0] == 4 )
		{ // 迁城
			city_move( actor_index, pvalue[1], pvalue[2] );
		}
		else if ( pvalue[0] == 5 )
		{ // 占领
			//已修改为单独命令
			//city_battle(actor_index, city_getptr(actor_index), pvalue[1], pvalue[2], pvalue[3]);

		}
		break;
	case ASKINFO_AREAENTER: // 区域进入
		if ( g_actors[actor_index].view_areaindex != pvalue[0] )
		{
			view_area_change( actor_index, pvalue[0] );
			g_actors[actor_index].view_areaindex = pvalue[0];
		}

		break;
	case ASKINFO_CHATCATCH:
		if ( pvalue[0] == 1 )
		{
			//最新的5条
			//获取频道的数据

			if ( pvalue[1] == 0 )
			{	// 世界频道 
				int endIndex = talk_cache_queue_end( 0 );
				for ( int tmpi = endIndex - 4; tmpi <= endIndex; tmpi++ )
				{
					SLK_NetS_Talk *pValue = talk_cache_queue_get( 0, tmpi );
					if ( pValue != NULL )
						netsend_talk_S( actor_index, SENDTYPE_ACTOR, pValue );
				}
			}
			else if ( pvalue[1] == 1 )
			{	// 联盟频道
				int endIndex = talk_cache_queue_end( 1 );
				for ( int tmpi = endIndex - 4; tmpi <= endIndex; tmpi++ )
				{
					SLK_NetS_Talk *pValue = talk_cache_queue_get( 1, tmpi );
					if ( pValue != NULL )
						netsend_talk_S( actor_index, SENDTYPE_ACTOR, pValue );
				}
			}
		}
		if ( pvalue[0] == 2 )
		{
			//历史的 5条
			if ( pvalue[1] == 0 )
			{
				//世界频道
				int endIndex = talk_cache_queue_sendtime_index( 0, pvalue[2] );
				for ( int tmpi = endIndex - 5; tmpi < endIndex; tmpi++ )
				{
					SLK_NetS_Talk *pValue = talk_cache_queue_get( 0, tmpi );
					if ( pValue != NULL )
						netsend_talk_S( actor_index, SENDTYPE_ACTOR, pValue );
				}
			}
			else if ( pvalue[1] == 1 )
			{
				//联盟频道
				int endIndex = talk_cache_queue_sendtime_index( 1, pvalue[2] );
				for ( int tmpi = endIndex - 5; tmpi < endIndex; tmpi++ )
				{
					SLK_NetS_Talk *pValue = talk_cache_queue_get( 1, tmpi );
					if ( pValue != NULL )
						netsend_talk_S( actor_index, SENDTYPE_ACTOR, pValue );
				}
			}
		}
		break;
	case ASKINFO_CHAT:

		break;
	case ASKINFO_ACTOR:
		if ( pvalue[0] == 1 )
		{
			// 增加聊天屏蔽
			actor_talk_chat_mask_list_add( actor_index, pvalue[1] );
		}
		else if ( pvalue[0] == 2 )
		{
			//取消聊天屏蔽
			actor_talk_chat_mask_list_del( actor_index, pvalue[1] );
		}
		break;
	case ASKINFO_STORAGE:
		if ( pvalue[0] == 1 )
		{
			//发送玩家背包列表
			item_list( actor_index );
		}
		else if ( pvalue[0] == 2 )
		{
			//查询单个道具的详细信息
			item_info( actor_index, pvalue[1] );
		}
		else if ( pvalue[0] == 3 )
		{
			//使用某个道具
			item_use( actor_index, pvalue[1], pvalue[2], pvalue[3], -1 );
		}
		else if ( pvalue[0] == 4 )
		{
			//背包 丢弃某个道具

		}
		break;
	case ASKINFO_HERO:
		if ( pvalue[0] == 1 )
		{
			//装备道具
			item_equipup( actor_index, pvalue[1], pvalue[2] );
		}
		if ( pvalue[0] == 2 )
		{
			//英雄列表
			hero_list( actor_index );
		}
		if ( pvalue[0] == 3 )
		{
			//英雄详细信息
			hero_info( actor_index, pvalue[1] );
		}
		if ( pvalue[0] == 4 )
		{
			//item_equip_list()
			//英雄的装备信息
			item_equip_list( actor_index, 0 );
		}
		break;
	case ASKINFO_TROOP:
		if( pvalue[0] == 0 )
		{
			// 训练,升级
			building_troop_train( actor_index, pvalue[1], pvalue[2], pvalue[3], pvalue[4], pvalue[5] );
		}

		else if ( pvalue[0] == 1 )
		{
			// 取消训练,升级
			building_troop_traincancel(actor_index, pvalue[1]);
		}
		else if ( pvalue[0] == 2 )
		{
			// 立即完成训练,升级
			building_troop_trainfinish(actor_index, pvalue[1]);
		}
		else if ( pvalue[0] == 3 )
		{
			// 解雇
			building_troop_abandon( actor_index, pvalue[1], pvalue[2], pvalue[3] );
		}
		break;
	}
	return 0;
}
