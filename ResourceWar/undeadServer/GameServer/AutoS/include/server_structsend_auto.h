#ifndef __STRUCTSEND0_AUTO_H
#define __STRUCTSEND0_AUTO_H
#include "server_struct_auto.h"

struct _slk_NetS_Login {
	int m_result;	//服务器返回-返回值
	short m_username_length;	//服务器返回-用户名长度
	char m_username[51];	//服务器返回-用户名
	short m_token_length;	//服务器返回-token长度
	char m_token[256];	//服务器返回-token
};
typedef struct _slk_NetS_Login SLK_NetS_Login;	//服务器端发送登录结果给客户端

struct _slk_ListInfo {
	int m_actorid;	//角色ID
	char m_name[22];	//角色名字
	char m_aclass;	//角色种族
	short m_level;	//角色等级
	char m_offset;	//角色位置
	char m_lockstat;	//锁定状态
	int m_delete_stoptime;	//删除时间
};
typedef struct _slk_ListInfo SLK_ListInfo;	//单个角色的列表信息

struct _slk_NetS_List {
	char m_actor_num;	//服务端返回-角色个数
	SLK_ListInfo m_listinfo[8];	//服务端返回-角色数组
};
typedef struct _slk_NetS_List SLK_NetS_List;	//服务器端发送角色列表

struct _slk_NetS_Create {
	char m_result;	//服务端返回-创建角色的结果
	int m_actorid;	//服务端返回-创建角色的编号
};
typedef struct _slk_NetS_Create SLK_NetS_Create;	//服务器返回创建角色结果

struct _slk_NetS_EnterInfo {
	int m_actorid;	//服务端返回-进入游戏角色ID
	int m_data_int;	//服务端返回-进入游戏额外数据[备用]
	short m_data_short;	//服务端返回-进入游戏额外数据[备用]
	char m_data_char;	//服务端返回-进入游戏额外数据[备用]
};
typedef struct _slk_NetS_EnterInfo SLK_NetS_EnterInfo;	//服务器返回进入游戏后的信息

struct _slk_NetS_Delete {
	int m_result;	//服务器返回-删除返回值
	int m_actorid;	//服务器返回-删除的角色编号
};
typedef struct _slk_NetS_Delete SLK_NetS_Delete;	//服务器返回删除角色

struct _slk_NetS_Heart {
	int m_value[2];	//心跳-数值
};
typedef struct _slk_NetS_Heart SLK_NetS_Heart;	//心跳

struct _slk_NetS_Notify {
	short m_msgid;	//服务器发送-短消息ID
	short m_valuenum;	//服务器发送-短消息参数个数
	int m_value[10];	//服务器发送-短消息数值参数
	short m_msg_length;	//服务器发送-短消息字符串消息长度
	char m_msg[256];	//服务器发送-短消息字符串参数
};
typedef struct _slk_NetS_Notify SLK_NetS_Notify;	//通用短消息

struct _slk_NetS_Talk {
	char m_type;	//服务器发送-聊天类型
	int m_actorid;	//服务器发送-聊天角色ID
	char m_name[22];	//服务器发送-聊天角色名
	short m_msg_length;	//服务器发送-聊天内容长度
	char m_msg[256];	//服务器发送-聊天内容
	char m_shape;	//服务器发送-聊天想想
	int m_sendtime;	//服务器发送-聊天时间
};
typedef struct _slk_NetS_Talk SLK_NetS_Talk;	//服务器发送聊天

struct _slk_NetS_FightIn {
	char m_srcpos;	//战斗位置
	short m_shape;	//英雄头像
	char m_namelen;	//名字长度
	char m_name[22];	//英雄名字
	char m_isenemy;	//是否防御方
	char m_posion;	//战格位置
	short m_corps;	//兵种编号
	int m_curnumber;	//士兵数量
	int m_maxnumber;	//最大士兵
	char m_ratenumber;	//士兵比例(0-100)
};
typedef struct _slk_NetS_FightIn SLK_NetS_FightIn;	//战斗开始

