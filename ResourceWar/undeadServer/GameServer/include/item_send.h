#ifndef _ITEM_SEND_H_
#define _ITEM_SEND_H_


// 发送使用失败信息
int item_sendnotuse( int actor_index, short itemindex, int err );

// 发送丢失物品信息
void item_sendlost( int actor_index, int target_index, short item_index, short item_num, char path );

// 发送获取物品信息
void item_sendget( int actor_index, int target_index, short item_index, short item_num, char path );

// 发送背包道具列表
int item_list( int actor_index );

// 发送装备列表
int item_equip_list( int actor_index, int hero_index );

// 发送道具详细信息
int item_info( int actor_index, short itemindex );
// 真正的发送道具信息函数
int item_info_withitem( Item *pItem, int actor_index, short itemindex );

#endif