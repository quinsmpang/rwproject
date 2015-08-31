#ifndef __MAPUNIT_H_
#define __MAPUNIT_H_

#define MAPUNIT_TYPE_CITY				1
#define MAPUNIT_TYPE_ARMY				2

// �����ͼ��ʾ��Ԫ
typedef struct _mapunit
{
	char type;					// ����1=City 2=ARMY 3...
	int index;					// ���Ͷ�Ӧ����
	int pre_index;				// ǰһ����Ԫ������
	int next_index;				// ��һ����Ԫ������
	int lastadd_areaindex;		// �ϴν����������
	char hide;
}MapUnit;

int mapunit_init();

// �������ͺͶ�Ӧ����ȷ����Ԫ����
int mapunit_getindex( char type, int index );

// ��ȡ��ʾ��Ԫ��Ӧ��������Ϣ
int mapunit_getattr( int unit_index, SLK_NetS_AddMapUnit *pAttr );

// ����Ҫ��ʾ�ĳǳػ������ӵ���ʾ��Ԫ
int mapunit_add( char type, int index );

// ���Ѿ������ĵ�ͼ��Ԫɾ����
int mapunit_del( char type, int index, int area_index );

// ��ʾ��Ԫ���������ͼ
int mapunit_enterworld( int unit_index, short posx, short posy );

// ��֯һ����Ԫ�������룬�뿪��������ݰ�
int mapunit_enterinfo( int unit_index, char *databuf, int *psize );
int mapunit_leaveinfo( int unit_index, char *databuf, int *psize );

// ��һ����ķ����¼�
int mapunit_enterarea( int unit_index, int area_index );
int mapunit_leavearea( int unit_index, int area_index );

int mapunit_resetinfo( int unit_index, char *databuf, int *psize );
int mapunit_resetarea( int unit_index, int area_index );

#endif
