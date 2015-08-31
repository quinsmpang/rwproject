#ifndef __MATCH_H
#define __MATCH_H
#include "utils.h"

#define MATCH_ACTORID_OFFSET	1000
int random( int min, int max );

/* 竞技场假人表检查 */
int match_check();

/* 清空数据表 */
int match_delete( char * database, char *szTable );


/* 伙伴结构 */
typedef struct _partner
{
	i64 m_partnerid;
	int m_partnerkind;
	short m_level;
	char m_partnercolor;
	short m_skillexp;
}Partner;

/* 角色结构 */
typedef struct _actor
{
	int m_actorid; 
	int m_rank;
	int m_aclass;
	int m_shape;
	char m_name[16];
	short m_level;
	int m_combatpower;
	int m_skillid[16];
	int m_skillexp[16];
	int m_skill_closeflag;
	int m_professional_level[4];
	Partner m_partner[4];
}Actor;


/* 竞技场角色列表创建 */
int match_create( int robotcount );

#endif
