#ifdef WIN32
#include <winsock2.h>
#include <windows.h>
#endif
#include <mysql.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include "db.h"
#include "actor.h"
#include "gameproc.h"
#include "item.h"
#include "hero.h"

extern MYSQL *myData;
extern MYSQL *myGame;

extern Actor *g_actors;
extern int g_maxactornum;

ItemKind *g_itemkind = NULL;
int g_itemkindnum;
i64 g_maxitemid;

int item_maxid_init()
{
	MYSQL_RES		*res;
	MYSQL_ROW		row;
	char	szSQL[1024];

	sprintf( szSQL, "select max(itemid) from actor_item" );
	if ( mysql_query( myGame, szSQL ) )
	{
		printf( "Query failed (%s) [%s](%d)\n", mysql_error( myGame ), __FUNCTION__, __LINE__ );
		write_gamelog( "%s", szSQL );
		if ( mysql_ping( myGame ) != 0 )
			db_reconnect_game();
		return -1;
	}
	res = mysql_store_result( myGame );

	if ( !((row = mysql_fetch_row( res ))) )
	{
		mysql_free_result( res );
		return -1;
	}
	if ( row[0] )
		g_maxitemid = atoll( row[0] );
	else
		g_maxitemid = 1000000; // 100��Ϊ�����Ҫ��������Ԥ��
	g_maxitemid++;
	mysql_free_result( res );
	return 0;
}

int itemkind_init()
{
	MYSQL_RES		*res;
	MYSQL_ROW		row;
	char	szSQL[2048];
	int itemkind;

	// ��ñ���������ItemKind
	sprintf( szSQL, "select max(itemkind) from item" );
	if ( mysql_query( myData, szSQL ) )
	{
		printf( "Query failed (%s) [%s](%d)\n", mysql_error( myData ), __FUNCTION__, __LINE__ );
		write_gamelog( "%s", szSQL );
		return -1;
	}
	res = mysql_store_result( myData );
	if ( (row = mysql_fetch_row( res )) )
	{
		if ( row[0] )
			g_itemkindnum = atoi( row[0] ) + 1;
		else
			g_itemkindnum = 0;
	}
	else
	{
		mysql_free_result( res );
		return -1;
	}
	mysql_free_result( res );

	// ����ռ�
	g_itemkind = (ItemKind *)malloc( sizeof(ItemKind)*g_itemkindnum );
	memset( g_itemkind, 0, sizeof(ItemKind)*g_itemkindnum );

	sprintf( szSQL, "select itemkind,name,level,price,type,situation,overlap,base_ability1,base_value1,base_ability2,base_value2,"
		"ability1,value_min1,value_max1,ability2,value_min2,value_max2,"
		"color_level "
		" from item" );
	if ( mysql_query( myData, szSQL ) )
	{
		printf( "Query failed (%s) [%s](%d)\n", mysql_error( myData ), __FUNCTION__, __LINE__ );
		write_gamelog( "%s", szSQL );
		return -1;
	}
	res = mysql_store_result( myData );

	while ( (row = mysql_fetch_row( res )) )
	{
		itemkind = atoi( row[0] );
		if ( itemkind <= 0 )
		{
			mysql_free_result( res );
			return -1;
		}
		else if ( itemkind >= g_itemkindnum )
			continue;

		g_itemkind[itemkind].m_kind = itemkind;
		int index = 1;
		strcpy( g_itemkind[itemkind].m_name, row[index++] );
		g_itemkind[itemkind].m_level = atoi( row[index++] );
		g_itemkind[itemkind].m_price = atoi( row[index++] );
		g_itemkind[itemkind].m_type = atoi( row[index++] );
		g_itemkind[itemkind].m_situation = atoi( row[index++] );
		g_itemkind[itemkind].m_overlap = atoi( row[index++] );
		if ( g_itemkind[itemkind].m_overlap < 1 )
			g_itemkind[itemkind].m_overlap = 1;
		g_itemkind[itemkind].m_base_ability[0] = atoi( row[index++] );
		g_itemkind[itemkind].m_base_value[0] = atoi( row[index++] );
		g_itemkind[itemkind].m_base_ability[1] = atoi( row[index++] );
		g_itemkind[itemkind].m_base_value[1] = atoi( row[index++] );

		g_itemkind[itemkind].m_ability[0] = atoi( row[index++] );
		g_itemkind[itemkind].m_value_min[0] = atoi( row[index++] );
		g_itemkind[itemkind].m_value_max[0] = atoi( row[index++] );
		g_itemkind[itemkind].m_ability[1] = atoi( row[index++] );
		g_itemkind[itemkind].m_value_min[1] = atoi( row[index++] );
		g_itemkind[itemkind].m_value_max[1] = atoi( row[index++] );
		
		g_itemkind[itemkind].m_color_level = atoi( row[index++] );

	}
	mysql_free_result( res );

	if ( item_maxid_init() < 0 )
		return -1;

	return 0;
}

