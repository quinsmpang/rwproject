#include <memory.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include "worldmap.h"
#include "city.h"
#include "actor.h"
#include "fight.h"
#include "utils.h"
#include "system.h"
#include "actor_send.h"
#include "mapunit.h"

extern Map g_map;
extern Actor *g_actors;
extern int g_maxactornum;

extern City *g_city;
extern int g_city_maxcount;
extern int g_city_maxindex;

extern MapUnit *g_mapunit;
extern int g_mapunit_maxcount;

int area_getoffset( int area_index, int *px, int *py )
{
	if ( area_index <= 0 )
	{
		*px = 0;
		*py = 0;
		return 0;
	}
	//int round = (int)((sqrt((float)area_index) + 1) / 2);	//4
	//int begin_index = ((round - 1) * 2 + 1) ^ 2;			//50
	//int local_index = area_index - begin_index;				//28
	//int local_offset = local_index / (round * 2);			//3
	//int local_value = (local_index % (round * 2)) - round + 1;

	//int R = (1 - (local_offset % 2))*(1 - (local_offset / 2) * 2);
	//int S = (local_offset % 2)*((local_offset / 2) * 2 - 1);

	//*px = round*R + local_value*S;
	//*py = round*S + local_value*R;

	*px = area_index % (g_map.m_nAreaXNum);
	*py = area_index / (g_map.m_nAreaXNum);
	return 0;
}

int area_getindex_fromgrid(int areax, int areay)
{
	//if (areax == 0 && areay == 0)
	//	return 0;
	//int round = max(abs(areax), abs(areay));

	//int local_offset;
	//int local_value;

	//int begin_index = ((round - 1) * 2 + 1) ^ 2;			//50

	//if (areax == round && areay >= -round && areay < round)
	//{
	//	local_offset = 0;
	//	local_value = (round - 1) - areay;
	//}
	//else if (areay == -round && areax >= -round && areax < round)
	//{
	//	local_offset = 1;
	//	local_value = (round - 1) - areax;
	//}
	//else if (areax == -round && areay > -round && areay <= round)
	//{
	//	local_offset = 2;
	//	local_value = areay - (1 - round);
	//}
	//else if (areay == round && areax > -round && areax <= round)
	//{
	//	local_offset = 3;
	//	local_value = areax - (1 - round);
	//}
	//return begin_index + local_offset*round * 2 + local_value;
	return areay*(g_map.m_nAreaXNum) + areax;
}

int area_getindex( int posx, int posy )
{
	int areax, areay;
	if( posx >= g_map.m_nMaxWidth )
		posx = g_map.m_nMaxWidth - 1;
	if( posy >= g_map.m_nMaxHeight )
		posy = g_map.m_nMaxHeight - 1;
	areax = (posx+1)/AREA_WIDTH;
	areay = (posy+1)/AREA_HEIGHT;
	//return area_getindex_fromgrid(areax, areay);
	return areay*(g_map.m_nAreaXNum) + areax;
}

