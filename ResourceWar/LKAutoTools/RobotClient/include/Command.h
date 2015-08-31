#ifndef COMMAND_H_
#define COMMAND_H_


// 本地消息
#define CMD_LOCALBASE		10
#define CMD_MAKEPATH		(CMD_LOCALBASE+1)

#define CMD_BASE			100
#define CMD_SHOW_MESSAGE	(CMD_BASE+1)
#define CMD_LOGIN			(CMD_BASE+2)	// 通知角色已经登入
#define CMD_ENTERINFO		(CMD_BASE+3)
#define CMD_LIST			(CMD_BASE+4)	// 列角色
#define CMD_CREATE			(CMD_BASE+5)	// 创建角色
#define CMD_DELETE			(CMD_BASE+6)	// 删除角色
#define CMD_ENTERGAME		(CMD_BASE+7)	// 通知角色进入游戏
#define CMD_ADDACTOR		(CMD_BASE+8)	// 通知一个新角色进入
#define CMD_DELACTOR		(CMD_BASE+9)	// 通知一个角色已经离开
#define CMD_MOVE			(CMD_BASE+10)	// 通知一个角色行走路径
#define CMD_TALK			(CMD_BASE+11)	// 通知一条消息
#define CMD_CHANGEMAP		(CMD_BASE+12)	// 切换场景
#define CMD_FIGHT			(CMD_BASE+13)	// 战斗
#define CMD_FIGHT_PROCESS	(CMD_BASE+14)	// 战斗处理
#define CMD_ADDTEAM			(CMD_BASE+15)	// 增加新成员
#define CMD_ITEMLIST		(CMD_BASE+16)	// 物品列表
#define CMD_ITEMUSE			(CMD_BASE+17)	// 使用物品
#define CMD_ITEMPUT			(CMD_BASE+18)	// 使用物品
#define CMD_COORDINATE		(CMD_BASE+32)	// 获得坐标
#define CMD_STSTCHANGE		(CMD_BASE+46)	// 角色状态改变

#define NOTIFY_NORMAL_MSG				0	// 普通消息
#define NOTIFY_SYSTEM_MSG				1	// 系统消息

#define STAT_WAIT	-1	// 等待状态-逻辑除了心跳什么都不做

#define STAT_NONE	0	// 逻辑什么都不做
#define STAT_FWAIT	1	// 战斗等待
#define STAT_BEGIN	2	// 未登陆状态-逻辑立即执行登陆
#define STAT_LOGIN	3	// 已经登陆状态-逻辑立即执行列表
#define STAT_LIST	4	// 已经列表状态-逻辑立即执行进入游戏
#define STAT_NORMAN	5	// 已经进入游戏状态-逻辑按时执行普通走路操作
#define STAT_FIGHT	6	// 已经进入战斗状态-逻辑立即执行攻击操作
#define STAT_FIGHTWAIT	7	// 战斗等待状态-逻辑立即执行等待一段时间后转换为fight状态
#define STAT_ENTER 8
#define STAT_CREATE 9
#define STAT_LOGOUT	10	// 登出

#define STAT_STEP1	11
#define STAT_STEP2	12
#define STAT_STEP3	13
#define STAT_STEP4	14
#define STAT_STEP5	15
#define STAT_STEP6	16
#define STAT_STEP7	17
#define STAT_STEP8	18
#define STAT_STEP9	19
#define STAT_STEP10	20
#define STAT_STEP11	21
#define STAT_STEP12	22
#define STAT_STEP13	23
#define STAT_STEP14	24
#define STAT_STEP15	25
#define STAT_STEP16	26
#define STAT_STEP17	27
#define STAT_STEP18	28
#define STAT_STEP19	29
#define STAT_STEP20	30
#define STAT_STEP21	31
#define STAT_STEP22	32
#define STAT_STEP23	33
#define STAT_STEP24	34
#define STAT_STEP25	35
#define STAT_STEP26	36
#define STAT_STEP27	37
#define STAT_STEP28	38
#define STAT_STEP29	39
#define STAT_STEP30	40
#define STAT_STEP31	41
#define STAT_STEP32	42
#define STAT_STEP33	43
#define STAT_STEP34	44
#define STAT_STEP35	45
#define STAT_STEP36	46
#define STAT_STEP37	47
#define STAT_STEP38	48
#define STAT_STEP39	49
#define STAT_STEP40	50
#define STAT_STEP40x 111
#define STAT_STEP41	51
#define STAT_STEP42	52
#define STAT_STEP43	53
#define STAT_STEP44	54
#define STAT_STEP45	55
#define STAT_STEP46	56
#define STAT_STEP47	57
#define STAT_STEP48	58
#define STAT_STEP49	59
#define STAT_STEP50	60
#define STAT_STEP51	61
#define STAT_STEP52	62
#define STAT_STEP53	63
#define STAT_STEP54	64
#define STAT_STEP55	65
#define STAT_STEP56	66
#define STAT_STEP57	67
#define STAT_STEP58	68
#define STAT_STEP59	69
#define STAT_STEP60	70
#define STAT_STEP61	71
#define STAT_STEP62	72
#define STAT_STEP63	73
#define STAT_STEP64	74
#define STAT_STEP65	75
#define STAT_STEP66	76
#define STAT_STEP67	77
#define STAT_STEP68	78
#define STAT_STEP69	79
#define STAT_STEP70	80
#define STAT_STEP71	81
#define STAT_STEP72	82
#define STAT_STEP73	83
#define STAT_STEP74	84
#define STAT_STEP75	85
#define STAT_STEP76	86
#define STAT_STEP77	87
#define STAT_STEP78	88
#define STAT_STEP79	89
#define STAT_STEP80	90
#define STAT_STEP81	91
#define STAT_STEP82	92
#define STAT_STEP83	93
#define STAT_STEP84	94
#define STAT_STEP85	95
#define STAT_STEP86	96
#define STAT_STEP87	97
#define STAT_STEP88	98
#define STAT_STEP89	99
#define STAT_STEP90	100
#define STAT_STEP91	101
#define STAT_STEP92	102
#define STAT_STEP93	103
#define STAT_STEP94	104
#define STAT_STEP95	105
#define STAT_STEP96	106
#define STAT_STEP97	107
#define STAT_STEP98	108
#define STAT_STEP99	109
#define STAT_STEP100	110

struct _splayer
{
	int actorid;
	int exec_code;
	char stat;
	short posx;
	short posy;
	short waitframe;
	char wayx;
	char wayy;
	int wait_times;
	char stat2;
	char wait_stat;
	char isconnected;
};
typedef struct _splayer SLK_Player;


struct _clk_NetS_Login {
	int m_result;	//返回值
	char m_ekey_type;	//密保类型
	char m_challenge[16];	//挑战码
	char m_username[51];	//用户名
	char m_token[65];	//token
};
typedef struct _clk_NetS_Login CLK_NetS_Login;	//服务器端发送登录结果

int Recv_Login( char *data, int size );
int Recv_Logout( char *data, int size );
int Recv_EnterGame( char *data, int size );
int Recv_Move( char *data, int size );
int Recv_Talk( char *data, int size );

void LK_CommandInit();
int LK_ProcessMsg( int cmd, char *msg, int size, int sock_index );

#endif