char *item_getname( int itemkind )
{
	if ( itemkind <= 0 || itemkind >= g_itemkindnum )
		return NULL;
	return g_itemkind[itemkind].m_name;
}

short item_getlevel( int itemkind )
{
	if ( itemkind <= 0 || itemkind >= g_itemkindnum )
		return 0;
	return g_itemkind[itemkind].m_level;
}

int item_getprice( int itemkind )
{
	if ( itemkind <= 0 || itemkind >= g_itemkindnum )
		return 0;
	return g_itemkind[itemkind].m_price;
}

short item_gettype( int itemkind )
{
	if ( itemkind <= 0 || itemkind >= g_itemkindnum )
		return 0;
	return g_itemkind[itemkind].m_type;
}

unsigned short item_getsituation( int itemkind )
{
	if ( itemkind <= 0 || itemkind >= g_itemkindnum )
		return 0;
	return g_itemkind[itemkind].m_situation;
}

short item_get_overlap( int itemkind )
{
	if ( itemkind <= 0 || itemkind >= g_itemkindnum )
		return 0;
	return g_itemkind[itemkind].m_overlap;
}

char item_getcolorlevel( int itemkind )
{
	if ( itemkind <= 0 || itemkind >= g_itemkindnum )
		return 0;
	return g_itemkind[itemkind].m_color_level;
}

short item_get_base_ability( int itemkind, char pos )
{
	int pos_sub = pos;
	if ( pos_sub < 0 || pos_sub > 1 )
		return 0;
	if ( itemkind <= 0 || itemkind >= g_itemkindnum )
		return 0;
	return g_itemkind[itemkind].m_base_ability[pos_sub];
}

int item_get_base_value( int itemkind, char pos )
{
	int pos_sub = pos;
	if ( pos_sub < 0 || pos_sub > 1 )
		return 0;
	if ( itemkind <= 0 || itemkind >= g_itemkindnum )
		return 0;
	return g_itemkind[itemkind].m_base_value[pos_sub];
}

int item_get_ability_value_withpos( int itemkind, char pos, short *ability, int *value )
{
	if ( itemkind <= 0 || itemkind >= g_itemkindnum )
		return 0;
	if ( pos < 0 || pos >= ITEM_ABILITY_NUM )
		return 0;
	*ability = g_itemkind[itemkind].m_ability[pos];
	*value = g_itemkind[itemkind].m_value_min[pos];
	return 0;
}

char item_get_ability_value( int itemkind, short ability, int *value )
{
	int i;
	if ( itemkind <= 0 || itemkind >= g_itemkindnum )
		return 0;
	for ( i = 0; i < 2; i++ )
	{
		if ( g_itemkind[itemkind].m_base_ability[i] == ability )
		{
			*value = g_itemkind[itemkind].m_base_value[i];
			return 1;
		}
	}
	for ( i = 0; i < ITEM_ABILITY_NUM; i++ )
	{
		if ( g_itemkind[itemkind].m_ability[i] == ability )
		{
			*value = g_itemkind[itemkind].m_value_min[i];
			return 1;
		}
	}
	return 0;
}

int _item_get_ability_value( Item *pItem, short objability )
{
	int tmpi;
	if ( !pItem )
		return 0;
	if ( pItem->m_kind <= 0 || pItem->m_num <= 0 )
		return 0;

	for ( tmpi = 0; tmpi < 2; tmpi++ )
	{
		if ( item_get_base_ability( pItem->m_kind, tmpi ) == objability )
			return item_get_base_value( pItem->m_kind, tmpi );
	}
	for ( tmpi = 0; tmpi < ITEM_ABILITY_NUM; tmpi++ )
	{
		if ( pItem->m_ability[tmpi] == objability )
			return pItem->m_value[tmpi];
	}
	return 0;
}

