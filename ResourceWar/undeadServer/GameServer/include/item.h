#ifndef _ITEM_H_
#define _ITEM_H_
#include "auto_actor_item.h"

#define ITEM_ABILITY_NUM	2	// ��ҵ��߶�̬���Ը���

/* ��Ʒ��ɫ */
#define ITEM_COLOR_LEVEL_WHITE	0 // ��ɫ
#define ITEM_COLOR_LEVEL_GREEN	1 // ��ɫ
#define ITEM_COLOR_LEVEL_BLUE	2 // ��ɫ
#define ITEM_COLOR_LEVEL_PURPLE	3 // ��ɫ
#define ITEM_COLOR_LEVEL_GOLD	4 // ��ɫ
#define ITEM_COLOR_LEVEL_RED	5 // ��ɫ
#define ITEM_COLOR_LEVEL_ORANGE 6 // ��ɫ
#define ITEM_COLOR_LEVEL_MAX	7 // �����ɫ����

// ����װ����λ����ʼ
#define ITEM_OFFSETBASE			0
#define EQUIP_OFFSETBASE		1000
#define HEROEQUIP_OFFSETBASE	2000

/* ����װ�����������*/
#define MAX_DEFAULT_ITEMNUM	80
#define MAX_ACTOR_ITEMNUM	100
#define MAX_ACTOR_EQUIPNUM	10

/* ��Ʒ���� */
#define ITEM_TYPE_NORMAL					0	// ��ͨʹ����
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

/* ��̬���� */
#define ITEM_ABILITY_NONE					0

/* ���߲��� */ 
#define ITEM_PROCESS_INFO		1 // ��ȡ������Ϣ
#define ITEM_PROCESS_USE		2 // ʹ�õ���
#define ITEM_PROCESS_EQUIP		3 // װ������
#define ITEM_PROCESS_UNEQUIP	4 // װ��ж��
#define ITEM_PROCESS_PUT		5 // �������߸���
#define ITEM_PROCESS_DROP		6 // ��������
#define ITEM_PROCESS_SELL		7 // ���۵���
#define ITEM_PROCESS_SETTLE		8 // ��������

/* ��Ʒ��Ϣ�ṹ */
typedef struct _itemkind
{
	int m_kind;							// ��Ʒ���� - ��������Ʒ������-itemkey
	char m_name[NAME_SIZE];				// ��Ʒ��
	short m_level;						// ����װ����Ҫ�ĵȼ�
	int m_price;						// �ۼ�
	short m_type;						// ��Ʒ���
	short m_overlap;					// �ѵ�����
	unsigned char m_situation;			// ʹ�ó���
	char m_color_level;					// ��ɫ����

	short m_base_ability[2];			// �����̶�����
	int m_base_value[2];				// �����̶�����ֵ
	short m_ability[ITEM_ABILITY_NUM];	// ��̬����
	int m_value_min[ITEM_ABILITY_NUM];	// ��̬����ֵ
	int m_value_max[ITEM_ABILITY_NUM];	// ��̬����ֵ

}ItemKind;

/* ������Ʒ�ɹ��󷵻صĽṹ */
typedef struct _itemout
{
	int m_itemkind;			// ��Ʒ����
	short m_item_offset;	// ��Ʒ����λ��
	short m_count;			// ���ӵĸ���
}ItemOut;

/* ��Ʒ��Ϣ��ʼ�� */
int itemkind_init();
int item_maxid_init();

/* ��ȡ��Ʒȫ��������Ϣ */
char *item_getname( int itemkind );
short item_getlevel( int itemkind );
int item_getprice( int itemkind );
short item_gettype( int itemkind );
unsigned short item_getsituation( int itemkind );
short item_get_overlap( int itemkind );
char item_getcolorlevel( int itemkind );

/* ��Ʒ��̬���� */
short item_get_base_ability( int itemkind, char pos );
int item_get_base_value( int itemkind, char pos );
int item_get_ability_value_withpos( int itemkind, char pos, short *ability, int *value );
char item_get_ability_value( int itemkind, short ability, int *value );
int _item_get_ability_value( Item *pItem, short ability );
int _item_set_ability_value( Item *pItem, short ability, int value );

/* ��offsetת���ɶ�Ӧ�����Խṹ */
Item *item_getptr( int actor_index, int offset );

/* ���������߲������� */
int item_load( int actor_index );	// ��Ҷ�ȡ����
int item_save( int actor_index, FILE *fp ); // ��ұ������е���
int item_insert( int actor_index, short offset ); // ����װ����ʱ����ã�����մ����ĵ���
int item_getitem( int actor_index, int itemkind, int itemnum, char path ); // ��һ�õ���
int item_create( int actor_index, int itemkind, int itemnum, ItemOut *pOut ); // ��Ҵ�������
int item_lost( int actor_index, int itemkind, int num, char path ); // �������������ߣ�����kind
int item_lostitem( int actor_index, int itemoffset, int num, char path ); // �������������ߣ�����offset
int item_deletebox( int actor_index, int item_offset, char type ); // ɾ��һ�����ӵĵ���
int item_hasitem( int actor_index, int itemkind, int num ); // �Ƿ��������������Ʒ
int item_getitemnum( int actor_index, int itemkind );// ��ȡ������ߵ����� 


// ---------------------------------- ��������Ӧ���߲��� item_proc.cpp -----------------------------------------------------
int item_use( int actor_index, short itemindex, short itemnum, int hero_index, int target_index ); // ��������Ӧ���߲���-ʹ��
int item_equipup( int actor_index, short hero_index, short itemindex ); // ��������Ӧ���߲���-װ����
int item_put( int actor_index, short item_resource, short item_target ); // ��������Ӧ���߲���-����λ�û�װ��ж��
int item_drop( int actor_index, short itemindex, short dorpcount ); // ��������Ӧ���߲���-����
int item_settle( int actor_index );   // ��������Ӧ���߲���-����


// ---------------------------------- ��������Ӧ���߲��� item_equip.cpp -----------------------------------------------------
int equip_new( Item *pitem, int itemkind );
int equip_create( int actor_index, int itemkind, int item_offset );

// ---------------------------------- ���������͵���Э�� item_send.cpp -----------------------------------------------------
int item_sendnotuse( int actor_index, short itemindex, int err );
void item_sendlost( int actor_index, int target_index, short item_index, short item_num, char path );
void item_sendget( int actor_index, int target_index, short item_index, short item_num, char path );
int item_list( int actor_index );	// ���͵����б�
int item_equip_list( int actor_index, int hero_index ); // ����װ���б�
int item_info( int actor_index, short itemindex ); // ���͵�����ϸ��Ϣ
int item_info_withitem( Item *pItem, int actor_index, short itemindex );// �����ķ��͵�����Ϣ����

#endif