#ifndef __MAIL_MSG_H
#define __MAIL_MSG_H

#define MAX_MAIL_SAVETIME		86400*7		// û�����ʼ�����ʱ��7�죬�и�����*2

#define MAIL_TITLE_MAXSIZE		32			// �ʼ����ⳤ��
#define MAIL_CONTENT_MAXSIZE	1024		// �ʼ���Ϣ����
#define MAIL_ATTACH_MAXSIZE		128			// �ʼ���������

#define MAIL_TYPE_SYSTEM	1	// ϵͳ�ʼ�
#define MAIL_TYPE_CHAT		2	// �����ʼ�

/* �����ʼ� �������ʼ����ͣ�������id�����������ƣ��ռ���id�����⣬���ݣ����� */
int mail_send( char type, int from_actorid, char *from_name, int to_actorid, char *title, char *content, char *attach );

/* ����Ƿ���δ���ʼ� */
int mail_check_read( int actor_index );

/* ��ȡ�ʼ��б� */
int mail_loadlist( int actor_index, i64 begin );

/* ��ȡ��ϸ���� */
int mail_read_content( int actor_index, i64 mailid );

/* ��ȡ���� */
int mail_read_attach( int actor_index, i64 mailid );

/* ɾ�� */
int mail_delete( int actor_index, i64 mailid );

/* �ʼ����ڼ�� */
int mail_overdue();

/* �ʼ����ڼ��ÿ������2���� */
void mail_overdue_everyday();
#endif
