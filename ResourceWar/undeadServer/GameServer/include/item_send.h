#ifndef _ITEM_SEND_H_
#define _ITEM_SEND_H_


// ����ʹ��ʧ����Ϣ
int item_sendnotuse( int actor_index, short itemindex, int err );

// ���Ͷ�ʧ��Ʒ��Ϣ
void item_sendlost( int actor_index, int target_index, short item_index, short item_num, char path );

// ���ͻ�ȡ��Ʒ��Ϣ
void item_sendget( int actor_index, int target_index, short item_index, short item_num, char path );

// ���ͱ��������б�
int item_list( int actor_index );

// ����װ���б�
int item_equip_list( int actor_index, int hero_index );

// ���͵�����ϸ��Ϣ
int item_info( int actor_index, short itemindex );
// �����ķ��͵�����Ϣ����
int item_info_withitem( Item *pItem, int actor_index, short itemindex );

#endif