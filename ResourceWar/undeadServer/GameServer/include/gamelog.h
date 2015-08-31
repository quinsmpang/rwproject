#ifndef __GAMELOG_H
#define __GAMELOG_H

#include "utils.h"

/* 操作 */
#define LOGOP_HERO				1	// 英雄
#define LOGOP_ITEMGET			2	// 获得道具
#define LOGOP_ITEMLOST			3	// 丢失道具
#define LOGOP_CITYRES			4	// 城池资源


/* 途径 */
#define PATH_SYSTEM					1	// 系统
#define PATH_GM						2	// GM
#define PATH_ITEMUSE				3	// 道具使用方式
#define PATH_SELL					4	// 售卖丢弃
#define PATH_BUILDING_GATHER		5	// 采集建筑资源

int log_init();
int wlog( char type, int op, char path, int object, int objectnum, i64 objectid, int source, int target );
int wcount();

#endif

