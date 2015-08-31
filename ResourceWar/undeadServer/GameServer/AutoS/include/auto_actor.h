#ifndef __ACTOR_AUTO_H
#define __ACTOR_AUTO_H
#include "define.h"
#include "server_netsend_auto.h"

struct _actor {
//--Automatically generated
				int actorid;	//��ɫ���
				int accountid;	//��½id
				i64 userid;	//�˺ű��
				char username[51];	//�˺���
				short admin;	//����Ա����
				char lastip[16];	//�ϴε�½ip
				int createtime;	//����ʱ��
				int forbidtime;	//����ʱ��
				int savetime;	//�ϴα���ʱ��
				unsigned char isexit;	//�Ƿ��뿪��Ϸ��
				short language;	//���ʹ�õı��ػ�����
				short view_areaindex;	//��ǰ�ӿڵĵ�ͼ����
				char name[22];	//��ɫ����
				char aclass;	//��ɫ����
				unsigned char shape;	//����
				unsigned char level;	//����
				i64 experience;	//����
				unsigned short leadership;	//�쵼��
				int city_index;	//�ǳ�����
				int token;	//��ʯ
				int total_charge;	//�ܳ�ֵ
				char vip;	//VIP����
				unsigned char itemext;	//��չ�ı�����λ��
				Item item[100];	//��ɫЯ���ĵ���
				Item equip[10];	//��ɫװ����
				Item hero_equip[320];	//Ӣ��װ����
				HeroAttr hero[32];	//Я����Ӣ��
				int chat_mask_list[64];	//���������б�
};
typedef struct _actor Actor;

int actor_load_auto( Actor *pActor, char *pTab );
int actor_save_auto( Actor *pActor, char *pTab, FILE *fp );

#endif
