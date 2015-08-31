#ifndef __HEROATTR_AUTO_H
#define __HEROATTR_AUTO_H
#include "define.h"

struct _actor_hero {
//--Automatically generated
				int m_actorid;	//所属角色id
				short m_offset;	//位置索引
				short m_herokind;	//英雄种类
				char m_name[22];	//英雄名称
				short m_shape;	//英雄形象
				short m_level;	//等级
				i64 m_experience;	//经验
				short m_feats;	//功勋
				char m_official;	//官职
				int m_mastership;	//统帅(所能统率部队的数量上限)
				char m_health;	//健康
				char m_grade;	//进阶
				char m_state;	//状态
				unsigned short m_attack;	//攻击
				unsigned short m_defance;	//防御
				unsigned short m_agility;	//敏捷
				unsigned short m_hp;	//生命
				short m_skill[4];	//技能
};
typedef struct _actor_hero HeroAttr;

typedef HeroAttr * ( *LPCB_GETHEROATTR)( int actor_index, int offset );
int actor_hero_load_auto( int actor_index, LPCB_GETHEROATTR pCB_GetHeroAttr, int actorid, char *pTab );
int actor_hero_save_auto( HeroAttr *pHeroAttr, char *pTab, FILE *fp );

#endif
