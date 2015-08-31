//#ifndef __CLUB_H
//#define __CLUB_H
//#include "define.h"
//
//#define		CLUB_CHECK_INDEX(index)			do{	if( index <= 0 || index >= g_club_maxcount) \
//	return -1; \
//}while ( 0 )
//
//#define		CLUB_GETID_BYINDEX(index, id) do{ if( index <= 0 || index >= g_club_maxcount) \
//	return -1; \
//	id = g_club[index].m_clubid; \
//if ( id <= 0 )\
//	return -1; \
//}while ( 0 )
//
//
//#define	MAX_MEMBER_COUNT		100		// ��������Ա����
//#define	CLUB_NOTIFY_SIZE		128		// �����³̳���
//#define	MAX_ASKER_COUNT			64		// ������������
//#define	MAX_CLUBBUSINESS		4		// ���������������
//#define	MAX_CLUBLEVEL			5		// �������ȼ�
//#define	MAX_CLUB_BUILDLEVEL		20		// ���ɽ������ȼ�
//#define	CLUB_MAINUPGRADE_ING	100		// ��������������ʾ
//#define	CLUB_SHOP_ITEM_COUNT	20		// �������ˢ�µ���Ʒ�������
//#define	CLUB_STORAGE_PAGECOUNT	80		// ���ɼ���ȼ��洢����Ʒ����
//#define	CLUB_BUSINESS_ITEMKIND	582		// �������̵���
//#define CLUBEVENT_MAX_COUNT		20		// ����¼���
//
///* ְ�� */
//#define	CLUBPLACE_LEADER	100	// ����
//#define	CLUBPLACE_FLEADER	90	// ������
//#define	CLUBPLACE_CAPTAIN4	84	// ��������
//#define	CLUBPLACE_CAPTAIN3	83	// ��ȸ����
//#define	CLUBPLACE_CAPTAIN2	82	// �׻�����
//#define	CLUBPLACE_CAPTAIN1	81	// ��������
//#define	CLUBPLACE_MEMBER	10	// ����
//#define	CLUBPLACE_LOW		1	// ѧͽ
//
///* �¼���� */
//enum
//{
//	CLUBEVENT_LEADER_TRANSFER = 0
//	, CLUBEVENT_NEWMEMBER
//	, CLUBEVENT_KICK
//	, CLUBEVENT_LEAVE
//	, CLUBEVENT_MODIFY_POST
//	, CLUBEVENT_PLACE_CHANGE
//	, CLUBEVENT_CREATE
//	, CLUBEVENT_CLUB_UPGRADE
//	, CLUBEVENT_CLUB_STOP
//	, CLUBEVENT_CLUB_BACKOUT
//	, CLUBEVENT_BUILD_UPGRADE
//	, CLUBEVENT_BUILD_STOP
//	, CLUBEVENT_BUILD_BACKOUT
//	, CLUBEVENT_SKILL_UPGRADE
//	, CLUBEVENT_BUY_HERB
//	, CLUBEVENT_MAINTE
//	, CLUBEVNT_MAX
//};
//
///* ���彨�� */
//typedef enum
//{
//	ClubBuildNormal = 0,
//	Club_burse,	//�˷�
//	Club_drug,	//ҩ��
//	Club_room,	//ס��
//	ClubBuildMax,
//}ClubBuild;
//
///* �����̵� */
//typedef struct _club_Shop
//{
//	short itemkind;
//	int price;
//	int shoplevel;
//	int value;
//}ClubShop;
//
///* �����̵�����ĵ��� */
//typedef struct _club_Randitem
//{
//	short itemkind;
//	char count; // ʣ������
//	int price;  // ��Ǯ
//}ClubRandItem;
//
///* ������Ϣ */
//typedef struct _club
//{
//	short	m_clubid;							// �����ΨһID
//	int		m_createtime;						// ����ʱ��
//	char	m_clubname[NAME_SIZE];				// ��������
//	char	m_creatername[NAME_SIZE];			// ������
//	char	m_leadername[NAME_SIZE];			// �峤����
//	char	m_level;							// ����ȼ�
//	int		m_money;							// �ʽ�
//	int		m_boom;								// ���ٶ�
//	int		m_repute;							// ����
//	char	m_intro[CLUB_NOTIFY_SIZE];			// �����³�
//	short	m_membercount;						// ��Ա����
//	int		m_asker_count;						// �����˵�����
//	int		m_online_count;						// ��������
//	int		m_online_member[MAX_MEMBER_COUNT];	// ���ߵĳ�Աactor_index���ٲ�ʹ��
//	int		m_online_actorid[MAX_MEMBER_COUNT]; // ���߳�Աactorid
//	int		m_fdate;							// ����ÿ�ռ�������
//	int		m_current_week;						// ��ǰ����������ͳ������ܹ��׶�
//	short	m_week_online[7];					// һ����ÿ����������
//	char	m_isbroken;							// �����Ƿ�����ɢ״̬
//	char	m_broken_days;						// ��ɢ����
//	int		m_mapid;							// ����˽�е�ͼ���
//
//	// �����з�������Ҫ������
//	char	m_isclub_upgrade;					// �����Ƿ�������
//	int		m_club_upgrade_exp;					// ����������ǰ�Ľ����
//	int		m_res_point;						// �ʲ� 
//	short	m_build_level[ClubBuildMax];		// �����ȼ�
//	int		m_curbuild_develop;					// ��ǰ��չ�Ľ���
//	int		m_curbuild_exp;						// ��ǰ��չ�����Ľ����
//
//	ClubRandItem	m_clubranditem[CLUB_SHOP_ITEM_COUNT]; // ����ÿСʱˢ�µ���Ʒ
//	int		m_pay_scale;						// ���ʱ���
//}Club;
//
///* ��Ա��Ϣ */
//typedef struct _clubmember
//{
//	int		m_actorid;				// ��Աid
//	short	m_clubid;				// �����ΨһID
//	char	m_actorname[NAME_SIZE];	// ����
//	short	m_level;				// �ȼ�
//	int		m_aclass;				// ְҵ
//	char	m_place;				// ְ�� 0-��ʾ������ 1-��ʾѧͽ
//	char	m_online;				// �Ƿ�����
//	int		m_entertime;			// ���ʱ��
//	int		m_offlinetime;			// ����ʱ��
//	int		m_paymoney;				// ���ܹ��ʷֺ�
//}ClubMember;
//
///* ���彨��������Ϣ */
//typedef struct _club_upgrade
//{
//	int need_money;
//	int need_boom;
//	int need_repute;
//	int progress;
//	int other;
//}ClubUpgrade;
//
//typedef enum
//{
//	CLUB_MAX_LEVEL = 1
//	, CLUB_MONEY
//	, CLUB_BOOM
//	, CLUB_REPUTE
//	, CLUB_NOTLEADER
//	, CLUB_LEVEL
//	, CLUB_NOCLUB
//	, CLUB_NOTOWNER
//	, CLUB_CARLOCK
//	, CLUB_UPGRADE_ING	// ���ڽ���
//	, CLUB_UPGRADE_BUILD_LIMIT // �����ȼ�������
//	, CLUB_STOP			// ûǮ ֹͣ��
//	, CLUB_POWER_NONE	// Ȩ�޲���
//}CLUB_UPGRADE_ERRCODE;
//
///* ���ɳ�ʼ��������������ʱ����� */
//int club_init();
//
///* �������м�����Ϣ */
//void club_save();
//
///* ���ɸ��� */
//int club_update( short club_index );
//
///* �������� */
//int club_create( int actor_index, char *clubname, char *intro );
//
///* ɾ������ */
//int club_delete( short club_index );
//
///* ��Ұ������ݶ�ȡ */
//int club_actor_load( int actor_index );
//
///* ��Ұ������ݱ��� */
//int club_actor_save( int actor_index );
//
///* �������� */
//short club_getindex( short clubid );
//
///* ����ID */
//short club_getid( short club_index );
//
///* �������� */
//char *club_getname( short club_index );
//
///* ����ְλ���� */
//char *club_getplacename( char place );
//
///* ���ɽ������� */
//char *club_getbuildname( char buildid );
//
///* ���ɵ�ͼ */
//int club_getmapid( int actor_index );
//
///* ������Ϣ */
//int club_baseinfo( int actor_index );
//
///* �������߳�Ա�б� */
//int club_onlinememberlist( int actor_index );
//
///* �����б� */
//int club_list( int actor_index, short page );
//
///* ���ɳ�Ա�б� */
//int club_memberlist( int actor_index, short page );
//
///* ���������б� */
//int club_askerlist( int actor_index, short clubid );
//
///* ��հ��������б� */
//int club_clear_asklist( int actor_index );
//
///* �����¼��б� */
//int club_eventlist( int actor_index, short pageno );
//
///* ��������¼� */
//int club_add_event( short clubid, int actorid, char *name, int otherid, char *othername, char event );
//
///* ���ɷ�����Ϣ */
//int club_sendmsg( int club_index, int datasize, char *databuf );
//
///* ������������� */
//int club_online( int actor_index, char online );
//
///* һ������ */
//int club_allask( int actor_index );
//
///* ������� */
//int club_ask( int actor_index, short clubid, short clubindex );
//
///* ���� */
//int club_allow( int actor_index, int targetid );
//
///* �ܾ� */
//int club_reject( int actor_index, int askerid );
//
///* ���������б��飬ÿСʱ���һ�Σ�ɾ������������Ϣ */
//int club_asklist_logic();
//
///* ���� */
//int club_in( int actor_index, short clubid );
//
///* �˰� */
//int club_out( int actorid, int club_index );
//
///* ��������뿪��� */
//int club_leave( int actor_index );
//
///* ���� */
//int club_kick( int actor_index, int targetid );
//
///* ��������������������߽�ɫid */
//int club_today_loginactor( int club_index, int actor_index );
//
///* ת������ */
//void club_online_change( int old_index, int new_index );
//
///* ת��ְλ */
//int club_change_place( short club_index, int memberid, char place );
//
///* �����巢�ʼ� */
//int club_mail_send( int actor_index, int clubid, char *msg, char type, int value1, int value2, int value3, int value4, int overdue );
//
///* �����߼� 1Сʱ1�� */
//int club_logic_hourly();
//
///* �޸��³� */
//int club_change_intro( int actor_index, char *intro );
//
///* �ﹱ */
//int club_change_member_contribute( int actor_index, int contribute );
//int club_update_membercontribute( int actor_index );
//int club_get_member_contribute( int actor_index );
//
///* ��ʼ�������̵� */
//int club_shop_init();
//
///* �����̵�ˢ�� */
//int club_shop_refurbish( int club_index );
//
///* �����̵���Ϣ���� */
//int club_shop_listinfo( int actor_index );
//
///* �����̵깺�� */
//int club_shop_buy( int actor_index, int itemindex );
//
///* ���ɽ����̵� */
//int club_tradeshop_listinfo( int actor_index );
//
///* ����ó���г����� */
//int club_tradeshop_buy( int actor_index, int id );
//
///* ����ó���г��������� */
//int club_tradeshop_sell_ask( int actor_index, int itemindex );
//
///* ����ó���г����� */
//int club_tradeshop_sell( int actor_index, int itemindex, int payment, int price );
//
//int club_shop_update( short club_index );
//int club_place_right( short club_index, char place, int right );
//int club_invitememeber( int actor_index, int actorID );
//
//int club_proc_invite( int actor_index, int targetid, short clubid, char type );
//int club_proc_openinvitedlg( int actor_index, int actorid );
//
//int club_memberproc( int actor_index, int actorid, char clubclass );
//
//int club_checkplace( int actor_index );
//
//void club_logic(); // �����߼� 1����1��
//
//int club_open_storage( int actor_index, char type );
//int club_open_bank( int actor_index, char type );
//int club_storagein( int actor_index, short clubid, short item_sel, short item_num );
//int club_storageout( int actor_index, short clubid, short item_sel, short item_num );
//int club_newstorage_db( short club_index, int item_offset, char fNewid );
//int club_deletestorage_db( short club_index, int item_offset );
//int club_storage_load( short club_index );
//int club_bankin( int actor_index, short clubid, int money );
//int club_bankout( int actor_index, short clubid, int money );
//int club_upgrade( int actor_index );
//
//int club_getquestdonetimes( short club_index, short questtype_offset );
//int club_setquestdonetimes( short club_index, short questtype_offset, int times );
//
//int club_add_boom_repute( int club_index, int boom, int repute );
//int club_add_money( int club_index, int money );
//
//int club_gm( int actor_index, int type, int value, int clubid );
//
//int club_getallclubnum(); // �õ����о��ֲ�����
//int club_getallclubnum_without_isbroken(); // �õ����о��ֲ���������������ɢ״̬��
//int club_getonlinemembernum( short club_index ); // �õ��������������ҵ�����
//int club_member_inmap( short club_index, int mapid ); // �õ����������ĳ�ŵ�ͼ����ҵ�����
//
//int club_reset_member_wcontribute( int club_index );
//int club_check_current_week( int club_index );
//
//int club_broken_logic();
//int club_check_leader_online( int club_index );
//
//int club_reset_broken();
//int club_allowenter_clubmap( int actor_index );
//
//// ����
//int club_set_money( int club_index, int value );
//int club_get_money( int club_index );
//int club_set_boom( int club_index, int value );
//int club_get_boom( int club_index );
//int club_set_repute( int club_index, int value );
//int club_set_res( int club_index, int value );
//int club_is_stop( int club_index );
//int club_level_fall( int club_index );
//
//int club_get_hourly_mainte_money( int club_index ); // ��ȡÿСʱά������
//int club_get_money_least( int club_index );			// ��ȡ�ʽ����
//int club_get_money_most( int club_index );			// ��ȡ�ʽ�����
//int club_get_member_max( int club_index );			// ��ȡ����Ա
//int club_get_hourly_makeres( int club_index );		// ��ȡÿСʱ��������Դ��
//int club_get_maxres( int club_index );				// ��ȡ��Դ����
//int club_get_business_award_least( int club_index );// ��ȡ���̽�������
//int club_get_studypower( int club_index );			// ��ȡ�о���
//int club_get_brush_drugcount( int club_index );		// ��ȡҩ��ˢҩ������
//int club_get_brush_drugcount_show( int club_index );// ��ȡҩ��ˢҩ������-������ʾ
//int club_get_repute_value( int club_index );		// ��ȡ����������
//
//int club_get_build_upgradeexp( int club_index, int build );	// ��ȡ�����������轨���
//int club_get_build_upgrademoney( int club_index, int build );	// ��ȡ�������������ʽ�
//int club_get_build_upgraderepute( int club_index, int build );	// ��ȡ����������������
//int club_get_build_upgradeboom( int club_index, int build );	// ��ȡ�����������跱��
//
//int club_add_clubupgrade_exp( int club_index, int value );	// ��ӵ�ǰ��������Ľ����
//int club_add_curbuild_exp( int club_index, int value );		// ��ӵ�ǰ���������Ľ����
//int club_add_curskill_exp( int club_index, int value );		// ��ӵ�ǰ�������ܵľ���
//
//int club_canupgrade_withbuild( int club_index );  // ���彨���ܷ���������
//int club_upgrade_issuccess( int club_index );		// ���������Ƿ�ɹ�
//
//int club_build_upgrade( int actor_index, int build ); // ���彨������
//int club_build_upgrade_issuccess( int club_index );  // ���彨�������Ƿ�ɹ�
//
//int club_upgrade( int actor_index ); // ��������
//int club_upgrade_info( int actor_index, short buildid );
//
//int club_degrade( int actor_index );
//int club_build_degrade( int actor_index, int build );
//
//int club_upgrade_stop( int actor_index );
//int club_build_upgrade_stop( int actor_index, int build );
//int club_send_buildinfo( int actor_index );
//
//
//// ����
//#define CLUB_BUSINESS_ITEM_COUNT 20
//#define CLUB_BUSINESS_SHOP_COUNT 5
//typedef struct _club_business_sellprice
//{
//	int shopid;
//	int itemkind[CLUB_BUSINESS_ITEM_COUNT];
//	int sellprice[CLUB_BUSINESS_ITEM_COUNT];
//	int price[CLUB_BUSINESS_ITEM_COUNT];
//	int value[CLUB_BUSINESS_ITEM_COUNT];
//}ClubBusinessSellPrice;
//// ����ר�õļ�������
//// �Ƿ�������
//int club_actor_isbusinessman( int actor_index );
//// ��ȡ��Ʊ��ֵ
//int club_actorget_bill_value( int actor_index, int *value, int *max_value );
//// ������Ʊ��ֵ
//int club_actorset_bill_value( int actor_index, int value, int max_value );
//// �Ƿ��������̵�
//int club_issellshop( int shopid );
//// ��ȡ����̵꣬�����Ʒ�ļ۸�
//int club_getsellprice( int shopid, int itemkind );
//// �۸��̵��id��ȡ
//int club_randomsellshop_init();
//int club_shopsellprice_init( int shopid );
//// ����۸�
//int club_randomsellprice( int shopid );
//
//
//// ҩ��ר��
//// ����ҩ�������ҩƷ���������
//int club_random_drugcount( int club_index, int totalcount );
//
//// �������ҩ����Ʒ����ĸ���
//int club_drugcount_total( int club_index );
//
//// �������ҩ����Ʒÿ��kind�ĸ���
//int club_drugcount_withkind( int club_index, int kind );
//
//// �������ҩ��������Ʒ����
//int club_set_drugcount_withkind( int club_index, int kind, int value );
//
//
//#endif 
