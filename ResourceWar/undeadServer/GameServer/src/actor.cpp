#ifdef WIN32
#include <winsock2.h>
#include <windows.h>
#endif
#include <mysql.h>
#include "db.h"
#include "netserver.h"

#ifndef WIN32
#include <sys/time.h>
#endif

#include "server_netsend_auto.h"
#include "auto_actor.h"
#include "utils.h"
#include "actor.h"
#include "actor_send.h"
#include "wqueue.h"
#include "define.h"
#include "client.h"
#include "account.h"
#include "city.h"
#include "building.h"
#include "hero.h"
#include "item.h"
#include "mail_msg.h"
#include "mapunit.h"
#include "system.h"
#include "actor_notify.h"

Actor *g_actors = NULL;
int g_actornum = 0;
int g_maxactorid = 0;
extern int g_maxactornum;
extern int g_save_wait;
extern MYSQL *myGame;

extern City *g_city;
extern int g_city_maxcount;
extern int g_city_maxindex;

extern SLK_NetS_Talk g_TalkCacheQueue[2][TALK_CACHE_QUEUE_COUNT];
//-----------------------------------------------------------------------------
// actors_init
// 函数说明: 角色数据初始化
//-----------------------------------------------------------------------------
int actors_init()
{
	MYSQL_RES		*res;
	MYSQL_ROW		row;
	char	szSQL[1024];

	sprintf(szSQL, "select max(actorid) from actor");
	if (mysql_query(myGame, szSQL))
	{
		printf("Query failed (%s)\n", mysql_error(myGame));
		write_gamelog("%s", szSQL);
		return -1;
	}
	res = mysql_store_result(myGame);

	if (!((row = mysql_fetch_row(res))))
	{
		mysql_free_result(res);
		return -1;
	}
	if (row[0])
		g_maxactorid = atoi(row[0]) + 1;
	else
		g_maxactorid = 1;

	mysql_free_result(res);

	// 设定不能低于最小的角色ID值
	if (g_maxactorid < MINACTORID)
	{ // 只要第一次就可以了，之后都会读最大的
		g_maxactorid = MINACTORID;
	}

	g_actors = (Actor *)malloc(sizeof(Actor)*g_maxactornum);
	memset(g_actors, 0, sizeof(Actor)*g_maxactornum);
	//printf( "%0.2fMB\n", sizeof(Actor)*g_maxactornum / 1024.0 / 1024.0 );
	printf("Actor  maxcount=%d  memory=%0.2fMB\n", g_maxactornum, (sizeof(Actor)*g_maxactornum) / 1024.0 / 1024.0);
	return 0;
}

//-----------------------------------------------------------------------------
// actors_save_everyone
// 函数说明: 所有在线角色保存
// 参数说明：【type==0 队列保存】 【type==1 立即保存】
//-----------------------------------------------------------------------------
int actors_save_everyone(char type)
{
	int tmpi;
	for (tmpi = 0; tmpi < g_maxactornum; tmpi++)
	{
		if (g_actors[tmpi].actorid > 0)
		{
			if (type == 0)
				actor_send_save(tmpi);
			else
				actor_real_saveall(tmpi, NULL);
		}
	}
	return 0;
}

//-----------------------------------------------------------------------------
// actors_on_core
// 函数说明: 服务器崩溃之后的存档
//-----------------------------------------------------------------------------
void actors_on_core()
{
	int tmpi;
	FILE *fp;
	char szSaveSqlFile[MAX_PATH];

	sprintf(szSaveSqlFile, "savesqlfile.sql");
	fp = fopen(szSaveSqlFile, "wb");
	if (fp == NULL)
		return;
	// 所有角色保存
	for (tmpi = 0; tmpi < g_maxactornum; tmpi++)
	{
		if (g_actors[tmpi].actorid > 0)
		{
			actor_real_saveall(tmpi, fp);
		}
	}
	// 所有城池保存
	city_save(fp);
	fclose(fp);
	return;
}

