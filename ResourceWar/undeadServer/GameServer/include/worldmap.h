#ifndef __WORLDMAP_H_
#define __WORLDMAP_H_

#include "area.h"

// �����ͼ
struct _map
{
	int				m_nMaxWidth;	// ��ͼ�Ŀ��(���鵥λ)
	int				m_nMaxHeight;	// ��ͼ�ĸ߶�(���鵥λ)
	short			m_nAreaXNum;
	short			m_nAreaYNum;
	Area			*m_aArea;
	unsigned char	**m_aTileData;	// ÿ�����ӵ�����
};
typedef struct _map Map;

int worldmap_init();
void worldmap_logic();
void worldmap_sendinfo( int actor_index );

// Ұ���ͼ
void outsidemap_sendinfo( int actor_index );
#endif
