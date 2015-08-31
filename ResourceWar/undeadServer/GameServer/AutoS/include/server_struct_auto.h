#ifndef __STRUCT_AUTO0_H
#define __STRUCT_AUTO0_H

#include "define.h"
#define CMDS_BASE	0
#define CMDC_BASE	0

#define CMDS_LOGIN	(CMDS_BASE+1)
#define CMDS_LIST	(CMDS_BASE+2)
#define CMDS_CREATE	(CMDS_BASE+3)
#define CMDS_ENTERINFO	(CMDS_BASE+4)
#define CMDS_DELETE	(CMDS_BASE+5)
#define CMDS_HEART	(CMDS_BASE+6)
#define CMDS_NOTIFY	(CMDS_BASE+7)
#define CMDS_TALK	(CMDS_BASE+8)
#define CMDS_FIGHTIN	(CMDS_BASE+9)
#define CMDS_FIGHTSET	(CMDS_BASE+10)
#define CMDS_FIGHTMOVE	(CMDS_BASE+11)
#define CMDS_FIGHTACTION	(CMDS_BASE+12)
#define CMDS_ACTORINFO	(CMDS_BASE+13)
#define CMDS_CITYRESINFO	(CMDS_BASE+14)
#define CMDS_CITYBUILDINGLIST	(CMDS_BASE+15)
#define CMDS_ADDMAPUNIT	(CMDS_BASE+16)
#define CMDS_DELMAPUNIT	(CMDS_BASE+17)
#define CMDS_ITEMUSE	(CMDS_BASE+18)
#define CMDS_ITEMPUT	(CMDS_BASE+19)
#define CMDS_ITEMSETTLE	(CMDS_BASE+20)
#define CMDS_LOSTITEM	(CMDS_BASE+21)
#define CMDS_GETITEM	(CMDS_BASE+22)
#define CMDS_ITEMLIST	(CMDS_BASE+23)
#define CMDS_ITEMINFO	(CMDS_BASE+24)
#define CMDS_BUILDTIMEINFO	(CMDS_BASE+25)
#define CMDS_BUILDTIMEINFOLIST	(CMDS_BASE+26)
#define CMDS_CITYRESCHANGEINFO	(CMDS_BASE+27)
#define CMDS_BUILDINGBASEINFO	(CMDS_BASE+28)
#define CMDS_BUILDINGABILITYINFO	(CMDS_BASE+29)
#define CMDS_WORLDMAPINFO	(CMDS_BASE+30)
#define CMDS_MAPUNITCORRDINATE	(CMDS_BASE+31)
#define CMDS_MARCHROUTE	(CMDS_BASE+32)
#define CMDS_OUTSIDEMAPINFO	(CMDS_BASE+33)
#define CMDS_MAILSIMPLEINFO	(CMDS_BASE+34)
#define CMDS_MAILLIST	(CMDS_BASE+35)
#define CMDS_MAILFULLINFO	(CMDS_BASE+36)
#define CMDS_FIGHTMSGSIMPLEINFO	(CMDS_BASE+37)
#define CMDS_FIGHTMSGLIST	(CMDS_BASE+38)
#define CMDS_FIGHTMSGINFO	(CMDS_BASE+39)
#define CMDS_BUILDINGABILITYLIST	(CMDS_BASE+40)
#define CMDS_CHATMASKLIST	(CMDS_BASE+41)
#define CMDS_TROOPINFO	(CMDS_BASE+42)
#define CMDS_TROOPINFOLIST	(CMDS_BASE+43)
#define CMDS_HEROINFOLIST	(CMDS_BASE+44)
#define CMDS_HEROINFO	(CMDS_BASE+45)

#define CMDC_LOGINED	(CMDC_BASE-2)
#define CMDC_TIMER	(CMDC_BASE-1)
#define CMDC_LOGIN	(CMDC_BASE+1)
#define CMDC_CREATE	(CMDC_BASE+2)
#define CMDC_LIST	(CMDC_BASE+3)
#define CMDC_ENTERGAME	(CMDC_BASE+4)
#define CMDC_DELETE	(CMDC_BASE+5)
#define CMDC_HEART	(CMDC_BASE+6)
#define CMDC_GMCMD	(CMDC_BASE+7)
#define CMDC_TALK	(CMDC_BASE+8)
#define CMDC_ASKINFO	(CMDC_BASE+9)
#define CMDC_MAILOP	(CMDC_BASE+10)
#define CMDC_CITYBATTLE	(CMDC_BASE+11)
struct _slk_NetU_Logined {
	int m_result;	//用户服务器返回-登录返回值
	int m_client_index;	//用户服务器返回-角色索引
	int m_authid;	//用户服务器返回-认证ID
	char m_userid[21];	//用户服务器返回-用户ID
	char m_username[51];	//用户服务器返回-用户名
	short m_usertype;	//用户服务器返回-用户类型
	char m_checkflag;	//用户服务器返回-检测标记
	char m_isdeep;	//用户服务器返回-要防沉迷
	char m_access_token[65];	//用户服务器返回-360的用户令牌
	int m_expires_in;	//用户服务器返回-360的令牌有效期
	char m_refresh_token[65];	//用户服务器返回-360的刷新令牌
	char m_deviceid[65];	//用户服务器返回-传回的设备ID
};
typedef struct _slk_NetU_Logined SLK_NetU_Logined;	//用户服务器返回登陆

int struct_NetU_Logined_send( char **pptr, int *psize, SLK_NetU_Logined *pValue );
int struct_NetU_Logined_recv( char **pptr, int *psize, SLK_NetU_Logined *pValue );

#endif