//-----------------------------------------------------------------------------
// actor_save
// 函数说明: 角色保存数据
//-----------------------------------------------------------------------------
int actor_save(int actor_index, FILE *fp)
{
	// 角色基本信息保存
	actor_save_auto(&g_actors[actor_index], "actor", fp);

	// 角色列表信息保存
	actor_update_list(actor_index);

	// 以下为其它所需数据保存
	// 英雄数据保存
	for (int offset = 0; offset < HERO_MAXCOUNT; offset++)
	{
		if (g_actors[actor_index].hero[offset].m_herokind <= 0)
			continue;
		actor_hero_save_auto(&g_actors[actor_index].hero[offset], "actor_hero", fp);
	}

	// 保存道具
	item_save(actor_index, fp);

	// 这里保存一下他的城池吧，可不保存
	city_single_save(&g_city[g_actors[actor_index].city_index], fp);
	return 0;
}

//-----------------------------------------------------------------------------
// actor_real_saveall
// 函数说明: 角色数据真正开始保存
//-----------------------------------------------------------------------------
int actor_real_saveall(int actor_index, FILE *fp)
{
	// 保存角色属性(慢)
	actor_save(actor_index, fp);
	g_actors[actor_index].savetime = 0;
	g_save_wait = 1;
	return 0;
}

//-----------------------------------------------------------------------------
// actor_send_save
// 函数说明: 发送保存信息到队列
//-----------------------------------------------------------------------------
int actor_send_save(int actor_index)
{
	if (g_save_wait)
	{
		if (add_wqueue(actor_index, WCMD_SAVE, NULL, 0, 0, 1) < 0)
		{
			write_gamelog("add_wqueue full");
			return actor_real_saveall(actor_index, NULL);
		}
		return 0;
	}
	else
	{
		return actor_real_saveall(actor_index, NULL);
	}
}

//-----------------------------------------------------------------------------
// actor_send_remove
// 函数说明: 发送下线信息到队列
//-----------------------------------------------------------------------------
int actor_send_remove(int actor_index)
{
	if (g_actors[actor_index].isexit == 2)
		return -1;
	g_actors[actor_index].isexit = 2;
	if (g_save_wait)
	{
		if (add_wqueue(actor_index, WCMD_LEAVE, NULL, 0, 0, 1) < 0)
		{
			write_gamelog("add_wqueue full");
			return actor_remove(actor_index);
		}
		return 0;
	}
	else
	{
		return actor_remove(actor_index);
	}
}

//-----------------------------------------------------------------------------
// actor_remove
// 函数说明: 角色清理并断开客户端
//-----------------------------------------------------------------------------
int actor_remove(int actor_index)
{
	if (actor_index < 0 || actor_index >= g_maxactornum)
		return -1;
	if (g_actors[actor_index].actorid <= 0)
		return -1;
	if (g_actors[actor_index].isexit == 0)
		return -1;

	// 角色更新在线状态
	actor_update_online(g_actors[actor_index].actorid, 0);
	account_out(actor_index, 0);

	// 保存角色属性(慢)
	actor_save(actor_index, NULL);

	// 城池关联索引清除
	if (g_actors[actor_index].city_index >= 0 && g_actors[actor_index].city_index < g_city_maxcount)
		g_city[g_actors[actor_index].city_index].actor_index = -1;

	// 清理actor_index位置的数据
	memset(&(g_actors[actor_index]), 0, sizeof(Actor));
	g_actors[actor_index].city_index = -1;
	g_actors[actor_index].view_areaindex = -1;

	// 总角色数自减
	g_actornum--;

	// 关闭这个socket
	net_closesock_direct(actor_index);
	g_save_wait = 1;
	return 0;
}

//-----------------------------------------------------------------------------
// actor_update_list
// 函数说明: 更新角色列表数据
//-----------------------------------------------------------------------------
int actor_update_list(int actor_index)
{
	char szSQL[1024];
	int actorid;
	char ActorName[64];

	if (actor_index < 0 || actor_index >= g_maxactornum)
		return -1;
	actorid = g_actors[actor_index].actorid;

	// 插入记录到actor
	db_escape((const char *)g_actors[actor_index].name, ActorName, 0);
	sprintf(szSQL, "update actor_list set name='%s',level='%d',logout_time='%d' where actorid='%d'", ActorName, g_actors[actor_index].level, (int)time(NULL), actorid);
	if (mysql_query(myGame, szSQL))
	{
		printf("Query failed (%s) [%s](%d)\n", mysql_error(myGame), __FUNCTION__, __LINE__);
		write_gamelog("%s", szSQL);
		if (mysql_ping(myGame) != 0)
			db_reconnect_game();
		return -3;
	}
	return 0;
}

