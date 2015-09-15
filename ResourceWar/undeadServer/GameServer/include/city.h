#ifndef _CITY_H_
#define _CITY_H_
#include "define.h"
#include "building.h"
#include "server_structrecv_auto.h"
#include "actor.h"

#define CityBuildingMax				35	// һ���ǳ����ӵ�ж��ٽ���
#define CityHandleQueueMax			2	// һ���ǳصĽ�����������
#define CityCorpsMax				5	// ��������
#define CityCorpsLevelMax			10	// ÿ�ֱ����ȼ�
#define CityUnderFire				32	// ����ܶ���֧���ӹ���

// �ǳ���������
typedef enum
{
	CityLairdType_Player = 0,		//	��ҵĳǳ�
	CityLairdType_Match = 1,		//	��ҳ���ʱ��Ķ��ֳǳ�
	CityLairdType_Robot = 2,		//  �����˳ǳ�
}CityLairdType;

// ��Դ���Ͷ���
typedef enum
{
	CityRes_Normal	= 0,		//	û��
	CityRes_Wood	= 1,		//	ľ��
	CityRes_Food	= 2,		//	��ʳ
	CityRes_Iron	= 3,		//  ��
	CityRes_Mithril = 4,		//  ����
	CityRes_Gold	= 5,		//	���
}CityRes;

// �ǳ�ӵ�еĲ��ӽṹ
typedef struct _citycorps
{
	char corps;	// ����
	char level; // ���ֵȼ�
	int count;	// ��������
}CityCorps;

// ��������Ϣ
typedef struct _under_fire_info
{
	int army_index;
}UnderFireInfo;

// һ���ǳصĽṹ
typedef struct _city
{
	// ��Ҫ�洢������
	int cityid;									// �ǳ�id
	char laird_type;							// ��������
	int laird_actorid;							// ����id
	char laird_name[NAME_SIZE];					// ��������
	unsigned char laird_shape;					// ��������
	unsigned char laird_level;					// �����ȼ�
	int laird_lastlogin;						// �����ϴε�¼ʱ��
	int match_cityid;							// ��ҳ������ֳǳ�id
	char aclass;								// �ǳ����� 0���� 1����
	char skin;									// �ǳ�Ƥ��
	int clubid;									// ���˱��
	short posx;									// ��������
	short posy;									// ��������
	int	wood;									// ľ��
	int	food;									// ��ʳ
	int iron;									// ��
	int mithril;								// ����	
	int gold;									// ���
	Building building[CityBuildingMax];			// �����б�
	int corps_num[CityCorpsMax][CityCorpsLevelMax];	// ������5�ֱ�ÿ��10��
	int corps_wound[CityCorpsMax][CityCorpsLevelMax];	// �˱�����5�ֱ�ÿ��10��

	// ���������ʱ���ݣ�����Ч��
	int actor_index;							// ����������ֵ��Ч
	int match_cityindex;						// ��ҳ������ֳǳ�����
	short club_index;							// ��������
	short handle_queue[CityHandleQueueMax];		// �����������У����ٱ�������

	UnderFireInfo underfire[CityUnderFire];		// ��������ǳص����в���
}City;

// ������������ȡ���гǳ���Ϣ���ڴ�
int city_load();

// �������رգ����гǳ���Ϣ�浽���ݿ�
int city_save( FILE *fp );
int city_single_save( City *pCity, FILE *fp );

// ���г���ÿ����߼�
void city_logic_sec();

// ���г���ÿ���ӵ��߼�
void city_logic_min();

// ÿ�춨ʱ�ݻ�һЩ�ǳ�
void city_logic_destroy();

// �������id�ҵ��ǳ�����
int city_getindex_withactorid( int actorid );

// ���ݳǳ�id�ҵ��ǳ�����
int city_getindex_withcityid( int cityid );

// ����һ���³ǳ�
int city_new( City *pCity );

// ɾ��һ���ǳ�
void city_del( City *pCity, int city_index );

// ���¼��������ʱ����
void city_reset( City *pCity );

// ��ȡ�����ҵĳǳ�����
City *city_getptr( int actor_index );

// ��ȡ�����ҳ������ֵĳǳ�����
City *city_match_getptr( int actor_index );

// սʤ���ֳǳ�
void city_beat_match_city( int city_index );

// ������Դ�ı�
void city_change_res( City *pCity, CityRes res, int value, char path );

// �ѱ��趨Ϊ����Ŀ��
void city_underfire( City *pCity, int army_index );

// ����
void city_battle( int actor_index, City *pCity, SLK_NetC_CityBattleInfo *info);

// Ǩ��
int city_move( int actor_index, short posx, short posy );

// ��������
int city_handle_queue_add( City *pCity, short buildingindex );
void city_handle_queue_del( City *pCity, short buildingindex );
void city_handle_queue_clear( City *pCity );

// --------------------------------------------- city_send.cpp ------------------------------------------------
// ��ҳ�����Ϣ
int city_info( int actor_index );

// ������Դ��Ϣ
void city_resinfo( int actor_index, City *pCity );

// ���н���������Ϣ
void city_buildinglist( int actor_index, City *pCity );

// ���н�����������ʱ����Ϣ
void city_building_buildtimeinfo( int actor_index, short buildingindex, Building *pBuilding );

// ���н�����������ʱ����Ϣ�б�
void city_building_buildtimeinfolist( int actor_index, City *pCity );

// ��Դ�ı�
void city_res_changeinfo( int actor_index, char restype, int resvalue, char path );

// ����һ�������Ļ�����Ϣ
void city_building_baseinfo( int actor_index, short buildingindex, Building *pBuilding );

// ����һ��������������Ϣ
void city_building_abilityinfo( int actor_index, short buildingindex, Building *pBuilding );

// ����һ��������������Ϣ
void city_building_abilitylist( int actor_index, City *pCity );

// ���ͳ��б�����Ϣ�б� 
void city_troop_sendinfolist( int actor_index, City* pCity );

// ���͵���������Ϣ����
void city_troop_sendinfo( int actor_index, City* pCity, int corps_type, int level );
// �ж��û��ĳ����Ƿ񱻹���/������
bool actor_city_is_underfire( int city_index );
// ���;������Ӧ�����
int actor_city_underfire_info( int actor_index );
#endif
