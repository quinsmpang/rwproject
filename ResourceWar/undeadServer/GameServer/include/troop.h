#ifndef _TROOP_H_
#define _TROOP_H_
#include "define.h"

#define TROOP_UPGRADE_RES_MAX	4	// 训练所需资源种类

// 兵种信息，根据等级设定详细信息
typedef struct _troopinfo
{
	short corps;				//  兵种
	char level;					//  兵种等级
	char corps_type;			//  1步兵，2骑兵，3飞兵，4法师，5弓兵
	char name[NAME_SIZE];		//  名称
	short shape;				//  形象
	unsigned int hp;			//  生命
	unsigned int attack;		//  攻击
	unsigned int defance;		//  防御
	unsigned int hit;			//  技巧
	unsigned int dodge;			//  格挡
	unsigned int crit;			//  暴击
	unsigned int agl;			//  韧性
	unsigned int crit_harm;		//  暴击伤害
	unsigned int crit_reduce;	//  暴击减免
	unsigned int mobility;		//  机动力
	unsigned int move_grid;		//  移动速度（格子）
	unsigned int attack_order;  //  攻击次序
	unsigned int attack_type;	//  攻击类型
	unsigned int defance_type;  //  防御类型
	unsigned int carry_res;		//  携带资源量
	char attack_range;			//	攻击范围
	int combat_power;			//	战力
	char upgrade_restype[TROOP_UPGRADE_RES_MAX];	//  训练所需资源类型
	int upgrade_resnum[TROOP_UPGRADE_RES_MAX];		//  训练所需资源数量
	int sec;					// 训练用时
	unsigned char open_level;	// 开放等级
	char aclass;				// 种族
	char buildingkind;			// 对应的生产建筑
}TroopInfo;

// 兵种信息，属于这个兵种的信息
typedef struct troop_corps_info
{
	short corps;				// 兵种
	float sol_modulus;			// 系数
	float hp_modulus;
	float atk_modulus;
	float def_modulus;
	float crit_modulus;
	float critharm_modulus;
	float hit_modulus;
	float agl_modulus;
	float dodge_modulus;
	float critreduce_modulus;
}TroopCorpsInfo;

int troop_info_init();
int troop_corps_info_init();

int troop_get_entry_position(short corps, char level, char isenemy);
char troop_get_attack_range(short corps, char level);
char troop_get_move_range(short corps, char level);
int troop_get_hp(short corps, char level);
int troop_get_attackaction(short corps, char level);

int troop_get_attacktype(short corps, char level);
int troop_get_defancetype(short corps, char level);

#endif