int _item_set_ability_value( Item *pItem, short objability, int value )
{
	int tmpi;
	if ( !pItem )
		return 0;
	if ( pItem->m_kind <= 0 || pItem->m_num <= 0 )
		return 0;
	for ( tmpi = 0; tmpi < ITEM_ABILITY_NUM; tmpi++ )
	{
		if ( pItem->m_ability[tmpi] == objability )
			pItem->m_value[tmpi] = value;
	}
	return 0;
}

// ��offsetת���ɶ�Ӧ�����Խṹ
Item *item_getptr( int actor_index, int offset )
{
	int tmpi = 0;
	if ( offset >= ITEM_OFFSETBASE && offset < ITEM_OFFSETBASE + MAX_ACTOR_ITEMNUM )
	{ // ����
		tmpi = offset - ITEM_OFFSETBASE;
		return &g_actors[actor_index].item[tmpi];
	}
	else if ( offset >= EQUIP_OFFSETBASE && offset < EQUIP_OFFSETBASE + MAX_ACTOR_EQUIPNUM )
	{ // ����װ����
		tmpi = offset - EQUIP_OFFSETBASE;
		return &g_actors[actor_index].equip[tmpi];
	}
	else if ( offset >= HEROEQUIP_OFFSETBASE && offset < HEROEQUIP_OFFSETBASE + MAX_ACTOR_EQUIPNUM*HERO_MAXCOUNT )
	{ // Ӣ��װ����
		tmpi = offset - HEROEQUIP_OFFSETBASE;
		return &g_actors[actor_index].hero_equip[tmpi];
	}
	return NULL;
}

// ���߶�ȡ
int item_load( int actor_index )
{
	int count = 0;
	if ( actor_index < 0 || actor_index >= g_maxactornum )
		return -1;
	actor_item_load_auto( actor_index, item_getptr, g_actors[actor_index].actorid, "actor_item" );

	// ��װ��
	//zitem_reset_equip( actor_index, member );
	return count;
}

// ���߱���
int item_save( int actor_index, FILE *fp )
{
	if ( actor_index < 0 || actor_index >= g_maxactornum )
		return -1;

	int max_itemnum = MAX_DEFAULT_ITEMNUM + g_actors[actor_index].itemext;
	if ( max_itemnum > MAX_ACTOR_ITEMNUM )
		max_itemnum = MAX_ACTOR_ITEMNUM;

	for ( int tmpi = 0; tmpi < max_itemnum; tmpi++ )
	{ // ����
		if ( g_actors[actor_index].item[tmpi].m_kind <= 0 )
			continue;
		g_actors[actor_index].item[tmpi].actorid = g_actors[actor_index].actorid;
		g_actors[actor_index].item[tmpi].offset = tmpi;
		actor_item_save_auto( &g_actors[actor_index].item[tmpi], "actor_item", fp );
	}
	for ( int tmpi = 0; tmpi < MAX_ACTOR_EQUIPNUM; tmpi++ )
	{ // ����װ����
		if ( g_actors[actor_index].equip[tmpi].m_kind <= 0 )
			continue;
		g_actors[actor_index].equip[tmpi].actorid = g_actors[actor_index].actorid;
		g_actors[actor_index].equip[tmpi].offset = tmpi + EQUIP_OFFSETBASE;
		actor_item_save_auto( &g_actors[actor_index].equip[tmpi], "actor_item", fp );
	}
	for ( int tmpi = 0; tmpi < MAX_ACTOR_EQUIPNUM*HERO_MAXCOUNT; tmpi++ )
	{ // Ӣ��װ����
		if ( g_actors[actor_index].hero_equip[tmpi].m_kind <= 0 )
			continue;
		g_actors[actor_index].hero_equip[tmpi].actorid = g_actors[actor_index].actorid;
		g_actors[actor_index].hero_equip[tmpi].offset = tmpi + HEROEQUIP_OFFSETBASE;
		actor_item_save_auto( &g_actors[actor_index].hero_equip[tmpi], "actor_item", fp );
	}
	return 0;
}