// ���������Ƴ������ʾ��Ԫ
void area_remove_mapunit( int unit_index )
{
	int tmpi;
	int area_index_cur;
	Area *pAreaLast;
	Area *pAreaCur = NULL;
	if ( g_mapunit[unit_index].lastadd_areaindex < 0 )
		return;

	// ֮ǰ��������
	pAreaLast = &g_map.m_aArea[g_mapunit[unit_index].lastadd_areaindex];
	if ( pAreaLast == NULL )
		return;

	// ��ȡ��ǰ���ڵ�����
	SLK_NetS_AddMapUnit info = { 0 };
	mapunit_getattr( unit_index, &info );
	area_index_cur = area_getindex( info.m_posx, info.m_posy );
	if ( area_index_cur >= 0 )
		pAreaCur = &g_map.m_aArea[area_index_cur];

	// ����ǰ��ĵ�Ԫ��������ĵ�Ԫ����
	if ( g_mapunit[unit_index].pre_index >= 0 )
	{
		tmpi = g_mapunit[unit_index].pre_index;
		g_mapunit[tmpi].next_index = g_mapunit[unit_index].next_index;
		if ( g_mapunit[tmpi].next_index == tmpi )
		{
			write_gamelog( "ActorListError:%d-%d", unit_index, tmpi );
			g_mapunit[tmpi].next_index = -1;
		}
	}
	if ( g_mapunit[unit_index].next_index >= 0 )
	{
		tmpi = g_mapunit[unit_index].next_index;
		g_mapunit[tmpi].pre_index = g_mapunit[unit_index].pre_index;
	}

	// ����������������ͷ���ͽ�����ͷָ��������һ��λ��
	if ( pAreaLast->unit_head == unit_index )
		pAreaLast->unit_head = g_mapunit[unit_index].next_index;
	if ( pAreaCur && pAreaCur->unit_head == unit_index )
	{
		pAreaCur->unit_head = g_mapunit[unit_index].next_index;
		write_gamelog( "WARNNING: Area1" );
	}

	// ����������������β���ͽ�����βָ������ǰһ��λ��
	if ( pAreaLast->unit_tail == unit_index )
		pAreaLast->unit_tail = g_mapunit[unit_index].pre_index;
	if ( pAreaCur && pAreaCur->unit_tail == unit_index )
	{
		pAreaCur->unit_tail = g_mapunit[unit_index].pre_index;
		write_gamelog( "WARNNING: Area2" );
	}

	// ���Լ���������Ϣ���
	g_mapunit[unit_index].pre_index = -1;
	g_mapunit[unit_index].next_index = -1;
	g_mapunit[unit_index].lastadd_areaindex = -1;
}

// ������������������ĵ�Ԫ����λ���Ƿ����������������п����������仯��
int area_checklist( int areaidx )
{
	int area_index;
	int head, tail;
	Area *pArea;
	SLK_NetS_AddMapUnit info = { 0 };
	if ( areaidx < 0 )
		return 0;
	pArea = &g_map.m_aArea[areaidx];
	if ( pArea->unit_head < 0 && pArea->unit_tail < 0 )
		return 0;

	head = pArea->unit_head;
	tail = pArea->unit_tail;

	if ( head >= 0 )
	{
		mapunit_getattr( head, &info );
		area_index = area_getindex( info.m_posx, info.m_posy );
		if ( area_index != areaidx )
		{
			pArea->unit_head = -1;
			pArea->unit_tail = -1;
			write_gamelog( "AreaHeadError: Area[%d] Head[%d(%d-%d)]", areaidx, area_index, tail, info.m_type );
			return -1;
		}
	}
	if ( tail >= 0 )
	{
		mapunit_getattr( tail, &info );
		area_index = area_getindex( info.m_posx, info.m_posy );
		if ( area_index != areaidx )
		{
			pArea->unit_head = -1;
			pArea->unit_tail = -1;
			write_gamelog( "AreaTailError: Area[%d] Tail[%d(%d-%d)]", areaidx, area_index, tail, info.m_type );
			return -1;
		}
	}
	return 0;
}

// ����ʾ��Ԫ��ӵ�����
int area_addmapunit( int unit_index, int area_index )
{
	Area *pArea = &g_map.m_aArea[area_index];
	if ( pArea == NULL || pArea->unit_tail == unit_index )
		return 0;
	// ���֮ǰ��������Ϣ���ȴ�֮ǰ�������Ƴ�
	area_remove_mapunit( unit_index );
	// ������������������
	area_checklist( area_index );

	if ( pArea->unit_head < 0 || pArea->unit_tail < 0 )
	{
		g_mapunit[unit_index].pre_index = -1;
		g_mapunit[unit_index].next_index = -1;
		pArea->unit_head = pArea->unit_tail = unit_index;
		g_mapunit[unit_index].lastadd_areaindex = area_index;
		return 0;
	}

	g_mapunit[unit_index].pre_index = pArea->unit_tail;
	g_mapunit[pArea->unit_tail].next_index = unit_index;
	if ( g_mapunit[pArea->unit_tail].next_index == pArea->unit_tail )
	{
		write_gamelog( "ActorListError:[%d-%d]", unit_index, pArea->unit_tail );
		g_mapunit[pArea->unit_tail].next_index = -1;
	}
	pArea->unit_tail = unit_index;
	g_mapunit[unit_index].lastadd_areaindex = area_index;
	return 0;
}

