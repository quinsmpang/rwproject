#ifndef NETPROCESS_H_
#define NETPROCESS_H_
#include "Game.h"

struct _act {
	char m_nPos;
	char m_nType;
	short m_nValue;
};
typedef struct _act SLK_Act;

struct _sactorlistinfo
{
	int actorid;
	char lockstat;
	int freepoint;
	int shape;
	char name[NAME_SIZE];
	char profession;
	char gender;
	short level;
};
typedef struct _sactorlistinfo SActorListInfo;

struct _giveitem
{
	short m_index;	// 物品栏中的序号
	short m_num;	// 物品个数
};
typedef struct _giveitem SLK_GiveItem;


#define ACTOR_STAT_STAND	0
#define ACTOR_STAT_WALK		1
#define ACTOR_STAT_FIGHT	2

#define ACTOR_STAT_UPGRADE	10
#define ACTOR_STAT_USEITEM	11


// 从服务器接收到的消息
#define CMDS_BASE			0
#define CMDS_LOGIN			(CMDS_BASE+1)	// 通知角色已经登入
#define CMDS_ENTERINFO		(CMDS_BASE+2)	
#define CMDS_LIST			(CMDS_BASE+3)	// 列角色
#define CMDS_CREATE			(CMDS_BASE+4)	// 创建角色
#define CMDS_DELETE			(CMDS_BASE+5)	// 删除角色
#define CMDS_ENTERGAME		(CMDS_BASE+6)	// 通知角色进入游戏
#define CMDS_ADDACTOR		(CMDS_BASE+7)	// 通知一个新角色进入
#define CMDS_DELACTOR		(CMDS_BASE+8)	// 通知一个角色已经离开
#define CMDS_MOVE			(CMDS_BASE+9)	// 通知一个角色行走路径
#define CMDS_TALK			(CMDS_BASE+10)	// 通知一条消息
#define CMDS_CHANGEMAP		(CMDS_BASE+11)	// 切换场景
#define CMDS_FIGHT			(CMDS_BASE+12)	// 战斗
#define CMDS_FIGHT_PROCESS	(CMDS_BASE+13)	// 战斗结果
#define CMDS_ADDTEAM		(CMDS_BASE+14)	// 增加新成员
#define CMDS_ITEMLIST		(CMDS_BASE+15)	// 物品列表
#define CMDS_ITEMUSE		(CMDS_BASE+16)	// 使用物品
#define CMDS_ITEMPUT		(CMDS_BASE+17)	// 使用物品
#define CMDS_EXPERIENCE		(CMDS_BASE+18)	// 角色获得经验(单向通知)
#define CMDS_ACTORINFO		(CMDS_BASE+19)	// 角色信息
#define CMDS_SMPINFO		(CMDS_BASE+20)	// 角色简单信息
#define CMDS_NPCTALK		(CMDS_BASE+21)	// 和NPC对话信息
#define CMDS_LOSTITEM		(CMDS_BASE+22)	// 失去物品
#define CMDS_GETITEM		(CMDS_BASE+23)	// 得到物品
#define CMDS_MONEY			(CMDS_BASE+24)	// 角色获得金钱(单向通知)
#define CMDS_ASKMAKETEAM	(CMDS_BASE+25)	// 获得请求组队的消息
#define CMDS_SYSNOTIFY		(CMDS_BASE+26)	// 系统通知
#define CMDS_REPLYMAKETEAM	(CMDS_BASE+27)	// 答复组队
#define CMDS_SETTEAMINFO	(CMDS_BASE+28)	// 组队信息
#define CMDS_LEAVETEAM		(CMDS_BASE+29)	// 离开队伍
#define CMDS_GETQUEST		(CMDS_BASE+30)	// 任务列表
#define CMDS_COORDINATE		(CMDS_BASE+31)	// 角色坐标信息
#define CMDS_ASKMAKEFRIEND	(CMDS_BASE+32)	// 增加好友的答复
#define CMDS_FRIENDMSG		(CMDS_BASE+33)	// 好友的消息
#define CMDS_OPENSHOP		(CMDS_BASE+34)	// 打开购买物品的对话框
#define CMDS_BUYITEM		(CMDS_BASE+35)	// 购买到了物品
#define CMDS_SKILLLIST		(CMDS_BASE+36)	// 技能列表
#define CMDS_ITEMINFO		(CMDS_BASE+37)	// 物品信息
#define CMDS_ACTORMODIFYINFO	(CMDS_BASE+38)	// 角色装备信息
#define CMDS_GETPETLIST		(CMDS_BASE+39)	// 宠物列表
#define CMDS_GETPETINFO		(CMDS_BASE+40)	// 宠物信息
#define CMDS_SETPETNAME		(CMDS_BASE+41)	// 宠物名字变更
#define CMDS_LOSTPET		(CMDS_BASE+42)	// 宠物丢失
#define CMDS_SETFIGHTPET	(CMDS_BASE+43)	// 设置宠物参战
#define CMDS_SHOWPET		(CMDS_BASE+44)	// 设置宠物观看
#define CMDS_STSTCHANGE		(CMDS_BASE+45)	// 角色状态改变
#define CMDS_NEWPET			(CMDS_BASE+46)	// 抓到的新宠物
#define CMDS_ITEMTALK		(CMDS_BASE+47)	// 传送场景答复
#define CMDS_SELLITEM		(CMDS_BASE+48)	// 出售了物品
#define CMDS_OPENSELL		(CMDS_BASE+49)	// 打开出售的对话框
#define CMDS_OPENTRADE		(CMDS_BASE+50)	// 打开交易的对话框
#define CMDS_TRADESET		(CMDS_BASE+51)	// 当对方设置交易的物品金钱或者取消设置
#define CMDS_TRADEOVER		(CMDS_BASE+52)	// 当双方决定了交易或者单方面取消交易，将收到结束
#define CMDS_OPENTAKE		(CMDS_BASE+53)	// 打开寄存处的对话框
#define CMDS_TAKEITEM		(CMDS_BASE+54)	// 寄存或者取回了物品
#define CMDS_OPENBANK		(CMDS_BASE+55)	// 打开银行的对话框
#define CMDS_TAKEMONEY		(CMDS_BASE+56)	// 存或者取了钱
#define CMDS_FRIENDINFO		(CMDS_BASE+57)	// 查询到的好友信息
#define CMDS_FRIENDONLINE	(CMDS_BASE+58)	// 好友上下线信息
#define CMDS_GETPREFIXLIST	(CMDS_BASE+59)	// 发送称谓列表
#define CMDS_SETPREFIX		(CMDS_BASE+60)	// 设置新的称谓
#define CMDS_SETBOOTHSIGN	(CMDS_BASE+61)	// 设置店铺招牌
#define CMDS_BOOTHREPLY		(CMDS_BASE+62)	// 答复店主
#define CMDS_BOOTHPROCESS	(CMDS_BASE+63)	// 通知店主(上下架成功失败)
#define CMDS_BOOTHLIST		(CMDS_BASE+64)	// 摊位的商品列表
#define CMDS_BOOTHBUY		(CMDS_BASE+65)	// 摊位购买
#define CMDS_TREASURETALK	(CMDS_BASE+66)	// 宝藏信息
#define CMDS_OPENSTONE		(CMDS_BASE+67)	// 打开镶嵌宝石对话框
#define CMDS_STONECOMPOSE	(CMDS_BASE+68)	// 宝石合成完毕
#define CMDS_ACTORSMALLINFO	(CMDS_BASE+69)	// 一些常更新的角色信息
#define CMDS_SKILLINFO		(CMDS_BASE+70)	// 单个技能信息
#define CMDS_CARDSTAT		(CMDS_BASE+71)	// 角色卡状态改变
#define CMDS_OPENSELLCARD	(CMDS_BASE+72)	// 打开寄售卡的对话框
#define CMDS_OPENBUYCARD	(CMDS_BASE+73)	// 打开购买卡的对话框
#define CMDS_REPAIRITEM		(CMDS_BASE+74)	// 修理完毕
#define CMDS_SURPRISEGET	(CMDS_BASE+75)	// 得到惊喜
#define CMDS_TEAMLIST		(CMDS_BASE+76)	// 队伍成员简单的列表


