#ifndef __SYSTEM_ASKINFO_H
#define __SYSTEM_ASKINFO_H
// �ͻ��˷������ļ�����

#define ASKINFO_NORMAL				0
#define ASKINFO_BUILDING			1 // ��������
#define ASKINFO_WORLDMAP			2 // �����ͼ����
#define ASKINFO_AREAENTER			3 // �������
#define ASKINFO_CHATCATCH			4//��ȡ���컺��
#define	ASKINFO_CHAT				5//������Ĳ���
#define	ASKINFO_ACTOR				6//����ҵĲ���
#define	ASKINFO_STORAGE				7//��������
#define ASKINFO_HERO				8//Ӣ�۲���
#define ASKINFO_TROOP				9 // ���ֲ���

int system_askinfo( int actor_index, int msgid, char *pstr, int *pvalue );

#endif
