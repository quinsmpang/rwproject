#ifndef __ACTOR_H
#define __ACTOR_H
#include "auto_actor.h"
#include "server_structsend_auto.h"
// ------------------------------- actor.c -----------------------------------
// ���������н�ɫ��ʼ��
int actors_init();

// �������߽�ɫ����
int actors_save_everyone( char type );

// ����������֮��Ĵ浵
void actors_on_core();

// �����ɫ����
int actor_save( int actor_index, FILE *fp );

// ��ʼ�����ɫ����
int actor_real_saveall( int actor_index, FILE *fp );

// ���ͱ�����Ϣ������
int actor_send_save( int actor_index );

// ��ɫ����������Ϣ������
int actor_send_remove( int actor_index );

// ��ɫ���߱��沢�Ͽ��ͻ���
int actor_remove( int actor_index );

// ���½�ɫ�б�����
int actor_update_list( int actor_index );

// �������ݿ��н�ɫ�������
int actor_update_online( int actorid, char isonline );

// ����ͻ���
int actor_del( int client_index );

// ������ɾ����ɫ
int actor_delete( i64 userid, int actorid );

// ����ɾ�����
int actor_delete_stoptime( i64 userid, int actorid );

// ����ɾ��
int actor_delete_cancel( int actorid );

// ���н�ɫ�߼�
void actors_logic();

// ��ɫ�߼�
void actor_logic( int actor_index );

// ��ɫ������Ϸ
int actor_entergame( int client_index, int actorid );

// ��ɫ������Ϸ����
int actor_enterworld( int client_index, int actorid, int actor_index );

// ��ɫ������Ϸ���ȡ����
int actor_load( int actor_index, int actorid );

// ��һ�ν�����ϷҪ��ʼ����ɫ��Ϣ
int actor_new( int actor_index );

// ������ɫ
int actor_create( int client_index, int aclass, char *szActorName );

// ������ɫ,���ݿ�
int actor_db_create( int client_index, int platid, i64 userid, char *username, SLK_ListInfo *pListInfo );


// ------------------------------- actor_func.c -----------------------------------
// ����������ȡ�û�id
i64 actor_getoffline_userid( int actorid );

// ���ݱ�Ż�ȡ�������
int actor_getindex_withid( int actorid );

// ���ؽ�ɫ�б�
int actor_getlist( int platid, i64 userid, SLK_NetS_List *pValue );

// ��֤һ�������ɫid�ǲ������ڵ�½������û�
int actor_verifyid( i64 userid, int actorid );

// �������
int actor_talk( int actor_index, char type, char *szMsg, int msgsize );

// ���������б�
int actor_talk_chat_mask_list( int actor_index );
int actor_talk_chat_mask_list_add( int actor_index, int actorid );
int actor_talk_chat_mask_list_del( int actor_index, int actorid );

// ��ɫ��Ϣ
int actor_getinfo( int actor_index );

// ------------------------------- actor_gmcmd.c -----------------------------------
int actor_command( int actor_index, short cmd, int *pValue, char *pMsg );


#endif
