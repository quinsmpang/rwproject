#ifndef _TROOP_H_
#define _TROOP_H_
#include "define.h"

#define TROOP_UPGRADE_RES_MAX	4	// ѵ��������Դ����

// ������Ϣ�����ݵȼ��趨��ϸ��Ϣ
typedef struct _troopinfo
{
	short corps;				//  ����
	char level;					//  ���ֵȼ�
	char corps_type;			//  1������2�����3�ɱ���4��ʦ��5����
	char name[NAME_SIZE];		//  ����
	short shape;				//  ����
	unsigned int hp;			//  ����
	unsigned int attack;		//  ����
	unsigned int defance;		//  ����
	unsigned int hit;			//  ����
	unsigned int dodge;			//  ��
	unsigned int crit;			//  ����
	unsigned int agl;			//  ����
	unsigned int crit_harm;		//  �����˺�
	unsigned int crit_reduce;	//  ��������
	unsigned int mobility;		//  ������
	unsigned int move_grid;		//  �ƶ��ٶȣ����ӣ�
	unsigned int attack_order;  //  ��������
	unsigned int attack_type;	//  ��������
	unsigned int defance_type;  //  ��������
	unsigned int carry_res;		//  Я����Դ��
	char attack_range;			//	������Χ
	int combat_power;			//	ս��
	char upgrade_restype[TROOP_UPGRADE_RES_MAX];	//  ѵ��������Դ����
	int upgrade_resnum[TROOP_UPGRADE_RES_MAX];		//  ѵ��������Դ����
	int sec;					// ѵ����ʱ
	unsigned char open_level;	// ���ŵȼ�
	char aclass;				// ����
	char buildingkind;			// ��Ӧ����������
}TroopInfo;

// ������Ϣ������������ֵ���Ϣ
typedef struct troop_corps_info
{
	short corps;				// ����
	float sol_modulus;			// ϵ��
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
