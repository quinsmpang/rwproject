#ifndef __MAPUNIT_H_
#define __MAPUNIT_H_

#define MAPUNIT_TYPE_CITY				1
#define MAPUNIT_TYPE_ARMY				2

// 世界地图显示单元
typedef struct _mapunit
{
	char type;					// 类型1=City 2=ARMY 3...
	int index;					// 类型对应索引
	int pre_index;				// 前一个单元的索引
	int next_index;				// 下一个单元的索引
	int lastadd_areaindex;		// 上次进入的区域编号
	char hide;
}MapUnit;

int mapunit_init();

// 根据类型和对应索引确定单元索引
int mapunit_getindex( char type, int index );

// 获取显示单元对应的属性信息
int mapunit_getattr( int unit_index, SLK_NetS_AddMapUnit *pAttr );

// 将需要显示的城池或军队添加到显示单元
int mapunit_add( char type, int index );

// 将已经关联的地图单元删除掉
int mapunit_del( char type, int index, int area_index );

// 显示单元进入世界地图
int mapunit_enterworld( int unit_index, short posx, short posy );

// 组织一个单元真正进入，离开区域的数据包
int mapunit_enterinfo( int unit_index, char *databuf, int *psize );
int mapunit_leaveinfo( int unit_index, char *databuf, int *psize );

// 单一区域的发送事件
int mapunit_enterarea( int unit_index, int area_index );
int mapunit_leavearea( int unit_index, int area_index );

int mapunit_resetinfo( int unit_index, char *databuf, int *psize );
int mapunit_resetarea( int unit_index, int area_index );

#endif
