//#ifndef __CLUB_H
//#define __CLUB_H
//#include "define.h"
//
//#define		CLUB_CHECK_INDEX(index)			do{	if( index <= 0 || index >= g_club_maxcount) \
//	return -1; \
//}while ( 0 )
//
//#define		CLUB_GETID_BYINDEX(index, id) do{ if( index <= 0 || index >= g_club_maxcount) \
//	return -1; \
//	id = g_club[index].m_clubid; \
//if ( id <= 0 )\
//	return -1; \
//}while ( 0 )
//
//
//#define	MAX_MEMBER_COUNT		100		// 帮派最大成员数量
//#define	CLUB_NOTIFY_SIZE		128		// 帮派章程长度
//#define	MAX_ASKER_COUNT			64		// 帮派申请人数
//#define	MAX_CLUBBUSINESS		4		// 帮派商人最大数量
//#define	MAX_CLUBLEVEL			5		// 帮派最大等级
//#define	MAX_CLUB_BUILDLEVEL		20		// 帮派建筑最大等级
//#define	CLUB_MAINUPGRADE_ING	100		// 帮派正在升级标示
//#define	CLUB_SHOP_ITEM_COUNT	20		// 帮派随机刷新的物品最大数量
//#define	CLUB_STORAGE_PAGECOUNT	80		// 帮派家族等级存储的物品个数
//#define	CLUB_BUSINESS_ITEMKIND	582		// 帮派跑商道具
//#define CLUBEVENT_MAX_COUNT		20		// 最多事件数
//
///* 职务 */
//#define	CLUBPLACE_LEADER	100	// 帮主
//#define	CLUBPLACE_FLEADER	90	// 副帮主
//#define	CLUBPLACE_CAPTAIN4	84	// 玄武堂主
//#define	CLUBPLACE_CAPTAIN3	83	// 朱雀堂主
//#define	CLUBPLACE_CAPTAIN2	82	// 白虎堂主
//#define	CLUBPLACE_CAPTAIN1	81	// 青龙堂主
//#define	CLUBPLACE_MEMBER	10	// 帮众
//#define	CLUBPLACE_LOW		1	// 学徒
//
///* 事件编号 */
//enum
//{
//	CLUBEVENT_LEADER_TRANSFER = 0
//	, CLUBEVENT_NEWMEMBER
//	, CLUBEVENT_KICK
//	, CLUBEVENT_LEAVE
//	, CLUBEVENT_MODIFY_POST
//	, CLUBEVENT_PLACE_CHANGE
//	, CLUBEVENT_CREATE
//	, CLUBEVENT_CLUB_UPGRADE
//	, CLUBEVENT_CLUB_STOP
//	, CLUBEVENT_CLUB_BACKOUT
//	, CLUBEVENT_BUILD_UPGRADE
//	, CLUBEVENT_BUILD_STOP
//	, CLUBEVENT_BUILD_BACKOUT
//	, CLUBEVENT_SKILL_UPGRADE
//	, CLUBEVENT_BUY_HERB
//	, CLUBEVENT_MAINTE
//	, CLUBEVNT_MAX
//};
//
///* 家族建筑 */
//typedef enum
//{
//	ClubBuildNormal = 0,
//	Club_burse,	//账房
//	Club_drug,	//药房
//	Club_room,	//住房
//	ClubBuildMax,
//}ClubBuild;
//
///* 帮派商店 */
//typedef struct _club_Shop
//{
//	short itemkind;
//	int price;
//	int shoplevel;
//	int value;
//}ClubShop;
//
///* 帮派商店随机的道具 */
//typedef struct _club_Randitem
//{
//	short itemkind;
//	char count; // 剩余数量
//	int price;  // 价钱
//}ClubRandItem;
//
///* 家族信息 */
//typedef struct _club
//{
//	short	m_clubid;							// 家族的唯一ID
//	int		m_createtime;						// 创建时间
//	char	m_clubname[NAME_SIZE];				// 家族名字
//	char	m_creatername[NAME_SIZE];			// 创建者
//	char	m_leadername[NAME_SIZE];			// 族长名字
//	char	m_level;							// 家族等级
//	int		m_money;							// 资金
//	int		m_boom;								// 繁荣度
//	int		m_repute;							// 声望
//	char	m_intro[CLUB_NOTIFY_SIZE];			// 家族章程
//	short	m_membercount;						// 成员个数
//	int		m_asker_count;						// 申请人的数量
//	int		m_online_count;						// 在线人数
//	int		m_online_member[MAX_MEMBER_COUNT];	// 在线的成员actor_index，速查使用
//	int		m_online_actorid[MAX_MEMBER_COUNT]; // 在线成员actorid
//	int		m_fdate;							// 用于每日检查的日期
//	int		m_current_week;						// 当前周数，用来统计玩家周贡献度
//	short	m_week_online[7];					// 一周内每天在线人数
//	char	m_isbroken;							// 家族是否处于溃散状态
//	char	m_broken_days;						// 溃散天数
//	int		m_mapid;							// 家族私有地图编号
//
//	// 建设研发部分需要的数据
//	char	m_isclub_upgrade;					// 家族是否升级中
//	int		m_club_upgrade_exp;					// 家族升级当前的建设度
//	int		m_res_point;						// 资材 
//	short	m_build_level[ClubBuildMax];		// 建筑等级
//	int		m_curbuild_develop;					// 当前发展的建筑
//	int		m_curbuild_exp;						// 当前发展建筑的建设度
//
//	ClubRandItem	m_clubranditem[CLUB_SHOP_ITEM_COUNT]; // 帮派每小时刷新的商品
//	int		m_pay_scale;						// 工资比例
//}Club;
//
///* 成员信息 */
//typedef struct _clubmember
//{
//	int		m_actorid;				// 成员id
//	short	m_clubid;				// 家族的唯一ID
//	char	m_actorname[NAME_SIZE];	// 名字
//	short	m_level;				// 等级
//	int		m_aclass;				// 职业
//	char	m_place;				// 职务 0-表示申请中 1-表示学徒
//	char	m_online;				// 是否在线
//	int		m_entertime;			// 入帮时间
//	int		m_offlinetime;			// 离线时间
//	int		m_paymoney;				// 积攒工资分红
//}ClubMember;
//
///* 家族建筑升级信息 */
//typedef struct _club_upgrade
//{
//	int need_money;
//	int need_boom;
//	int need_repute;
//	int progress;
//	int other;
//}ClubUpgrade;
//
//typedef enum
//{
//	CLUB_MAX_LEVEL = 1
//	, CLUB_MONEY
//	, CLUB_BOOM
//	, CLUB_REPUTE
//	, CLUB_NOTLEADER
//	, CLUB_LEVEL
//	, CLUB_NOCLUB
//	, CLUB_NOTOWNER
//	, CLUB_CARLOCK
//	, CLUB_UPGRADE_ING	// 正在建设
//	, CLUB_UPGRADE_BUILD_LIMIT // 建筑等级不满足
//	, CLUB_STOP			// 没钱 停止了
//	, CLUB_POWER_NONE	// 权限不足
//}CLUB_UPGRADE_ERRCODE;
//
///* 帮派初始化，服务器启动时候调用 */
//int club_init();
//
///* 保存所有家族信息 */
//void club_save();
//
///* 帮派更新 */
//int club_update( short club_index );
//
///* 创建帮派 */
//int club_create( int actor_index, char *clubname, char *intro );
//
///* 删除帮派 */
//int club_delete( short club_index );
//
///* 玩家帮派数据读取 */
//int club_actor_load( int actor_index );
//
///* 玩家帮派数据保存 */
//int club_actor_save( int actor_index );
//
///* 帮派索引 */
//short club_getindex( short clubid );
//
///* 帮派ID */
//short club_getid( short club_index );
//
///* 帮派名称 */
//char *club_getname( short club_index );
//
///* 帮派职位名称 */
//char *club_getplacename( char place );
//
///* 帮派建筑名称 */
//char *club_getbuildname( char buildid );
//
///* 帮派地图 */
//int club_getmapid( int actor_index );
//
///* 帮派信息 */
//int club_baseinfo( int actor_index );
//
///* 帮派在线成员列表 */
//int club_onlinememberlist( int actor_index );
//
///* 帮派列表 */
//int club_list( int actor_index, short page );
//
///* 帮派成员列表 */
//int club_memberlist( int actor_index, short page );
//
///* 帮派申请列表 */
//int club_askerlist( int actor_index, short clubid );
//
///* 清空帮派申请列表 */
//int club_clear_asklist( int actor_index );
//
///* 帮派事件列表 */
//int club_eventlist( int actor_index, short pageno );
//
///* 帮派添加事件 */
//int club_add_event( short clubid, int actorid, char *name, int otherid, char *othername, char event );
//
///* 帮派发送消息 */
//int club_sendmsg( int club_index, int datasize, char *databuf );
//
///* 帮派上下线情况 */
//int club_online( int actor_index, char online );
//
///* 一键申请 */
//int club_allask( int actor_index );
//
///* 申请加入 */
//int club_ask( int actor_index, short clubid, short clubindex );
//
///* 允许 */
//int club_allow( int actor_index, int targetid );
//
///* 拒绝 */
//int club_reject( int actor_index, int askerid );
//
///* 家族申请列表检查，每小时检查一次，删除过期申请信息 */
//int club_asklist_logic();
//
///* 进帮 */
//int club_in( int actor_index, short clubid );
//
///* 退帮 */
//int club_out( int actorid, int club_index );
//
///* 玩家主动离开帮会 */
//int club_leave( int actor_index );
//
///* 踢人 */
//int club_kick( int actor_index, int targetid );
//
///* 计算今天在线人数和在线角色id */
//int club_today_loginactor( int club_index, int actor_index );
//
///* 转换索引 */
//void club_online_change( int old_index, int new_index );
//
///* 转换职位 */
//int club_change_place( short club_index, int memberid, char place );
//
///* 给家族发邮件 */
//int club_mail_send( int actor_index, int clubid, char *msg, char type, int value1, int value2, int value3, int value4, int overdue );
//
///* 家族逻辑 1小时1次 */
//int club_logic_hourly();
//
///* 修改章程 */
//int club_change_intro( int actor_index, char *intro );
//
///* 帮贡 */
//int club_change_member_contribute( int actor_index, int contribute );
//int club_update_membercontribute( int actor_index );
//int club_get_member_contribute( int actor_index );
//
///* 初始化帮派商店 */
//int club_shop_init();
//
///* 帮派商店刷新 */
//int club_shop_refurbish( int club_index );
//
///* 帮派商店信息发送 */
//int club_shop_listinfo( int actor_index );
//
///* 帮派商店购买 */
//int club_shop_buy( int actor_index, int itemindex );
//
///* 帮派交易商店 */
//int club_tradeshop_listinfo( int actor_index );
//
///* 帮派贸易市场购买 */
//int club_tradeshop_buy( int actor_index, int id );
//
///* 帮派贸易市场购买请求 */
//int club_tradeshop_sell_ask( int actor_index, int itemindex );
//
///* 帮派贸易市场购买 */
//int club_tradeshop_sell( int actor_index, int itemindex, int payment, int price );
//
//int club_shop_update( short club_index );
//int club_place_right( short club_index, char place, int right );
//int club_invitememeber( int actor_index, int actorID );
//
//int club_proc_invite( int actor_index, int targetid, short clubid, char type );
//int club_proc_openinvitedlg( int actor_index, int actorid );
//
//int club_memberproc( int actor_index, int actorid, char clubclass );
//
//int club_checkplace( int actor_index );
//
//void club_logic(); // 家族逻辑 1分钟1次
//
//int club_open_storage( int actor_index, char type );
//int club_open_bank( int actor_index, char type );
//int club_storagein( int actor_index, short clubid, short item_sel, short item_num );
//int club_storageout( int actor_index, short clubid, short item_sel, short item_num );
//int club_newstorage_db( short club_index, int item_offset, char fNewid );
//int club_deletestorage_db( short club_index, int item_offset );
//int club_storage_load( short club_index );
//int club_bankin( int actor_index, short clubid, int money );
//int club_bankout( int actor_index, short clubid, int money );
//int club_upgrade( int actor_index );
//
//int club_getquestdonetimes( short club_index, short questtype_offset );
//int club_setquestdonetimes( short club_index, short questtype_offset, int times );
//
//int club_add_boom_repute( int club_index, int boom, int repute );
//int club_add_money( int club_index, int money );
//
//int club_gm( int actor_index, int type, int value, int clubid );
//
//int club_getallclubnum(); // 得到所有俱乐部数量
//int club_getallclubnum_without_isbroken(); // 得到所有俱乐部数量，不包括溃散状态的
//int club_getonlinemembernum( short club_index ); // 得到这个家族在线玩家的数量
//int club_member_inmap( short club_index, int mapid ); // 得到这个家族在某张地图的玩家的数量
//
//int club_reset_member_wcontribute( int club_index );
//int club_check_current_week( int club_index );
//
//int club_broken_logic();
//int club_check_leader_online( int club_index );
//
//int club_reset_broken();
//int club_allowenter_clubmap( int actor_index );
//
//// 建设
//int club_set_money( int club_index, int value );
//int club_get_money( int club_index );
//int club_set_boom( int club_index, int value );
//int club_get_boom( int club_index );
//int club_set_repute( int club_index, int value );
//int club_set_res( int club_index, int value );
//int club_is_stop( int club_index );
//int club_level_fall( int club_index );
//
//int club_get_hourly_mainte_money( int club_index ); // 获取每小时维护费用
//int club_get_money_least( int club_index );			// 获取资金底线
//int club_get_money_most( int club_index );			// 获取资金上限
//int club_get_member_max( int club_index );			// 获取最大成员
//int club_get_hourly_makeres( int club_index );		// 获取每小时产生的资源数
//int club_get_maxres( int club_index );				// 获取资源上限
//int club_get_business_award_least( int club_index );// 获取跑商奖励下限
//int club_get_studypower( int club_index );			// 获取研究力
//int club_get_brush_drugcount( int club_index );		// 获取药房刷药的数量
//int club_get_brush_drugcount_show( int club_index );// 获取药房刷药的数量-用于显示
//int club_get_repute_value( int club_index );		// 获取声望增减量
//
//int club_get_build_upgradeexp( int club_index, int build );	// 获取建筑升级所需建设度
//int club_get_build_upgrademoney( int club_index, int build );	// 获取建筑升级所需资金
//int club_get_build_upgraderepute( int club_index, int build );	// 获取建筑升级所需声望
//int club_get_build_upgradeboom( int club_index, int build );	// 获取建筑升级所需繁荣
//
//int club_add_clubupgrade_exp( int club_index, int value );	// 添加当前升级家族的建设度
//int club_add_curbuild_exp( int club_index, int value );		// 添加当前升级建筑的建设度
//int club_add_curskill_exp( int club_index, int value );		// 添加当前升级技能的经验
//
//int club_canupgrade_withbuild( int club_index );  // 家族建筑能否满足升级
//int club_upgrade_issuccess( int club_index );		// 家族升级是否成功
//
//int club_build_upgrade( int actor_index, int build ); // 家族建筑升级
//int club_build_upgrade_issuccess( int club_index );  // 家族建筑升级是否成功
//
//int club_upgrade( int actor_index ); // 帮派升级
//int club_upgrade_info( int actor_index, short buildid );
//
//int club_degrade( int actor_index );
//int club_build_degrade( int actor_index, int build );
//
//int club_upgrade_stop( int actor_index );
//int club_build_upgrade_stop( int actor_index, int build );
//int club_send_buildinfo( int actor_index );
//
//
//// 跑商
//#define CLUB_BUSINESS_ITEM_COUNT 20
//#define CLUB_BUSINESS_SHOP_COUNT 5
//typedef struct _club_business_sellprice
//{
//	int shopid;
//	int itemkind[CLUB_BUSINESS_ITEM_COUNT];
//	int sellprice[CLUB_BUSINESS_ITEM_COUNT];
//	int price[CLUB_BUSINESS_ITEM_COUNT];
//	int value[CLUB_BUSINESS_ITEM_COUNT];
//}ClubBusinessSellPrice;
//// 跑商专用的几个函数
//// 是否是商人
//int club_actor_isbusinessman( int actor_index );
//// 获取银票的值
//int club_actorget_bill_value( int actor_index, int *value, int *max_value );
//// 设置银票的值
//int club_actorset_bill_value( int actor_index, int value, int max_value );
//// 是否是跑商商店
//int club_issellshop( int shopid );
//// 获取这个商店，这个商品的价格
//int club_getsellprice( int shopid, int itemkind );
//// 价格商店的id读取
//int club_randomsellshop_init();
//int club_shopsellprice_init( int shopid );
//// 随机价格
//int club_randomsellprice( int shopid );
//
//
//// 药店专用
//// 家族药店用随机药品并分配个数
//int club_random_drugcount( int club_index, int totalcount );
//
//// 计算家族药店物品总类的个数
//int club_drugcount_total( int club_index );
//
//// 计算家族药店物品每个kind的个数
//int club_drugcount_withkind( int club_index, int kind );
//
//// 计算家族药店设置物品个数
//int club_set_drugcount_withkind( int club_index, int kind, int value );
//
//
//#endif 
