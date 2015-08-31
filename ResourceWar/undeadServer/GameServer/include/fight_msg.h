#ifndef _FIGHT_MSG_H_
#define _FIGHT_MSG_H_
#include "server_netsend_auto.h"

#define MAX_FIGHTMSG_SAVETIME		86400*7		// 战报保存时间7天

// 战报类型
#define MAX_FIGHTMSG_TYPE_FIGHT_CITY	0
#define MAX_FIGHTMSG_TYPE_FIGHT_NPC		1
#define MAX_FIGHTMSG_TYPE_RES			20
#define MAX_FIGHTMSG_TYPE_SPY_CITY		30
#define MAX_FIGHTMSG_TYPE_SPY_ARMY		31
#define MAX_FIGHTMSG_TYPE_SPY_RES		32

/* 发送战报 */
int fight_msg_send( int actorid, SLK_NetS_FightMsgInfo *pValue );

/* 检查是否有未读战报 */
int fight_msg_check_read( int actor_index );

/* 战报列表 */
int fight_msg_loadlist( int actor_index, i64 maxid );

/* 战报详情 */
int fight_msg_read( int actor_index, i64 msgid );

/* 删除战报 */
int fight_msg_delete( int actor_index, i64 msgid );

/* 战报过期检查 */
int fight_msg_overdue();
#endif
