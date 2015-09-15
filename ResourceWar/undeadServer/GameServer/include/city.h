#ifndef _CITY_H_
#define _CITY_H_
#include "define.h"
#include "building.h"
#include "server_structrecv_auto.h"
#include "actor.h"

#define CityBuildingMax				35	// 一个城池最多拥有多少建筑
#define CityHandleQueueMax			2	// 一个城池的建筑操作队列
#define CityCorpsMax				5	// 兵种数量
#define CityCorpsLevelMax			10	// 每种兵最大等级
#define CityUnderFire				32	// 最多受多少支部队攻击

// 城池领主类型
typedef enum
{
	CityLairdType_Player = 0,		//	玩家的城池
	CityLairdType_Match = 1,		//	玩家出生时候的对手城池
	CityLairdType_Robot = 2,		//  机器人城池
}CityLairdType;

// 资源类型定义
typedef enum
{
	CityRes_Normal	= 0,		//	没有
	CityRes_Wood	= 1,		//	木材
	CityRes_Food	= 2,		//	粮食
	CityRes_Iron	= 3,		//  铁
	CityRes_Mithril = 4,		//  秘银
	CityRes_Gold	= 5,		//	金币
}CityRes;

// 城池拥有的部队结构
typedef struct _citycorps
{
	char corps;	// 兵种
	char level; // 兵种等级
	int count;	// 兵种数量
}CityCorps;

// 攻击方信息
typedef struct _under_fire_info
{
	int army_index;
}UnderFireInfo;

// 一个城池的结构
typedef struct _city
{
	// 需要存储的数据
	int cityid;									// 城池id
	char laird_type;							// 领主类型
	int laird_actorid;							// 领主id
	char laird_name[NAME_SIZE];					// 领主名称
	unsigned char laird_shape;					// 领主形象
	unsigned char laird_level;					// 领主等级
	int laird_lastlogin;						// 领主上次登录时间
	int match_cityid;							// 玩家出生对手城池id
	char aclass;								// 城池种类 0人类 1亡灵
	char skin;									// 城池皮肤
	int clubid;									// 联盟编号
	short posx;									// 世界坐标
	short posy;									// 世界坐标
	int	wood;									// 木材
	int	food;									// 粮食
	int iron;									// 铁
	int mithril;								// 秘银	
	int gold;									// 金币
	Building building[CityBuildingMax];			// 建筑列表
	int corps_num[CityCorpsMax][CityCorpsLevelMax];	// 兵数量5种兵每种10级
	int corps_wound[CityCorpsMax][CityCorpsLevelMax];	// 伤兵数量5种兵每种10级

	// 计算出的临时数据，提升效率
	int actor_index;							// 玩家在线这个值有效
	int match_cityindex;						// 玩家出生对手城池索引
	short club_index;							// 联盟索引
	short handle_queue[CityHandleQueueMax];		// 建筑操作队列，减少遍历次数

	UnderFireInfo underfire[CityUnderFire];		// 攻击这个城池的所有部队
}City;

// 服务器启动读取所有城池信息到内存
int city_load();

// 服务器关闭，所有城池信息存到数据库
int city_save( FILE *fp );
int city_single_save( City *pCity, FILE *fp );

// 所有城市每秒的逻辑
void city_logic_sec();

// 所有城市每分钟的逻辑
void city_logic_min();

// 每天定时摧毁一些城池
void city_logic_destroy();

// 根据玩家id找到城池索引
int city_getindex_withactorid( int actorid );

// 根据城池id找到城池索引
int city_getindex_withcityid( int cityid );

// 创建一个新城池
int city_new( City *pCity );

// 删除一个城池
void city_del( City *pCity, int city_index );

// 重新计算城市临时数据
void city_reset( City *pCity );

// 获取这个玩家的城池属性
City *city_getptr( int actor_index );

// 获取这个玩家出生对手的城池数据
City *city_match_getptr( int actor_index );

// 战胜对手城池
void city_beat_match_city( int city_index );

// 城市资源改变
void city_change_res( City *pCity, CityRes res, int value, char path );

// 已被设定为攻击目标
void city_underfire( City *pCity, int army_index );

// 出征
void city_battle( int actor_index, City *pCity, SLK_NetC_CityBattleInfo *info);

// 迁城
int city_move( int actor_index, short posx, short posy );

// 操作队列
int city_handle_queue_add( City *pCity, short buildingindex );
void city_handle_queue_del( City *pCity, short buildingindex );
void city_handle_queue_clear( City *pCity );

// --------------------------------------------- city_send.cpp ------------------------------------------------
// 玩家城市信息
int city_info( int actor_index );

// 城市资源信息
void city_resinfo( int actor_index, City *pCity );

// 城市建筑基本信息
void city_buildinglist( int actor_index, City *pCity );

// 城市建筑升级或建造时间信息
void city_building_buildtimeinfo( int actor_index, short buildingindex, Building *pBuilding );

// 城市建筑升级或建造时间信息列表
void city_building_buildtimeinfolist( int actor_index, City *pCity );

// 资源改变
void city_res_changeinfo( int actor_index, char restype, int resvalue, char path );

// 发送一个建筑的基本信息
void city_building_baseinfo( int actor_index, short buildingindex, Building *pBuilding );

// 发送一个建筑的属性信息
void city_building_abilityinfo( int actor_index, short buildingindex, Building *pBuilding );

// 发送一个建筑的属性信息
void city_building_abilitylist( int actor_index, City *pCity );

// 发送城市兵种信息列表 
void city_troop_sendinfolist( int actor_index, City* pCity );

// 发送单个兵种信息更新
void city_troop_sendinfo( int actor_index, City* pCity, int corps_type, int level );
// 判断用户的城市是否被攻击/出征了
bool actor_city_is_underfire( int city_index );
// 发送军情给对应的玩家
int actor_city_underfire_info( int actor_index );
#endif