//-----------------------------------------------------------------------------
// actor_update_online
// 函数说明: 更新数据库中角色在线情况
//-----------------------------------------------------------------------------
int actor_update_online(int actorid, char isonline)
{
	char szSQL[256];
	sprintf(szSQL, "UPDATE actor_list SET online='%d' WHERE actorid=%d", isonline, actorid);
	if (mysql_query(myGame, szSQL))
	{
		printf("Query failed (%s)\n", mysql_error(myGame));
		write_gamelog("%s", szSQL);
		if (mysql_ping(myGame) != 0)
			db_reconnect_game();
		return -1;
	}
	return 0;
}

//-----------------------------------------------------------------------------
// actor_del
// 函数说明: 清除客户端
//-----------------------------------------------------------------------------
int actor_del(int client_index)
{
	if (client_index < 0)
		return -1;
	if (g_actors[client_index].actorid != 0)
	{
		if (g_actors[client_index].isexit == 0)
			g_actors[client_index].isexit = 1;
		return 0;
	}
	// 退出标志
	return 0;
}

//-----------------------------------------------------------------------------
// actor_delete
// 函数说明: 真正的删除角色
//-----------------------------------------------------------------------------
int actor_delete(i64 userid, int actorid)
{
	char	szSQL[1024];
	char szUserID[21];
	lltoa(userid, szUserID, 10);

	// 如果该角色在线
	if (actor_getindex_withid(actorid) >= 0)
	{
		return -1;
	}

	// 更新数据库
	sprintf(szSQL, "update actor_list set offset='-1' where userid='%s' and actorid='%d'", szUserID, actorid);
	if (mysql_query(myGame, szSQL))
	{
		printf("Query failed (%s) [%s](%d)\n", mysql_error(myGame), __FUNCTION__, __LINE__);
		write_gamelog("%s", szSQL);
		if (mysql_ping(myGame) != 0)
			db_reconnect_game();
		return -1;
	}
	return 0;
}

//-----------------------------------------------------------------------------
// actor_delete_stoptime
// 函数说明: 设置删除标记时间
//-----------------------------------------------------------------------------
int actor_delete_stoptime(i64 userid, int actorid)
{
	MYSQL_RES		*res;
	MYSQL_ROW		row;
	char	szSQL[1024];

	int timedate = 0; // 当前时间
	char szUserID[21];
	int level = 0;
	lltoa(userid, szUserID, 10);

	sprintf(szSQL, "select level from actor_list where userid='%s' and actorid='%d'", szUserID, actorid);
	if (mysql_query(myGame, szSQL))
	{
		printf("Query failed (%s) [%s](%d)\n", mysql_error(myGame), __FUNCTION__, __LINE__);
		write_gamelog("%s", szSQL);
		if (mysql_ping(myGame) != 0)
			db_reconnect_game();
		return -1;
	}
	res = mysql_store_result(myGame);
	if ((row = mysql_fetch_row(res)))
	{
		level = atoi(row[0]);
	}
	else
	{
		mysql_free_result(res);
		return -1;
	}
	mysql_free_result(res);

	if (actor_getindex_withid(actorid) >= 0)
	{
		return -1;
	}

#ifdef _DEBUG
	return actor_delete(userid, actorid);
#else
	if ( level < 20 )
		return actor_delete( userid, actorid );
	else if ( level < 30 )
		timedate = (int)time( NULL ) + (60 * 10); // 30级10分钟
	else if ( level < 50 )
		timedate = (int)time( NULL ) + (60 * 60 * 1); // 50级1小时
	else if ( level < 60 )
		timedate = (int)time( NULL ) + (60 * 60 * 2); // 60级2小时
	else if ( level < 70 )
		timedate = (int)time( NULL ) + (60 * 60 * 3); // 70级3小时
	else if ( level < 80 )
		timedate = (int)time( NULL ) + (60 * 60 * 4); // 80级6小时
	else if ( level < 150 )
		timedate = (int)time( NULL ) + (60 * 60 * 24 * 1); // 80级以上1天
#endif

	sprintf(szSQL, "update actor_list set delete_stoptime='%d' where userid='%s' and actorid='%d'", timedate, szUserID, actorid);
	if (mysql_query(myGame, szSQL))
	{
		printf("Query failed (%s)\n", mysql_error(myGame));
		write_gamelog("%s", szSQL);
		if (mysql_ping(myGame) != 0)
			db_reconnect_game();
		return -1;
	}

	// 更新删除时间
	return timedate;
}