struct _slk_NetS_FightSet {
	short m_turn;	//当前回合
	char m_fightflag;	//结束标志
};
typedef struct _slk_NetS_FightSet SLK_NetS_FightSet;	//回合开始/结束/战斗结束

struct _slk_NetS_FightMove {
	char m_srcpos;	//战斗位置
	char m_movstep;	//移动步数
};
typedef struct _slk_NetS_FightMove SLK_NetS_FightMove;	//单位移动指令

struct _slk_NetS_FightAction {
	char m_srcpos;	//发起位置
	char m_tgtpos;	//目标位置
	char m_action;	//动作
	int m_damage;	//伤害
	int m_srcstat;	//发起状态
	int m_tgtstat;	//目标状态
	char m_nexttgt;	//是否有下个
};
typedef struct _slk_NetS_FightAction SLK_NetS_FightAction;	//单位动作指令

struct _slk_ArmyCorrdinate {
	int m_unit_index;	//地图单元-序号
	short m_posx;	//地图单元-当前坐标
	short m_posy;	//地图单元-当前坐标
};
typedef struct _slk_ArmyCorrdinate SLK_ArmyCorrdinate;	//地图显示单元-部队位置

struct _slk_NetS_CityResInfo {
	int m_wood;	//城池基本资源信息
	int m_food;	//城池基本资源信息
	int m_iron;	//城池基本资源信息
	int m_mithril;	//城池基本资源信息
	int m_gold;	//城池基本资源信息
	char m_towncenter_level;	//城池基本资源信息
	char m_enter_worldmap;	//城池基本资源信息
};
typedef struct _slk_NetS_CityResInfo SLK_NetS_CityResInfo;	//城池资源信息

struct _slk_NetS_BuildingBaseInfo {
	unsigned char m_kind;	//城市建筑基本信息-种类
	char m_level;	//城市建筑基本信息-等级
	unsigned char m_offset;	//城市建筑基本信息-建筑索引
};
typedef struct _slk_NetS_BuildingBaseInfo SLK_NetS_BuildingBaseInfo;	//一个建筑基本信息

struct _slk_NetS_CityBuildingList {
	short m_count;	//城市建筑基本信息-建筑数量
	SLK_NetS_BuildingBaseInfo m_list[40];	//城市建筑基本信息
};
typedef struct _slk_NetS_CityBuildingList SLK_NetS_CityBuildingList;	//城池建筑基本信息列表

struct _slk_NetS_ActorInfo {
	int m_actorid;	//服务器发送玩家基本信息-角色编号
	char m_name[22];	//服务器发送玩家基本信息-角色名称
	char m_aclass;	//服务器发送玩家基本信息-角色种类
	short m_shape;	//服务器发送玩家基本信息-形象
	short m_level;	//服务器发送玩家基本信息-级别
	i64 m_experience;	//服务器发送玩家基本信息-经验
	i64 m_experience_max;	//服务器发送玩家基本信息-经验上限
	short m_leadership;	//服务器发送玩家基本信息-领导力
	int m_token;	//服务器发送玩家基本信息-钻石
	char m_viplevel;	//服务器发送玩家基本信息-VIP等级
};
typedef struct _slk_NetS_ActorInfo SLK_NetS_ActorInfo;	//角色基本信息

struct _slk_NetS_ItemUse {
	short m_itemoffset;	//物品位置
	short m_usenum;	//使用个数或装备位置
	int m_effres;	//影响，一般用于特效显示
};
typedef struct _slk_NetS_ItemUse SLK_NetS_ItemUse;	//道具使用|装备

struct _slk_NetS_ItemPut {
	short m_res_offset;	//物品源
	short m_res_num;	//物品源个数
	short m_target_offset;	//物品目标
	short m_target_num;	//目标个数
};
typedef struct _slk_NetS_ItemPut SLK_NetS_ItemPut;	//道具放置|卸下