// ����װ����ʱ����ã�����մ����ĵ���
int item_insert( int actor_index, short offset )
{
	int tmpi = 0;
	if ( actor_index < 0 || actor_index >= g_maxactornum )
		return -1;
	if ( offset >= ITEM_OFFSETBASE && offset < ITEM_OFFSETBASE + MAX_ACTOR_ITEMNUM )
	{ // ����
		tmpi = offset;
		if ( g_actors[actor_index].item[tmpi].m_kind <= 0 )
			return -1;
		g_actors[actor_index].item[tmpi].m_itemid = g_maxitemid;
		g_maxitemid++;
		g_actors[actor_index].item[tmpi].actorid = g_actors[actor_index].actorid;
		g_actors[actor_index].item[tmpi].offset = tmpi;
		actor_item_save_auto( &g_actors[actor_index].item[tmpi], "actor_item", NULL );
	}
	else if ( offset >= EQUIP_OFFSETBASE && offset < EQUIP_OFFSETBASE + MAX_ACTOR_EQUIPNUM )
	{ // ����װ����
		tmpi = offset - EQUIP_OFFSETBASE;
		if ( g_actors[actor_index].equip[tmpi].m_kind <= 0 )
			return -1;
		g_actors[actor_index].equip[tmpi].m_itemid = g_maxitemid;
		g_maxitemid++;
		g_actors[actor_index].equip[tmpi].actorid = g_actors[actor_index].actorid;
		g_actors[actor_index].equip[tmpi].offset = tmpi + EQUIP_OFFSETBASE;
		actor_item_save_auto( &g_actors[actor_index].equip[tmpi], "actor_item", NULL );
	}
	else if ( offset >= HEROEQUIP_OFFSETBASE && offset < HEROEQUIP_OFFSETBASE + MAX_ACTOR_EQUIPNUM*HERO_MAXCOUNT )
	{ // Ӣ��װ����
		tmpi = offset - HEROEQUIP_OFFSETBASE;
		if ( g_actors[actor_index].hero_equip[tmpi].m_kind <= 0 )
			return -1;
		g_actors[actor_index].hero_equip[tmpi].m_itemid = g_maxitemid;
		g_maxitemid++;
		g_actors[actor_index].hero_equip[tmpi].actorid = g_actors[actor_index].actorid;
		g_actors[actor_index].hero_equip[tmpi].offset = tmpi + HEROEQUIP_OFFSETBASE;
		actor_item_save_auto( &g_actors[actor_index].hero_equip[tmpi], "actor_item", NULL );
	}
	else
		return -1;
	return 0;
}

// ��õ���
int item_getitem( int actor_index, int itemkind, int itemnum, char path )
{
	if ( actor_index < 0 || actor_index >= g_maxactornum )
		return -1;
	if ( itemkind <= 0 || itemkind >= g_itemkindnum )
		return -1;
	if ( itemnum <= 0 )
		return -1;

	Item * pitem = NULL;
	int item_offset;
	char type;
	short itemtype;
	ItemOut ItemOut[MAX_ACTOR_ITEMNUM] = { 0 };
	int item_count = 0;

	// ��������
	item_count = item_create( actor_index, itemkind, itemnum, ItemOut );
	if ( item_count <= 0 )
		return -1;

	itemtype = item_gettype( itemkind );
	item_offset = ItemOut[0].m_item_offset;

	type = 1;
	for ( int tmpi = 0; tmpi < item_count; tmpi++ )
	{
		item_offset = ItemOut[tmpi].m_item_offset;
		if ( item_offset >= 0 && item_offset < MAX_ACTOR_ITEMNUM )
			pitem = &g_actors[actor_index].item[item_offset];
		else
			continue;

		item_sendget( actor_index, -1, item_offset, ItemOut[tmpi].m_count, path );
		// ��¼log
		if ( tmpi == item_count - 1 )
			type = 0;
		wlog( type, LOGOP_ITEMGET, path, itemkind, ItemOut[tmpi].m_count, pitem->m_itemid, g_actors[actor_index].actorid, 0 );
	}
	return 0;
}

