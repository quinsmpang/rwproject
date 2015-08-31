#ifndef __WORLDMAP_H_
#define __WORLDMAP_H_

#include "area.h"

// 世界地图
struct _map
{
	int				m_nMaxWidth;	// 地图的宽度(方块单位)
	int				m_nMaxHeight;	// 地图的高度(方块单位)
	short			m_nAreaXNum;
	short			m_nAreaYNum;
	Area			*m_aArea;
	unsigned char	**m_aTileData;	// 每个格子的数据
};
typedef struct _map Map;

int worldmap_init();
void worldmap_logic();
void worldmap_sendinfo( int actor_index );

// 野外地图
void outsidemap_sendinfo( int actor_index );
#endif
