#ifndef _FIGHT_MSG_H_
#define _FIGHT_MSG_H_
#include "server_netsend_auto.h"

#define MAX_FIGHTMSG_SAVETIME		86400*7		// ս������ʱ��7��

// ս������
#define MAX_FIGHTMSG_TYPE_FIGHT_CITY	0
#define MAX_FIGHTMSG_TYPE_FIGHT_NPC		1
#define MAX_FIGHTMSG_TYPE_RES			20
#define MAX_FIGHTMSG_TYPE_SPY_CITY		30
#define MAX_FIGHTMSG_TYPE_SPY_ARMY		31
#define MAX_FIGHTMSG_TYPE_SPY_RES		32

/* ����ս�� */
int fight_msg_send( int actorid, SLK_NetS_FightMsgInfo *pValue );

/* ����Ƿ���δ��ս�� */
int fight_msg_check_read( int actor_index );

/* ս���б� */
int fight_msg_loadlist( int actor_index, i64 maxid );

/* ս������ */
int fight_msg_read( int actor_index, i64 msgid );

/* ɾ��ս�� */
int fight_msg_delete( int actor_index, i64 msgid );

/* ս�����ڼ�� */
int fight_msg_overdue();
#endif
