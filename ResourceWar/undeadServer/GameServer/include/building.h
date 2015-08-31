#ifndef _BUILDING_H_
#define _BUILDING_H_
// 唯一建筑
#define	BUILDING_TownCenter		1	//	王城/圣殿
#define	BUILDING_Wall			2	//	城墙/城墙
#define	BUILDING_Drunkery		3	//	酒馆/灵堂
#define	BUILDING_Market			4	//  市场/市场
#define	BUILDING_GamblingHouse	5	//  赌场/赌场
#define	BUILDING_Library		6	//	图书馆/神圣之地
#define	BUILDING_WishingPool	7	//	许愿池/祭坛
#define	BUILDING_Embassy		8	//	大使馆/幽堂
#define	BUILDING_Smithy			9	//	铁匠铺/铁匠铺
#define	BUILDING_DrillGround	10	//	校场/检阅场	
#define	BUILDING_StoreHouse		11	//	仓库/地窖
// 资源
#define	BUILDING_WoodFactory	12	//	木材厂/聚灵器
#define	BUILDING_Farm			13	//	农田/魂器
#define	BUILDING_IronFactory	14	//	铁矿厂/炼化室
#define	BUILDING_Mithril		15	//	秘银穴/深坑
// 部队
#define	BUILDING_House			16	//	民居/地穴
#define	BUILDING_Barracks		17	//	兵营
#define	BUILDING_Stable			18	//	马厩
#define	BUILDING_ShootingRange	19	//	靶场
#define	BUILDING_Tower			20	//	塔楼
#define	BUILDING_Magician		21	//	法师营地
#define	BUILDING_Craftsman		22	//	工匠坊

#define BUILDING_MAXLEVEL			60	// 建筑最大等级
#define	BUILDING_INFO_LIMIT_MAX		2	// 升级或建造限制条件数量
#define	BUILDING_INFO_RES_MAX		4	// 升级或建造限所需资源种类
#define	BUILDING_INFO_VALUE_MAX		4	// 建筑信息附加属性个数

// 建筑当前状态
#define BUILDING_STATE_NORMAL			0 // 缺省状态
#define BUILDING_STATE_CREATE_ING		1 // 建造中
#define BUILDING_STATE_CREATE_ED		2 // 已经建造完成，特殊使用，需点击方可真正的建造完毕
#define BUILDING_STATE_UPGRADE_ING		3 // 升级中
#define BUILDING_STATE_UPGRADE_ED		4 // 升级完成，特殊使用，需点击方可真正的升级完毕
#define BUILDING_STATE_DELETE_ING		5 // 拆除中
#define BUILDING_STATE_DELETE_ED		6 // 拆除完成，特殊使用，需点击方可真正的拆除成功
#define BUILDING_STATE_TROOPTRAIN_ING	7 // 训练中
#define BUILDING_STATE_TROOPTRAIN_ED	8 // 训练完成
#define BUILDING_STATE_TROOPUP_ING		9 // 升级兵种中
#define BUILDING_STATE_TROOPUP_ED		10 // 升级兵种完成
#define BUILDING_STATE_TROOPHEAL_ING	11 // 治疗中
#define BUILDING_STATE_TROOPHEAL_ED		12 // 治疗完成

// 建筑建造升级详细信息
typedef struct _buildingconfig  
{
	char limit_kind[BUILDING_INFO_LIMIT_MAX];		// 需其它建筑达到多少级
	char limit_level[BUILDING_INFO_LIMIT_MAX];		// 需其它建筑达到多少级
	char restype[BUILDING_INFO_RES_MAX];			// 需要资源
	int resvalue[BUILDING_INFO_RES_MAX];			// 需要资源数量
	char worker;									// 需要农民数量
	int sec;										// 升级或建造所需时间
	int value[BUILDING_INFO_VALUE_MAX];				// 额外附加数据
}BuildingUpgradeConfig;

// 建筑
typedef struct _buildingupgrade
{
	BuildingUpgradeConfig *info;
	char maxlevel;
}BuildingUpgrade;

// 建筑信息
typedef struct _buildinglimit
{
	short maxnum[1];	// 可建造数量
	short maxlevel[1];	// 可升级最大等级
}BuildingLimit;

// 建筑信息
typedef struct _buildingnew
{
	unsigned char buildingindex;
	unsigned char buildingkind;
	char level;	
}BuildingNew;

// 城池的每个建筑信息
typedef struct _building
{
	unsigned char buildingkind;			// 建筑种类
	char level;							// 建筑等级
	char state;							// 当前状态
	int begintime;						// 开始时间
	int value[BUILDING_INFO_VALUE_MAX]; // 附加属性
}Building;

// 建筑信息初始化
int building_upgrade_init();

// 建筑限制信息初始化
int building_limit_init();

// 建筑初始创建信息初始化
int building_new_init();

// 城池建筑信息读取
int building_load( struct _city *pCity );

// 所有城池建筑信息保存
int building_save( struct _city *pCity, FILE *fp );

// 一个城池建筑信息保存
int building_single_save( int cityid, short offset, Building *pBuilding, FILE *fp );

// 建造一个建筑
int building_create( struct _city *pCity, short buildingindex, short buildingkind );

// 升级一个建筑
int building_upgrade( struct _city *pCity, short buildingindex );

// 立即完成
int building_instantly_finish( struct _city *pCity, short buildingindex );

// 删除一个建筑
int building_delete( struct _city *pCity, short buildingindex );

// 移动一个建筑
int building_move( struct _city *pCity, short buildingindex, unsigned char posx, unsigned char posy );

// 采集
int building_gather( struct _city *pCity, short buildingindex );

// 造兵时间检查
int building_troop_checktime( struct _city *pCity, short buildingindex );

// 建筑建造或升级检查时间
int building_checktime( struct _city *pCity, short buildingindex );

// 建筑建造或升级操作完成
int building_finish( struct _city *pCity, short buildingindex );

// 这个建筑数量
int building_getnum( struct _city *pCity, short buildingkind );

// 获取一个空闲索引
int building_getemptyindex( struct _city *pCity );

// 获取处理所需时间
int building_getneedtime( Building *pBuilding );

// 兵种
// 兵种训练, op : 1-训练，2-升级
int building_troop_train( int actor_index, int buildingindex, char op, short corps, char level, int num );
// 受伤
int building_troop_wound( int actor_index, short corps, char level, int num );
// 死亡，解雇
int building_troop_abandon( int actor_index, short corps, char level, int num );
// 取消训练
int building_troop_traincancel( int actor_index, int buildingindex );
// 立即完成训练
int building_troop_trainfinish( int actor_index, int buildingindex );
// 完成操作
int building_troop_ontrainfinish( int actor_index, int buildingindex );
// 完成操作
int building_troop_ontrainget( int actor_index, int buildingindex );

#endif

