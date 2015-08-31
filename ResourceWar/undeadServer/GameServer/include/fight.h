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

// ʹ������
#define EFF_ENEMY			0		// �Եз�ʹ��
#define EFF_OUR				1		// ����ʹ��
#define EFF_MYSELF			2		// �����Լ�ʹ��

#define MAX_RECORDBUFF_SIZE		81920			// ¼�����������С

typedef struct _fightmove {
	short turns;		// ��ǰ�غ���
	char srcpos;
	char movstep;
}FightMove;

typedef struct _fightaction {
	short turns;		// ��ǰ�غ���
	char srcpos;
	char tgtpos;
	char action;
	int damage;
	int srcstat;
	int tgtstat;
	char nexttgt;
}FightAction;

struct _fight {
	unsigned char isdefance;		// ս��״̬		0 ��ʾ����������� 1��ʾ��ҷ���(����ͨ��������PVE��,PVPһֱΪ0)
	unsigned char level;			// ս���ȼ�
	unsigned short cur_turns;		// ��ǰ�غ���
	char type;						// ս������
	char attack_type;				// 0-attackΪ������ 1-defenceΪ������
	int attack_num;					// ����������
	int defence_num;				// ����������
	char winner;					// 

	short attack_factor;
	short defence_factor;

	char move_sort[MAX_TROOPS_COUNT*2];				// ս������λ������
	char attack_sort[MAX_TROOPS_COUNT*2];			// ս������λ������

	int lastacttype;				// ���һ���ж�������
	int lastactpos;					// ���һ���ж���λ��
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