struct _slk_NetS_ItemSettle {
	short m_itemnum;	//整理背包的个数
	short m_itemoffset[100];	//重置的物品位置
};
typedef struct _slk_NetS_ItemSettle SLK_NetS_ItemSettle;	//道具整理

struct _slk_NetS_LostItem {
	short m_itemoffset;	//物品位置
	short m_itemnum;	//数量
	int m_targetid;	//赋予的对象id
	char m_path;	//途径
};
typedef struct _slk_NetS_LostItem SLK_NetS_LostItem;	//丢失道具

struct _slk_NetS_GetItem {
	short m_itemoffset;	//物品位置
	int m_kind;	//物品种类
	short m_type;	//物品类型
	char m_color;	//颜色
	short m_num;	//数量
	int m_targetid;	//赋予的对象id
	char m_path;	//途径
};
typedef struct _slk_NetS_GetItem SLK_NetS_GetItem;	//获得道具

struct _slk_ItemSmpInfo {
	short m_offset;	//物品位置
	short m_kind;	//物品种类
	short m_type;	//物品类型
	short m_num;	//数量
	char m_colorlevel;	//物品颜色
};
typedef struct _slk_ItemSmpInfo SLK_ItemSmpInfo;	//道具简易信息

struct _slk_NetS_ItemList {
	char m_itemext;	//扩展的格子
	short m_kindnum;	//物品数量
	SLK_ItemSmpInfo m_item[100];	//物品
};
typedef struct _slk_NetS_ItemList SLK_NetS_ItemList;	//道具列表

struct _slk_ItemAttr {
	short m_type;	//物品属性所属类型
	short m_ability;	//物品属性
	int m_value;	//物品数据
	int m_addvalue;	//物品增加数据
};
typedef struct _slk_ItemAttr SLK_ItemAttr;	//道具属性

struct _slk_NetS_ItemInfo {
	short m_itemoffset;	//物品位置
	short m_itemkind;	//装备kind,校验
	short m_type;	//物品类型
	short m_level;	//一般等级
	char m_color_level;	//颜色分级
	int m_price;	//售价
	short m_attr_num;	//属性个数
	SLK_ItemAttr m_attr[10];	//属性
};
typedef struct _slk_NetS_ItemInfo SLK_NetS_ItemInfo;	//道具信息

struct _slk_NetS_BuildTimeInfo {
	unsigned char m_offset;	//建筑索引
	int m_time;	//建筑完成时间
	char m_state;	//建筑状态
};
typedef struct _slk_NetS_BuildTimeInfo SLK_NetS_BuildTimeInfo;	//建造时间信息

struct _slk_NetS_BuildTimeInfoList {
	unsigned char m_count;	//有时间信息的建筑数量
	SLK_NetS_BuildTimeInfo m_timeinfo[16];	//建造的时间信息
	int m_servertime;	//服务器时间戳
};
typedef struct _slk_NetS_BuildTimeInfoList SLK_NetS_BuildTimeInfoList;	//建筑的建造时间列表

struct _slk_NetS_CityResChangeInfo {
	char m_restype;	//资源类型
	int m_resvalue;	//资源数量
	char m_path;	//改变途径
};
typedef struct _slk_NetS_CityResChangeInfo SLK_NetS_CityResChangeInfo;	//城市资源改变

struct _slk_NetS_BuildingAbilityInfo {
	unsigned char m_offset;	//建筑索引
	int m_value[4];	//建筑属性
};
typedef struct _slk_NetS_BuildingAbilityInfo SLK_NetS_BuildingAbilityInfo;	//一个建筑属性信息

