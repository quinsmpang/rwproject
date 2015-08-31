#ifndef __SYSTEM_ASKINFO_H
#define __SYSTEM_ASKINFO_H
// 客户端发过来的简单请求

#define ASKINFO_NORMAL				0
#define ASKINFO_BUILDING			1 // 建筑操作
#define ASKINFO_WORLDMAP			2 // 世界地图操作
#define ASKINFO_AREAENTER			3 // 区域进入
#define ASKINFO_CHATCATCH			4//获取聊天缓存
#define	ASKINFO_CHAT				5//对聊天的操作
#define	ASKINFO_ACTOR				6//对玩家的操作
#define	ASKINFO_STORAGE				7//背包操作
#define ASKINFO_HERO				8//英雄操作
#define ASKINFO_TROOP				9 // 兵种操作

int system_askinfo( int actor_index, int msgid, char *pstr, int *pvalue );

#endif
