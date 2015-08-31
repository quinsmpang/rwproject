
#ifndef __GMCMD_H
#define __GMCMD_H

#define GMC_BASE			0
#define GMC_TEST			(GMC_BASE+1)	// 脚本使用
#define GMC_SC				(GMC_BASE+2)	// 脚本使用
#define GMC_SYSTALK			(GMC_BASE+3)	// 系统公告
#define GMC_EXP				(GMC_BASE+4)	// 经验
#define GMC_RES				(GMC_BASE+5)	// 金币
#define GMC_FIGHT			(GMC_BASE+9)	// 战斗
#define GMC_CITY			(GMC_BASE+10)	// 存城池信息

// GM指令使用信息
enum GMERROR
{
	GMERROR_NORMAL = 0,
	GMERROR_ACCESS_DENIED = -100, // 无权限
};
#endif