struct _slk_NetS_AddMapUnit {
	int m_unit_index;	//地图单元-序号
	char m_type;	//地图单元-类型
	unsigned char m_shape;	//地图单元-形象
	char m_stat;	//地图单元-状态
	char m_lastway;	//地图单元-朝向
	short m_posx;	//地图单元-当前坐标
	short m_posy;	//地图单元-当前坐标
	short m_tposx;	//地图单元-目的坐标
	short m_tposy;	//地图单元-目的坐标
	char m_namelen;	//地图单元-名称长度
	char m_name[22];	//地图单元-名称
	unsigned char m_level;	//地图单元-等级
	unsigned char m_underfire_count;	//地图单元-被多少部队攻击
	SLK_ArmyCorrdinate m_underfire_frompos[32];	//地图单元-被多少部队攻击坐标
};
typedef struct _slk_NetS_AddMapUnit SLK_NetS_AddMapUnit;	//显示单元进入地图

struct _slk_NetS_DelMapUnit {
	int m_unit_index;	//地图单元-序号
};
typedef struct _slk_NetS_DelMapUnit SLK_NetS_DelMapUnit;	//显示单元离开地图

struct _slk_NetS_WorldMapInfo {
	short m_map_width;	//世界地图大小（格子数）
	short m_map_height;	//世界地图大小（格子数）
	char m_area_width;	//区域大小（一个区域几个地图格子）
	char m_area_height;	//区域大小（一个区域几个地图格子）
	short m_map_area_xnum;	//一个地图有多少区域
	short m_map_area_ynum;	//一个地图有多少区域
	short m_my_city_posx;	//我的城池位置
	short m_my_city_posy;	//我的城池位置
	int m_my_city_unit_index;	//我的城池地图单元-序号
};
typedef struct _slk_NetS_WorldMapInfo SLK_NetS_WorldMapInfo;	//世界地图信息

struct _slk_NetS_MapUnitCorrdinate {
	int m_unit_index;	//地图单元-序号
	short m_posx;	//地图单元-当前坐标
	short m_posy;	//地图单元-当前坐标
};
typedef struct _slk_NetS_MapUnitCorrdinate SLK_NetS_MapUnitCorrdinate;	//地图显示单元位置

struct _slk_NetS_MarchRoute {
	int m_from_unit_index;	//行军路线-出发单元
	short m_from_posx;	//行军路线-出发位置
	short m_from_posy;	//行军路线-出发位置
	int m_to_unit_index;	//行军路线-到达单元
	short m_to_posx;	//行军路线-到达位置
	short m_to_posy;	//行军路线-到达位置
};
typedef struct _slk_NetS_MarchRoute SLK_NetS_MarchRoute;	//创建部队时候创建的行军路线

struct _slk_NetS_OutsideMapInfo {
	short m_map_width;	//野外地图大小（格子数）
	short m_map_height;	//野外地图大小（格子数）
	short m_my_city_posx;	//我的城池位置
	short m_my_city_posy;	//我的城池位置
	short m_match_city_posx;	//对手城池位置
	short m_match_city_posy;	//对手城池位置
};
typedef struct _slk_NetS_OutsideMapInfo SLK_NetS_OutsideMapInfo;	//野外地图信息

struct _slk_NetS_MailSimpleInfo {
	i64 m_mailid;	//邮件id
	char m_type;	//邮件类型
	short m_from_name_length;	//发送人名称长度
	char m_from_name[22];	//发送人名称
	short m_title_length;	//标题长度
	char m_title[32];	//标题
	short m_simple_content_length;	//简单内容长度
	char m_simple_content[32];	//简单内容
	char m_has_attach;	//是否有附件
	int m_recvtime;	//接收时间
	int m_deltime;	//删除时间
	char m_read;	//已读标记
	char m_attach_isget;	//附件是否领取
};
typedef struct _slk_NetS_MailSimpleInfo SLK_NetS_MailSimpleInfo;	//邮件简单信息

struct _slk_NetS_MailList {
	short m_count;	//邮件数量
	SLK_NetS_MailSimpleInfo m_list[10];	//邮件列表
};
typedef struct _slk_NetS_MailList SLK_NetS_MailList;	//邮件列表

