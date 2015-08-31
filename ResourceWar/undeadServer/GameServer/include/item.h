#ifndef _ITEM_H_
#define _ITEM_H_
#include "auto_actor_item.h"

#define ITEM_ABILITY_NUM	2	// 玩家道具动态属性个数

/* 物品颜色 */
#define ITEM_COLOR_LEVEL_WHITE	0 // 白色
#define ITEM_COLOR_LEVEL_GREEN	1 // 绿色
#define ITEM_COLOR_LEVEL_BLUE	2 // 蓝色
#define ITEM_COLOR_LEVEL_PURPLE	3 // 紫色
#define ITEM_COLOR_LEVEL_GOLD	4 // 金色
#define ITEM_COLOR_LEVEL_RED	5 // 红色
#define ITEM_COLOR_LEVEL_ORANGE 6 // 橙色
#define ITEM_COLOR_LEVEL_MAX	7 // 最大颜色数量

// 背包装备的位置起始
#define ITEM_OFFSETBASE			0
#define EQUIP_OFFSETBASE		1000
#define HEROEQUIP_OFFSETBASE	2000

/* 背包装备栏最大数量*/
#define MAX_DEFAULT_ITEMNUM	80
#define MAX_ACTOR_ITEMNUM	100
#define MAX_ACTOR_EQUIPNUM	10

/* 物品类型 */
#define ITEM_TYPE_NORMAL					0	// 普通使用类
#define ITEM_TYPE_EQUIP1					1	// 
#define ITEM_TYPE_EQUIP2					2	// 
#define ITEM_TYPE_EQUIP3					3	// 
#define ITEM_TYPE_EQUIP4					4	// 
#define ITEM_TYPE_EQUIP5					5	// 
#define ITEM_TYPE_EQUIP6					6	// 
#define ITEM_TYPE_EQUIP7					7	// 
#define ITEM_TYPE_EQUIP8					8   // 
#define ITEM_TYPE_EQUIP9					9   //
#define ITEM_TYPE_EQUIP10					10  //

/* 动态属性 */
#define ITEM_ABILITY_NONE					0

/* 道具操作 */ 
#define ITEM_PROCESS_INFO		1 // 获取道具信息
#define ITEM_PROCESS_USE		2 // 使用道具
#define ITEM_PROCESS_EQUIP		3 // 装备穿上
#define ITEM_PROCESS_UNEQUIP	4 // 装备卸下
#define ITEM_PROCESS_PUT		5 // 交换道具格子
#define ITEM_PROCESS_DROP		6 // 丢弃道具
#define ITEM_PROCESS_SELL		7 // 出售道具
#define ITEM_PROCESS_SETTLE		8 // 背包整理

/* 物品信息结构 */
typedef struct _itemkind
{
	int m_kind;							// 物品种类 - 决定了物品的名字-itemkey
	char m_name[NAME_SIZE];				// 物品名
	short m_level;						// 决定装备需要的等级
	int m_price;						// 售价
	short m_type;						// 物品类别
	short m_overlap;					// 堆叠数量
	unsigned char m_situation;			// 使用场合
	char m_color_level;					// 颜色级别

	short m_base_ability[2];			// 基础固定属性
	int m_base_value[2];				// 基础固定属性值
	short m_ability[ITEM_ABILITY_NUM];	// 动态属性
	int m_value_min[ITEM_ABILITY_NUM];	// 动态属性值
	int m_value_max[ITEM_ABILITY_NUM];	// 动态属性值

}ItemKind;

/* 创建物品成功后返回的结构 */
typedef struct _itemout
{
	int m_itemkind;			// 物品种类
	short m_item_offset;	// 物品栏的位置
	short m_count;			// 增加的个数
}ItemOut;

/* 物品信息初始化 */
int itemkind_init();
int item_maxid_init();

/* 获取物品全局配置信息 */
char *item_getname( int itemkind );
short item_getlevel( int itemkind );
int item_getprice( int itemkind );
short item_gettype( int itemkind );
unsigned short item_getsituation( int itemkind );
short item_get_overlap( int itemkind );
char item_getcolorlevel( int itemkind );

/* 物品动态属性 */
short item_get_base_ability( int itemkind, char pos );
int item_get_base_value( int itemkind, char pos );
int item_get_ability_value_withpos( int itemkind, char pos, short *ability, int *value );
char item_get_ability_value( int itemkind, short ability, int *value );
int _item_get_ability_value( Item *pItem, short ability );
int _item_set_ability_value( Item *pItem, short ability, int value );

/* 将offset转换成对应的属性结构 */
Item *item_getptr( int actor_index, int offset );

/* 服务器道具操作部分 */
int item_load( int actor_index );	// 玩家读取道具
int item_save( int actor_index, FILE *fp ); // 玩家保存所有道具
int item_insert( int actor_index, short offset ); // 创建装备的时候调用，保存刚创建的道具
int item_getitem( int actor_index, int itemkind, int itemnum, char path ); // 玩家获得道具
int item_create( int actor_index, int itemkind, int itemnum, ItemOut *pOut ); // 玩家创建道具
int item_lost( int actor_index, int itemkind, int num, char path ); // 服务器丢掉道具，根据kind
int item_lostitem( int actor_index, int itemoffset, int num, char path ); // 服务器丢掉道具，根据offset
int item_deletebox( int actor_index, int item_offset, char type ); // 删除一个格子的道具
int item_hasitem( int actor_index, int itemkind, int num ); // 是否有这个数量的物品
int item_getitemnum( int actor_index, int itemkind );// 获取这个道具的数量 


// ---------------------------------- 服务器响应道具操作 item_proc.cpp -----------------------------------------------------
int item_use( int actor_index, short itemindex, short itemnum, int hero_index, int target_index ); // 服务器响应道具操作-使用
int item_equipup( int actor_index, short hero_index, short itemindex ); // 服务器响应道具操作-装备上
int item_put( int actor_index, short item_resource, short item_target ); // 服务器响应道具操作-交换位置或装备卸下
int item_drop( int actor_index, short itemindex, short dorpcount ); // 服务器响应道具操作-丢弃
int item_settle( int actor_index );   // 服务器响应道具操作-整理


// ---------------------------------- 服务器响应道具操作 item_equip.cpp -----------------------------------------------------
int equip_new( Item *pitem, int itemkind );
int equip_create( int actor_index, int itemkind, int item_offset );

// ---------------------------------- 服务器发送道具协议 item_send.cpp -----------------------------------------------------
int item_sendnotuse( int actor_index, short itemindex, int err );
void item_sendlost( int actor_index, int target_index, short item_index, short item_num, char path );
void item_sendget( int actor_index, int target_index, short item_index, short item_num, char path );
int item_list( int actor_index );	// 发送道具列表
int item_equip_list( int actor_index, int hero_index ); // 发送装备列表
int item_info( int actor_index, short itemindex ); // 发送道具详细信息
int item_info_withitem( Item *pItem, int actor_index, short itemindex );// 真正的发送道具信息函数

#endif
