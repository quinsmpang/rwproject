#ifndef __ARMY_H_
#define __ARMY_H_
#include "define.h"


// 作战单位
typedef struct _troopunit
{
	short offset;	//位置索引
	short hero_kind;	//英雄种类
	char hero_name[22];	//英雄名称
	short hero_shape;	//英雄形象
	short hero_level;	//等级
	short hero_skill[4];	//技能
	char skill_cd[4];			// 技能冷却

	short corps;				//  兵种
	char level;					//  兵种等级
	int maxnumber;		//  总人数
	int fightnumber;		//  出战人数
	int curnumber;		//  当前人数
	int attack;		//  攻击
	int defance;		//  防御
	int hit;			//  技巧
	int dodge;			//  格挡
	int crit;			//  暴击
	int agl;			//  韧性
	int crit_harm;		//  暴击伤害
	int crit_reduce;	//  暴击减免

	char fight_position;		//  当前在战场中的格子位置
	char isdead;				//  已经死亡吗
	int stat;					//  buff状态
}TroopUnit;

#define MAX_TROOPS_COUNT	10
#define MAX_HEROES_COUNT	3

#define ARMY_STAT_STAND		0	// 站立
#define ARMY_STAT_WALK		1	// 行军中
#define ARMY_STAT_FIGHT		2	// 战斗中
#define ARMY_STAT_OCCUPY	3	// 驻扎

//行走路径结构
typedef struct _walkpath
{
	int m_pathx[150];	//行走路径x
	int m_pathy[150];	//行走路径y
	unsigned short m_len;	//路径长度
	unsigned short m_pos;	//当前行动地点
}SLK_WalkPath;

typedef struct _army
{
	int actorid;				// >0 玩家id <0 npcid ==0临时战斗
	char isnpc;

	int city_index;				// 属于哪个城池
	int cityid;					// 城池id
	int unit_index;				// 在显示对象池里的索引 

	char armyname[NAME_SIZE];	// 部队名称
	short shape;				// 部队形象
	char hide;					// 是否显示
	unsigned char lastway;		// 角色的方向

	unsigned char stat;			// 角色的状态,0表示站立 1表示行走 2 战斗
	int	stat_time;				// 状态时间戳
	short from_posx;			// 出发位置
	short from_posy;			// 出发位置
	short target_posx;			// 目标位置
	short target_posy;			// 目标位置
	short posx;					// 当前位置
	short posy;					// 当前位置
	short walk_len;				// 步长
	short walk_distance;		// 行走距离
	short walk_total_distance;	// 行走总距离，格子数
	short walk_remaining_time;	// 到达目的地剩余时间


	int troops_count;
	TroopUnit troops[MAX_TROOPS_COUNT];

	short heroes_offset[MAX_HEROES_COUNT];
}Army;

typedef struct _enemy_army
{
	int enemyid;
	int troops_count;
	int herokind[MAX_TROOPS_COUNT];
	char herolevel[MAX_TROOPS_COUNT];
	short corps[MAX_TROOPS_COUNT];
	char corpslevel[MAX_TROOPS_COUNT];
	int number[MAX_TROOPS_COUNT];
}EnemyArmy;

int army_init();
int army_create( int actorid, char isnpc, short shape, char *pname );
int army_addtroop(int army_index, TroopUnit *pTroopUnit);
int army_addenemy(int army_index, int enemyid);
void army_alllogic( );
void army_logic( int army_index );
int army_marchroute( int actor_index, int from_unit_index, short from_posx, short from_posy, int to_unit_index, short to_posx, short to_posy );
#endif