struct _slk_NetS_MailFullInfo {
	i64 m_mailid;	//邮件id
	short m_content_length;	//完整内容长度
	char m_content[1024];	//完整内容
	short m_attach_length;	//附件内容长度
	char m_attach[128];	//附件内容
	char m_read;	//已读标记
};
typedef struct _slk_NetS_MailFullInfo SLK_NetS_MailFullInfo;	//邮件完整内容信息

struct _slk_NetS_FightMsgSimpleInfo {
	i64 m_msgid;	//战报消息id
	char m_read;	//已读标记
	int m_recvtime;	//时间
	char m_status;	//状态
	char m_target_type;	//战报目标类型
	short m_title_length;	//战报标题长度
	char m_title[32];	//战报标题
};
typedef struct _slk_NetS_FightMsgSimpleInfo SLK_NetS_FightMsgSimpleInfo;	//战报简单消息

struct _slk_NetS_FightMsgList {
	short m_count;	//战报列表数量
	SLK_NetS_FightMsgSimpleInfo m_list[10];	//战报列表
};
typedef struct _slk_NetS_FightMsgList SLK_NetS_FightMsgList;	//战报列表

struct _slk_NetS_FightMsgInfo {
	i64 m_msgid;	//战报消息id
	char m_read;	//已读标记
	int m_recvtime;	//时间
	short m_posx;	//发生地坐标
	short m_posy;	//发生地坐标
	char m_status;	//状态
	char m_target_type;	//对方类型
	int m_target_id;	//对方类型对应id
	short m_target_posx;	//对方位置
	short m_target_posy;	//对方位置
	int m_wood;	//资源-木头
	int m_iron;	//资源-铁
	int m_food;	//资源-食物
	i64 m_recordid;	//录像
	short m_title_length;	//标题长度
	char m_title[32];	//标题
};
typedef struct _slk_NetS_FightMsgInfo SLK_NetS_FightMsgInfo;	//战报完整信息

struct _slk_NetS_BuildingAbilityList {
	short m_count;	//建筑属性列表个数
	SLK_NetS_BuildingAbilityInfo m_list[35];	//建筑属性列表
};
typedef struct _slk_NetS_BuildingAbilityList SLK_NetS_BuildingAbilityList;	//建筑属性信息列表

struct _slk_NetS_ChatMaskList {
	short m_count;	//聊天屏蔽玩家列表
	int m_chat_mask_list[64];	//聊天屏蔽玩家列表
};
typedef struct _slk_NetS_ChatMaskList SLK_NetS_ChatMaskList;	//聊天屏蔽列表

struct _slk_NetS_TroopInfo {
	short m_corpstype;	//兵种类型
	char m_level;	//等级
	int m_have;	//拥有数量
	int m_wound;	//受伤数量
};
typedef struct _slk_NetS_TroopInfo SLK_NetS_TroopInfo;	//兵种信息

struct _slk_NetS_TroopInfoList {
	short m_count;	//列表长度
	SLK_NetS_TroopInfo m_troop_list[50];	//兵种信息列表
};
typedef struct _slk_NetS_TroopInfoList SLK_NetS_TroopInfoList;	//兵种信息列表

struct _slk_NetS_HeroSmpInfo {
	short m_offset;	//英雄索引
	short m_herokind;	//英雄类别
	short m_level;	//英雄等级
	char m_official;	//英雄官职
};
typedef struct _slk_NetS_HeroSmpInfo SLK_NetS_HeroSmpInfo;	//英雄简略信息

struct _slk_NetS_HeroList {
	short m_count;	//英雄列表个数
	SLK_NetS_HeroSmpInfo m_list[32];	//英雄列表
};
typedef struct _slk_NetS_HeroList SLK_NetS_HeroList;	//英雄列表

struct _slk_NetS_HeroInfo {
	short m_offset;	//英雄索引
	char m_name[22];	//英雄名称
	short m_attack;	//英雄攻击力
	short m_defance;	//英雄防御
	short m_agility;	//英雄敏捷
	short m_hp;	//英雄血量
	i64 m_experience;	//英雄经验
	short m_skill[4];	//英雄技能
	short m_equip[6];	//英雄装备
};
typedef struct _slk_NetS_HeroInfo SLK_NetS_HeroInfo;	//英雄详细信息

