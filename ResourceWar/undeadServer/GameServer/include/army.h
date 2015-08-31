#ifndef __ARMY_H_
#define __ARMY_H_
#include "define.h"


// ��ս��λ
typedef struct _troopunit
{
	short offset;	//λ������
	short hero_kind;	//Ӣ������
	char hero_name[22];	//Ӣ������
	short hero_shape;	//Ӣ������
	short hero_level;	//�ȼ�
	short hero_skill[4];	//����
	char skill_cd[4];			// ������ȴ

	short corps;				//  ����
	char level;					//  ���ֵȼ�
	int maxnumber;		//  ������
	int fightnumber;		//  ��ս����
	int curnumber;		//  ��ǰ����
	int attack;		//  ����
	int defance;		//  ����
	int hit;			//  ����
	int dodge;			//  ��
	int crit;			//  ����
	int agl;			//  ����
	int crit_harm;		//  �����˺�
	int crit_reduce;	//  ��������

	char fight_position;		//  ��ǰ��ս���еĸ���λ��
	char isdead;				//  �Ѿ�������
	int stat;					//  buff״̬
}TroopUnit;

#define MAX_TROOPS_COUNT	10
#define MAX_HEROES_COUNT	3

#define ARMY_STAT_STAND		0	// վ��
#define ARMY_STAT_WALK		1	// �о���
#define ARMY_STAT_FIGHT		2	// ս����
#define ARMY_STAT_OCCUPY	3	// פ��

//����·���ṹ
typedef struct _walkpath
{
	int m_pathx[150];	//����·��x
	int m_pathy[150];	//����·��y
	unsigned short m_len;	//·������
	unsigned short m_pos;	//��ǰ�ж��ص�
}SLK_WalkPath;

typedef struct _army
{
	int actorid;				// >0 ���id <0 npcid ==0��ʱս��
	char isnpc;

	int city_index;				// �����ĸ��ǳ�
	int cityid;					// �ǳ�id
	int unit_index;				// ����ʾ������������ 

	char armyname[NAME_SIZE];	// ��������
	short shape;				// ��������
	char hide;					// �Ƿ���ʾ
	unsigned char lastway;		// ��ɫ�ķ���

	unsigned char stat;			// ��ɫ��״̬,0��ʾվ�� 1��ʾ���� 2 ս��
	int	stat_time;				// ״̬ʱ���
	short from_posx;			// ����λ��
	short from_posy;			// ����λ��
	short target_posx;			// Ŀ��λ��
	short target_posy;			// Ŀ��λ��
	short posx;					// ��ǰλ��
	short posy;					// ��ǰλ��
	short walk_len;				// ����
	short walk_distance;		// ���߾���
	short walk_total_distance;	// �����ܾ��룬������
	short walk_remaining_time;	// ����Ŀ�ĵ�ʣ��ʱ��


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
