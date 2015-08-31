#ifndef __STRUCTRECV0_AUTO_H
#define __STRUCTRECV0_AUTO_H
#include "server_struct_auto.h"

struct _slk_NetC_Login {
	short m_username_length;	//客户端请求-用户名长度
	char m_username[51];	//客户端请求-用户名
	short m_password_length;	//客户端请求-密码长度
	char m_password[256];	//客户端请求-密码
	short m_deviceid_length;	//客户端请求-设备标示长度
	char m_deviceid[65];	//客户端请求-设备标示
};
typedef struct _slk_NetC_Login SLK_NetC_Login;	//客户端请求登录

struct _slk_NetC_Create {
	char m_aclass;	//客户端请求-创建的角色种类
	short m_name_length;	//客户端请求-创建的角色名称长度
	char m_name[22];	//客户端请求-创建的角色名称
};
typedef struct _slk_NetC_Create SLK_NetC_Create;	//客户端请求创建角色

struct _slk_NetC_Heart {
	int m_value[2];	//心跳-数值
};
typedef struct _slk_NetC_Heart SLK_NetC_Heart;	//心跳

struct _slk_NetC_Gmcmd {
	short m_cmd;	//客户端发来-GM指令
	int m_value[4];	//客户端发来-GM指令数值参数
	short m_msg_length;	//客户端发来-GM指令字符串长度
	char m_msg[256];	//客户端发来-GM指令字符串参数
};
typedef struct _slk_NetC_Gmcmd SLK_NetC_Gmcmd;	//GM指令

struct _slk_NetC_Talk {
	char m_type;	//客户端发来-聊天类型
	short m_msg_length;	//客户端发来-聊天内容长度
	char m_msg[256];	//客户端发来-聊天内容
};
typedef struct _slk_NetC_Talk SLK_NetC_Talk;	//客户端发来聊天

struct _slk_NetC_AskInfo {
	short m_msgid;	//客户端发送-短请求ID
	short m_valuenum;	//客户端发送-短请求参数个数
	int m_value[10];	//客户端发送-短请求参数
	short m_msg_length;	//客户端发送-短请求字符串长度
	char m_msg[256];	//客户端发送-短请求字符串
};
typedef struct _slk_NetC_AskInfo SLK_NetC_AskInfo;	//客户端发送短请求

struct _slk_NetC_MailOp {
	i64 m_mailid;	//客户端发送-邮件id
	char m_op;	//客户端发送-操作
};
typedef struct _slk_NetC_MailOp SLK_NetC_MailOp;	//邮件操作

struct _slk_NetC_CityBattleTroopList {
	short m_corps;	//出征-士兵兵种
	char m_level;	//出征-士兵等级
	int m_count;	//出征-士兵个数
};
typedef struct _slk_NetC_CityBattleTroopList SLK_NetC_CityBattleTroopList;	//出征-兵种内容

struct _slk_NetC_CityBattleInfo {
	int m_to_unit_index;	//出征-目标对象
	short m_to_posx;	//出征-目标位置X
	short m_to_posy;	//出征-目标位置Y
	short m_troop_count;	//出征-兵种个数
	SLK_NetC_CityBattleTroopList m_troop_list[10];	//出征-兵种列表
	short m_hero_count;	//出征-英雄列表数量
	short m_hero_list[3];	//出征-英雄列表
};
typedef struct _slk_NetC_CityBattleInfo SLK_NetC_CityBattleInfo;	//出征-全部信息

int struct_NetC_Login_recv( char **pptr, int *psize, SLK_NetC_Login *pValue );
int struct_NetC_Create_recv( char **pptr, int *psize, SLK_NetC_Create *pValue );
int struct_NetC_Heart_recv( char **pptr, int *psize, SLK_NetC_Heart *pValue );
int struct_NetC_Gmcmd_recv( char **pptr, int *psize, SLK_NetC_Gmcmd *pValue );
int struct_NetC_Talk_recv( char **pptr, int *psize, SLK_NetC_Talk *pValue );
int struct_NetC_AskInfo_recv( char **pptr, int *psize, SLK_NetC_AskInfo *pValue );
int struct_NetC_MailOp_recv( char **pptr, int *psize, SLK_NetC_MailOp *pValue );
int struct_NetC_CityBattleTroopList_recv( char **pptr, int *psize, SLK_NetC_CityBattleTroopList *pValue );
int struct_NetC_CityBattleInfo_recv( char **pptr, int *psize, SLK_NetC_CityBattleInfo *pValue );

#endif