struct _slk_NetS_CityBattleTroopList {
	short m_corps;	//出征-士兵兵种
	char m_level;	//出征-士兵等级
	int m_count;	//出征-士兵个数
};
typedef struct _slk_NetS_CityBattleTroopList SLK_NetS_CityBattleTroopList;	//出征-部队信息

struct _slk_NetS_CityBattleActorList {
	char m_name[22];	//出征-玩家名称
	int m_actor_index;	//出征-玩家索引
};
typedef struct _slk_NetS_CityBattleActorList SLK_NetS_CityBattleActorList;	//出征-玩家信息

struct _slk_NetS_CityBattleInfo {
	char m_actor_count;	//出征-玩家个数
	SLK_NetS_CityBattleActorList m_actor_list[5];	//出征-玩家列表
	char m_troop_count;	//出征-兵种个数
	SLK_NetS_CityBattleTroopList m_troop_list[10];	//出征-兵种列表
	short m_pos_x;	//出征-发起者的坐标
	short m_pos_y;	//出征-发起者的坐标
	int m_remaining_time;	//出征-剩余时间
	char m_type;	//出征-行动类别(普通攻击/集结攻击)
};
typedef struct _slk_NetS_CityBattleInfo SLK_NetS_CityBattleInfo;	//出征-战斗整体信息

