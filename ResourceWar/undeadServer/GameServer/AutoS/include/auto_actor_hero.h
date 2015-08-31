#ifndef __HEROATTR_AUTO_H
#define __HEROATTR_AUTO_H
#include "define.h"

struct _actor_hero {
//--Automatically generated
				int m_actorid;	//������ɫid
				short m_offset;	//λ������
				short m_herokind;	//Ӣ������
				char m_name[22];	//Ӣ������
				short m_shape;	//Ӣ������
				short m_level;	//�ȼ�
				i64 m_experience;	//����
				short m_feats;	//��ѫ
				char m_official;	//��ְ
				int m_mastership;	//ͳ˧(����ͳ�ʲ��ӵ���������)
				char m_health;	//����
				char m_grade;	//����
				char m_state;	//״̬
				unsigned short m_attack;	//����
				unsigned short m_defance;	//����
				unsigned short m_agility;	//����
				unsigned short m_hp;	//����
				short m_skill[4];	//����
};
typedef struct _actor_hero HeroAttr;

typedef HeroAttr * ( *LPCB_GETHEROATTR)( int actor_index, int offset );
int actor_hero_load_auto( int actor_index, LPCB_GETHEROATTR pCB_GetHeroAttr, int actorid, char *pTab );
int actor_hero_save_auto( HeroAttr *pHeroAttr, char *pTab, FILE *fp );

#endif