// ����ʾ��Ԫ�Ƴ�������
int area_delmapunit( int unit_index, int area_index )
{
	area_remove_mapunit( unit_index );
	return 0;
}

// һ������ƶ���Ļ���뵽������Ҫ��������е�����
int area_enter( int actor_index, int area_index )
{
	int cur_index;
	int next_index;

	char msg[2048] = {0};
	int size = 0;

	// �������еĵ�Ԫ��Ͻ������Ϣ���͸��Լ�
	cur_index = g_map.m_aArea[area_index].unit_head;
	while( cur_index >= 0 )
	{
		next_index = g_mapunit[cur_index].next_index;
		if( cur_index < 0 )
		{
			cur_index = next_index;
			continue;
		}
		if ( g_mapunit[cur_index].type == 0 )
		{
			cur_index = next_index;
			continue;
		}
		// ��֯���ݰ�
		if ( mapunit_enterinfo( cur_index, msg + sizeof(short), &size ) < 0 )
		{
			*(unsigned short *)msg = size;
			// ���͸��Լ�
			sendtoclient( actor_index, msg, size + sizeof(short) );
			memset( msg, 0, 2048 );
			size = 0;
			mapunit_enterinfo( cur_index, msg + sizeof(short), &size );
		}
		cur_index = next_index;
	}

	if( size > 0 )
	{
		*(unsigned short *)msg = size;
		// ���͸��Լ�
		sendtoclient( actor_index, msg, size + sizeof(short) );
	}
	return 0;
}

// һ������ƶ���Ļ�뿪������Ҫ��������뿪������
int area_leave( int actor_index, int area_index )
{
	int cur_index;
	int next_index;

	char msg[2048] = {0};
	int size = 0;
	if ( area_index < 0 )
		return -1;
	// �������еĵ�Ԫ����뿪����Ϣ���͸��Լ�
	cur_index = g_map.m_aArea[area_index].unit_head;
	while( cur_index >= 0 )
	{
		next_index = g_mapunit[cur_index].next_index;
		if( cur_index < 0 )
		{
			cur_index = next_index;
			continue;
		}
		if ( g_mapunit[cur_index].type == 0 )
		{
			cur_index = next_index;
			continue;
		}
		// ��֯���ݰ�
		if ( mapunit_leaveinfo( cur_index, msg + sizeof(short), &size ) < 0 )
		{
			*(unsigned short *)msg = size;
			// ���͸��Լ�
			sendtoclient( actor_index, msg, size + sizeof(short) );
			memset( msg, 0, 2048 );
			size = 0;
			mapunit_leaveinfo( cur_index, msg + sizeof(short), &size );
		}
		cur_index = next_index;
	}

	if( size > 0 )
	{
		*(unsigned short *)msg = size;
		// ���͸��Լ�
		sendtoclient( actor_index, msg, size + sizeof(short) );
	}
	return 0;
}