int struct_NetS_Login_send( char **pptr, int *psize, SLK_NetS_Login *pValue );
int struct_ListInfo_send( char **pptr, int *psize, SLK_ListInfo *pValue );
int struct_NetS_List_send( char **pptr, int *psize, SLK_NetS_List *pValue );
int struct_NetS_Create_send( char **pptr, int *psize, SLK_NetS_Create *pValue );
int struct_NetS_EnterInfo_send( char **pptr, int *psize, SLK_NetS_EnterInfo *pValue );
int struct_NetS_Delete_send( char **pptr, int *psize, SLK_NetS_Delete *pValue );
int struct_NetS_Heart_send( char **pptr, int *psize, SLK_NetS_Heart *pValue );
int struct_NetS_Notify_send( char **pptr, int *psize, SLK_NetS_Notify *pValue );
int struct_NetS_Talk_send( char **pptr, int *psize, SLK_NetS_Talk *pValue );
int struct_NetS_FightIn_send( char **pptr, int *psize, SLK_NetS_FightIn *pValue );
int struct_NetS_FightSet_send( char **pptr, int *psize, SLK_NetS_FightSet *pValue );
int struct_NetS_FightMove_send( char **pptr, int *psize, SLK_NetS_FightMove *pValue );
int struct_NetS_FightAction_send( char **pptr, int *psize, SLK_NetS_FightAction *pValue );
int struct_ArmyCorrdinate_send( char **pptr, int *psize, SLK_ArmyCorrdinate *pValue );
int struct_NetS_CityResInfo_send( char **pptr, int *psize, SLK_NetS_CityResInfo *pValue );
int struct_NetS_BuildingBaseInfo_send( char **pptr, int *psize, SLK_NetS_BuildingBaseInfo *pValue );
int struct_NetS_CityBuildingList_send( char **pptr, int *psize, SLK_NetS_CityBuildingList *pValue );
int struct_NetS_ActorInfo_send( char **pptr, int *psize, SLK_NetS_ActorInfo *pValue );
int struct_NetS_ItemUse_send( char **pptr, int *psize, SLK_NetS_ItemUse *pValue );
int struct_NetS_ItemPut_send( char **pptr, int *psize, SLK_NetS_ItemPut *pValue );
int struct_NetS_ItemSettle_send( char **pptr, int *psize, SLK_NetS_ItemSettle *pValue );
int struct_NetS_LostItem_send( char **pptr, int *psize, SLK_NetS_LostItem *pValue );
int struct_NetS_GetItem_send( char **pptr, int *psize, SLK_NetS_GetItem *pValue );
int struct_ItemSmpInfo_send( char **pptr, int *psize, SLK_ItemSmpInfo *pValue );
int struct_NetS_ItemList_send( char **pptr, int *psize, SLK_NetS_ItemList *pValue );
int struct_ItemAttr_send( char **pptr, int *psize, SLK_ItemAttr *pValue );
int struct_NetS_ItemInfo_send( char **pptr, int *psize, SLK_NetS_ItemInfo *pValue );
int struct_NetS_BuildTimeInfo_send( char **pptr, int *psize, SLK_NetS_BuildTimeInfo *pValue );
int struct_NetS_BuildTimeInfoList_send( char **pptr, int *psize, SLK_NetS_BuildTimeInfoList *pValue );
int struct_NetS_CityResChangeInfo_send( char **pptr, int *psize, SLK_NetS_CityResChangeInfo *pValue );
int struct_NetS_BuildingAbilityInfo_send( char **pptr, int *psize, SLK_NetS_BuildingAbilityInfo *pValue );
int struct_NetS_AddMapUnit_send( char **pptr, int *psize, SLK_NetS_AddMapUnit *pValue );
int struct_NetS_DelMapUnit_send( char **pptr, int *psize, SLK_NetS_DelMapUnit *pValue );
int struct_NetS_WorldMapInfo_send( char **pptr, int *psize, SLK_NetS_WorldMapInfo *pValue );
int struct_NetS_MapUnitCorrdinate_send( char **pptr, int *psize, SLK_NetS_MapUnitCorrdinate *pValue );
int struct_NetS_MarchRoute_send( char **pptr, int *psize, SLK_NetS_MarchRoute *pValue );
int struct_NetS_OutsideMapInfo_send( char **pptr, int *psize, SLK_NetS_OutsideMapInfo *pValue );
int struct_NetS_MailSimpleInfo_send( char **pptr, int *psize, SLK_NetS_MailSimpleInfo *pValue );
int struct_NetS_MailList_send( char **pptr, int *psize, SLK_NetS_MailList *pValue );
int struct_NetS_MailFullInfo_send( char **pptr, int *psize, SLK_NetS_MailFullInfo *pValue );
int struct_NetS_FightMsgSimpleInfo_send( char **pptr, int *psize, SLK_NetS_FightMsgSimpleInfo *pValue );
int struct_NetS_FightMsgList_send( char **pptr, int *psize, SLK_NetS_FightMsgList *pValue );
int struct_NetS_FightMsgInfo_send( char **pptr, int *psize, SLK_NetS_FightMsgInfo *pValue );
int struct_NetS_BuildingAbilityList_send( char **pptr, int *psize, SLK_NetS_BuildingAbilityList *pValue );
int struct_NetS_ChatMaskList_send( char **pptr, int *psize, SLK_NetS_ChatMaskList *pValue );
int struct_NetS_TroopInfo_send( char **pptr, int *psize, SLK_NetS_TroopInfo *pValue );
int struct_NetS_TroopInfoList_send( char **pptr, int *psize, SLK_NetS_TroopInfoList *pValue );
int struct_NetS_HeroSmpInfo_send( char **pptr, int *psize, SLK_NetS_HeroSmpInfo *pValue );
int struct_NetS_HeroList_send( char **pptr, int *psize, SLK_NetS_HeroList *pValue );
int struct_NetS_HeroInfo_send( char **pptr, int *psize, SLK_NetS_HeroInfo *pValue );
int struct_NetS_CityBattleTroopList_send( char **pptr, int *psize, SLK_NetS_CityBattleTroopList *pValue );
int struct_NetS_CityBattleActorList_send( char **pptr, int *psize, SLK_NetS_CityBattleActorList *pValue );
int struct_NetS_CityBattleInfo_send( char **pptr, int *psize, SLK_NetS_CityBattleInfo *pValue );

#endif