// ��ָ���ĸ��ӿ�ʼ��ѯ�����Ʒ�����еĵ�һ���������
static int _item_find_empty( Actor *pActor, int itemkind, int *pOffset )
{
	if ( pActor == NULL )
		return -1;
	if ( itemkind <= 0 || itemkind >= g_itemkindnum )
		return -1;

	Item * pitem = NULL;
	int item_offset = -1;
	int first_empty = -1;
	int remain_num = 0;
	int max_itemnum = MAX_DEFAULT_ITEMNUM + pActor->itemext;
	if ( max_itemnum > MAX_ACTOR_ITEMNUM )
		max_itemnum = MAX_ACTOR_ITEMNUM;

	// ��ѯ�����ĸ���
	if ( *pOffset < max_itemnum )
	{
		for ( int tmpi = *pOffset; tmpi < max_itemnum; tmpi++ )
		{
			if ( tmpi >= 0 && tmpi < max_itemnum )
				pitem = &pActor->item[tmpi];
			else
				continue;
			if ( pitem->m_kind == g_itemkind[itemkind].m_kind )
			{
				remain_num = g_itemkind[itemkind].m_overlap - pitem->m_num;
				if ( remain_num > 0 )
				{
					*pOffset = tmpi;
					return remain_num;
				}
			}
		}
		*pOffset = max_itemnum;
	}

	// ��ѯȫ�յĸ���
	remain_num = g_itemkind[itemkind].m_overlap;
	item_offset = *pOffset - MAX_ACTOR_ITEMNUM;

	for ( int tmpi = item_offset; tmpi < max_itemnum; tmpi++ )
	{
		if ( tmpi >= 0 && tmpi < max_itemnum )
			pitem = &pActor->item[tmpi];
		else
			continue;
		if ( pitem->m_kind <= 0 )
		{
			*pOffset = tmpi + MAX_ACTOR_ITEMNUM;
			return remain_num;
		}
	}
	return 0;
}

// ��������
int item_create( int actor_index, int itemkind, int itemnum, ItemOut *pOut )
{
	if ( actor_index < 0 || actor_index >= g_maxactornum )
		return -1;
	if ( itemkind <= 0 || itemkind >= g_itemkindnum )
		return -1;
	if ( itemnum <= 0 )
		return -1;
	Item * pitem = NULL;
	int find_offset = 0;
	int remain_item = 0;
	int remain_count = 0;
	int totle_remain = 0;

	int item_offset;
	int tmpi, tmpj;
	int tmpmin, tmpmax;
	short itemtype;

	Actor *pActor = &g_actors[actor_index];

	while ( 1 )
	{
		// ��һ�����Դ��itemkind�Ŀ�λ��
		remain_item = _item_find_empty( pActor, itemkind, &find_offset );
		if ( remain_item <= 0 )
			return -1;
		if ( totle_remain + remain_item > itemnum )
			remain_item = itemnum - totle_remain;
		pOut[remain_count].m_item_offset = (find_offset < MAX_ACTOR_ITEMNUM) ? find_offset : find_offset - MAX_ACTOR_ITEMNUM;
		pOut[remain_count].m_count = remain_item;
		pOut[remain_count].m_itemkind = itemkind;
		remain_count++;
		find_offset++;
		totle_remain += remain_item;
		if ( totle_remain >= itemnum )
			break;
	}

	for ( tmpi = 0; tmpi < remain_count; tmpi++ )
	{
		item_offset = pOut[tmpi].m_item_offset;
		remain_item = pOut[tmpi].m_count;
		if ( item_offset >= 0 && item_offset < MAX_ACTOR_ITEMNUM )
			pitem = &pActor->item[item_offset];
		else
			continue;

		if ( pitem->m_num > 0 )
			pitem->m_num += remain_item;
		else
		{
			memset( pitem, 0, sizeof(Item) );
			pitem->m_kind = g_itemkind[itemkind].m_kind;
			pitem->m_num = remain_item;
			itemtype = item_gettype( pitem->m_kind );
			if ( itemtype >= ITEM_TYPE_EQUIP1 && itemtype <= ITEM_TYPE_EQUIP10 )
			{ // �����װ��
				equip_create( actor_index, itemkind, item_offset );
			}
			else
			{ // ��������
				for ( tmpj = 0; tmpj < ITEM_ABILITY_NUM; tmpj++ )
				{
					pitem->m_ability[tmpj] = g_itemkind[itemkind].m_ability[tmpj];
					tmpmin = g_itemkind[itemkind].m_value_min[tmpj];
					tmpmax = g_itemkind[itemkind].m_value_max[tmpj];
					if ( tmpmin == tmpmax )
						pitem->m_value[tmpj] = tmpmin;
					else if ( tmpmax - tmpmin < 3 )
						pitem->m_value[tmpj] = tmpmin + rand() % (tmpmax - tmpmin + 1);
					else
						pitem->m_value[tmpj] = tmpmin + rand() % (tmpmax - tmpmin + 1);
				}

			}

			// ��ɫ �����ɫ��Ϣ�����壬�Ե�ǰΪ��������Ϊ0
			if ( g_itemkind[itemkind].m_color_level >= 0 && pitem->m_color_level <= 0 )
				pitem->m_color_level = g_itemkind[itemkind].m_color_level;

			// ����������ݵ����ݿ�
			item_insert( actor_index, item_offset );
		}
	}
	return remain_count;
}

