#ifndef __ACTOR_NOTIFY_H
#define __ACTOR_NOTIFY_H

#define NOTIFY_NORMAL		0	// 服务端发文字信息
#define NOTIFY_TEXTTABLE	1	// 服务端发文字表信息
#define NOTIFY_ITEMNUM		2	// 整理背包更新道具数量
#define NOTIFY_WORLDMAP		3	// 世界地图操作
#define NOTIFY_MAIL			4	// 邮件操作
#define NOTIFY_CHAT			5	// 聊天操作
#define	NOTIFY_ACTOR		6	//对玩家的操作

// 发送给角色短消息 无参数
int actor_notify( int actor_index, short msgid, const char *msg );

// 发送给角色短消息 带参数
int actor_notify_value( int actor_index, short msgid, char count, const int *data, const char *msg );

#endif
