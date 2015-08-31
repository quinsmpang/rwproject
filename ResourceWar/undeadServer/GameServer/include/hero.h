#ifndef _HERO_H_
#define _HERO_H_
#include "define.h"

#define HERO_CORPS_MAX	3	// Ӣ�۱�����������
#define HERO_SKILL_MAX	4	// Ӣ�ۼ�������
#define HERO_MAXCOUNT	32	// Ӣ���������

/* Ӣ��״̬ */
enum HeroState
{
	HeroState_Normal = 0,	// ����
	HeroState_Fighting,		// ����
	HeroState_Activity,		// ���
};

/* Ӣ�۹�ְ */
enum HeroOfficial
{
	HeroOfficial_Normal = 0,// �޹�ְ
};

/* ����״�� */
enum HeroHealth
{
	HeroHealth_Normal = 0,	// ����
	HeroHealth_SlightWound,	// ����
	HeroHealth_SevereWound,	// ����
};


/* Ӣ���������� */
typedef struct _heroinfo
{
	short kind;						// Ӣ������
	char name[NAME_SIZE];			// ����
	short shape;					// ����
	short per_attack;				// ����ռ��
	short per_defance;				// ����ռ��
	short per_agility;				// ����ռ��
	short per_hp;					// ����ռ��
	float growth_base;				// �ɳ�
	short *corps;					// ��������|1|2|3|
	char corps_count;				// ������Ӧ������
	short *skill;					// ����|1|2|3|
	char skill_count;				// ӵ�м�������
}HeroInfo;

/* Ӣ���������ݳ�ʼ�� */
int hero_info_init();

/* Ӣ�ۻ�ȡ���� */
HeroAttr *actor_hero_getattr( int actor_index, int offset );

/* ��һ�ȡһ��Ӣ�� */
int actor_gethero( int actor_index, int herokind, char path );


int hero_list( int actor_index );
int hero_info( int actor_index, int offset );
#endif
