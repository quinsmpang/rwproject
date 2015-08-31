#ifndef __STRUCTRECV0_AUTO_H
#define __STRUCTRECV0_AUTO_H
#include "server_struct_auto.h"

struct _slk_NetC_Login {
	short m_username_length;	//�ͻ�������-�û�������
	char m_username[51];	//�ͻ�������-�û���
	short m_password_length;	//�ͻ�������-���볤��
	char m_password[256];	//�ͻ�������-����
	short m_deviceid_length;	//�ͻ�������-�豸��ʾ����
	char m_deviceid[65];	//�ͻ�������-�豸��ʾ
};
typedef struct _slk_NetC_Login SLK_NetC_Login;	//�ͻ��������¼

struct _slk_NetC_Create {
	char m_aclass;	//�ͻ�������-�����Ľ�ɫ����
	short m_name_length;	//�ͻ�������-�����Ľ�ɫ���Ƴ���
	char m_name[22];	//�ͻ�������-�����Ľ�ɫ����
};
typedef struct _slk_NetC_Create SLK_NetC_Create;	//�ͻ������󴴽���ɫ

struct _slk_NetC_Heart {
	int m_value[2];	//����-��ֵ
};
typedef struct _slk_NetC_Heart SLK_NetC_Heart;	//����

struct _slk_NetC_Gmcmd {
	short m_cmd;	//�ͻ��˷���-GMָ��
	int m_value[4];	//�ͻ��˷���-GMָ����ֵ����
	short m_msg_length;	//�ͻ��˷���-GMָ���ַ�������
	char m_msg[256];	//�ͻ��˷���-GMָ���ַ�������
};
typedef struct _slk_NetC_Gmcmd SLK_NetC_Gmcmd;	//GMָ��

struct _slk_NetC_Talk {
	char m_type;	//�ͻ��˷���-��������
	short m_msg_length;	//�ͻ��˷���-�������ݳ���
	char m_msg[256];	//�ͻ��˷���-��������
};
typedef struct _slk_NetC_Talk SLK_NetC_Talk;	//�ͻ��˷�������

struct _slk_NetC_AskInfo {
	short m_msgid;	//�ͻ��˷���-������ID
	short m_valuenum;	//�ͻ��˷���-�������������
	int m_value[10];	//�ͻ��˷���-���������
	short m_msg_length;	//�ͻ��˷���-�������ַ�������
	char m_msg[256];	//�ͻ��˷���-�������ַ���
};
typedef struct _slk_NetC_AskInfo SLK_NetC_AskInfo;	//�ͻ��˷��Ͷ�����

struct _slk_NetC_MailOp {
	i64 m_mailid;	//�ͻ��˷���-�ʼ�id
	char m_op;	//�ͻ��˷���-����
};
typedef struct _slk_NetC_MailOp SLK_NetC_MailOp;	//�ʼ�����

struct _slk_NetC_CityBattleTroopList {
	short m_corps;	//����-ʿ������
	char m_level;	//����-ʿ���ȼ�
	int m_count;	//����-ʿ������
};
typedef struct _slk_NetC_CityBattleTroopList SLK_NetC_CityBattleTroopList;	//����-��������

struct _slk_NetC_CityBattleInfo {
	int m_to_unit_index;	//����-Ŀ�����
	short m_to_posx;	//����-Ŀ��λ��X
	short m_to_posy;	//����-Ŀ��λ��Y
	short m_troop_count;	//����-���ָ���
	SLK_NetC_CityBattleTroopList m_troop_list[10];	//����-�����б�
	short m_hero_count;	//����-Ӣ���б�����
	short m_hero_list[3];	//����-Ӣ���б�
};
typedef struct _slk_NetC_CityBattleInfo SLK_NetC_CityBattleInfo;	//����-ȫ����Ϣ

int struct_NetC_Login_recv( char **pptr, int *psize, SLK_NetC_Login *pValue );
int struct_NetC_Create_recv( char **pptr, int *psize, SLK_NetC_Create *pValue );
int struct_NetC_Heart_recv( char **pptr, int *psize, SLK_NetC_Heart *pValue );
int struct_NetC_Gmcmd_recv( char **pptr, int *psize, SLK_NetC_Gmcmd *pValue );
int struct_NetC_Talk_recv( char **pptr, int *psize, SLK_NetC_Talk *pValue );
int struct_NetC_AskInfo_recv( char **pptr, int *psize, SLK_NetC_AskInfo *pValue );
int struct_NetC_MailOp_recv( char **pptr, int *psize, SLK_NetC_MailOp *pValue );
int struct_NetC_CityBattleTroopList_recv( char **pptr, int *psize, SLK_NetC_CityBattleTroopList *pValue );
int struct_NetC_CityBattleInfo_recv( char **pptr, int *psize, SLK_NetC_CityBattleInfo *pValue );

#endif
