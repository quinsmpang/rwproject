#ifndef __ACTOR_AUTO_H
#define __ACTOR_AUTO_H
#include "define.h"
#include "server_netsend_auto.h"

struct _actor {
//--Automatically generated
				int actorid;	//角色编号
				int accountid;	//登陆id
				i64 userid;	//账号编号
				char username[51];	//账号名
				short admin;	//管理员级别
				char lastip[16];	//上次登陆ip
				int createtime;	//创建时间
				int forbidtime;	//禁言时间
				int savetime;	//上次保存时间
				unsigned char isexit;	//是否离开游戏中
				short language;	//玩家使用的本地化语言
				short view_areaindex;	//当前视口的地图区域
				char name[22];	//角色名称
				char aclass;	//角色种类
				unsigned char shape;	//形象
				unsigned char level;	//级别
				i64 experience;	//经验
				unsigned short leadership;	//领导力
				int city_index;	//城池索引
				int token;	//钻石
				int total_charge;	//总充值
				char vip;	//VIP级别
				unsigned char itemext;	//扩展的背包栏位数
				Item item[100];	//角色携带的道具
				Item equip[10];	//角色装备栏
				Item hero_equip[320];	//英雄装备栏
				HeroAttr hero[32];	//携带的英雄
				int chat_mask_list[64];	//聊天屏蔽列表
};
typedef struct _actor Actor;

int actor_load_auto( Actor *pActor, char *pTab );
int actor_save_auto( Actor *pActor, char *pTab, FILE *fp );

#endif
