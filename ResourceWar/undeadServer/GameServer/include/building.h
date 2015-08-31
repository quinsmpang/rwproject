#ifndef _BUILDING_H_
#define _BUILDING_H_
// Ψһ����
#define	BUILDING_TownCenter		1	//	����/ʥ��
#define	BUILDING_Wall			2	//	��ǽ/��ǽ
#define	BUILDING_Drunkery		3	//	�ƹ�/����
#define	BUILDING_Market			4	//  �г�/�г�
#define	BUILDING_GamblingHouse	5	//  �ĳ�/�ĳ�
#define	BUILDING_Library		6	//	ͼ���/��ʥ֮��
#define	BUILDING_WishingPool	7	//	��Ը��/��̳
#define	BUILDING_Embassy		8	//	��ʹ��/����
#define	BUILDING_Smithy			9	//	������/������
#define	BUILDING_DrillGround	10	//	У��/���ĳ�	
#define	BUILDING_StoreHouse		11	//	�ֿ�/�ؽ�
// ��Դ
#define	BUILDING_WoodFactory	12	//	ľ�ĳ�/������
#define	BUILDING_Farm			13	//	ũ��/����
#define	BUILDING_IronFactory	14	//	����/������
#define	BUILDING_Mithril		15	//	����Ѩ/���
// ����
#define	BUILDING_House			16	//	���/��Ѩ
#define	BUILDING_Barracks		17	//	��Ӫ
#define	BUILDING_Stable			18	//	���
#define	BUILDING_ShootingRange	19	//	�г�
#define	BUILDING_Tower			20	//	��¥
#define	BUILDING_Magician		21	//	��ʦӪ��
#define	BUILDING_Craftsman		22	//	������

#define BUILDING_MAXLEVEL			60	// �������ȼ�
#define	BUILDING_INFO_LIMIT_MAX		2	// ��������������������
#define	BUILDING_INFO_RES_MAX		4	// ����������������Դ����
#define	BUILDING_INFO_VALUE_MAX		4	// ������Ϣ�������Ը���

// ������ǰ״̬
#define BUILDING_STATE_NORMAL			0 // ȱʡ״̬
#define BUILDING_STATE_CREATE_ING		1 // ������
#define BUILDING_STATE_CREATE_ED		2 // �Ѿ�������ɣ�����ʹ�ã��������������Ľ������
#define BUILDING_STATE_UPGRADE_ING		3 // ������
#define BUILDING_STATE_UPGRADE_ED		4 // ������ɣ�����ʹ�ã����������������������
#define BUILDING_STATE_DELETE_ING		5 // �����
#define BUILDING_STATE_DELETE_ED		6 // �����ɣ�����ʹ�ã��������������Ĳ���ɹ�
#define BUILDING_STATE_TROOPTRAIN_ING	7 // ѵ����
#define BUILDING_STATE_TROOPTRAIN_ED	8 // ѵ�����
#define BUILDING_STATE_TROOPUP_ING		9 // ����������
#define BUILDING_STATE_TROOPUP_ED		10 // �����������
#define BUILDING_STATE_TROOPHEAL_ING	11 // ������
#define BUILDING_STATE_TROOPHEAL_ED		12 // �������

// ��������������ϸ��Ϣ
typedef struct _buildingconfig  
{
	char limit_kind[BUILDING_INFO_LIMIT_MAX];		// �����������ﵽ���ټ�
	char limit_level[BUILDING_INFO_LIMIT_MAX];		// �����������ﵽ���ټ�
	char restype[BUILDING_INFO_RES_MAX];			// ��Ҫ��Դ
	int resvalue[BUILDING_INFO_RES_MAX];			// ��Ҫ��Դ����
	char worker;									// ��Ҫũ������
	int sec;										// ������������ʱ��
	int value[BUILDING_INFO_VALUE_MAX];				// ���⸽������
}BuildingUpgradeConfig;

// ����
typedef struct _buildingupgrade
{
	BuildingUpgradeConfig *info;
	char maxlevel;
}BuildingUpgrade;

// ������Ϣ
typedef struct _buildinglimit
{
	short maxnum[1];	// �ɽ�������
	short maxlevel[1];	// ���������ȼ�
}BuildingLimit;

// ������Ϣ
typedef struct _buildingnew
{
	unsigned char buildingindex;
	unsigned char buildingkind;
	char level;	
}BuildingNew;

// �ǳص�ÿ��������Ϣ
typedef struct _building
{
	unsigned char buildingkind;			// ��������
	char level;							// �����ȼ�
	char state;							// ��ǰ״̬
	int begintime;						// ��ʼʱ��
	int value[BUILDING_INFO_VALUE_MAX]; // ��������
}Building;

// ������Ϣ��ʼ��
int building_upgrade_init();

// ����������Ϣ��ʼ��
int building_limit_init();

// ������ʼ������Ϣ��ʼ��
int building_new_init();

// �ǳؽ�����Ϣ��ȡ
int building_load( struct _city *pCity );

// ���гǳؽ�����Ϣ����
int building_save( struct _city *pCity, FILE *fp );

// һ���ǳؽ�����Ϣ����
int building_single_save( int cityid, short offset, Building *pBuilding, FILE *fp );

// ����һ������
int building_create( struct _city *pCity, short buildingindex, short buildingkind );

// ����һ������
int building_upgrade( struct _city *pCity, short buildingindex );

// �������
int building_instantly_finish( struct _city *pCity, short buildingindex );

// ɾ��һ������
int building_delete( struct _city *pCity, short buildingindex );

// �ƶ�һ������
int building_move( struct _city *pCity, short buildingindex, unsigned char posx, unsigned char posy );

// �ɼ�
int building_gather( struct _city *pCity, short buildingindex );

// ���ʱ����
int building_troop_checktime( struct _city *pCity, short buildingindex );

// ����������������ʱ��
int building_checktime( struct _city *pCity, short buildingindex );

// ��������������������
int building_finish( struct _city *pCity, short buildingindex );

// �����������
int building_getnum( struct _city *pCity, short buildingkind );

// ��ȡһ����������
int building_getemptyindex( struct _city *pCity );

// ��ȡ��������ʱ��
int building_getneedtime( Building *pBuilding );

// ����
// ����ѵ��, op : 1-ѵ����2-����
int building_troop_train( int actor_index, int buildingindex, char op, short corps, char level, int num );
// ����
int building_troop_wound( int actor_index, short corps, char level, int num );
// ���������
int building_troop_abandon( int actor_index, short corps, char level, int num );
// ȡ��ѵ��
int building_troop_traincancel( int actor_index, int buildingindex );
// �������ѵ��
int building_troop_trainfinish( int actor_index, int buildingindex );
// ��ɲ���
int building_troop_ontrainfinish( int actor_index, int buildingindex );
// ��ɲ���
int building_troop_ontrainget( int actor_index, int buildingindex );

#endif

