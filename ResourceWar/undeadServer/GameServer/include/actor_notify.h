#ifndef __ACTOR_NOTIFY_H
#define __ACTOR_NOTIFY_H

#define NOTIFY_NORMAL		0	// ����˷�������Ϣ
#define NOTIFY_TEXTTABLE	1	// ����˷����ֱ���Ϣ
#define NOTIFY_ITEMNUM		2	// ���������µ�������
#define NOTIFY_WORLDMAP		3	// �����ͼ����
#define NOTIFY_MAIL			4	// �ʼ�����
#define NOTIFY_CHAT			5	// �������
#define	NOTIFY_ACTOR		6	//����ҵĲ���

// ���͸���ɫ����Ϣ �޲���
int actor_notify( int actor_index, short msgid, const char *msg );

// ���͸���ɫ����Ϣ ������
int actor_notify_value( int actor_index, short msgid, char count, const int *data, const char *msg );

#endif