//-----------------------------------------------------------------------------
// actor_delete_cancel
// 函数说明: 放弃删除
//-----------------------------------------------------------------------------
int actor_delete_cancel(int actorid)
{
	char szSQL[1024];
	sprintf(szSQL, "UPDATE actor_list SET delete_stoptime=0 WHERE actorid=%d", actorid);
	if (mysql_query(myGame, szSQL))
	{
		printf("Query failed (%s)\n", mysql_error(myGame));
		write_gamelog("%s", szSQL);
		if (mysql_ping(myGame) != 0)
			db_reconnect_game();
		return -1;
	}
	if (mysql_affected_rows(myGame) == 0)
	{
		// 如果没有更新到任一行,则说明没删除过
		return -1;
	}
	return 0;
}

//-----------------------------------------------------------------------------
// actors_logic
// 函数说明: 所有角色逻辑 5帧一次
//-----------------------------------------------------------------------------
void actors_logic()
{
	int tmpi;
	for (tmpi = 0; tmpi < g_maxactornum; tmpi++)
	{
		if (g_actors[tmpi].actorid > 0)
		{
			if (client_isexited(tmpi) && g_actors[tmpi].isexit <= 0)
				g_actors[tmpi].isexit = 1;
			actor_logic(tmpi);
		}
		// 退出标志
		else if (client_isexited(tmpi))
		{
			net_closesock_direct(tmpi);
		}
	}
}

//-----------------------------------------------------------------------------
// actor_logic
// 函数说明: 角色逻辑 5帧一次
//-----------------------------------------------------------------------------
void actor_logic(int actor_index)
{
	if (g_actors[actor_index].actorid == 0)
		return;
	if (g_actors[actor_index].isexit == 2)
		return;

	// 除GM外，控制加速
	//if ( g_actors[actor_index].admin < 90 && g_actors[actor_index].checkspeed_time == 0 )
	//{
	//	if ( g_actors[actor_index].checkspeed_frame > 0 )
	//		g_actors[actor_index].checkspeed_frame--;
	//	if ( g_actors[actor_index].checkspeed_frame <= 0 )
	//	{
	//		SLK_NetS_Heart Value = {};
	//		int timestamp = 3 + rand() % 4;
	//		Value.m_value[0] = timestamp * 35;	// 给5帧的误差
	//		netsend_heart_S( actor_index, SENDTYPE_ACTOR, &Value );
	//		g_actors[actor_index].checkspeed_time = (int)time( NULL ) + timestamp;
	//	}
	//}

	if (g_actors[actor_index].isexit == 1 /*&&
		(g_actors[actor_index].stat != ACTOR_STAT_FIGHT || g_actors[actor_index].fight_index < 0)*/)
	{
		actor_send_remove(actor_index);
		return;
	}
}

//-----------------------------------------------------------------------------
// actor_change_index
// 函数说明: 重登陆的索引转换
//-----------------------------------------------------------------------------
int actor_change_index(int old_index, int new_index)
{
	ACTOR_CHECK_INDEX(old_index);
	ACTOR_CHECK_INDEX(new_index);
	memcpy(&g_actors[new_index], &g_actors[old_index], sizeof(Actor));
	g_actors[new_index].isexit = 0;

	// 城池索引关联
	g_city[g_actors[old_index].city_index].actor_index = new_index;

	// 清空旧角色
	memset(&(g_actors[old_index]), 0, sizeof(Actor));
	g_actors[old_index].city_index = -1;
	g_actors[old_index].view_areaindex = -1;
	return 0;
}

//-----------------------------------------------------------------------------
// actor_entergame
// 函数说明: 玩家进入游戏
//-----------------------------------------------------------------------------
int actor_entergame(int client_index, int actorid)
{
	// 已经进入游戏的不能再次进入
	if (client_getstat(client_index) >= CLIENT_STAT_ENTERGAME)
		return -1;

	// 验证一下这个角色id是不是属于登陆的这个用户
	if (actor_verifyid(client_getuserid(client_index), actorid) < 0)
		return -1;

	// 这里判断玩家是否在游戏中
	int actor_index = actor_getindex_withid(actorid);
	if (actor_index < 0)
	{
		actor_enterworld(client_index, actorid, -1);
	}
	else
	{
		actor_enterworld(client_index, actorid, actor_index);
	}
	client_setstat(client_index, CLIENT_STAT_ENTERGAME);
	return 0;
}

