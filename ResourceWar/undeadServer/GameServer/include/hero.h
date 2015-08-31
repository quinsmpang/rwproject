#ifndef _HERO_H_
#define _HERO_H_
#include "define.h"

#define HERO_CORPS_MAX	3	// 英雄兵种适性数量
#define HERO_SKILL_MAX	4	// 英雄技能数量
#define HERO_MAXCOUNT	32	// 英雄最大数量

/* 英雄状态 */
enum HeroState
{
	HeroState_Normal = 0,	// 空闲
	HeroState_Fighting,		// 出征
	HeroState_Activity,		// 活动中
};

/* 英雄官职 */
enum HeroOfficial
{
	HeroOfficial_Normal = 0,// 无官职
};

/* 身体状况 */
enum HeroHealth
{
	HeroHealth_Normal = 0,	// 健康
	HeroHealth_SlightWound,	// 轻伤
	HeroHealth_SevereWound,	// 重伤
};


/* 英雄配置数据 */
typedef struct _heroinfo
{
	short kind;						// 英雄种类
	char name[NAME_SIZE];			// 名称
	short shape;					// 形象
	short per_attack;				// 攻击占比
	short per_defance;				// 防御占比
	short per_agility;				// 敏捷占比
	short per_hp;					// 体力占比
	float growth_base;				// 成长
	short *corps;					// 兵种适性|1|2|3|
	char corps_count;				// 兵种适应性数量
	short *skill;					// 技能|1|2|3|
	char skill_count;				// 拥有技能数量
}HeroInfo;

/* 英雄配置数据初始化 */
int hero_info_init();

/* 英雄获取属性 */
HeroAttr *actor_hero_getattr( int actor_index, int offset );

/* 玩家获取一个英雄 */
int actor_gethero( int actor_index, int herokind, char path );


int hero_list( int actor_index );
int hero_info( int actor_index, int offset );
#endif