// ��ʾ��Ԫλ�ñ仯��Ҫ���µ�����
int mapunit_area_change( int unit_index, int new_posx, int new_posy, char flag )
{
	int old_area_index,new_area_index;
	int tmpi, tmpj, tmpx, tmpy;
	int areax, areay;
	int leave_area[9];
	int leave_num;
	int enter_area[9];
	int enter_num;
	int reset_area[9];
	int reset_num;
	if ( unit_index < 0 || unit_index >= g_mapunit_maxcount )
		return -1;
	// �ų����ܵ��ٽ����
	if (new_posx >= g_map.m_nMaxWidth)
		new_posx = g_map.m_nMaxWidth - 1;
	if (new_posy >= g_map.m_nMaxHeight)
		new_posy = g_map.m_nMaxHeight - 1;

	old_area_index = g_mapunit[unit_index].lastadd_areaindex;
	new_area_index = area_getindex( new_posx, new_posy );

	// û�з��������л�
	if ( old_area_index == new_area_index )
	{
		if ( flag )
		{
			reset_num = 0;
			area_getoffset( old_area_index, &areax, &areay );
			for ( tmpx = areax - 1; tmpx <= areax + 1; tmpx++ )
			{
				if ( tmpx < 0 || tmpx >= g_map.m_nAreaXNum )
					continue;
				for ( tmpy = areay - 1; tmpy <= areay + 1; tmpy++ )
				{
					if ( tmpy < 0 || tmpy >= g_map.m_nAreaYNum )
						continue;
					reset_area[reset_num++] = area_getindex_fromgrid( tmpx, tmpy );
				}
			}
			for ( tmpi = 0; tmpi < reset_num; tmpi++ )
			{ // ��Ҫ���귢���ı�
				mapunit_resetarea( unit_index, reset_area[tmpi] );
			}
		}
		return 0;
	}

	leave_num = 0;
	enter_num = 0;
	reset_num = 0;

	area_getoffset(old_area_index, &areax, &areay);

	leave_num = 0;
	for( tmpx = areax-1; tmpx <= areax+1; tmpx++ )
	{
		if( tmpx < 0 || tmpx >=g_map.m_nAreaXNum )
			continue;
		for( tmpy = areay-1; tmpy <= areay+1; tmpy++ )
		{
			if( tmpy < 0 || tmpy >=g_map.m_nAreaYNum )
				continue;
			leave_area[leave_num++] = area_getindex_fromgrid(tmpx, tmpy);
		}
	}

	area_getoffset(new_area_index, &areax, &areay);

	enter_num = 0;
	for( tmpx = areax-1; tmpx <= areax+1; tmpx++ )
	{
		if( tmpx < 0 || tmpx >=g_map.m_nAreaXNum )
			continue;
		for( tmpy = areay-1; tmpy <= areay+1; tmpy++ )
		{
			if( tmpy < 0 || tmpy >=g_map.m_nAreaYNum )
				continue;
			enter_area[enter_num++] = area_getindex_fromgrid(tmpx, tmpy);
		}
	}

	for( tmpi = 0; tmpi < leave_num; tmpi++ )
	{
		for( tmpj = 0; tmpj < enter_num; tmpj++ )
		{
			if( leave_area[tmpi] == enter_area[tmpj] )
			{
				reset_area[reset_num++] = leave_area[tmpi];
				leave_area[tmpi] = -1;
				enter_area[tmpj] = -1;
			}
		}
	}

	area_delmapunit( unit_index, old_area_index );
	area_addmapunit( unit_index, new_area_index );

	for( tmpi = 0; tmpi < leave_num; tmpi++ )
	{
		if( leave_area[tmpi] != -1 )
		{
			// ��������������˷����뿪
			mapunit_leavearea( unit_index, leave_area[tmpi] );
		}
	}

	for( tmpi = 0; tmpi < enter_num; tmpi++ )
	{
		if( enter_area[tmpi] != -1 )
		{
			// ��������������˷��ͽ���
			mapunit_enterarea( unit_index, enter_area[tmpi] );
		}
	}

	for( tmpi = 0; tmpi < reset_num; tmpi++ )
	{
		// ��Ҫ���귢���ı�
		mapunit_resetarea( unit_index, reset_area[tmpi] );
	}
	// �Խ�ɫ�Լ�����,Ӧ����һЩ����ɾ����һЩ����Ľ���
	//��Ҫ����

	return 0;
}