//-----------------------------------------------------------------------------
// actor_enterworld
// 函数说明: 角色进入游戏世界
//-----------------------------------------------------------------------------
int actor_enterworld(int client_index, int actorid, int actor_index)
{
	ACTOR_CHECK_INDEX(client_index);
	SLK_NetS_EnterInfo Value = {};
	// 如果玩家已经存在
	if (actor_index >= 0)
	{
		// 索引对应数据转换，踢旧玩家
		if (actor_index != client_index)
		{
			account_out(actor_index, 1);
			actor_change_index(actor_index, client_index);
			net_closesock_direct(actor_index);
		}
		// 赋予新连接
		actor_index = client_index;

	}
	else
	{
		// 获取一个空闲的角色索引
		actor_index = client_index;

		if (actor_load(actor_index, actorid) < 0)
			return -1;

		// 角色总数增加
		g_actornum++;
		actor_update_online(actorid, 1);
		write_gamelog("[进入游戏(actor_enterworld)][%s]", g_actors[actor_index].name);
	}

	// 登入log
	g_actors[actor_index].accountid = account_in(actor_index, 0);

	// 发送进入游戏
	Value.m_actorid = g_actors[actor_index].actorid;
	Value.m_data_int = 0;
	Value.m_data_short = 0;
	Value.m_data_char = 0;
	netsend_enterinfo_S(client_index, SENDTYPE_ACTOR, &Value);

	// 角色信息
	actor_getinfo(actor_index);

	// 城市信息
	city_info(actor_index);

	// 以上信息发送之后才可让玩家进入场景

	// 以下是进入场景后发送即可的信息
	// 检查是否有未读邮件
	mail_check_read(actor_index);

	// 发送聊天屏蔽列表
	actor_talk_chat_mask_list(actor_index);

	// 发送最近的聊天信息给他
	SLK_NetS_Talk *pValue = talk_cache_queue_get(0, talk_cache_queue_end(0));
	if (pValue)
		netsend_talk_S(actor_index, SENDTYPE_ACTOR, pValue);

	// 判断用户的城市是否被攻击了
	if ( actor_city_is_underfire( g_actors[actor_index].city_index ) )
	{
		actor_notify( actor_index, NOTIFY_ACTOR, NULL );
	}
	
	return 0;
}

//-----------------------------------------------------------------------------
// actor_load
// 函数说明: 角色进入游戏后读取数据
//-----------------------------------------------------------------------------
int actor_load(int actor_index, int actorid)
{
	ACTOR_CHECK_INDEX(actor_index);
	g_actors[actor_index].actorid = actorid;
	g_actors[actor_index].userid = client_getuserid(actor_index);

	/* 读取玩家信息 */
	actor_load_auto(&g_actors[actor_index], "actor");
	g_actors[actor_index].admin = client_getusertype(actor_index);
	memcpy(g_actors[actor_index].lastip, client_getip(actor_index), 15);
	g_actors[actor_index].language = client_getlanguage(actor_index);

	/* 找到自己的城池 */
	g_actors[actor_index].city_index = city_getindex_withactorid(actorid);
	/* 如果没找到,并且等级为0级，是第一次进入游戏 */
	if (g_actors[actor_index].city_index < 0)
	{
		if (g_actors[actor_index].level == 0)
		{
			actor_new(actor_index);
		}
		else
		{
			// 他的城没了，什么情况
			actor_new(actor_index);
			write_gamelog("city_index < 0 actorid=%d", actorid);
			return -1;
		}
	}

	/* 关联索引 */
	g_city[g_actors[actor_index].city_index].actor_index = actor_index;
	g_city[g_actors[actor_index].city_index].laird_lastlogin = (int)time(NULL);

	/* 初始化一些不需要存档的数据 */
	g_actors[actor_index].view_areaindex = -1;

	/* 其它系统数据读取 */
	// 英雄数据
	actor_hero_load_auto(actor_index, actor_hero_getattr, g_actors[actor_index].actorid, "actor_hero");

	// 道具数据
	item_load(actor_index);
	return 0;
}