// 从客户端要发出的消息
#define CMDC_BASE			0
//#define CMDC_TIMER			(CMDC_BASE+1)	// 服务器专用消息,客户端不使用
#define CMDC_LOGIN			(CMDC_BASE+2)	// 登入服务器
#define CMDC_ENTERINFO		(CMDC_BASE+3)
#define CMDC_LIST			(CMDC_BASE+4)	// 列角色
#define CMDC_CREATE			(CMDC_BASE+5)	// 创建角色
#define CMDC_DELETE			(CMDC_BASE+6)	// 删除角色
#define CMDC_ENTERGAME		(CMDC_BASE+7)	// 进入游戏
#define CMDC_LEAVEGAME		(CMDC_BASE+8)	// 离开游戏
#define CMDC_MOVE			(CMDC_BASE+9)	// 移动主角
#define CMDC_TALK			(CMDC_BASE+10)	// 主角说话
#define CMDC_FIGHT			(CMDC_BASE+11)	// 发动攻击
#define CMDC_MAKETEAM		(CMDC_BASE+12)	// 发起组队(该指令已经作废)
#define CMDC_ITEMLIST		(CMDC_BASE+13)	// 获取物品列表
#define CMDC_ITEMUSE		(CMDC_BASE+14)	// 使用物品
#define CMDC_ITEMPUT		(CMDC_BASE+15)	// 物品放置到格子
#define CMDC_ACTORINFO		(CMDC_BASE+16)	// 角色信息
#define CMDC_TOUCHNPC		(CMDC_BASE+17)	// 接触NPC
#define CMDC_GIVEITEM		(CMDC_BASE+18)	// 给予物品
#define CMDC_ASKMAKETEAM	(CMDC_BASE+19)	// 请求组队
#define CMDC_REPLYMAKETEAM	(CMDC_BASE+20)	// 答复组队
#define CMDC_PROCESSTEAM	(CMDC_BASE+21)	// 组队操作
#define CMDC_GETQUEST		(CMDC_BASE+22)	// 获取任务列表
#define CMDC_ATTACK			(CMDC_BASE+23)	// 攻击他人
#define CMDC_ASKMAKEFRIEND	(CMDC_BASE+24)	// 好友申请
#define CMDC_FRIENDMSG		(CMDC_BASE+25)	// 好友消息
#define CMDC_APPLYATTR		(CMDC_BASE+26)	// 更新属性点
#define CMDC_SHOPBUY		(CMDC_BASE+27)	// 商店购物
#define CMDC_ITEMDROP		(CMDC_BASE+28)	// 丢弃物品
#define CMDC_GMCOMMAND		(CMDC_BASE+29)	// GM指令
#define CMDC_SKILLLIST		(CMDC_BASE+30)	// 技能列表
#define CMDC_ITEMINFO		(CMDC_BASE+31)	// 物品信息
#define CMDC_GETPETLIST		(CMDC_BASE+32)	// 宠物列表
#define CMDC_PETPROCESS		(CMDC_BASE+33)	// 宠物操作 0取属性 1释放 2设置战斗 3设置观看
#define CMDC_SETPETNAME		(CMDC_BASE+34)	// 宠物名字变更
#define CMDC_APPLYPETATTR	(CMDC_BASE+35)	// 宠物属性变更
#define CMDC_STATCHANGE		(CMDC_BASE+36)	// 角色状态改变的广播
#define CMDC_SHOPSELL		(CMDC_BASE+37)	// 物品出售
#define CMDC_REQUESTTRADE	(CMDC_BASE+38)	// 发出交易请求
#define CMDC_TRADESET		(CMDC_BASE+39)	// 设置交易的物品金钱 - 无取消设置
#define CMDC_TRADEOVER		(CMDC_BASE+40)	// 发出交易已经确定或者取消
#define CMDC_SHOPTAKE		(CMDC_BASE+41)	// 寄存和取回
#define CMDC_SHOPBANK		(CMDC_BASE+42)	// 存和取钱
#define CMDC_QUERYFRIEND	(CMDC_BASE+43)	// 查询好友
#define CMDC_FRIENDINFO		(CMDC_BASE+44)	// 好友资料
#define CMDC_DELETEFRIEND	(CMDC_BASE+45)	// 删除好友
#define CMDC_PROCESSPREFIX	(CMDC_BASE+46)	// 处理称谓,0设置 1删除
#define CMDC_ASKBOOTH		(CMDC_BASE+47)	// 请求摆摊
#define CMDC_CHANGEBOOTHNAME (CMDC_BASE+48)	// 改摊名
#define CMDC_BOOTHPROCESS	(CMDC_BASE+49)	// 处理摆摊,上架和下架
#define CMDC_OPENBOOTH		(CMDC_BASE+50)	// 打开玩家的摊位
#define CMDC_BOOTHBUY		(CMDC_BASE+51)	// 购买摊位上的商品
#define CMDC_SETCURSKILL	(CMDC_BASE+52)	// 设置技能
#define CMDC_FRIENDGROUPMOVE (CMDC_BASE+53)	// 将好友移动到组
#define CMDC_FRIENDGROUPCHG	(CMDC_BASE+54)	// 将分组改名
#define CMDC_STONECOMPOSE	(CMDC_BASE+55)	// 宝石合成
#define CMDC_SETCONFIG		(CMDC_BASE+56)	// 设置玩家的配置
#define CMDC_HEARTBEAT		(CMDC_BASE+57)	// 心跳
#define CMDC_CARDSELL		(CMDC_BASE+58)	// 卖卡
#define CMDC_CARDBUY		(CMDC_BASE+59)	// 买卡
#define CMDC_REPAIRITEM		(CMDC_BASE+60)	// 修理装备
#define CMDC_SURPRISESET	(CMDC_BASE+61)	// 设置惊喜

void LK_NetInit();

int Net_HeartBeat( int sock_index );

int Net_Login( char *szUserName, char *szPassWord, int sock_index );
int Net_Logout( int sock_index );
int Net_List( int sock_index );
int Net_Create( char nPosition, char *name, int sock_index );
int Net_Delete( char nOffset, int sock_index );
int Net_EnterGame( int actorid, int sock_index );
int Net_LeaveGame( int sock_index );
int Net_Move( int posx, int posy, int sock_index );
int Net_Talk( const char *pText, char nType, int sock_index );
int Net_Fight( SLK_Act *pAct, int sock_index );
int Net_GMCommand( short nCmd, int value1, int value2, int value3, int value4, char *pMsg, int sock_index );
int Net_TouchNpc( char nType, char nSelected, int nActorID, int sock_index );
int Net_ItemUse( short nItemIndex, short nItemNum, char nPetIndex, int sock_index );

#endif
