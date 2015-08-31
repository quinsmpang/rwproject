#ifndef __FIGHT_H_
#define __FIGHT_H_
#include "army.h"
#include "fight_recode.h"
#include "fight_send.h"
#include "fight_calc.h"

#define FIGHT_TYPE_PVE	0
#define FIGHT_TYPE_PVP	1

#define MAX_TURN	99

#define FIGHT_WIN					1
#define FIGHT_LOSE					2

// 使用限制
#define EFF_ENEMY			0		// 对敌方使用
#define EFF_OUR				1		// 己方使用
#define EFF_MYSELF			2		// 仅仅自己使用

#define MAX_RECORDBUFF_SIZE		81920			// 录像整包缓冲大小

typedef struct _fightmove {
	short turns;		// 当前回合数
	char srcpos;
	char movstep;
}FightMove;

typedef struct _fightaction {
	short turns;		// 当前回合数
	char srcpos;
	char tgtpos;
	char action;
	int damage;
	int srcstat;
	int tgtstat;
	char nexttgt;
}FightAction;

struct _fight {
	unsigned char isdefance;		// 战斗状态		0 表示玩家主动进攻 1表示玩家防守(防守通常出现在PVE中,PVP一直为0)
	unsigned char level;			// 战场等级
	unsigned short cur_turns;		// 当前回合数
	char type;						// 战斗类型
	char attack_type;				// 0-attack为攻击方 1-defence为攻击方
	int attack_num;					// 攻击方人数
	int defence_num;				// 防御方人数
	char winner;					// 

	short attack_factor;
	short defence_factor;

	char move_sort[MAX_TROOPS_COUNT*2];				// 战场攻击位置排序
	char attack_sort[MAX_TROOPS_COUNT*2];			// 战场攻击位置排序

	int lastacttype;				// 最后一次行动的类型
	int lastactpos;					// 最后一次行动的位置
	Army *pAttackArmy;
	Army *pDefenceArmy;

	int movstep;
	int actstep;
	FightMove *pMove;
	FightAction *pAction;

};
typedef struct _fight Fight;

int fight_init();
int fight_oneturn();
int fight_create(int attack_index, int defance_index, char isDef);

#endif