// �ӿڱ仯��������Ϣ���µ��ӿ�
int view_area_change( int actor_index, int new_area_index )
{
	int tmpi, tmpj, tmpx, tmpy;
	int areax, areay;
	int leave_area[9];
	int leave_num = 0;
	int enter_area[9];
	int enter_num = 0;
	int reset_area[9];
	int reset_num = 0;
	int old_area_index = g_actors[actor_index].view_areaindex;
	if ( old_area_index == new_area_index )
		return -1;
	if ( new_area_index < 0 )
		return -1;
	area_getoffset( old_area_index, &areax, &areay );
	leave_num = 0;
	for ( tmpx = areax - 1; tmpx <= areax + 1; tmpx++ )
	{
		if ( tmpx < 0 || tmpx >= g_map.m_nAreaXNum )
			continue;
		for ( tmpy = areay - 1; tmpy <= areay + 1; tmpy++ )
		{
			if ( tmpy < 0 || tmpy >= g_map.m_nAreaYNum )
				continue;
			leave_area[leave_num++] = area_getindex_fromgrid( tmpx, tmpy );
		}
	}

	area_getoffset( new_area_index, &areax, &areay );
	enter_num = 0;
	for ( tmpx = areax - 1; tmpx <= areax + 1; tmpx++ )
	{
		if ( tmpx < 0 || tmpx >= g_map.m_nAreaXNum )
			continue;
		for ( tmpy = areay - 1; tmpy <= areay + 1; tmpy++ )
		{
			if ( tmpy < 0 || tmpy >= g_map.m_nAreaYNum )
				continue;
			enter_area[enter_num++] = area_getindex_fromgrid( tmpx, tmpy );
		}
	}

	for ( tmpi = 0; tmpi < leave_num; tmpi++ )
	{
		for ( tmpj = 0; tmpj < enter_num; tmpj++ )
		{
			if ( leave_area[tmpi] == enter_area[tmpj] )
			{
				reset_area[reset_num++] = leave_area[tmpi];
				leave_area[tmpi] = -1;
				enter_area[tmpj] = -1;
			}
		}
	}

	for ( tmpi = 0; tmpi < leave_num; tmpi++ )
	{
		if ( leave_area[tmpi] != -1 )
		{
			// ����Ҫ�뿪��������Ϣ���͸��Լ�
			area_leave( actor_index, leave_area[tmpi] );
		}
	}

	for ( tmpi = 0; tmpi < enter_num; tmpi++ )
	{
		if ( enter_area[tmpi] != -1 )
		{
			// ����Ҫ�����������Ϣ���͸��Լ�
			area_enter( actor_index, enter_area[tmpi] );
		}
	}
	return 0;
}

//-----------------------------------------------------------------------------
// area_clearmsg
// ����˵��: �����������,���������ݷ��͸����б��������
// ����    : mapid - 
//           area_index - 
// ����ֵ  : int
//-----------------------------------------------------------------------------
int area_clearmsg( int area_index )
{
	int areax, areay;
	int tmpx, tmpy;
	int other_area_index;
	int pos;
	Area *pArea;

	if ( g_map.m_nAreaXNum == 0 )
		return 0; // ����г�0�Ŀ��ܾͷ���
	area_getoffset(area_index, &areax, &areay);

	pArea = &g_map.m_aArea[area_index];
	pos = pArea->pos == 0? 1 : 0;

	if( pArea->actor_size[pos] < 0 )
		pArea->actor_size[pos] = 0;

	if( pArea->actor_size[pos] <= 0 )
	{
		pArea->pos = pos;
		return 1;
	}

	for( tmpx = areax-1; tmpx <= areax+1; tmpx++ )
	{
		if( tmpx < 0 || tmpx >=g_map.m_nAreaXNum )
			continue;
		for( tmpy = areay-1; tmpy <= areay+1; tmpy++ )
		{
			if( tmpy < 0 || tmpy >=g_map.m_nAreaYNum )
				continue;
			other_area_index = area_getindex_fromgrid(tmpx, tmpy);

			for (int tmpi = 0; tmpi < g_maxactornum; tmpi++)
			{
				if (g_actors[tmpi].actorid > 0 && g_actors[tmpi].view_areaindex == other_area_index)
				{
					*(unsigned short *)pArea->actor_msg[pos] = pArea->actor_size[pos];
					sendtoclient(tmpi, pArea->actor_msg[pos], pArea->actor_size[pos] + sizeof(unsigned short));
				}
			}
		}
	}
	pArea->actor_size[pos] = 0;
	pArea->pos = pos;
	return 0;
}