//-----------------------------------------------------------------------------
// actor_load
// 函数说明: 第一次进入游戏要初始化角色信息
//-----------------------------------------------------------------------------
int actor_new(int actor_index)
{
	ACTOR_CHECK_INDEX(actor_index);
	/* 所有的非字符串类型的varbin类型在这里初始化为0 */

	/* 初始信息 */
	g_actors[actor_index].level = 1;

	/* 给这个玩家创建一个城池 */
	City city = { 0 };
	city.laird_type = CityLairdType_Player;
	city.laird_actorid = g_actors[actor_index].actorid;
	memcpy(city.laird_name, g_actors[actor_index].name, sizeof(char)*NAME_SIZE);
	city.laird_shape = g_actors[actor_index].shape;
	city.aclass = g_actors[actor_index].aclass;
	city.posx = rand() % 240;
	city.posy = rand() % 240;
	g_actors[actor_index].city_index = city_new(&city);
	mapunit_add(MAPUNIT_TYPE_CITY, g_actors[actor_index].city_index);

	/* 给他创建一个对手城池 */
	City match_city = { 0 };
	match_city.laird_type = CityLairdType_Match;
	match_city.laird_actorid = g_actors[actor_index].actorid;
	memcpy(match_city.laird_name, "Match", sizeof(char)*NAME_SIZE);
	match_city.laird_shape = 1;
	match_city.aclass = 1;
	match_city.posx = city.posx;
	match_city.posy = city.posy;
	int match_cityindex = city_new(&match_city);
	g_city[g_actors[actor_index].city_index].match_cityindex = match_cityindex;
	g_city[g_actors[actor_index].city_index].match_cityid = g_city[match_cityindex].cityid;
	return 0;
}

//-----------------------------------------------------------------------------
// actor_create
// 函数说明: 创建角色
//-----------------------------------------------------------------------------
int actor_create(int client_index, int aclass, char *szActorName)
{
	int result = 0;
	i64 userid;
	int platid;
	char *username;
	if (aclass < 0)
		return -1;
	client_setwait(client_index, 0);

	SLK_ListInfo ListInfo = { 0 };
	/* 获取创建角色所必须的数据库用信息 */
	userid = client_getuserid(client_index);
	platid = client_getplatid(client_index);
	username = client_getusername(client_index);
	strcpy(ListInfo.m_name, szActorName);
	ListInfo.m_aclass = aclass;
	ListInfo.m_offset = 0;
	result = actor_db_create(client_index, platid, userid, username, &ListInfo);
	if (result > 0)
		result = 0;
	/* 发送到客户端创建是否成功 */
	SLK_NetS_Create Value = { 0 };
	Value.m_actorid = ListInfo.m_actorid;
	Value.m_result = result;
	netsend_create_S(client_index, SENDTYPE_ACTOR, &Value);

	/* 测试模式成功直接进入游戏 */

	return 0;
}

//-----------------------------------------------------------------------------
// actor_db_create
// 函数说明: 创建角色,数据库
//-----------------------------------------------------------------------------
int actor_db_create(int client_index, int platid, i64 userid, char *username, SLK_ListInfo *pListInfo)
{
	char szSQL[1024];
	int actorid;
	char ActorName[64];
	int thistime;
	char szUserID[21];
	thistime = (int)time(NULL);
	lltoa(userid, szUserID, 10);

	// 获得角色ID
	actorid = g_maxactorid++;

	// 创建角色初始没有名字
	sprintf( pListInfo->m_name, "%d", actorid );

	// 插入记录到 actor_list
	db_escape((const char *)pListInfo->m_name, ActorName, 0);
	sprintf(szSQL, "insert into actor_list (actorid,platid,userid,username,offset,create_time,name,aclass) \
				   															values('%d','%d','%s','%s','%d','%d','%s','%d')", \
																			actorid, platid, szUserID, username, 0, thistime, ActorName, pListInfo->m_aclass);
	if (mysql_query(myGame, szSQL))
	{
		printf("Query failed (%s) [%s](%d)\n", mysql_error(myGame), __FUNCTION__, __LINE__);
		write_gamelog("%s", szSQL);
		if (mysql_ping(myGame) != 0)
		{
			db_reconnect_game();
			return -1;
		}
		return -3;// 可能是角色重名了
	}

	// 插入记录到 actor
	sprintf(szSQL, "insert into actor (actorid,name,aclass,lastip,createtime) \
				   															values('%d','%s','%d','%s','%d')", \
																			actorid, ActorName, pListInfo->m_aclass, client_getip(client_index), thistime);
	if (mysql_query(myGame, szSQL))
	{
		printf("Query failed (%s) [%s](%d)\n", mysql_error(myGame), __FUNCTION__, __LINE__);
		write_gamelog("%s", szSQL);
		if (mysql_ping(myGame) != 0)
			db_reconnect_game();
		return -4;	// 可能是id错误同名了
	}

	pListInfo->m_actorid = actorid;
	return actorid;
}