// ������Ʒ��kind����ʧ������Ʒ
int item_lost( int actor_index, int itemkind, int num, char path )
{
	int tmpi;
	short itemindex[MAX_ACTOR_ITEMNUM];
	short itemnum[MAX_ACTOR_ITEMNUM];
	int lostnum;
	short itemtype;

	if ( actor_index < 0 || actor_index >= g_maxactornum )
		return -1;
	int max_itemnum = MAX_DEFAULT_ITEMNUM + g_actors[actor_index].itemext;

	if ( max_itemnum > MAX_ACTOR_ITEMNUM )
		max_itemnum = MAX_ACTOR_ITEMNUM;

	itemtype = item_gettype( itemkind );
	lostnum = 0;
	for ( tmpi = 0; tmpi < max_itemnum; tmpi++ )
	{
		if ( g_actors[actor_index].item[tmpi].m_kind == itemkind &&
			g_actors[actor_index].item[tmpi].m_num > 0 )
		{
			itemindex[lostnum] = tmpi;
			if ( g_actors[actor_index].item[tmpi].m_num >= num )
			{
				itemnum[lostnum] = num;
				num = 0;
				lostnum++;
				break;
			}
			else
			{
				itemnum[lostnum] = g_actors[actor_index].item[tmpi].m_num;
				num -= g_actors[actor_index].item[tmpi].m_num;
				lostnum++;
				if ( num <= 0 )
					break;
			}
		}
	}
	if ( num > 0 )
	{
		return -1;
	}
	for ( tmpi = 0; tmpi < lostnum; tmpi++ )
	{
		if ( item_lostitem( actor_index, itemindex[tmpi], itemnum[tmpi], path ) < 0 )
			return -1;
	}

	return 0;
}

// ϵͳ������Ʒ, �������Ҳ����
int item_lostitem( int actor_index, int itemoffset, int num, char path )
{
	Item *pitem = NULL;
	int itemkind;
	if ( actor_index < 0 || actor_index >= g_maxactornum )
		return -1;
	if ( num <= 0 )
		return -1;
	if ( itemoffset >= 0 && itemoffset < MAX_ACTOR_ITEMNUM )
		pitem = &g_actors[actor_index].item[itemoffset];
	else
		return -1;

	if ( pitem->m_kind <= 0 )
		return -1;
	if ( pitem->m_num < num )
		return -1;

	wlog( 0, LOGOP_ITEMLOST, path, pitem->m_kind, num, pitem->m_itemid, g_actors[actor_index].actorid, 0 );

	itemkind = pitem->m_kind;
	pitem->m_num -= num;
	if ( pitem->m_num <= 0 )
		item_deletebox( actor_index, itemoffset, 0 );

	// ���ͽ�ɫʧȥ��Ʒ
	item_sendlost( actor_index, -1, itemoffset, num, path );
	return itemkind;
}

// ɾ��һ�����ӵĵ���
int item_deletebox( int actor_index, int item_offset, char type )
{
	char	szSQL[1024];
	char bigint[21];
	Item *pItem = NULL;
	if ( actor_index < 0 || actor_index >= g_maxactornum )
		return -1;
	if ( type == 0 )
	{
		if ( item_offset >= 0 && item_offset < MAX_ACTOR_ITEMNUM )
			pItem = &g_actors[actor_index].item[item_offset];
		else
			return -1;
	}
	else if ( type == 2 )
		pItem = &g_actors[actor_index].equip[item_offset - EQUIP_OFFSETBASE];

	// ���ݿ�ɾ��
	lltoa( pItem->m_itemid, bigint, 10 );
	sprintf( szSQL, "delete from actor_item where itemid='%s'", bigint );
	if ( mysql_query( myGame, szSQL ) )
	{
		printf( "Query failed (%s)\n", mysql_error( myGame ) );
		write_gamelog( "%s", szSQL );
		if ( mysql_ping( myGame ) != 0 )
			db_reconnect_game();
		return -1;
	}
	// �ڴ�ɾ��
	memset( pItem, 0, sizeof(Item) );
	return 0;
}