// ������
// ���ｫ���������Ϣ�洢����,���߼�֡�������͸�����������й���������
int area_sendmsg( int area_index, int datasize, char *databuf )
{
	Area *pArea;
	pArea = &g_map.m_aArea[area_index];

	// ���͸�������������
	if( pArea->actor_size[pArea->pos] + datasize > 2040 )
	{
		area_clearmsg( area_index );
	}
	memcpy( pArea->actor_msg[pArea->pos] + pArea->actor_size[pArea->pos] + sizeof(unsigned short), databuf, datasize );
	pArea->actor_size[pArea->pos]+=datasize;
	if( pArea->actor_size[pArea->pos] > 2048 )
		exit(0);

	return 0;
}

// ����ʱ����, �������͸�ָ�������еĽ�ɫ, �������򲻻ᷢ��
// �ڲ�ʹ�ú���, ��actors_enterarea��actors_leavearea��ʹ��, ����������Ϣʹ��area_sendmsg����
int area_send( int area_index, int datasize, char *databuf )
{
	int tmpi;
	for ( tmpi = 0; tmpi < g_maxactornum; tmpi++ )
	{
		if ( g_actors[tmpi].actorid > 0 && g_actors[tmpi].view_areaindex == area_index )
		{
			sendtoclient( tmpi, databuf, datasize );
		}
	}

	return 0;
}

//-----------------------------------------------------------------------------
// area_sendactorinfo
// ����˵��: ����ָ������λ���������������е�����NPC����ҵ���Ϣ(����������)
//           ��ָ��socket,��Ӧ�������н�ɫ���ƶ�Ŀ�ĵ���Ϣ������ҵ�״̬��Ϣ(��ʱû��)
//           ��ҽ�����Ϸ����ս�������Ļָ�ʱ���ȡNPC��Ϣ
// ����    : client_index - 
//           mapid - 
//           posx - 
//           posy - 
// ����ֵ  : int
//-----------------------------------------------------------------------------
int area_sendunitinfo( int client_index, int posx, int posy )
{
	int areax, areay;
	int tmpx,tmpy,next_index;
	int area_index;
	int cur_index;

	char msg[2048] = {0};
	int size = 0;

	areax = posx/AREA_WIDTH;
	areay = posy/AREA_HEIGHT;

	for( tmpx = areax-1; tmpx <= areax+1; tmpx++ )
	{
		if( tmpx < 0 || tmpx >=g_map.m_nAreaXNum )
			continue;
		for( tmpy = areay-1; tmpy <= areay+1; tmpy++ )
		{
			if( tmpy < 0 || tmpy >=g_map.m_nAreaYNum )
				continue;
			area_index = area_getindex_fromgrid(tmpx, tmpy);

			cur_index = g_map.m_aArea[area_index].unit_head;
			while( cur_index >= 0 )
			{
				next_index = g_mapunit[cur_index].next_index;
				// process
				if( cur_index < 0 )
				{
					cur_index = next_index;
					continue;
				}
				if ( g_mapunit[cur_index].type == 0 )
				{
					cur_index = next_index;
					continue;
				}
				if ( mapunit_enterinfo( cur_index, msg + sizeof(short), &size ) < 0 )
				{
					*(unsigned short *)msg = size;
					sendtoclient( client_index, msg, size+sizeof(short) );
//					write_gamelog( "[size:%d][actor_enterinfo]", size );
					memset( msg, 0, 2048 );
					size = 0;
					mapunit_enterinfo( cur_index, msg + sizeof(short), &size );
				}
				cur_index = next_index;
			}
		}
	}

	if( size > 0 )
	{
		*(unsigned short *)msg = size;
		sendtoclient( client_index, msg, size+sizeof(short) );
		write_gamelog( "[size:%d][AREA_GETACTORINFO][%d]", size, g_mapunit[client_index].type );
	}
	return 0;
}

// ������һ������, һ����ɫ��Ӧ���area, ������Ϣ��Ҫ���
// Ҫά����һ���ͻ��˵�����
// ÿ�������е�����Ҫ���͵��˸�����������
void area_logic( int area_index )
{
	Area *pArea;
	pArea = &g_map.m_aArea[area_index];

	pArea->sendcount--;
	if( pArea->sendcount < 0 )
	{
		area_clearmsg( area_index );
		pArea->sendcount = 1;
	}
}