// 玩家聊天
int actor_talk(int actor_index, char type, char *szMsg, int msgsize)
{
	ACTOR_CHECK_INDEX(actor_index);
	SLK_NetS_Talk Value = {};
	Value.m_actorid = g_actors[actor_index].actorid;
	strcpy(Value.m_name, g_actors[actor_index].name);
	Value.m_type = type;
	strcpy(Value.m_msg, szMsg);
	Value.m_msg_length = msgsize;
	Value.m_shape = 0;
	Value.m_sendtime = (int)time(NULL);
	if (g_actors[actor_index].admin > 100)
	{
		if (Value.m_msg_length < 128)
		{
			Value.m_msg_length += 5;
			strcat(Value.m_msg, "<gm>");
			netsend_talk_S(actor_index, SENDTYPE_ACTOR, &Value);
		}
	}
	else
	{
		if (type == TALK_CHANNEL_ACTOR_WORLD)
		{
			netsend_talk_S(actor_index, SENDTYPE_WORLD, &Value);
			talk_cache_queue_add(0, &Value);
		}
		else if (type == TALK_CHANNEL_ACTOR_CLUB)
		{
			netsend_talk_S(actor_index, SENDTYPE_CLUB, &Value);
			talk_cache_queue_add(1, &Value);
		}
	}
	return 0;
}

// 聊天屏蔽列表
int actor_talk_chat_mask_list(int actor_index)
{
	ACTOR_CHECK_INDEX(actor_index);
	SLK_NetS_ChatMaskList info = { 0 };
	for (int tmpi = 0; tmpi < CHAT_MASK_LIST_MAXCOUNT; tmpi++)
	{
		if (g_actors[actor_index].chat_mask_list[tmpi] > 0)
		{
			info.m_chat_mask_list[info.m_count] = g_actors[actor_index].chat_mask_list[tmpi];
			info.m_count += 1;
		}
	}
	netsend_chatmasklist_S(actor_index, SENDTYPE_ACTOR, &info);
	return 0;
}
// 增加聊天屏蔽列表
int actor_talk_chat_mask_list_add(int actor_index, int actorid)
{
	ACTOR_CHECK_INDEX(actor_index);
	char success = 0;
	if (g_actors[actor_index].actorid == actorid)
		return 0;

	bool isAdd = false;

	for (int tmpi = 0; tmpi < CHAT_MASK_LIST_MAXCOUNT; tmpi++)
	{
		if (g_actors[actor_index].chat_mask_list[tmpi] == actorid)
		{
			isAdd = true;
			break;
		}
	}

	if (!isAdd)
	{
		for (int tmpi = 0; tmpi < CHAT_MASK_LIST_MAXCOUNT; tmpi++)
		{
			if (g_actors[actor_index].chat_mask_list[tmpi] <= 0)
			{
				g_actors[actor_index].chat_mask_list[tmpi] = actorid;
				success = 1;
				break;
			}
		}
	}
	int value[3] = { 0 };
	value[0] = 1;
	value[1] = actorid;
	value[2] = success;
	actor_notify_value(actor_index, NOTIFY_ACTOR, 3, value, NULL);
	return 0;
}
// 取消聊天屏蔽列表
int actor_talk_chat_mask_list_del(int actor_index, int actorid)
{
	ACTOR_CHECK_INDEX(actor_index);
	char success = 0;
	if (g_actors[actor_index].actorid == actorid)
		return 0;
	for (int tmpi = 0; tmpi < CHAT_MASK_LIST_MAXCOUNT; tmpi++)
	{
		if (g_actors[actor_index].chat_mask_list[tmpi] == actorid)
		{
			g_actors[actor_index].chat_mask_list[tmpi] = 0;
			success = 1;
			break;
		}
	}
	int value[3] = { 0 };
	value[0] = 2;
	value[1] = actorid;
	value[2] = success;
	actor_notify_value(actor_index, NOTIFY_ACTOR, 3, value, NULL);
	return 0;
}
