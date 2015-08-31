//#include <stdio.h>
//#include <math.h>
//#ifdef WIN32
//#include <assert.h>
//#include <winsock2.h>
//#include <mswsock.h>
//#endif
//#include <mysql.h>
//#include <time.h>
//#include "db.h"
//#include "actor.h"
//#include "club.h"
//#include "gameproc.h"
//#include "gamelog.h"
//#include "utils.h"
//#include "actor_send.h"
//#include "mail_msg.h"
//
//extern MYSQL *myGame;
//extern MYSQL *myData;
//
//extern Actor *g_actors;
//extern int g_maxactornum;
//
//Club *g_club = NULL;
//short g_club_maxcount = 0;
//
//int g_maxclubmember[MAX_CLUB_BUILDLEVEL + 1] = { 0 }; // �����Ա����
//int g_maxclubmoneymost[MAX_CLUB_BUILDLEVEL + 1] = { 0 }; // �����ʽ��������
//ClubUpgrade g_clubupgrade[MAX_CLUBLEVEL + 1] = { 0 }; // ������������
//ClubUpgrade g_clubbuildupgrade[ClubBuildMax][MAX_CLUB_BUILDLEVEL + 1] = { 0 };// ���彨������
//int g_clubbuild_hourlymoney[ClubBuildMax][MAX_CLUB_BUILDLEVEL + 1] = { 0 };	// ���彨��ÿСʱά������
//int g_club_hourly_makeres[MAX_CLUB_BUILDLEVEL + 1] = { 0 };  // ����ÿСʱ������Դ��
//int g_club_maxres[MAX_CLUB_BUILDLEVEL + 1] = { 0 }; // �����Դ
//int g_club_business_award_least[MAX_CLUB_BUILDLEVEL + 1] = { 0 };  // ���̽�������
//int g_club_studypower[MAX_CLUB_BUILDLEVEL + 1] = { 0 };  // �о���
//int g_club_brush_drugcount[MAX_CLUB_BUILDLEVEL + 1] = { 0 };  // ҩ��ˢҩ��
//
//char *g_szClubBuildName[] = { "", "���", "�о���", "ס��", "�ֿ�", "ҩ��" };
//int g_club_drugkind[CLUB_SHOP_ITEM_COUNT] = { 5, 6, 7, 15, 16, 17, 25, 26, 27, };// ����ҩƷ
//ClubBusinessSellPrice	g_business_sell[CLUB_BUSINESS_SHOP_COUNT]; // ������Ʒ������
//
//int g_club_create_money = 500000;
//int g_club_base_money = 500000;
//
//ClubShop *g_clubshop = NULL;
//int g_clubshopitemcount = 0;
///* ���ɳ�ʼ��������������ʱ����� */
//int club_init()
//{
//	int tmpi;
//	MYSQL_RES	*res;
//	MYSQL_ROW	row;
//	char	szSQL[1024];
//	short club_index;
//	short clubid;
//
//	// �õ��ɷ�������id
//	sprintf( szSQL, "select max(clubid) from club" );
//	if ( mysql_query( myGame, szSQL ) )
//	{
//		printf( "Query failed (%s)\n", mysql_error( myGame ) );
//		write_gamelog( "%s", szSQL );
//		return -1;
//	}
//	res = mysql_store_result( myGame );
//	if ( (row = mysql_fetch_row( res )) )
//	{
//		if ( row[0] )
//			g_max_clubid = atoi( row[0] ) + 1;
//		else
//			g_max_clubid = 1;
//	}
//	else
//	{
//		g_max_clubid = 1;
//		mysql_free_result( res );
//		return -1;
//	}
//	mysql_free_result( res );
//
//	// ����ռ�
//	g_club = (Club *)malloc( sizeof(Club)*MAX_CLUB_COUNT );
//	memset( g_club, 0, sizeof(Club)*MAX_CLUB_COUNT );
//	club_index = 1;
//
//	sprintf( szSQL, "select * from club" );
//	if ( mysql_query( myGame, szSQL ) )
//	{
//		printf( "Query failed (%s)\n", mysql_error( myGame ) );
//		write_gamelog( "%s", szSQL );
//		return -1;
//	}
//	res = mysql_store_result( myGame );
//	while ( (row = mysql_fetch_row( res )) )
//	{
//		clubid = atoi( row[0] );
//		if ( clubid <= 0 )
//		{
//			mysql_free_result( res );
//			return -1;
//		}
//		int index = 1;
//		g_club[club_index].m_clubid			= clubid;
//		strcpy( g_club[club_index].m_clubname, row[index++] );
//		g_club[club_index].m_createtime		= atoi( row[index++] );
//		strcpy( g_club[club_index].m_creatername, row[index++] );
//		strcpy( g_club[club_index].m_leadername, row[index++] );
//		g_club[club_index].m_level			= atoi( row[index++] );
//		g_club[club_index].m_money			= atoi( row[index++] );
//		g_club[club_index].m_boom			= atoi( row[index++] );
//		g_club[club_index].m_repute			= atoi( row[index++] );
//		strcpy( g_club[club_index].m_intro, row[index++] );
//		g_club[club_index].m_fdate			= atoi( row[index++] );
//		g_club[club_index].m_current_week	= atoi( row[index++] );
//		for ( tmpi = 0; tmpi < 7; tmpi++ )
//		{
//			g_club[club_index].m_week_online[tmpi]	= atoi( row[index++] );
//		}
//		g_club[club_index].m_isbroken				= atoi( row[index++] );
//		g_club[club_index].m_broken_days			= atoi( row[index++] );
//		g_club[club_index].m_pay_scale				= atoi( row[index++] );
//		g_club[club_index].m_isclub_upgrade			= atoi( row[index++] );
//		g_club[club_index].m_club_upgrade_exp		= atoi( row[index++] );
//		g_club[club_index].m_res_point				= atoi( row[index++] );
//		g_club[club_index].m_curbuild_develop		= atoi( row[index++] );
//		g_club[club_index].m_curbuild_exp			= atoi( row[index++] );
//		g_club[club_index].m_build_level[1]			= atoi( row[index++] );
//		g_club[club_index].m_build_level[2]			= atoi( row[index++] );
//		g_club[club_index].m_build_level[3]			= atoi( row[index++] );
//		g_club[club_index].m_build_level[4]			= atoi( row[index++] );
//		g_club[club_index].m_build_level[5]			= atoi( row[index++] );
//		g_club[club_index].m_mapid					= map_create_copy( CLUB_MAPID );
//
//		for ( tmpi = 0; tmpi < MAX_MEMBER_COUNT; tmpi++ )
//		{
//			g_club[club_index].m_online_member[tmpi] = -1;
//		}
//
//		// ˢ�̵�
//		club_shop_refurbish( club_index );
//		club_index++;
//	}
//	mysql_free_result( res );
//
//	//// ÿ�������ҩ����ҩƷ����
//	//for ( tmpi = 0; tmpi < MAX_CLUB_COUNT; tmpi++ )
//	//{
//	//	if ( g_club[tmpi].m_clubid > 0 )
//	//	{
//	//		sprintf( szSQL, "select * from club_shop where clubid='%d' ", g_club[tmpi].m_clubid );
//	//		if ( mysql_query( myGame, szSQL ) )
//	//		{
//	//			printf( "Query failed (%s)\n", mysql_error( myGame ) );
//	//			write_gamelog( "%s", szSQL );
//	//			return -1;
//	//		}
//	//		res = mysql_store_result( myGame );
//	//		while ( (row = mysql_fetch_row( res )) )
//	//		{
//	//			g_club[tmpi].m_drugcount[0] = atoi( row[1] );
//	//			g_club[tmpi].m_drugcount[1] = atoi( row[2] );
//	//			g_club[tmpi].m_drugcount[2] = atoi( row[3] );
//	//			g_club[tmpi].m_drugcount[3] = atoi( row[4] );
//	//			g_club[tmpi].m_drugcount[4] = atoi( row[5] );
//	//			g_club[tmpi].m_drugcount[5] = atoi( row[6] );
//	//			g_club[tmpi].m_drugcount[6] = atoi( row[7] );
//	//			g_club[tmpi].m_drugcount[7] = atoi( row[8] );
//	//			g_club[tmpi].m_drugcount[8] = atoi( row[9] );
//	//		}
//	//		mysql_free_result( res );
//	//	}
//	//}
//
//	// �����ÿ�����������
//	for ( tmpi = 0; tmpi < MAX_CLUB_COUNT; tmpi++ )
//	{
//		if ( g_club[tmpi].m_clubid > 0 )
//		{
//			// ��ʽ��Ա����
//			sprintf( szSQL, "select count(*) from club_member where clubid='%d' and place > 0", g_club[tmpi].m_clubid );
//			if ( mysql_query( myGame, szSQL ) )
//			{
//				printf( "Query failed (%s)\n", mysql_error( myGame ) );
//				write_gamelog( "%s", szSQL );
//				return -1;
//			}
//			res = mysql_store_result( myGame );
//			if ( (row = mysql_fetch_row( res )) )
//				g_club[tmpi].m_membercount = atoi( row[0] );
//			else
//				g_club[tmpi].m_membercount = 0;
//			mysql_free_result( res );
//
//			// �����˵�����
//			sprintf( szSQL, "select count(*) from club_member where clubid='%d' and place = 0", g_club[tmpi].m_clubid );
//			if ( mysql_query( myGame, szSQL ) )
//			{
//				printf( "Query failed (%s)\n", mysql_error( myGame ) );
//				write_gamelog( "%s", szSQL );
//				return -1;
//			}
//			res = mysql_store_result( myGame );
//			if ( (row = mysql_fetch_row( res )) )
//				g_club[tmpi].m_asker_count = atoi( row[0] );
//			else
//				g_club[tmpi].m_asker_count = 0;
//			mysql_free_result( res );
//		}
//	}
//
//	// �������ɳ�Ա����
//	for ( tmpi = 0; tmpi <= MAX_CLUB_BUILDLEVEL; tmpi++ )
//	{
//		g_maxclubmember[tmpi] = tmpi * 10 + 40;
//		g_maxclubmember[tmpi] = g_maxclubmember[tmpi]>MAX_MEMBER_COUNT ? MAX_MEMBER_COUNT : g_maxclubmember[tmpi];
//	}
//	// ��������ʽ�����
//	for ( tmpi = 0; tmpi <= MAX_CLUB_BUILDLEVEL; tmpi++ )
//	{
//		g_maxclubmoneymost[tmpi] = (2000 + 1000 * tmpi) * 10000;
//	}
//	// ������������
//	for ( tmpi = 0; tmpi <= MAX_CLUBLEVEL; tmpi++ )
//	{
//		g_clubupgrade[tmpi].need_money = 10000000;		// �ʽ�
//		g_clubupgrade[tmpi].need_boom = 1000;			// ����
//		g_clubupgrade[tmpi].need_repute = 1000;			// ����
//		g_clubupgrade[tmpi].progress = 1100 + 50 * tmpi;// �����
//		//g_clubupgrade[tmpi].other = 4 * (tmpi - 1);		// ��������������������ﵽ4*�ȼ���
//		g_clubupgrade[tmpi].other = 4 * (tmpi - 1);		// �������������ڵ���������ȼ�*5��
//	}
//	// ���彨���������裬�ȼ����ƣ����ɵȼ�*3
//	for ( tmpi = 0; tmpi <= MAX_CLUB_BUILDLEVEL; tmpi++ )
//	{
//		// �˷�
//		g_clubbuildupgrade[Club_burse][tmpi].need_money = (50 + 10 * tmpi) * 10000;
//		g_clubbuildupgrade[Club_burse][tmpi].need_boom = 200;
//		g_clubbuildupgrade[Club_burse][tmpi].need_repute = 200;
//		g_clubbuildupgrade[Club_burse][tmpi].progress = 100 + 50 * tmpi;
//		g_clubbuildupgrade[Club_burse][tmpi].other = 0;
//		// ס��
//		g_clubbuildupgrade[Club_room][tmpi].need_money = (50 + 10 * tmpi) * 10000;
//		g_clubbuildupgrade[Club_room][tmpi].need_boom = 200;
//		g_clubbuildupgrade[Club_room][tmpi].need_repute = 200;
//		g_clubbuildupgrade[Club_room][tmpi].progress = 100 + 50 * tmpi;
//		g_clubbuildupgrade[Club_room][tmpi].other = 0;
//		// ҩ��
//		g_clubbuildupgrade[Club_drug][tmpi].need_money = (100 + 20 * tmpi) * 10000;
//		g_clubbuildupgrade[Club_drug][tmpi].need_boom = 200;
//		g_clubbuildupgrade[Club_drug][tmpi].need_repute = 200;
//		g_clubbuildupgrade[Club_drug][tmpi].progress = 100 + 50 * tmpi;
//		g_clubbuildupgrade[Club_drug][tmpi].other = 0;
//	}
//	// ���彨��ÿСʱά������
//	for ( tmpi = 0; tmpi <= MAX_CLUB_BUILDLEVEL; tmpi++ )
//	{
//		// �˷�
//		g_clubbuild_hourlymoney[Club_burse][tmpi] = (int)(900 * pow( 1.3, tmpi ));
//		// ס��
//		g_clubbuild_hourlymoney[Club_room][tmpi] = (int)(900 * pow( 1.3, tmpi ));
//		// ҩ��
//		g_clubbuild_hourlymoney[Club_drug][tmpi] = (int)(600 * pow( 1.3, tmpi ));
//	}
//	// ÿСʱ������Դ
//	for ( tmpi = 0; tmpi <= MAX_CLUB_BUILDLEVEL; tmpi++ )
//	{
//		g_club_hourly_makeres[tmpi] = 50 + tmpi * 25;
//	}
//	for ( tmpi = 0; tmpi <= MAX_CLUB_BUILDLEVEL; tmpi++ )
//	{
//		g_club_maxres[tmpi] = 50 + tmpi * 100;
//	}
//	// ���̽�������
//	g_club_business_award_least[0] = 0;
//	g_club_business_award_least[1] = 500;
//	g_club_business_award_least[2] = 1000;
//	for ( tmpi = 3; tmpi <= MAX_CLUB_BUILDLEVEL; tmpi++ )
//	{
//		g_club_business_award_least[tmpi] = 1000 + tmpi * 160;
//	}
//	// �о���
//	for ( tmpi = 0; tmpi <= MAX_CLUB_BUILDLEVEL; tmpi++ )
//	{
//		g_club_studypower[tmpi] = 2000 + tmpi * 500;
//	}
//	// ҩ��ˢҩ����
//	for ( tmpi = 0; tmpi <= MAX_CLUB_BUILDLEVEL; tmpi++ )
//	{
//		g_club_brush_drugcount[tmpi] = 10 + 5 * tmpi;
//	}
//
//	// �����ÿ������Ĳֿ�
//	//for ( tmpi = 1; tmpi < MAX_CLUB_COUNT; tmpi++ )
//	//{
//	//	club_storage_load( tmpi );
//	//}
//	// ���ݿ�������ҵ�Ϊ�����ߣ���ֹ�������쳣ʱδ��������
//	sprintf( szSQL, "update club_member set online=0" );
//	if ( mysql_query( myGame, szSQL ) )
//	{
//		printf( "Query failed (%s)\n", mysql_error( myGame ) );
//		write_gamelog( "%s", szSQL );
//		return -1;
//	}
//
//	return 0;
//}
//
///* �������м�����Ϣ */
//void club_save()
//{
//	int tmpi;
//	for ( tmpi = 0; tmpi < MAX_CLUB_COUNT; tmpi++ )
//	{
//		if ( g_club[tmpi].m_clubid > 0 )
//		{
//			club_update( tmpi );
//		}
//	}
//}
//
///* ���ɸ��� */
//int club_update( short club_index )
//{
//	char szSQL[2048];
//	char szDesc[512];
//	CLUB_CHECK_INDEX( club_index );
//
//	db_escape( g_club[club_index].m_intro, szDesc, CLUB_NOTIFY_SIZE*sizeof(char) );
//	sprintf( szSQL, "replace into club (clubid, clubname,createtime,creatername,leadername,level,money,boom,repute,intro,fdate,current_week,sunonline_count,"
//		"mononline_count,tuesonline_count,wedonline_count,thursonline_count,fridayonline_count,satonline_count,isbroken,broken_days,pay_scale,"
//		"m_isclub_upgrade,m_club_upgrade_exp,m_res_point,m_curbuild_develop,m_curbuild_exp,"
//		"m_build_level1,m_build_level2,m_build_level3,m_build_level4,m_build_level5) "
//		"values('%d','%s', '%d', '%s', '%s', '%d', '%d', '%d', '%d', '%s', '%d', '%d', '%d',"
//		"'%d', '%d', '%d', '%d', '%d', '%d', '%d', '%d', '%d',"
//		"'%d', '%d', '%d', '%d' ,'%d' ,"
//		"'%d' ,'%d' ,'%d' ,'%d' ,'%d') ",
//		g_club[club_index].m_clubid, g_club[club_index].m_clubname, g_club[club_index].m_createtime, g_club[club_index].m_creatername, g_club[club_index].m_leadername, g_club[club_index].m_level, g_club[club_index].m_money, g_club[club_index].m_boom, g_club[club_index].m_repute, szDesc,
//		g_club[club_index].m_fdate, g_club[club_index].m_current_week, 
//		g_club[club_index].m_week_online[0], 
//		g_club[club_index].m_week_online[1],
//		g_club[club_index].m_week_online[2], 
//		g_club[club_index].m_week_online[3], 
//		g_club[club_index].m_week_online[4],
//		g_club[club_index].m_week_online[5], 
//		g_club[club_index].m_week_online[6],
//		g_club[club_index].m_isbroken, g_club[club_index].m_broken_days, g_club[club_index].m_pay_scale,
//		g_club[club_index].m_isclub_upgrade,
//		g_club[club_index].m_club_upgrade_exp,
//		g_club[club_index].m_res_point,
//		g_club[club_index].m_curbuild_develop,
//		g_club[club_index].m_curbuild_exp,
//		g_club[club_index].m_build_level[1],
//		g_club[club_index].m_build_level[2],
//		g_club[club_index].m_build_level[3],
//		g_club[club_index].m_build_level[4],
//		g_club[club_index].m_build_level[5] );
//	if ( mysql_query( myGame, szSQL ) )
//	{
//		printf( "Query failed (%s)\n", mysql_error( myGame ) );
//		write_gamelog( "%s", szSQL );
//		return -1;
//	}
//	// �洢����ҩ��
//	//club_shop_update( club_index );
//	return 0;
//}
//
///* �������� */
//int club_create( int actor_index, char *clubname, char *intro )
//{
//	int tmpi;
//	short clubid;
//	short club_index = 0;
//
//	// �Ѿ��а��ɵĲ������ٽ���
//	if ( g_actors[actor_index].club.m_club_index > 0 && g_actors[actor_index].club.m_place > 0 )
//	{
//		actor_notify( actor_index, 0, -1, "���˳���ǰ���ɣ��ſɽ����µİ��ɣ�" );
//		return -1;
//	}
//
//	// �������ɳ�Ա
//	if ( g_actors[actor_index].club.m_club_index > 0 && g_actors[actor_index].club.m_place != 0 )
//	{
//		actor_notify( actor_index, 0, -1, "���˳���ǰ���ɣ��ſɽ����µİ��ɣ�" );
//		return -1;
//	}
//
//	if ( g_actors[actor_index].pattr[0]->level < 40 )
//	{
//		actor_notify( actor_index, 0, -1, "�ȼ�40���ſɴ�������" );
//		return -1;
//	}
//
//	// �������,����������������Ĳ�������
//	for ( tmpi = 1; tmpi < MAX_CLUB_COUNT; tmpi++ )
//	{
//		if ( g_club[tmpi].m_clubid == 0 )
//			continue;
//		if ( strcmp( g_club[tmpi].m_clubname, clubname ) == 0 )
//		{
//			actor_notify( actor_index, 0, -1, "�Ѿ�������ͬ���Ƶİ���" );
//			return 0;
//		}
//	}
//
//	for ( tmpi = 1; tmpi < MAX_CLUB_COUNT; tmpi++ )
//	{
//		if ( g_club[tmpi].m_clubid == 0 )
//		{
//			club_index = tmpi;
//			break;
//		}
//	}
//	if ( club_index == 0 )
//		return -1;
//
//	// �ʽ�����޷�����  ���治���ټ�������,��ΪǮ�Ѿ�����
//	if ( item_getmoney( actor_index, -g_club_create_money, PATH_SYSTEM ) < 0 )
//	{
//		actor_notify( actor_index, NOTIFY_NOMONEY, 0, "����#37������֧����" );
//		return -1;
//	}
//
//	clubid = g_max_clubid++;
//	g_club[club_index].m_clubid = clubid;
//	g_club[club_index].m_createtime = (int)time( NULL );
//	strcpy( g_club[club_index].m_clubname, clubname );
//	strcpy( g_club[club_index].m_creatername, g_actors[actor_index].pattr[0]->name );
//	strcpy( g_club[club_index].m_leadername, g_actors[actor_index].pattr[0]->name );
//	strcpy( g_club[club_index].m_intro, intro );
//	g_club[club_index].m_level = 1;
//	g_club[club_index].m_money = g_club_base_money;
//	g_club[club_index].m_boom = 1000;
//	g_club[club_index].m_repute = 1000;
//	g_club[club_index].m_membercount = 1;
//	g_club[club_index].m_fdate = 0;
//	g_club[club_index].m_pay_scale = 200;
//	g_club[club_index].m_mapid = map_create_copy( CLUB_MAPID );
//	g_club[club_index].m_online_member[0] = actor_index;
//	for ( tmpi = 1; tmpi < MAX_MEMBER_COUNT; tmpi++ )
//	{
//		g_club[club_index].m_online_member[tmpi] = -1;
//	}
//
//	g_club[club_index].m_isclub_upgrade = 0;			// �����Ƿ�������
//	g_club[club_index].m_club_upgrade_exp = 0;			// ����������ǰ�Ľ����
//	g_club[club_index].m_res_point = 0;					// �ʲ� 
//	g_club[club_index].m_build_level[0] = 0;
//	for ( tmpi = 1; tmpi < ClubBuildMax; tmpi++ )
//	{
//		g_club[club_index].m_build_level[tmpi] = 0;		// �����ȼ�
//	}
//	g_club[club_index].m_curbuild_develop = ClubBuildNormal; // ��ǰ��չ�Ľ���
//	g_club[club_index].m_curbuild_exp = 0;				     // ��ǰ��չ�����Ľ����
//
//	g_actors[actor_index].club.m_club_index = club_index;
//	g_actors[actor_index].club.m_place = CLUBPLACE_LEADER;
//	g_actors[actor_index].club.m_entertime = (int)time( NULL );
//
//	actor_notify( actor_index, 0, -1, "�����ɹ�" );
//	club_update( club_index );
//	club_actor_save( actor_index );
//	club_add_event( clubid, g_actors[actor_index].actorid, g_actors[actor_index].pattr[0]->name, 0, 0, CLUBEVENT_CREATE );
//	prefix_change( actor_index, 1, 8 );
//	wlog( 0, LOGOP_CLUBCREATE, PATH_SYSTEM, clubid, 0, 0, g_actors[actor_index].actorid, 0 );
//
//	int value[1] = { 0 };
//	value[0] = 5;
//	actor_notify_value( actor_index, NOTIFY_CLUB, 1, value, -1, NULL );
//	return 0;
//}
//
//// һ������Ľ�ɢ
//int club_delete( short club_index )
//{
//	MYSQL_RES		*res;
//	MYSQL_ROW		row;
//	char	szSQL[1024];
//	int memberid;
//	// ���������г�Ա����ɾ����Ϣ
//	sprintf( szSQL, "select actorid from club_member where clubid='%d'", g_club[club_index].m_clubid );
//	if ( mysql_query( myGame, szSQL ) )
//	{
//		printf( "Query failed (%s)\n", mysql_error( myGame ) );
//		write_gamelog( "%s", szSQL );
//		return -1;
//	}
//	res = mysql_store_result( myGame );
//	while ( (row = mysql_fetch_row( res )) )
//	{
//		memberid = atoi( row[0] );
//		mail_send( -157, memberid, "���ź������İ����Ѿ���ɢ��", 0, 0, 0, 0, 0, 0 );
//		int member_actorindex = actor_getindex_withid( memberid );
//		if ( member_actorindex >= 0 && member_actorindex < g_maxactornum )
//			prefix_delete( member_actorindex, 1 );
//		else
//			prefix_delete_db( memberid, 1 );
//	}
//	mysql_free_result( res );
//	// ɾ�������ĵ�ͼ
//	map_delete_copy( g_club[club_index].m_mapid );
//	// ɾ��������
//	sprintf( szSQL, "delete from club_asklist where clubid='%d'", g_club[club_index].m_clubid );
//	if ( mysql_query( myGame, szSQL ) )
//	{
//		printf( "Query failed (%s)\n", mysql_error( myGame ) );
//		write_gamelog( "%s", szSQL );
//		return -1;
//	}
//	// ɾ����Ա
//	sprintf( szSQL, "delete from club_member where clubid='%d'", g_club[club_index].m_clubid );
//	if ( mysql_query( myGame, szSQL ) )
//	{
//		printf( "Query failed (%s)\n", mysql_error( myGame ) );
//		write_gamelog( "%s", szSQL );
//		return -1;
//	}
//	// ɾ������
//	sprintf( szSQL, "delete from club where clubid='%d'", g_club[club_index].m_clubid );
//	if ( mysql_query( myGame, szSQL ) )
//	{
//		printf( "Query failed (%s)\n", mysql_error( myGame ) );
//		write_gamelog( "%s", szSQL );
//		return -1;
//	}
//	// ɾ�������¼�
//	sprintf( szSQL, "delete from club_event where clubid='%d'", g_club[club_index].m_clubid );
//	if ( mysql_query( myGame, szSQL ) )
//	{
//		printf( "Query failed (%s)\n", mysql_error( myGame ) );
//		write_gamelog( "%s", szSQL );
//		return -1;
//	}
//	// ���ɽ�ɢ����ô��Ҫ��ó���г�����Ʒ���������
//	sprintf( szSQL, "select actorid, itemkind from club_tradeshop where clubid=%d", g_club[club_index].m_clubid );
//	if ( mysql_query( myGame, szSQL ) )
//	{
//		printf( "Query failed (%s) [%s](%d)\n", mysql_error( myGame ), __FUNCTION__, __LINE__ );
//		write_gamelog( "%s", szSQL );
//		return -1;
//	}
//	res = mysql_store_result( myGame );
//	while ( (row = mysql_fetch_row( res )) )
//	{
//		int actorid = atoi( row[0] );
//		int itemkind = atoi( row[1] );
//
//		char szMsg[256] = { 0 };
//		sprintf( szMsg, "������������ó���г������Ʒ|%d|%d|", itemkind, 1 );
//		mail_send( -157, memberid, szMsg, 2, 0, 0, 5, 0, 0 );
//	}
//	mysql_free_result( res );
//
//	// ����Ʒȫ��ɾ��
//	sprintf( szSQL, "delete from club_tradeshop where clubid='%d'", g_club[club_index].m_clubid );
//	if ( mysql_query( myGame, szSQL ) )
//	{
//		printf( "Query failed (%s)\n", mysql_error( myGame ) );
//		write_gamelog( "%s", szSQL );
//		return -1;
//	}
//
//	wlog( 0, LOGOP_CLUBDELETE, PATH_SYSTEM, g_club[club_index].m_clubid, 0, 0, 0, 0 );
//	// �����ڴ�
//	memset( &g_club[club_index], 0, sizeof(Club) );
//	return 0;
//}
//
///* ��Ұ������ݶ�ȡ */
//int club_actor_load( int actor_index )
//{
//	char szSQL[512];
//	MYSQL_RES		*res;
//	MYSQL_ROW		row;
//	int club_index;
//	char place;
//	if ( actor_index < 0 || actor_index >= g_maxactornum )
//		return -1;
//
//	sprintf( szSQL, "select clubid, place, total_contribute, current_contribute, entertime, offlinetime from club_member where actorid=%d", g_actors[actor_index].actorid );
//	if ( mysql_query( myGame, szSQL ) )
//	{
//		printf( "Query failed (%s)\n", mysql_error( myGame ) );
//		write_gamelog( "%s", szSQL );
//		return -1;
//	}
//	res = mysql_store_result( myGame );
//	if ( (row = mysql_fetch_row( res )) )
//	{
//		club_index = club_getindex( atoi( row[0] ) );
//		if ( club_index <= 0 )
//		{
//			club_out( g_actors[actor_index].actorid, club_index );
//			mysql_free_result( res );
//			return -1;
//		}
//		place = atoi( row[1] );
//		g_actors[actor_index].club.m_total_contribute = atoi( row[2] );
//		g_actors[actor_index].club.m_current_contribute = atoi( row[3] );
//		g_actors[actor_index].club.m_entertime = atoi( row[4] );
//		g_actors[actor_index].club.m_offlinetime = atoi( row[5] );
//		if ( club_index != g_actors[actor_index].club.m_club_index || place != g_actors[actor_index].club.m_place )
//		{
//			g_actors[actor_index].club.m_club_index = club_index;
//			g_actors[actor_index].club.m_place = place;
//		}
//	}
//	else
//	{
//		g_actors[actor_index].club.m_club_index = 0;
//		g_actors[actor_index].club.m_place = 0;
//		g_actors[actor_index].club.m_total_contribute = 0;
//		g_actors[actor_index].club.m_current_contribute = 0;
//		g_actors[actor_index].club.m_entertime = 0;
//		quest_set( actor_index, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 );			// �����������
//		//quest_set( actor_index, QUESTINDEX_WORK, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 );			// ���彨���������
//		//quest_set( actor_index, QUESTINDEX_BUSINESSMAN, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 );  // �����������
//		//item_lost( actor_index, CLUB_BUSINESS_ITEMKIND, 1 ); // ֧ƱҲ�ջ�
//	}
//	mysql_free_result( res );
//	if ( g_actors[actor_index].club.m_place > CLUBPLACE_MEMBER )
//	{
//		if ( club_is_stop( g_actors[actor_index].club.m_club_index ) )
//		{
//			char szMsg[256] = { 0 };
//			sprintf( szMsg, "�����ʽ��ѵ���ͷ�Χ��" );
//			actor_notify( actor_index, 0, (short)strlen( szMsg ) + 1, szMsg );
//		}
//	}
//	// ֪ͨ��������
//	club_online( actor_index, 1 );
//	return 0;
//}
//
///* ��Ұ������ݱ��� */
//int club_actor_save( int actor_index )
//{
//	char szSQL[512];
//	if ( actor_index < 0 || actor_index >= g_maxactornum )
//		return -1;
//	int club_index = g_actors[actor_index].club.m_club_index;
//	CLUB_CHECK_INDEX( club_index );
//
//	sprintf( szSQL, "replace into club_member ( actorid,clubid,name,level,aclass,place,total_contribute,current_contribute,online,entertime,offlinetime ) "
//		"values('%d','%d','%s','%d','%d','%d','%d','%d','%d','%d','%d')",
//		g_actors[actor_index].actorid, g_club[club_index].m_clubid, g_actors[actor_index].pattr[0]->name, g_actors[actor_index].pattr[0]->level, g_actors[actor_index].aclass,
//		g_actors[actor_index].club.m_place, g_actors[actor_index].club.m_total_contribute, g_actors[actor_index].club.m_current_contribute,
//		g_actors[actor_index].club.m_online, g_actors[actor_index].club.m_entertime, g_actors[actor_index].club.m_offlinetime );
//	if ( mysql_query( myGame, szSQL ) )
//	{
//		printf( "Query failed (%s)\n", mysql_error( myGame ) );
//		write_gamelog( "%s", szSQL );
//		return -1;
//	}
//	return 0;
//}
//
///* �������� */
//short club_getindex( short clubid )
//{
//	int tmpi;
//	if ( clubid <= 0 )
//		return 0;
//	for ( tmpi = 1; tmpi < MAX_CLUB_COUNT; tmpi++ )
//	{
//		if ( g_club[tmpi].m_clubid == clubid )
//		{
//			return tmpi;
//		}
//	}
//	return 0;
//}
//
///* ����ID */
//short club_getid( short club_index )
//{
//	if ( club_index <= 0 || club_index >= MAX_CLUB_COUNT )
//		return 0;
//	return g_club[club_index].m_clubid;
//}
//
///* �������� */
//char *club_getname( short club_index )
//{
//	if ( club_index <= 0 || club_index >= MAX_CLUB_COUNT )
//		return NULL;
//	return g_club[club_index].m_clubname;
//}
//
///* ����ְλ���� */
//char *club_getplacename( char place )
//{
//	if ( place == CLUBPLACE_LEADER )	// ����
//		return "����";
//	else if ( place == CLUBPLACE_FLEADER )	// ������
//		return "������";
//	else if ( place == CLUBPLACE_CAPTAIN4 )	// ��������
//		return "��������";
//	else if ( place == CLUBPLACE_CAPTAIN3 )	// ��ȸ����
//		return "��ȸ����";
//	else if ( place == CLUBPLACE_CAPTAIN2 )	// �׻�����
//		return "�׻�����";
//	else if ( place == CLUBPLACE_CAPTAIN1 )	// ��������
//		return "��������";
//	else if ( place == CLUBPLACE_MEMBER )	// ����
//		return "����";
//	else if ( place == CLUBPLACE_LOW )	// ѧͽ
//		return "ѧͽ";
//	return "";
//}
//
///* ���ɽ������� */
//char *club_getbuildname( char buildid )
//{
//	if ( buildid == 0 )
//		return "����";
//	else if ( buildid == Club_burse )
//		return "�˷�";
//	else if ( buildid == Club_drug )
//		return "�̵�";
//	else if ( buildid == Club_room )
//		return "ס��";
//	return "";
//}
//
///* ���ɵ�ͼ */
//int club_getmapid( int actor_index )
//{
//	short club_index;
//	if ( actor_index < 0 || actor_index >= g_maxactornum )
//		return -1;
//	club_index = g_actors[actor_index].club.m_club_index;
//	CLUB_CHECK_INDEX( club_index );
//	return g_club[club_index].m_mapid;
//
//}
//
///* ������Ϣ */
//int club_baseinfo( int actor_index )
//{
//	if ( actor_index < 0 || actor_index >= g_maxactornum )
//		return -1;
//	int club_index = g_actors[actor_index].club.m_club_index;
//	CLUB_CHECK_INDEX( club_index );
//
//	SLK_NetS_ClubBaseInfo pValue = { 0 };
//	// ������Ϣ����
//	pValue.m_clubid = g_club[club_index].m_clubid;
//	pValue.m_clublevel = g_club[club_index].m_level;
//	pValue.m_membercount = g_club[club_index].m_membercount;
//	pValue.m_maxmember = club_get_member_max( club_index );
//	pValue.m_intro_length = strlen( g_club[club_index].m_intro );
//	memcpy( pValue.m_clubname, g_club[club_index].m_clubname, NAME_SIZE*sizeof(char) );
//	memcpy( pValue.m_leadername, g_club[club_index].m_leadername, NAME_SIZE*sizeof(char) );
//	memcpy( pValue.m_intro, g_club[club_index].m_intro, pValue.m_intro_length*sizeof(char) );
//	// ������Ϣ����
//	pValue.m_build_level[0] = g_club[club_index].m_level;
//	pValue.m_build_level[Club_burse] = g_club[club_index].m_build_level[Club_burse];
//	pValue.m_build_level[Club_drug] = g_club[club_index].m_build_level[Club_drug];
//	pValue.m_build_level[Club_room] = g_club[club_index].m_build_level[Club_room];
//	pValue.m_needmoney[0] = g_clubupgrade[g_club[club_index].m_level].need_money;
//	pValue.m_needmoney[Club_burse] = club_get_build_upgrademoney( club_index, Club_burse );
//	pValue.m_needmoney[Club_drug] = club_get_build_upgrademoney( club_index, Club_drug );
//	pValue.m_needmoney[Club_room] = club_get_build_upgrademoney( club_index, Club_room );
//	pValue.m_money = g_club[club_index].m_money;
//	pValue.m_max_money = club_get_money_most( club_index );
//	pValue.m_mainte_money = abs(club_get_hourly_mainte_money( club_index ));
//	pValue.m_least_money = club_get_money_least( club_index );
//	pValue.m_share_money = 0;
//
//	netsend_clubbaseinfo_S( actor_index, SENDTYPE_ACTOR, &pValue );
//	return 0;
//}
//
///* �������߳�Ա�б� */
//int club_onlinememberlist( int actor_index )
//{
//	if ( actor_index < 0 || actor_index >= g_maxactornum )
//		return -1;
//	int club_index = g_actors[actor_index].club.m_club_index;
//	CLUB_CHECK_INDEX( club_index );
//	SLK_NetS_ClubOnlineMember pValue = { 0 };
//	// ���߳�Ա
//	for ( int tmpi = 0; tmpi < MAX_MEMBER_COUNT; tmpi++ )
//	{
//		int online_member = g_club[club_index].m_online_member[tmpi];
//		if ( online_member >= 0 && online_member < g_maxactornum )
//		{
//			memcpy( pValue.m_onlinemember[pValue.m_count].m_name, g_actors[online_member].pattr[0]->name, NAME_SIZE*sizeof(char) );
//			pValue.m_onlinemember[pValue.m_count].m_level = g_actors[online_member].pattr[0]->level;
//			pValue.m_onlinemember[pValue.m_count].m_place = g_actors[online_member].club.m_place;
//			pValue.m_onlinemember[pValue.m_count].m_actorid = g_actors[online_member].actorid;
//			pValue.m_onlinemember[pValue.m_count].m_aclass = g_actors[online_member].aclass;
//			pValue.m_count++;
//			if ( pValue.m_count % 10 == 0 )
//			{ // ÿ10����
//				netsend_clubonlinemember_S( actor_index, SENDTYPE_ACTOR, &pValue );
//				pValue.m_count = 0;
//			}
//		}
//	}
//	if ( pValue.m_count > 0 )
//	{
//		netsend_clubonlinemember_S( actor_index, SENDTYPE_ACTOR, &pValue );
//	}
//
//	return 0;
//}
//
///* �����б� */
//int club_list( int actor_index, short page )
//{
//	short count = 0;
//	SLK_NetS_ClubList pValue = { 0 };
//	for ( int tmpi = 0; tmpi < MAX_CLUB_COUNT; tmpi++ )
//	{
//		if ( g_club[tmpi].m_clubid > 0 )
//		{
//			pValue.m_clubinfo[pValue.m_count].m_club_index = tmpi;
//			pValue.m_clubinfo[pValue.m_count].m_clubid = g_club[tmpi].m_clubid;
//			pValue.m_clubinfo[pValue.m_count].m_clublevel = g_club[tmpi].m_level;
//			pValue.m_clubinfo[pValue.m_count].m_membercount = g_club[tmpi].m_membercount;
//			memcpy( pValue.m_clubinfo[pValue.m_count].m_clubname, g_club[tmpi].m_clubname, NAME_SIZE*sizeof(char) );
//			memcpy( pValue.m_clubinfo[pValue.m_count].m_leadername, g_club[tmpi].m_leadername, NAME_SIZE*sizeof(char) );
//			pValue.m_count++;
//			if ( pValue.m_count % 10 == 0 )
//			{ // ÿ10����
//				netsend_clublist_S( actor_index, SENDTYPE_ACTOR, &pValue );
//				pValue.m_count = 0;
//			}
//		}
//	}
//	if ( pValue.m_count > 0 )
//	{
//		netsend_clublist_S( actor_index, SENDTYPE_ACTOR, &pValue );
//	}
//	return 0;
//}
//
///* ���ɳ�Ա�б� */
//int club_memberlist( int actor_index, short page )
//{
//	MYSQL_RES		*res;
//	MYSQL_ROW		row;
//	char	szSQL[1024];
//	if ( actor_index < 0 || actor_index >= g_maxactornum )
//		return -1;
//	int club_index = g_actors[actor_index].club.m_club_index;
//	CLUB_CHECK_INDEX( club_index );
//	SLK_NetS_ClubMember pValue = { 0 };
//	sprintf( szSQL, "select * from club_member where clubid='%d'", g_club[club_index].m_clubid );
//	if ( mysql_query( myGame, szSQL ) )
//	{
//		printf( "Query failed (%s)\n", mysql_error( myGame ) );
//		write_gamelog( "%s", szSQL );
//		return -1;
//	}
//	res = mysql_store_result( myGame );
//	while ( (row = mysql_fetch_row( res )) )
//	{
//		pValue.m_memberinfo[pValue.m_count].m_actorid = atoi( row[0] );
//		memcpy( pValue.m_memberinfo[pValue.m_count].m_name, row[2], NAME_SIZE*sizeof(char) );
//		pValue.m_memberinfo[pValue.m_count].m_level = atoi( row[3] );
//		pValue.m_memberinfo[pValue.m_count].m_aclass = atoi( row[4] );
//		pValue.m_memberinfo[pValue.m_count].m_place = atoi( row[5] );
//		pValue.m_memberinfo[pValue.m_count].m_total_contribute = atoi( row[6] );
//		pValue.m_memberinfo[pValue.m_count].m_current_contribute = atoi( row[8] );
//		pValue.m_memberinfo[pValue.m_count].m_entertime = atoi( row[10] );
//		pValue.m_memberinfo[pValue.m_count].m_offlinetime = atoi( row[11] );
//		pValue.m_count++;
//		if ( pValue.m_count % 20 == 0 )
//		{ // ÿ20����
//			netsend_clubmemberlist_S( actor_index, SENDTYPE_ACTOR, &pValue );
//			pValue.m_count = 0;
//		}
//	}
//	if ( pValue.m_count > 0 )
//	{
//		netsend_clubmemberlist_S( actor_index, SENDTYPE_ACTOR, &pValue );
//	}
//	return 0;
//}
//
///* ���������б� */
//int club_askerlist( int actor_index, short clubid )
//{
//	MYSQL_RES		*res;
//	MYSQL_ROW		row;
//	char	szSQL[1024];
//	if ( actor_index < 0 || actor_index >= g_maxactornum )
//		return -1;
//	int club_index = g_actors[actor_index].club.m_club_index;
//	CLUB_CHECK_INDEX( club_index );
//	SLK_NetS_ClubAskMember pValue = { 0 };
//	sprintf( szSQL, "select * from club_asklist where clubid='%d'", g_club[club_index].m_clubid );
//	if ( mysql_query( myGame, szSQL ) )
//	{
//		printf( "Query failed (%s)\n", mysql_error( myGame ) );
//		write_gamelog( "%s", szSQL );
//		return -1;
//	}
//	res = mysql_store_result( myGame );
//	while ( (row = mysql_fetch_row( res )) )
//	{
//		pValue.m_askmember[pValue.m_count].m_actorid = atoi( row[1] );
//		memcpy( pValue.m_askmember[pValue.m_count].m_name, row[2], NAME_SIZE*sizeof(char) );
//		pValue.m_askmember[pValue.m_count].m_level = atoi( row[3] );
//		pValue.m_askmember[pValue.m_count].m_aclass = atoi( row[4] );
//		pValue.m_askmember[pValue.m_count].m_place = 0;
//		pValue.m_count++;
//		if ( pValue.m_count % 20 == 0 )
//		{ // ÿ20����
//			netsend_clubaskmember_S( actor_index, SENDTYPE_ACTOR, &pValue );
//			pValue.m_count = 0;
//		}
//	}
//	if ( pValue.m_count > 0 )
//	{
//		netsend_clubaskmember_S( actor_index, SENDTYPE_ACTOR, &pValue );
//	}
//	return 0;
//}
//
///* ��հ��������б� */
//int club_clear_asklist( int actor_index )
//{
//	char szSQL[1024];
//	ACTOR_CHECK_INDEX( actor_index );
//	short club_index = g_actors[actor_index].club.m_club_index;
//	CLUB_CHECK_INDEX( club_index );
//	if ( g_actors[actor_index].club.m_place < CLUBPLACE_CAPTAIN1 )
//	{
//		actor_notify( actor_index, 0, -1, "���Ȩ�޲���" );
//		return -1;
//	}
//	sprintf( szSQL, "delete from club_asklist where clubid='%d'", g_club[club_index].m_clubid );
//	if ( mysql_query( myGame, szSQL ) )
//	{
//		printf( "Query failed (%s)\n", mysql_error( myGame ) );
//		write_gamelog( "%s", szSQL );
//		return -1;
//	}
//	int value[1] = { 0 };
//	value[0] = 2;
//	actor_notify_value( actor_index, NOTIFY_CLUB, 1, value, -1, NULL );
//	return 0;
//}
//
//
///* �����¼��б� */
//int club_eventlist( int actor_index, short pageno )
//{
//	MYSQL_RES *res;
//	MYSQL_ROW row;
//	char szSQL[1024];
//	ACTOR_CHECK_INDEX( actor_index );
//	int club_index = g_actors[actor_index].club.m_club_index;
//	CLUB_CHECK_INDEX( club_index );
//	SLK_NetS_ClubEvent pValue = { 0 };
//	sprintf( szSQL, "select event, name, othername, otherid, happentime from club_event where clubid='%d' order by happentime desc", g_club[club_index].m_clubid );
//	if ( mysql_query( myGame, szSQL ) )
//	{
//		printf( "Query failed (%s)\n", mysql_error( myGame ) );
//		write_gamelog( "%s", szSQL );
//		return -1;
//	}
//	res = mysql_store_result( myGame );
//	while ( (row = mysql_fetch_row( res )) )
//	{
//		pValue.m_clubevent[pValue.m_count].m_eventtype = atoi( row[0] );
//		memcpy( pValue.m_clubevent[pValue.m_count].m_actorname, row[1], NAME_SIZE*sizeof(char) );
//		memcpy( pValue.m_clubevent[pValue.m_count].m_targetname, row[2], NAME_SIZE*sizeof(char) );
//		pValue.m_clubevent[pValue.m_count].m_targetid = atoi( row[3] );
//		pValue.m_clubevent[pValue.m_count].m_happentime = atoi( row[4] );
//		pValue.m_count++;
//		if ( pValue.m_count % 20 == 0 )
//		{ // ÿ20����
//			netsend_clubeventlist_S( actor_index, SENDTYPE_ACTOR, &pValue );
//			pValue.m_count = 0;
//		}
//	}
//	if ( pValue.m_count > 0 )
//	{
//		netsend_clubeventlist_S( actor_index, SENDTYPE_ACTOR, &pValue );
//	}
//	return 0;
//}
//
///* ��������¼� */
//int club_add_event( short clubid, int actorid, char *name, int otherid, char *othername, char event )
//{
//	char szName[64];
//	MYSQL_RES		*res;
//	MYSQL_ROW		row;
//	char szOtherName[64];
//	char szSQL[1024];
//	char szNull[1];
//	int happentime = (int)time( 0 );
//	int count;
//	int oldtime;
//	szNull[0] = 0;
//	if ( name )
//		db_escape( (const char *)name, szName, 0 );
//	else
//		db_escape( (const char *)szNull, szName, 0 );
//
//	if ( othername )
//		db_escape( (const char *)othername, szOtherName, 0 );
//	else
//		db_escape( (const char *)szNull, szOtherName, 0 );
//	sprintf( szSQL, "select count(*) from club_event where clubid='%d'", clubid );
//	if ( mysql_query( myGame, szSQL ) )
//	{
//		printf( "Query failed (%s)\n", mysql_error( myGame ) );
//		write_gamelog( "%s", szSQL );
//		return -1;
//	}
//	res = mysql_store_result( myGame );
//	if ( (row = mysql_fetch_row( res )) )
//	{
//		count = atoi( row[0] );
//	}
//	mysql_free_result( res );
//	if ( count >= CLUBEVENT_MAX_COUNT )
//	{
//		sprintf( szSQL, "select min(happentime) from club_event where clubid='%d'", clubid );
//		if ( mysql_query( myGame, szSQL ) )
//		{
//			printf( "Query failed (%s)\n", mysql_error( myGame ) );
//			write_gamelog( "%s", szSQL );
//			return -1;
//		}
//		res = mysql_store_result( myGame );
//		if ( (row = mysql_fetch_row( res )) )
//		{
//			oldtime = atoi( row[0] );
//		}
//		mysql_free_result( res );
//
//		sprintf( szSQL, "delete from club_event where happentime='%d' and clubid='%d'", oldtime, clubid );
//		if ( mysql_query( myGame, szSQL ) )
//		{
//			printf( "Query failed (%s)\n", mysql_error( myGame ) );
//			write_gamelog( "%s", szSQL );
//			return -1;
//		}
//	}
//	sprintf( szSQL, "insert into club_event ( clubid,actorid,name,otherid,othername,event,happentime) values('%d','%d','%s','%d','%s','%d','%d')", \
//		clubid, actorid, szName, otherid, szOtherName, event, happentime );
//	if ( mysql_query( myGame, szSQL ) )
//	{
//		printf( "Query failed (%s)\n", mysql_error( myGame ) );
//		write_gamelog( "%s", szSQL );
//		return -1;
//	}
//	return 0;
//}
//
///* ���ɷ�����Ϣ */
//int club_sendmsg( int club_index, int datasize, char *databuf )
//{
//	int tmpi;
//	int tmpindex;
//	if ( datasize > 0 )
//	{
//		for ( tmpi = 0; tmpi < MAX_MEMBER_COUNT; tmpi++ )
//		{
//			tmpindex = g_club[club_index].m_online_member[tmpi];
//			if ( tmpindex < 0 )
//				continue;
//			if ( g_actors[tmpindex].club.m_club_index != club_index )
//			{
//				g_club[club_index].m_online_member[tmpi] = -1;
//				continue;
//			}
//			sendtoclient( tmpi, databuf, datasize + sizeof(short) );
//		}
//	}
//	return 0;
//}
//
//// ��ɫ����,���Ҹ������ݿ���ְλ�����ߵ����
//int club_online( int actor_index, char online )
//{
//	int tmpi;
//	int tmpindex;
//	int empty_pos = -1;
//	int find_pos = -1;
//	if ( actor_index < 0 || actor_index >= g_maxactornum )
//		return -1;
//	short club_index = g_actors[actor_index].club.m_club_index;
//	CLUB_CHECK_INDEX( club_index );
//	if ( g_actors[actor_index].club.m_place != 0 )
//	{
//		for ( tmpi = 0; tmpi < MAX_MEMBER_COUNT; tmpi++ )
//		{
//			tmpindex = g_club[club_index].m_online_member[tmpi];
//			if ( tmpindex < 0 )
//			{
//				if ( empty_pos < 0 )
//					empty_pos = tmpi;
//				continue;
//			}
//			if ( tmpindex == actor_index )
//			{
//				find_pos = tmpi;
//				break;
//			}
//		}
//		if ( online )
//		{
//			time_t now;
//			struct tm *nowp;
//			extern int g_tick;
//			time( &now );
//			nowp = localtime( &now );
//			if ( find_pos < 0 && empty_pos >= 0 )
//			{
//				g_club[club_index].m_online_member[empty_pos] = actor_index;
//				g_club[club_index].m_online_count++;
//				if ( !club_today_loginactor( club_index, actor_index ) )
//				{
//					g_club[club_index].m_week_online[nowp->tm_wday] += 1;
//				}
//			}
//		}
//		else
//		{
//			if ( find_pos >= 0 )
//			{
//				g_club[club_index].m_online_member[find_pos] = -1;
//				g_club[club_index].m_online_count--;
//				if ( g_club[club_index].m_online_count < 0 )
//					g_club[club_index].m_online_count = 0;
//			}
//		}
//	}
//
//	if ( online == 0 )
//	{// ����
//		g_actors[actor_index].club.m_online = 0;
//		g_actors[actor_index].club.m_offlinetime = (int)time( NULL );
//		club_actor_save( actor_index );
//	}
//	else if ( online == 1 )
//	{// ����
//		g_actors[actor_index].club.m_online = 1;
//		g_actors[actor_index].club.m_offlinetime = 0;
//		club_actor_save( actor_index );
//	}
//	else if ( online == 2 )
//	{ // �¼���
//		g_actors[actor_index].club.m_online = 1;
//		g_actors[actor_index].club.m_offlinetime = 0;
//		g_actors[actor_index].club.m_entertime = (int)time( NULL );
//		club_actor_save( actor_index );
//	}
//	
//	if ( g_actors[actor_index].club.m_place == CLUBPLACE_LEADER )
//	{ // ����
//		char szMsg[256] = { 0 };
//		if ( online == 0 )
//		{ // ����
//			if ( g_actors[actor_index].aclass == GENDER_MALE )
//			{
//				sprintf( szMsg, "����ΰ��ġ�Ӣ�����䡢������桢ò���˰����˼��˰��������������������صİ�����%s��žž�����ˣ���", g_actors[actor_index].pattr[0]->name );
//			}
//			else if ( g_actors[actor_index].aclass == GENDER_FEMALE )
//			{
//				sprintf( szMsg, "�����������ʡ���̬��ǧ����ׯ���š���Ư�����޿ɰ�������������˶�������%s��������ӯ��ɺɺ���졢�������ֵ����ˣ���", g_actors[actor_index].pattr[0]->name );
//			}
//		}
//		else if ( online == 1 )
//		{ // ����
//			if ( g_actors[actor_index].aclass == GENDER_MALE )
//			{
//				sprintf( szMsg, "����ΰ��ġ�Ӣ�����䡢������桢ò���˰����˼��˰��������������������صİ�����%s��žž�������ˣ���ҹ��ƻ�������", g_actors[actor_index].pattr[0]->name );
//			}
//			else if ( g_actors[actor_index].aclass == GENDER_FEMALE )
//			{
//				sprintf( szMsg, "�����������ʡ���̬��ǧ����ׯ���š���Ư�����޿ɰ�������������˶�������%s��̤���貨΢���������������Ȼ�������жӻ�ӭ��", g_actors[actor_index].pattr[0]->name );
//			}
//		}
//		if ( szMsg[0] != 0 )
//		{
//			actors_talk( actor_index, CHANNEL_TYPE_ORG, szMsg, (int)strlen( szMsg ), 0 );
//		}
//	}
//	return 0;
//}
//
//int club_shop_update( short club_index )
//{
//	//char szSQL[2048];
//
//	//CLUB_CHECK_INDEX( club_index );
//
//	//sprintf( szSQL, "REPLACE INTO club_shop (clubid,count0,count1,count2,count3,count4,count5,count6,count7,count8) "
//	//	" VALUES (%d, %d, %d, %d, %d, %d, %d, %d, %d, %d)",
//	//	g_club[club_index].m_clubid,
//	//	g_club[club_index].m_drugcount[0],
//	//	g_club[club_index].m_drugcount[1],
//	//	g_club[club_index].m_drugcount[2],
//	//	g_club[club_index].m_drugcount[3],
//	//	g_club[club_index].m_drugcount[4],
//	//	g_club[club_index].m_drugcount[5],
//	//	g_club[club_index].m_drugcount[6],
//	//	g_club[club_index].m_drugcount[7],
//	//	g_club[club_index].m_drugcount[8] );
//	//if ( mysql_query( myGame, szSQL ) )
//	//{
//	//	printf( "Query failed (%s)\n", mysql_error( myGame ) );
//	//	write_gamelog( "%s", szSQL );
//	//	return -1;
//	//}
//	return 0;
//}
//
///* һ������ */
//int club_allask( int actor_index )
//{
//	ACTOR_CHECK_INDEX( actor_index );
//	if ( g_actors[actor_index].club.m_club_index > 0 )
//		return -1;
//	if ( g_actors[actor_index].pattr[0]->level < 20 )
//	{
//		actor_notify( actor_index, 0, -1, "��ĵȼ�����20����" );
//		return -1;
//	}
//	for ( int tmpi = 0; tmpi < MAX_CLUB_COUNT; tmpi++ )
//	{
//		if ( g_club[tmpi].m_clubid > 0 )
//		{
//			if ( g_club[tmpi].m_membercount < club_get_member_max( tmpi ) )
//			{ // û��Ա��һ��������
//				club_ask( actor_index, g_club[tmpi].m_clubid, tmpi );
//			}
//		}
//	}
//	return 0;
//}
//
///* ������� */
//int club_ask( int actor_index, short clubid, short clubindex )
//{
//	char szSQL[2048];
//	int nowtick = 0;
//	short club_index;
//	ACTOR_CHECK_INDEX( actor_index );
//	if ( g_actors[actor_index].club.m_club_index > 0 )
//		return -1;
//
//	if ( g_actors[actor_index].pattr[0]->level < 20 )
//	{
//		actor_notify( actor_index, 0, -1, "��ĵȼ�����20����" );
//		return -1;
//	}
//
//	if ( clubindex > 0  )
//	{
//		club_index = clubindex;
//	}
//	else
//	{
//		club_index = club_getindex( clubid );
//	}
//	CLUB_CHECK_INDEX( club_index );
//	//nowtick = (int)time( NULL );
//	//if ( g_actors[actor_index].club_leavetime + 259200 > nowtick )
//	//{
//	//	sprintf( szMsg, "%dСʱ��ſ��ٴμ������", (g_actors[actor_index].club_leavetime + 259200 - nowtick) / 3600 );
//	//	actor_notify( actor_index, 0, (short)strlen( szMsg ), szMsg );
//	//	return;
//	//}
//	sprintf( szSQL, "REPLACE INTO club_asklist (clubid,actorid,name,level,aclass,optime) VALUES (%d, %d, '%s', %d, %d, %d)",
//		g_club[club_index].m_clubid, g_actors[actor_index].actorid,
//		g_actors[actor_index].pattr[0]->name, g_actors[actor_index].pattr[0]->level,
//		g_actors[actor_index].aclass, (int)time(NULL));
//	if ( mysql_query( myGame, szSQL ) )
//	{
//		printf( "Query failed (%s)\n", mysql_error( myGame ) );
//		write_gamelog( "%s", szSQL );
//		return -1;
//	}
//	return 0;
//}
//
///*  �ж�askerid�ĺϷ���,Ҳ���Ѿ����������б��� */
//int club_check_asker( short club_index, int askerid, ClubMember *memberinfo )
//{
//	MYSQL_RES		*res;
//	MYSQL_ROW		row;
//	char	szSQL[256];
//	sprintf( szSQL, "select * from club_asklist where actorid='%d' and clubid='%d'", askerid, g_club[club_index].m_clubid );
//	if ( mysql_query( myGame, szSQL ) )
//	{
//		printf( "Query failed (%s)\n", mysql_error( myGame ) );
//		write_gamelog( "%s", szSQL );
//		return -1;
//	}
//	res = mysql_store_result( myGame );
//	if ( (row = mysql_fetch_row( res )) )
//	{
//		memberinfo->m_actorid = atoi(row[1]);
//		memcpy( memberinfo->m_actorname, row[2], sizeof(char)*NAME_SIZE );
//		memberinfo->m_level = atoi( row[3] );
//		memberinfo->m_aclass = atoi( row[4] );
//		mysql_free_result( res );
//	}
//	else
//	{
//		mysql_free_result( res );
//		return -1;
//	}
//	return 0;
//}
//
///* ���� */
//int club_allow( int actor_index, int askerid )
//{
//	char szSQL[2048];
//	ACTOR_CHECK_INDEX( actor_index );
//	short club_index = g_actors[actor_index].club.m_club_index;
//	CLUB_CHECK_INDEX( club_index );
//	int clubid = g_club[club_index].m_clubid;
//	if ( g_actors[actor_index].club.m_place < CLUBPLACE_CAPTAIN1 )
//	{
//		actor_notify( actor_index, 0, -1, "��û�����Ȩ��" );
//		return -1;
//	}
//
//	// ����̫��,�Ӳ�����
//	if ( g_club[club_index].m_membercount >= club_get_member_max( club_index ) )
//	{
//		actor_notify( actor_index, 0, -1, "�Ѿ�������ǰ�����������" );
//		return -1;
//	}
//
//	// �ж�askerid�ĺϷ���,Ҳ���Ѿ����������б���
//	ClubMember memberinfo = { 0 };
//	if ( club_check_asker( club_index, askerid, &memberinfo ) < 0 )
//	{
//		actor_notify( actor_index, 0, -1, "���������Ѿ������������ɻ򳬹�ʱ��" );
//		return -1;
//	}
//
//	int askindex = actor_getindex_withid( askerid );
//	if ( askindex >= 0 )
//	{// ����������
//		club_in( askindex, clubid );
//		memberinfo.m_level = g_actors[askindex].pattr[0]->level;
//		memberinfo.m_online = 1;
//		memberinfo.m_offlinetime = 0;
//		prefix_change( askindex, 1, 2 );
//		actor_setprefix( askindex );
//	}
//	else
//	{
//		memberinfo.m_online = 0;
//		memberinfo.m_offlinetime = (int)time( NULL );
//		prefix_change_db( askerid, 1, 2 );
//	}
//	
//
//	sprintf( szSQL, "replace into club_member ( actorid,clubid,name,level,aclass,place,total_contribute,current_contribute,online,entertime,offlinetime ) "
//		"values(%d,%d,'%s',%d,%d,%d,%d,%d,%d,%d,%d)",
//		askerid, clubid, memberinfo.m_actorname, memberinfo.m_level, memberinfo.m_aclass, CLUBPLACE_MEMBER, 0, 0, memberinfo.m_online, (int)time( NULL ), memberinfo.m_offlinetime );
//	if ( mysql_query( myGame, szSQL ) )
//	{
//		printf( "Query failed (%s)\n", mysql_error( myGame ) );
//		write_gamelog( "%s", szSQL );
//		return -1;
//	}
//
//	// ���
//	sprintf( szSQL, "DELETE FROM club_asklist WHERE actorid=%d", askerid );
//	if ( mysql_query( myGame, szSQL ) )
//	{
//		printf( "Query failed (%s)\n", mysql_error( myGame ) );
//		write_gamelog( "%s", szSQL );
//		return -1;
//	}
//	// �¼�
//	club_add_event( clubid, memberinfo.m_actorid, memberinfo.m_actorname, 0, 0, CLUBEVENT_NEWMEMBER );
//	return 0;
//}
//
///* �ܾ� */
//int club_reject( int actor_index, int askerid )
//{
//	short club_index = g_actors[actor_index].club.m_club_index;
//	CLUB_CHECK_INDEX( club_index );
//	int clubid = g_club[club_index].m_clubid;
//	if ( g_actors[actor_index].club.m_place < CLUBPLACE_CAPTAIN1 )
//	{
//		actor_notify( actor_index, 0, -1, "��û�����Ȩ��" );
//		return -1;
//	}
//	char szSQL[2048];
//	sprintf( szSQL, "DELETE FROM club_asklist WHERE clubid=%d AND actorid=%d", clubid, askerid );
//	if ( mysql_query( myGame, szSQL ) )
//	{
//		printf( "Query failed (%s)\n", mysql_error( myGame ) );
//		write_gamelog( "%s", szSQL );
//		return -1;
//	}
//	return 0;
//}
//
///* ���������б��飬ÿСʱ���һ�Σ�ɾ������������Ϣ */
//int club_asklist_logic()
//{
//	char szSQL[2048];
//	sprintf( szSQL, "DELETE FROM club_asklist WHERE optime < %d", (int)time(NULL)-3600 );
//	if ( mysql_query( myGame, szSQL ) )
//	{
//		printf( "Query failed (%s)\n", mysql_error( myGame ) );
//		write_gamelog( "%s", szSQL );
//		return -1;
//	}
//	return 0;
//}
//
///* ���� */
//int club_in( int actor_index, short clubid )
//{
//	//char szMsg[256];
//	ACTOR_CHECK_INDEX( actor_index );
//	short club_index = club_getindex( clubid );
//	CLUB_CHECK_INDEX( club_index );
//	g_club[club_index].m_asker_count--;
//	g_club[club_index].m_membercount++;
//	g_actors[actor_index].club.m_club_index = club_index;
//	g_actors[actor_index].club.m_place = CLUBPLACE_MEMBER;
//	g_actors[actor_index].club.m_current_contribute = 0;
//	g_actors[actor_index].club.m_total_contribute = 0;
//	g_actors[actor_index].club.m_entertime = (int)time( NULL );
//	g_actors[actor_index].club.m_offlinetime = 0;
//	club_online( actor_index, 2 );
//	//actor_notify( actor_index, NOTIFY_CLUBCHANGE, (int)strlen( g_club[club_index].m_clubname ), g_club[club_index].m_clubname );
//	//g_actors[actor_index].open_club_frame = 0;
//	//sprintf( szMsg, "%s�����˼��壬�����ǻ�ӭ�³�Ա��", g_actors[actor_index].pattr[0]->name );
//	//szLen = (int)(strlen( szMsg ) + 1);
//	//wlog( 0, LOGOP_ENTERCLUB, PATH_SYSTEM, g_club[club_index].m_clubid, 0, 0, g_actors[actor_index].id, 0 );
//	//actors_talk( actor_index, CHANNEL_TYPE_ORG, szMsg, szLen, 0 );
//	return 0;
//}
//
///* ���� */
//int club_out( int actorid, int club_index )
//{
//	char szSQL[1024];
//	sprintf( szSQL, "delete from club_member where actorid='%d'", actorid );
//	if ( mysql_query( myGame, szSQL ) )
//	{
//		printf( "Query failed (%s)\n", mysql_error( myGame ) );
//		write_gamelog( "%s", szSQL );
//		return -1;
//	}
//	prefix_delete_db( actorid, 1 );
//	// ���ߵĲ���
//	int actor_index = actor_getindex_withid( actorid );
//	if ( actor_index < 0 || actor_index >= g_maxactornum )
//	{
//		prefix_delete_db( actorid, 1 );
//		return -1;
//	}
//	if ( club_index > 0 )
//	{// �����б���ɾ�������Ա
//		for ( int tmpi = 0; tmpi < MAX_MEMBER_COUNT; tmpi++ )
//		{
//			if ( g_club[club_index].m_online_member[tmpi] == actor_index )
//			{
//				g_club[club_index].m_online_member[tmpi] = -1;
//				break;
//			}
//		}
//		g_club[club_index].m_membercount--;
//	}
//	g_actors[actor_index].club.m_club_index = 0;
//	g_actors[actor_index].club.m_place = 0;
//	g_actors[actor_index].club.m_total_contribute = 0;
//	g_actors[actor_index].club.m_current_contribute = 0;
//	g_actors[actor_index].club.m_entertime = 0;
//	g_actors[actor_index].club.m_offlinetime = 0;
//	prefix_delete( actor_index, 1 );
//	quest_set( actor_index, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ); // �����������
//	//quest_set( actor_index, QUESTINDEX_WORK, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ); // ���彨���������
//	//quest_set( actor_index, QUESTINDEX_BUSINESSMAN, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ); // �����������
//	//item_lost( actor_index, CLUB_BUSINESS_ITEMKIND, 1 ); // ֧ƱҲ�ջ�
//	return 0;
//}
//
///* ��������뿪��� */
//int club_leave( int actor_index )
//{
//	MYSQL_RES		*res;
//	MYSQL_ROW		row;
//	char	szSQL[1024];
//	int	continue_id;
//	char szMsg[256];
//
//	ACTOR_CHECK_INDEX( actor_index );
//	short club_index = g_actors[actor_index].club.m_club_index;
//	CLUB_CHECK_INDEX( club_index );
//	if ( g_actors[actor_index].club.m_place == CLUBPLACE_LEADER )
//	{ // �����뿪
//		continue_id = 0;
//		// ��ѯ��һ����ְ��ߵĵĳ�Ա,�����峤��ְ��
//		sprintf( szSQL, "select actorid from club_member where clubid='%d' and place=%d ORDER by place desc limit 1", g_club[club_index].m_clubid, CLUBPLACE_FLEADER );
//		if ( mysql_query( myGame, szSQL ) )
//		{
//			printf( "Query failed (%s)\n", mysql_error( myGame ) );
//			write_gamelog( "%s", szSQL );
//			return -1;
//		}
//		res = mysql_store_result( myGame );
//		if ( (row = mysql_fetch_row( res )) )
//		{
//			continue_id = atoi( row[0] );
//		}
//		mysql_free_result( res );
//		if ( continue_id )
//		{
//			// �ӹܰ���
//			club_change_place( club_index, continue_id, CLUBPLACE_LEADER );
//		}
//		else
//		{
//			// û�и�����, ���������˳�����, ���ǰ���ֻ���Լ�һ������
//			if ( 1 == g_club[club_index].m_membercount )
//			{
//				club_out( g_actors[actor_index].actorid, club_index );
//				club_delete( club_index );
//				return 0;
//			}
//			else
//			{
//				actor_notify( actor_index, 0, -1, "��ָ��һ�����������˳�����" );
//				return -1;
//			}
//		}
//	}
//	
//	sprintf( szMsg, "%s�뿪�˰��ɣ�", g_actors[actor_index].pattr[0]->name );
//	actors_talk( actor_index, CHANNEL_TYPE_ORG, szMsg, (int)(strlen( szMsg ) + 1), 0 );
//	club_out( g_actors[actor_index].actorid, club_index );
//	club_add_event( g_club[club_index].m_clubid, g_actors[actor_index].actorid, g_actors[actor_index].pattr[0]->name, 0, 0, CLUBEVENT_LEAVE );
//	return 0;
//}
//
///* ���� */
//int club_kick( int actor_index, int targetid )
//{
//	MYSQL_RES	*res;
//	MYSQL_ROW	row;
//	char	szSQL[1024];
//	char szMsg[256];
//	char targetname[NAME_SIZE];
//	char targetplace = 0;
//	ACTOR_CHECK_INDEX( actor_index );
//	short club_index = g_actors[actor_index].club.m_club_index;
//	CLUB_CHECK_INDEX( club_index );
//	if ( g_actors[actor_index].club.m_place < CLUBPLACE_CAPTAIN1 )
//	{
//		actor_notify( actor_index, 0, -1, "��������ְλ����Ȩ��" );
//		return -1;
//	}
//	if ( g_actors[actor_index].actorid == targetid )
//	{
//		actor_notify( actor_index, 0, -1, "�㲻�������Լ�" );
//		return -1;
//	}
//
//	// ���Ŀ��ְλ����Щְλֻ�ܰ����߳�
//	sprintf( szSQL, "select place from club_member where actorid=%d", targetid );
//	if ( mysql_query( myGame, szSQL ) )
//	{
//		printf( "Query failed (%s)\n", mysql_error( myGame ) );
//		write_gamelog( "%s", szSQL );
//		return -1;
//	}
//	res = mysql_store_result( myGame );
//	if ( (row = mysql_fetch_row( res )) )
//	{
//		targetplace = atoi( row[0] );
//	}
//	mysql_free_result( res );
//
//	if ( g_actors[actor_index].club.m_place < CLUBPLACE_LEADER )
//	{
//		if ( targetplace == CLUBPLACE_LEADER ||
//			targetplace == CLUBPLACE_FLEADER ||
//			targetplace == CLUBPLACE_CAPTAIN1 ||
//			targetplace == CLUBPLACE_CAPTAIN2 ||
//			targetplace == CLUBPLACE_CAPTAIN3 ||
//			targetplace == CLUBPLACE_CAPTAIN4 )
//		{
//			actor_notify( actor_index, 0, -1, "��������ְλ�����룬ֻ�а�������Ȩ�ޣ�" );
//			return -1;
//		}
//	}
//	actor_getname_withid( targetid, targetname );
//	sprintf( szMsg, "%s �� %s �����˰��ɣ�", g_actors[actor_index].pattr[0]->name, targetname );
//	actors_talk( actor_index, CHANNEL_TYPE_ORG, szMsg, (int)(strlen( szMsg ) + 1), 0 );
//	club_out( targetid, club_index );
//	club_add_event( g_club[club_index].m_clubid, g_actors[actor_index].actorid, g_actors[actor_index].pattr[0]->name, targetid, targetname, CLUBEVENT_KICK );
//
//	int target_index = actor_getindex_withid( targetid );
//	if ( target_index >= 0 && target_index < g_maxactornum )
//	{
//		actor_notify( target_index, 0, -1, "�㱻�����˰��ɣ�" );
//	}
//	else
//	{
//		mail_send( -157, targetid, "�㱻�����˰���", 0, 0, 0, 0, 0, 0 );
//	}
//	return 0;
//}
//
///* ��������������������߽�ɫid */
//int club_today_loginactor( int club_index, int actor_index )
//{
//	int tmpi;
//	int freepos = -1;
//	if ( club_index <= 0 || club_index >= MAX_CLUB_COUNT )
//		return 1;
//	if ( actor_index < 0 || actor_index >= g_maxactornum )
//		return 1;
//	for ( tmpi = 0; tmpi < MAX_MEMBER_COUNT; tmpi++ )
//	{
//		if ( g_club[club_index].m_online_actorid[tmpi] <= 0 )
//		{
//			freepos = tmpi;
//			continue;
//		}
//		if ( g_actors[actor_index].actorid == g_club[club_index].m_online_actorid[tmpi] )
//			return 1;
//	}
//	if ( freepos >= 0 )
//		g_club[club_index].m_online_actorid[freepos] = g_actors[actor_index].actorid;
//	return 0;
//}
//
//// ת������,��ʵ�ʸı�֮ǰ����
//void club_online_change( int old_index, int new_index )
//{
//	int tmpi;
//	short club_index;
//
//	club_index = g_actors[old_index].club.m_club_index;
//
//	if ( new_index < 0 || old_index < 0 || g_actors[old_index].club.m_place == 0 )
//		return;
//
//	for ( tmpi = 0; tmpi < MAX_MEMBER_COUNT; tmpi++ )
//	{
//		if ( g_club[club_index].m_online_member[tmpi] == old_index )
//		{
//			g_club[club_index].m_online_member[tmpi] = new_index;
//			break;
//		}
//	}
//	return;
//}
//
//// ת��ְλ
//int club_change_place( short club_index, int memberid, char place )
//{
//	MYSQL_RES *res;
//	MYSQL_ROW row;
//	char szSQL[1024];
//	char membername[64];
//	int placenum = 0;
//	int maxnum = 0;
//	char szMsg[256] = { 0 };
//	if ( place <= 0 )
//		return -1;
//	// ÿ�ֲ�ͬ��ְ�������������Ƶ�, ��Ҫ����, �Ȳ�ѯ������
//	sprintf( szSQL, "select count(*) from club_member where clubid='%d' and place='%d'", g_club[club_index].m_clubid, place );
//	if ( mysql_query( myGame, szSQL ) )
//	{
//		printf( "Query failed (%s)\n", mysql_error( myGame ) );
//		write_gamelog( "%s", szSQL );
//		return -1;
//	}
//	res = mysql_store_result( myGame );
//	if ( (row = mysql_fetch_row( res )) )
//	{
//		placenum = atoi( row[0] );
//	}
//	else
//	{
//		mysql_free_result( res );
//		return -1;
//	}
//	mysql_free_result( res );
//
//	maxnum = -1;
//	switch ( place )
//	{
//	case CLUBPLACE_FLEADER:
//		maxnum = 1;
//		break;
//	case CLUBPLACE_CAPTAIN1:
//		maxnum = 1;
//		break;
//	case CLUBPLACE_CAPTAIN2:
//		maxnum = 1;
//		break;
//	case CLUBPLACE_CAPTAIN3:
//		maxnum = 1;
//		break;
//	case CLUBPLACE_CAPTAIN4:
//		maxnum = 1;
//		break;
//	}
//	if ( maxnum > 0 && placenum >= maxnum )
//	{
//		// ��ְλ��������
//		return -2;
//	}
//
//	sprintf( szSQL, "update club_member set place='%d' where actorid='%d' and clubid='%d'", place, memberid, g_club[club_index].m_clubid );
//	if ( mysql_query( myGame, szSQL ) )
//	{
//		printf( "Query failed (%s)\n", mysql_error( myGame ) );
//		write_gamelog( "%s", szSQL );
//		return -1;
//	}
//
//	sprintf( szSQL, "select name from club_member where actorid='%d' and clubid='%d' and place > 0", memberid, g_club[club_index].m_clubid );
//	if ( mysql_query( myGame, szSQL ) )
//	{
//		printf( "Query failed (%s)\n", mysql_error( myGame ) );
//		write_gamelog( "%s", szSQL );
//		return -1;
//	}
//	res = mysql_store_result( myGame );
//	if ( (row = mysql_fetch_row( res )) )
//	{
//		strcpy( membername, row[0] );
//		mysql_free_result( res );
//	}
//	else
//	{
//		mysql_free_result( res );
//		return -1;
//	}
//
//	if ( place != CLUBPLACE_LEADER )
//	{
//		club_add_event( g_club[club_index].m_clubid, memberid, membername, place, club_getplacename( place ), CLUBEVENT_PLACE_CHANGE );
//	}
//	if ( place == CLUBPLACE_LEADER )
//	{
//		strcpy( g_club[club_index].m_leadername, membername );
//		club_add_event( g_club[club_index].m_clubid, memberid, g_club[club_index].m_leadername, place, club_getplacename( place ), CLUBEVENT_LEADER_TRANSFER );
//	}
//	
//	// �������
//	int actor_index = actor_getindex_withid( memberid );
//	if ( actor_index >= 0 && actor_index < g_maxactornum )
//	{
//		if ( g_actors[actor_index].club.m_place == place )
//			return 0;
//		g_actors[actor_index].club.m_place = place;
//		sprintf( szMsg, "����%s��ְλ�Ѿ��ı�", g_club[club_index].m_clubname );
//		actor_notify( actor_index, NOTIFY_NORMAL_MSG, (int)strlen( szMsg ), szMsg );
//		switch ( place )
//		{
//		case CLUBPLACE_LEADER:
//			prefix_change( actor_index, 1, 8 );
//			break;
//		case CLUBPLACE_FLEADER:
//			prefix_change( actor_index, 1, 7 );
//			break;
//		case CLUBPLACE_CAPTAIN4:
//			prefix_change( actor_index, 1, 3 );
//			break;
//		case CLUBPLACE_CAPTAIN3:
//			prefix_change( actor_index, 1, 4 );
//			break;
//		case CLUBPLACE_CAPTAIN2:
//			prefix_change( actor_index, 1, 5 );
//			break;
//		case CLUBPLACE_CAPTAIN1:
//			prefix_change( actor_index, 1, 6 );
//			break;
//		case CLUBPLACE_MEMBER:
//			prefix_change( actor_index, 1, 2 );
//			break;
//		case CLUBPLACE_LOW:
//			prefix_change( actor_index, 1, 1 );
//			break;
//		}
//	}
//	else
//	{
//		sprintf( szMsg, "����%s��ְλ�Ѿ��ı�", g_club[club_index].m_clubname );
//		mail_send( -157, memberid, szMsg, 0, 0, 0, 0, 0, 0 );
//	}
//	return 0;
//}
//
///* �����巢�ʼ� */
//int club_mail_send( int actor_index, int clubid, char *msg, char type, int value1, int value2, int value3, int value4, int overdue )
//{
//	MYSQL_RES *res;
//	MYSQL_ROW row;
//	char	szSQL[1024];
//	int memberid;
//	int actorid = -1;
//	if ( actor_index >= 0 && actor_index < g_maxactornum )
//	{
//		actorid = g_actors[actor_index].actorid;
//	}
//	// ���������г�Ա����ɾ����Ϣ
//	sprintf( szSQL, "select actorid from club_member where clubid='%d'", clubid );
//	if ( mysql_query( myGame, szSQL ) )
//	{
//		printf( "Query failed (%s)\n", mysql_error( myGame ) );
//		write_gamelog( "%s", szSQL );
//		return -1;
//	}
//	res = mysql_store_result( myGame );
//	while ( (row = mysql_fetch_row( res )) )
//	{
//		memberid = atoi( row[0] );
//		if ( actorid != memberid )
//			mail_send( actor_index, memberid, msg, type, value1, value2, value3, value4, overdue );
//	}
//	mysql_free_result( res );
//	return 0;
//}
//
//// �����߼� 1Сʱ1��
//int club_logic_hourly()
//{
//	int tmpi;
//	char szMsg[512] = { 0 };
//	char szText[5][64] = { 0 };
//	int mainte_money = 0;
//	int drugcount = 0;
//	time_t t;
//	struct tm *newtime;
//	time( &t );
//	newtime = localtime( &t );
//
//	for ( tmpi = 0; tmpi < MAX_CLUB_COUNT; tmpi++ )
//	{
//		if ( g_club[tmpi].m_clubid > 0 )
//		{
//			// ά���� ������
//			if ( newtime->tm_hour == 0 )
//			{
//				mainte_money = club_get_hourly_mainte_money( tmpi );
//				club_set_money( tmpi, mainte_money );
//				club_add_event( g_club[tmpi].m_clubid, 0, 0, mainte_money, 0, CLUBEVENT_MAINTE );
//				sprintf( szText[0], "�����ά�������ѽ��㣬�۳��ʽ�%d��", -mainte_money );
//			} 
//
//			// ÿСʱ���̵�ˢ��
//			if ( club_shop_refurbish( tmpi ) >= 0 )
//			{
//				sprintf( szText[0], "�����̵���ˢ�£�" );
//			}
//
//			// ˢҩ����
//			//drugcount = club_get_brush_drugcount( tmpi );
//			//club_random_drugcount( tmpi, drugcount );
//
//		}
//
//	}
//	return 0;
//}
//
///* �޸��³� */
//int club_change_intro( int actor_index, char *intro )
//{
//	if ( !intro )
//		return -1;
//	ACTOR_CHECK_INDEX( actor_index );
//	short club_index = g_actors[actor_index].club.m_club_index;
//	CLUB_CHECK_INDEX( club_index );
//	if ( g_actors[actor_index].club.m_place < CLUBPLACE_CAPTAIN1 )
//	{
//		actor_notify( actor_index, 0, -1, "��������ְλ����Ȩ��" );
//		return -1;
//	}
//	strncpy( g_club[club_index].m_intro, intro, sizeof(char)*CLUB_NOTIFY_SIZE );
//	club_update( club_index );
//	club_add_event( g_club[club_index].m_clubid, 0, g_actors[actor_index].pattr[0]->name, 0, 0, CLUBEVENT_MODIFY_POST );
//	int value[1] = { 0 };
//	value[0] = 9;
//	actor_notify_value( actor_index, NOTIFY_CLUB, 1, value, -1, g_club[club_index].m_intro );
//	return 0;
//}
//
//int club_change_member_contribute( int actor_index, int contribute )
//{
//	char szMsg[128] = { 0 };
//	ACTOR_CHECK_INDEX( actor_index );
//	CLUB_CHECK_INDEX( g_actors[actor_index].club.m_club_index );
//
//	if ( g_actors[actor_index].club.m_total_contribute < 0 )
//		g_actors[actor_index].club.m_total_contribute = 0;
//	if ( g_actors[actor_index].club.m_current_contribute < 0 )
//		g_actors[actor_index].club.m_current_contribute = 0;
//
//	if ( contribute > 0 )
//	{
//		g_actors[actor_index].club.m_total_contribute += contribute;
//	}
//
//	if ( contribute != 0 )
//	{
//		g_actors[actor_index].club.m_current_contribute += contribute;
//		if ( g_actors[actor_index].club.m_current_contribute < 0 )
//			g_actors[actor_index].club.m_current_contribute = 0;
//
//		club_update_membercontribute( actor_index );
//
//		if ( contribute < 0 )
//			sprintf( szMsg, "&w��ʧȥ��&g%d&w����ɹ���", -contribute );
//		else
//			sprintf( szMsg, "&w������&g%d&w����ɹ���", contribute );
//	}
//
//	return 0;
//}
//
//int club_update_membercontribute( int actor_index )
//{
//	char szSQL[2048];
//	ACTOR_CHECK_INDEX( actor_index );
//	sprintf( szSQL, "update club_member set total_contribute='%d', current_contribute='%d' where actorid='%d' ",
//		g_actors[actor_index].club.m_total_contribute, g_actors[actor_index].club.m_current_contribute, g_actors[actor_index].actorid );
//	if ( mysql_query( myGame, szSQL ) )
//	{
//		printf( "Query failed (%s)\n", mysql_error( myGame ) );
//		write_gamelog( "%s", szSQL );
//		return -1;
//	}
//	return 0;
//}
//
//int club_get_member_contribute( int actor_index )
//{
//	ACTOR_CHECK_INDEX( actor_index );
//	CLUB_CHECK_INDEX( g_actors[actor_index].club.m_club_index );
//	return g_actors[actor_index].club.m_current_contribute;
//}
//
///* ��ʼ�������̵� */
//int club_shop_init()
//{
//	MYSQL_RES		*res;
//	MYSQL_ROW		row;
//	char	szSQL[1024];
//	sprintf( szSQL, "select max(itemid) from shop_club" );
//	if ( mysql_query( myData, szSQL ) )
//	{
//		printf( "Query failed (%s) [%s](%d)\n", mysql_error( myData ), __FUNCTION__, __LINE__ );
//		write_gamelog( "%s", szSQL );
//		return -1;
//	}
//	res = mysql_store_result( myData );
//	if ( (row = mysql_fetch_row( res )) )
//	{
//		if ( row[0] )
//			g_clubshopitemcount = atoi( row[0] ) + 1;
//		else
//			g_clubshopitemcount = 0;
//	}
//	else
//	{
//		mysql_free_result( res );
//		return -1;
//	}
//	mysql_free_result( res );
//	if ( g_clubshopitemcount <= 0 )
//	{
//		return 0;
//	}
//	// ����ռ�
//	g_clubshop = (ClubShop *)malloc( sizeof(ClubShop)*g_clubshopitemcount );
//	memset( g_clubshop, 0, sizeof(ClubShop)*g_clubshopitemcount );
//
//	sprintf( szSQL, "select itemid,itemkind,price,shoplevel,value from shop_club" );
//	if ( mysql_query( myData, szSQL ) )
//	{
//		printf( "Query failed (%s) [%s](%d)\n", mysql_error( myData ), __FUNCTION__, __LINE__ );
//		write_gamelog( "%s", szSQL );
//		return -1;
//	}
//	res = mysql_store_result( myData );
//
//	int itemindex;
//	while ( (row = mysql_fetch_row( res )) )
//	{
//		itemindex = atoi( row[0] );
//		if ( itemindex <= 0 || itemindex >= g_clubshopitemcount )
//		{
//			mysql_free_result( res );
//			return -1;
//		}
//		g_clubshop[itemindex].itemkind = atoi( row[1] );
//		g_clubshop[itemindex].price = atoi( row[2] );
//		g_clubshop[itemindex].shoplevel = atoi( row[3] );
//		g_clubshop[itemindex].value = atoi( row[4] );
//		if ( g_clubshop[itemindex].value <= 0 )
//			g_clubshop[itemindex].value = 1;
//	}
//	mysql_free_result( res );
//	return 0;
//}
//
///* �����̵�ˢ�� */
//int club_shop_refurbish( int club_index )
//{
//	CLUB_CHECK_INDEX( club_index );
//	memset( g_club[club_index].m_clubranditem, 0, sizeof(ClubRandItem)*CLUB_SHOP_ITEM_COUNT );
//	if ( g_club[club_index].m_build_level[Club_drug] <= 0 )
//		return -1;
//	ClubShop shop[128] = { 0 };
//	int count = 0;
//	int totalvalue = 0;
//	for ( int tmpi = 0; tmpi < g_clubshopitemcount; tmpi++ )
//	{ // �ҳ��ȼ����ϵ����е��ߣ���Ϊ�������
//		if ( g_clubshop[tmpi].shoplevel <= g_club[club_index].m_build_level[Club_drug] )
//		{
//			memcpy( &shop[count], &g_clubshop[tmpi], sizeof(ClubShop) );
//			totalvalue += g_clubshop[tmpi].value;
//			count++;
//		}
//	}
//
//	// û���������������ѡ
//	int step = 0;
//	int itemcount = 0;
//	while ( itemcount < g_club[club_index].m_build_level[Club_drug] && step < 32 )
//	{
//		// ����ֵ
//		if ( totalvalue <= 0 )
//			break;
//		int randvalue = rand() % totalvalue;
//		for ( int tmpi = 0; tmpi < count; tmpi++ )
//		{
//			if ( randvalue < shop[tmpi].value )
//			{
//				g_club[club_index].m_clubranditem[itemcount].itemkind = shop[tmpi].itemkind;
//				g_club[club_index].m_clubranditem[itemcount].count = 1;
//				g_club[club_index].m_clubranditem[itemcount].price = shop[tmpi].price;
//				itemcount += 1;
//
//				//// ȥ�����������Ϣ
//				//totalvalue -= shop[tmpi].value;
//				//memset( &shop[tmpi], 0, sizeof(ClubShop) );
//				//if ( tmpi < count - 1 )
//				//{
//				//	memmove( &shop[tmpi], &shop[tmpi + 1], sizeof(ClubShop)*(count - tmpi) );
//				//}
//				//count -= 1;
//				break;
//			}
//			randvalue -= shop[tmpi].value;
//		}
//		step += 1;
//	}
//	return 0;
//}
//
///* �����̵���Ϣ���� */
//int club_shop_listinfo( int actor_index )
//{
//	ACTOR_CHECK_INDEX( actor_index );
//	short club_index = g_actors[actor_index].club.m_club_index;
//	CLUB_CHECK_INDEX( club_index );
//	SLK_NetS_ShopList Value = {};
//	Value.m_itemnum = 0;
//	for ( int tmpi = 0; tmpi < CLUB_SHOP_ITEM_COUNT; tmpi++ )
//	{
//		int itemkind = g_club[club_index].m_clubranditem[tmpi].itemkind;
//		if ( itemkind <= 0 )
//			continue;
//		Value.m_item[Value.m_itemnum].m_itemindex = tmpi;
//		Value.m_item[Value.m_itemnum].m_itemkind = itemkind;
//		Value.m_item[Value.m_itemnum].m_itemtype = item_gettype( itemkind );
//		Value.m_item[Value.m_itemnum].m_color = item_getcolorlevel( itemkind );
//		if ( Value.m_item[Value.m_itemnum].m_itemtype >= ITEM_TYPE_EQUIP1 && Value.m_item[Value.m_itemnum].m_itemtype <= ITEM_TYPE_EQUIP6 )
//		{
//			item_get_ability_value_withpos( itemkind, 0, &Value.m_item[Value.m_itemnum].m_ability1, &Value.m_item[Value.m_itemnum].m_value1 );
//			item_get_ability_value_withpos( itemkind, 1, &Value.m_item[Value.m_itemnum].m_ability2, &Value.m_item[Value.m_itemnum].m_value2 );
//		}
//		else
//		{
//			Value.m_item[Value.m_itemnum].m_ability1 = item_get_base_ability( itemkind, 0 );
//			Value.m_item[Value.m_itemnum].m_ability2 = item_get_base_ability( itemkind, 1 );
//			Value.m_item[Value.m_itemnum].m_value1 = item_get_base_value( itemkind, 0 );
//			Value.m_item[Value.m_itemnum].m_value2 = item_get_base_value( itemkind, 1 );
//		}
//
//		if ( Value.m_item[Value.m_itemnum].m_itemtype == ITEM_TYPE_MATERIAL )
//		{
//			item_get_ability_value_withpos( itemkind, 0, &Value.m_item[Value.m_itemnum].m_ability3, &Value.m_item[Value.m_itemnum].m_value3 );
//		}
//
//		Value.m_item[Value.m_itemnum].m_count = g_club[club_index].m_clubranditem[tmpi].count;
//		Value.m_item[Value.m_itemnum].m_price = g_club[club_index].m_clubranditem[tmpi].price;
//		Value.m_item[Value.m_itemnum].m_payment = PAYMENT_CLUB;
//		Value.m_item[Value.m_itemnum].m_limitbuy = 0;
//		Value.m_item[Value.m_itemnum].m_limitnum = 0;
//		Value.m_activitypoint = club_get_member_contribute( actor_index );
//		Value.m_itemnum++;
//	}
//
//	if ( Value.m_itemnum == 0 )
//	{
//		actor_notify( actor_index, 0, -1, "�̵�Ļ����Ѿ������ˣ��Ȼ������ɣ�" );
//		return -1;
//	}
//	Value.m_type = SHOPEX_CLUBDRUG;
//	netsend_shoplist_S( actor_index, SENDTYPE_ACTOR, &Value );
//	return 0;
//}
//
///* �����̵깺�� */
//int club_shop_buy( int actor_index, int itemindex )
//{
//	ACTOR_CHECK_INDEX( actor_index );
//	short club_index = g_actors[actor_index].club.m_club_index;
//	CLUB_CHECK_INDEX( club_index );
//	if ( itemindex < 0 || itemindex >= CLUB_SHOP_ITEM_COUNT )
//		return -1;
//	int itemkind = g_club[club_index].m_clubranditem[itemindex].itemkind;
//	if ( itemkind <= 0 )
//		return -1;
//	if ( g_club[club_index].m_clubranditem[itemindex].count <= 0 )
//	{
//		actor_notify( actor_index, 0, -1, "�ܱ�Ǹ�������Ʒ�Ѿ��۳�ȥ��" );
//		return -1;
//	}
//
//	char paymentname[16] = { 0 };
//	int actor_price = shopex_payment( actor_index, PAYMENT_CLUB, paymentname );
//	int needprice = g_club[club_index].m_clubranditem[itemindex].price;
//	itemkind = g_club[club_index].m_clubranditem[itemindex].itemkind;
//
//	// �ж�ͭǮ
//	if ( actor_price < needprice )
//	{
//		char szMsg[64] = { 0 };
//		sprintf( szMsg, "����%s����!", paymentname );
//		actor_notify( actor_index, 0, (short)strlen( szMsg ), szMsg );
//		return -1;
//	}
//
//	// ֮�����־�úüǼǣ��������мǣ�
//	// ����
//	if ( item_getitem( actor_index, itemkind, 1, 0, 0, ITEM_RULE_SHOP, 0, PATH_SHOPCLUB ) < 0 )
//	{
//		char szMsg[64] = { 0 };
//		sprintf( szMsg, "���ı�����λ����!" );
//		actor_notify( actor_index, 0, (short)strlen( szMsg ), szMsg );
//		return -1;
//	}
//	shopex_addpayment( actor_index, PAYMENT_CLUB, -needprice );
//	g_club[club_index].m_clubranditem[itemindex].count = 0;
//	club_shop_listinfo( actor_index );
//	return 0;
//}
//
///* ���ɽ����̵� */
//int club_tradeshop_listinfo( int actor_index )
//{
//	ACTOR_CHECK_INDEX( actor_index );
//	short club_index = g_actors[actor_index].club.m_club_index;
//	CLUB_CHECK_INDEX( club_index );
//	SLK_NetS_ShopList Value = {};
//	Value.m_itemnum = 0;
//	MYSQL_RES		*res;
//	MYSQL_ROW		row;
//	char	szSQL[1024];
//	sprintf( szSQL, "select id, itemkind, payment, price from club_tradeshop where clubid=%d order by optime asc;", g_club[club_index].m_clubid );
//	if ( mysql_query( myGame, szSQL ) )
//	{
//		printf( "Query failed (%s) [%s](%d)\n", mysql_error( myGame ), __FUNCTION__, __LINE__ );
//		write_gamelog( "%s", szSQL );
//		return -1;
//	}
//	res = mysql_store_result( myGame );
//	while ( (row = mysql_fetch_row( res )) )
//	{
//		int id = atoi( row[0] );
//		int itemkind = atoi( row[1] );
//		int payment = atoi( row[2] );
//		int price = atoi( row[3] );
//		if ( itemkind <= 0 )
//			continue;
//		Value.m_item[Value.m_itemnum].m_itemindex = id;
//		Value.m_item[Value.m_itemnum].m_itemkind = itemkind;
//		Value.m_item[Value.m_itemnum].m_itemtype = item_gettype( itemkind );
//		Value.m_item[Value.m_itemnum].m_color = item_getcolorlevel( itemkind );
//		if ( Value.m_item[Value.m_itemnum].m_itemtype >= ITEM_TYPE_EQUIP1 && Value.m_item[Value.m_itemnum].m_itemtype <= ITEM_TYPE_EQUIP6 )
//		{
//			item_get_ability_value_withpos( itemkind, 0, &Value.m_item[Value.m_itemnum].m_ability1, &Value.m_item[Value.m_itemnum].m_value1 );
//			item_get_ability_value_withpos( itemkind, 1, &Value.m_item[Value.m_itemnum].m_ability2, &Value.m_item[Value.m_itemnum].m_value2 );
//		}
//		else
//		{
//			Value.m_item[Value.m_itemnum].m_ability1 = item_get_base_ability( itemkind, 0 );
//			Value.m_item[Value.m_itemnum].m_ability2 = item_get_base_ability( itemkind, 1 );
//			Value.m_item[Value.m_itemnum].m_value1 = item_get_base_value( itemkind, 0 );
//			Value.m_item[Value.m_itemnum].m_value2 = item_get_base_value( itemkind, 1 );
//		}
//
//		if ( Value.m_item[Value.m_itemnum].m_itemtype == ITEM_TYPE_MATERIAL )
//		{
//			item_get_ability_value_withpos( itemkind, 0, &Value.m_item[Value.m_itemnum].m_ability3, &Value.m_item[Value.m_itemnum].m_value3 );
//		}
//
//		Value.m_item[Value.m_itemnum].m_count = 1;
//		Value.m_item[Value.m_itemnum].m_price = price;
//		Value.m_item[Value.m_itemnum].m_payment = payment;
//		Value.m_item[Value.m_itemnum].m_limitbuy = 0;
//		Value.m_item[Value.m_itemnum].m_limitnum = 0;
//		Value.m_activitypoint = club_get_member_contribute( actor_index );
//		Value.m_itemnum++;
//	}
//	mysql_free_result( res );
//	Value.m_type = SHOPEX_CLUBTRADE;
//	netsend_shoplist_S( actor_index, SENDTYPE_ACTOR, &Value );
//
//	if ( Value.m_itemnum == 0 )
//	{
//		actor_notify( actor_index, 0, -1, "ó���г���ʱû�л����ˣ���Щ�ϳɾ�ͻ��Ԫ���ǿ��Խ��׵ģ�" );
//	}
//	return 0;
//}
//
///* ����ó���г����� */
//int club_tradeshop_buy( int actor_index, int id )
//{
//	ACTOR_CHECK_INDEX( actor_index );
//	short club_index = g_actors[actor_index].club.m_club_index;
//	CLUB_CHECK_INDEX( club_index );
//	MYSQL_RES * res;
//	MYSQL_ROW	row;
//	char szSQL[1024];
//	char paymentname[16] = { 0 };
//	int actorid = 0;
//	int itemkind = 0;
//	int payment = 0;
//	int price = 0;
//	int buytimes = actor_get_today_times( actor_index, TODAY_INDEX_CLUBTRADETIMES );
//	if ( buytimes >= 3 )
//	{
//		actor_notify( actor_index, 0, -1, "ÿ��ÿ��ɽ��а���ó��3��" );
//		return -1;
//	}
//	sprintf( szSQL, "select actorid, itemkind, payment, price from club_tradeshop where clubid=%d and id=%d", g_club[club_index].m_clubid, id );
//	if ( mysql_query( myGame, szSQL ) )
//	{
//		printf( "Query failed (%s) [%s](%d)\n", mysql_error( myGame ), __FUNCTION__, __LINE__ );
//		write_gamelog( "%s", szSQL );
//		return -1;
//	}
//	res = mysql_store_result( myGame );
//	if ( (row = mysql_fetch_row( res )) )
//	{
//		actorid = atoi( row[0] );
//		itemkind = atoi( row[1] );
//		payment = atoi( row[2] );
//		price = atoi( row[3] );
//		mysql_free_result( res );
//	}
//	else
//	{
//		mysql_free_result( res );
//		actor_notify( actor_index, 0, -1, "�ܱ�Ǹ�������Ʒ�Ѿ���������" );
//		return -1;
//	}
//
//	// �ж�
//	int actor_price = shopex_payment( actor_index, payment, paymentname );
//	if ( actor_price < price )
//	{
//		char szMsg[64] = { 0 };
//		sprintf( szMsg, "����%s����!", paymentname );
//		actor_notify( actor_index, 0, (short)strlen( szMsg ), szMsg );
//		return -1;
//	}
//
//	// ֮�����־�úüǼǣ��������мǣ�
//	// ����
//	if ( item_getitem( actor_index, itemkind, 1, 0, 0, ITEM_RULE_SHOP, 0, PATH_SHOPCLUBTRADE ) < 0 )
//	{
//		char szMsg[64] = { 0 };
//		sprintf( szMsg, "���ı�����λ����!" );
//		actor_notify( actor_index, 0, (short)strlen( szMsg ), szMsg );
//		return -1;
//	}
//	// �۳�
//	shopex_addpayment( actor_index, payment, -price );
//
//	// ��Ǯ��������
//	char szMsg[256] = { 0 };
//	if ( payment == PAYMENT_MONEY )
//		payment = -12;
//	else if ( payment == PAYMENT_TOKEN )
//		payment = -13;
//	else if ( payment == PAYMENT_CLUB )
//		payment = -17;
//	else
//		payment = -17;
//	sprintf( szMsg, "&c%s&w���������ڰ���ó���г������Ʒ&g%s&w������|%d|%d|", g_actors[actor_index].pattr[0]->name, item_getname( itemkind ), payment, price );
//	mail_send( -157, actorid, szMsg, 2, 0, 0, 5, 0, 0 );
//
//	// ����Ʒɾ��
//	sprintf( szSQL, "delete from club_tradeshop where clubid='%d' and id='%d'", g_club[club_index].m_clubid, id );
//	if ( mysql_query( myGame, szSQL ) )
//	{
//		printf( "Query failed (%s)\n", mysql_error( myGame ) );
//		write_gamelog( "%s", szSQL );
//		return -1;
//	}
//
//	// ��ӹ������
//	if ( actorid != g_actors[actor_index].actorid )
//		actor_add_today_times( actor_index, TODAY_INDEX_CLUBTRADETIMES );
//
//	// ��һ���б�
//	club_tradeshop_listinfo( actor_index );
//
//	// ��¼LOG
//	wlog( 0, LOGOP_CLUBTREAD, PATH_SHOPCLUBTRADE, itemkind, payment, price, g_actors[actor_index].actorid, actorid );
//	return 0;
//}
//
///* ����ó���г��������� */
//int club_tradeshop_sell_ask( int actor_index, int itemindex )
//{
//	ACTOR_CHECK_INDEX( actor_index );
//	short club_index = g_actors[actor_index].club.m_club_index;
//	if ( club_index <= 0 || club_index >= MAX_CLUB_COUNT )
//	{
//		actor_notify( actor_index, 0, -1, "��Ҫ������ɲſ�������������" );
//		return -1;
//	}
//	Item * pitem = NULL;
//	if ( itemindex >= 0 && itemindex < MAX_ACTOR_ITEMNUM )
//		pitem = &g_actors[actor_index].item[itemindex];
//	else
//		return -1;
//
//	int itemtype = item_gettype( pitem->m_kind );
//	if ( !(itemtype == ITEM_TYPE_MATERIAL || itemtype == ITEM_TYPE_MIX || itemtype == ITEM_TYPE_PARTNER_STONE) )
//	{
//		actor_notify( actor_index, 0, -1, "ֻ�кϳɾ��Ԫ��ſɷ���ó���г�!" );
//		return -1;
//	}
//
//	int value[2] = { 0 };
//	value[0] = 11;
//	value[1] = itemindex;
//	actor_notify_value( actor_index, NOTIFY_CLUB, 2, value, -1, "" );
//	return 0;
//}
//
///* ����ó���г����� */
//int club_tradeshop_sell( int actor_index, int itemindex, int payment, int price )
//{
//	ACTOR_CHECK_INDEX( actor_index );
//	short club_index = g_actors[actor_index].club.m_club_index;
//	if ( club_index <= 0 || club_index >= MAX_CLUB_COUNT )
//	{
//		actor_notify( actor_index, 0, -1, "��Ҫ������ɲſ�������������" );
//		return -1;
//	}
//	Item * pitem = NULL;
//	MYSQL_RES * res;
//	MYSQL_ROW	row;
//	char szSQL[1024];
//	if ( itemindex >= 0 && itemindex < MAX_ACTOR_ITEMNUM )
//		pitem = &g_actors[actor_index].item[itemindex];
//	else
//		return -1;
//
//	int itemtype = item_gettype( pitem->m_kind );
//	if ( !(itemtype == ITEM_TYPE_MATERIAL || itemtype == ITEM_TYPE_MIX || itemtype == ITEM_TYPE_PARTNER_STONE) )
//	{
//		actor_notify( actor_index, 0, -1, "ֻ�кϳɾ��Ԫ��ſɷ���ó���г�!" );
//		return -1;
//	}
//	// ������Ƿ���������
//	sprintf( szSQL, "SELECT count(*) FROM club_tradeshop WHERE actorid=%d", g_actors[actor_index].actorid );
//	if ( mysql_query( myGame, szSQL ) )
//	{
//		printf( "Query failed (%s)\n", mysql_error( myGame ) );
//		write_gamelog( "%s", szSQL );
//		return -1;
//	}
//	res = mysql_store_result( myGame );
//	if ( row = mysql_fetch_row( res ) )
//	{
//		if ( atoi( row[0] ) >= 2 )
//		{
//			actor_notify( actor_index, 0, -1, "����ó���г��Ļ��ﲻ�ܳ���2��" );
//			mysql_free_result( res );
//			return -1;
//		}
//	}
//	mysql_free_result( res );
//
//	// �����¼�¼
//	sprintf( szSQL, "INSERT INTO club_tradeshop (clubid,actorid,name,itemkind,payment,price,optime) VALUES( '%d','%d','%s','%d','%d','%d','%d' )",
//		g_club[club_index].m_clubid, g_actors[actor_index].actorid, g_actors[actor_index].pattr[0]->name, pitem->m_kind, payment, price, (int)time(NULL) );
//	if ( mysql_query( myGame, szSQL ) )
//	{
//		printf( "Query failed (%s) [%s](%d)\n", mysql_error( myGame ), __FUNCTION__, __LINE__ );
//		write_gamelog( "%s", szSQL );
//		if ( mysql_ping( myGame ) != 0 )
//			db_reconnect_game();
//		return -1;
//	}
//
//	item_lostitem( actor_index, itemindex, 1, 0, PATH_SHOPCLUBTRADE );
//	return 0;
//}
//
///*
//int club_clear_askers( int actor_index, short clubID )
//{
//	char	szSQL[1024];
//	MYSQL_RES		*res;
//	MYSQL_ROW		row;
//	char tmpbuf[2048];
//	short tmpsize;
//	char *ptr, *ptrsize;
//	int askerid;
//	// Ȩ���ж�
//
//	if ( g_club[g_actors[actor_index].club_index].m_clubid != clubID )
//		return -1;
//
//	// ���
//	sprintf( szSQL, "select actorid from club_member where clubid='%d' and clubclass='0'", clubID );
//	if ( mysql_query( myGame, szSQL ) )
//	{
//		printf( "Query failed (%s)\n", mysql_error( myGame ) );
//		write_gamelog( "%s", szSQL );
//		return -1;
//	}
//
//	res = mysql_store_result( myGame );
//	while ( (row = mysql_fetch_row( res )) )
//	{
//		askerid = atoi( row[0] );
//		if ( askerid > 0 )
//		{
//			system_cmd( askerid, SYSTEMCMD_CLUB_LEAVE, clubID, 2 );
//			sprintf( szSQL, "delete from club_member where actorid='%d' and clubid='%d'", askerid, clubID );
//			if ( mysql_query( myGame, szSQL ) )
//			{
//				printf( "Query failed (%s)\n", mysql_error( myGame ) );
//				write_gamelog( "%s", szSQL );
//				mysql_free_result( res );
//				return -1;
//			}
//		}
//	}
//	mysql_free_result( res );
//
//	g_club[g_actors[actor_index].club_index].m_asker_count = 0;
//
//	ptr = tmpbuf;
//	tmpsize = 0;
//	ptr += sizeof(short);
//	*(short *)ptr = CMDS_CLEARASKERS;			ptr += sizeof(short); tmpsize += sizeof(short);
//	ptrsize = ptr;								ptr += sizeof(short); tmpsize += sizeof(short);
//	*(short *)ptrsize = tmpsize - sizeof(short)* 2;
//	*(unsigned short *)tmpbuf = tmpsize;
//
//	sendtoqueue( actor_index, tmpbuf, tmpsize + sizeof(short) );
//	write_gamelog( "[size:%d][<-CMDS_CLEARASKERS][%d]", tmpsize, g_actors[actor_index].id );
//
//	return 0;
//}
//
//// ���һ��������
//int club_add_asker( short club_index, int actor_index )
//{
//	int tmpclub;
//	char szSQL[1024];
//	char ActorName[NAME_SIZE * 2];
//
//	if ( g_club[club_index].m_asker_count >= MAX_ASKER_COUNT )
//	{
//		ACTOR_NORMAL_NOTIFY( actor_index, "�ü������������Ѿ��ﵽ���ޣ�" );
//		return -1;
//	}
//	if ( actor_index < 0 || actor_index >= g_maxactornum )
//		return -1;
//
//	if ( g_actors[actor_index].club_index > 0 )
//	{
//		if ( g_actors[actor_index].club_place > 0 )
//		{
//			// ����Ѿ���ĳ���ɵĳ�Ա
//			return -1;
//		}
//		else if ( g_actors[actor_index].club_index == club_index )
//		{
//			// ����Ѿ��ύ������
//			return -1;
//		}
//		else
//		{
//			// �Ѿ���ĳ���ɵ�������, ɾ��ǰ���ɵ�����asker_count, �������ݿ��еļ���id
//			tmpclub = g_actors[actor_index].club_index;
//			g_club[tmpclub].m_asker_count--;
//
//			sprintf( szSQL, "update club_member set clubid='%d' where actorid='%d'", g_club[club_index].m_clubid, g_actors[actor_index].id );
//			if ( mysql_query( myGame, szSQL ) )
//			{
//				printf( "Query failed (%s)\n", mysql_error( myGame ) );
//				write_gamelog( "%s", szSQL );
//				return -1;
//			}
//
//			g_club[club_index].m_asker_count++;
//			g_actors[actor_index].club_index = club_index;
//			g_actors[actor_index].club_place = 0;
//			return 0;
//		}
//	}
//	g_club[club_index].m_asker_count++;
//	g_actors[actor_index].club_index = club_index;
//	g_actors[actor_index].club_place = 0;
//	db_escape( (const char *)g_actors[actor_index].player_attr.name, ActorName, 0 );
//
//	sprintf( szSQL, "insert into club_member(actorid,clubid,name,shape,gender,aclass,level,clubclass,online) values( '%d','%d','%s','%d','%d','%d','%d','%d','%d')", \
//		g_actors[actor_index].id, g_club[club_index].m_clubid, ActorName, g_actors[actor_index].player_attr.shape, \
//		g_actors[actor_index].player_attr.gender, g_actors[actor_index].aclass, \
//		g_actors[actor_index].player_attr.level, g_actors[actor_index].club_place, 1 );
//	if ( mysql_query( myGame, szSQL ) )
//	{
//		printf( "Query failed (%s)\n", mysql_error( myGame ) );
//		write_gamelog( "%s", szSQL );
//		return -1;
//	}
//	return 0;
//}
//
//int club_proc_invite( int actor_index, int targetid, short clubid, char type )
//{
//	int target_index;
//	short club_index;
//	char szSQL[1024];
//	char actorName[64];
//	char szMsg[256] = { 0 };
//	int nowtick = 0;
//	target_index = actors_getindex_withid( targetid );
//	if ( target_index < 0 )
//		return -1;
//	club_index = club_getindex( clubid );
//	if ( club_index <= 0 || club_index >= g_max_clubid )
//		return -1;
//
//	if ( !type )
//	{
//		club_send_result( target_index, CLUBMSG_REFUSE_INVITE );
//		return -1;
//	}
//
//	if ( g_club[club_index].m_membercount >= club_get_member_max( club_index ) )
//	{
//		club_send_result( actor_index, CLUBMSG_MEMBER_FULL );
//		return -1;
//	}
//
//	if ( g_actors[actor_index].player_attr.level < 20 )
//	{
//		sprintf( szMsg, "��ĵȼ�����" );
//		actor_notify( actor_index, 0, (short)strlen( szMsg ), szMsg );
//		return -1;
//	}
//	if ( g_actors[actor_index].club_index > 0 && g_actors[actor_index].club_place > 0 )
//	{
//		sprintf( szMsg, "�����м��壬�����뿪��ǰ����" );
//		actor_notify( actor_index, 0, (short)strlen( szMsg ), szMsg );
//		return -1;
//	}
//	nowtick = acttime_get_nowtick( );
//	if ( g_actors[actor_index].club_leavetime + 259200 > nowtick )
//	{
//		sprintf( szMsg, "%dСʱ��ſ��ٴμ������", (g_actors[actor_index].club_leavetime + 259200 - nowtick) / 3600 );
//		actor_notify( actor_index, 0, (short)strlen( szMsg ), szMsg );
//		return -1;
//	}
//
//	sprintf( szSQL, "delete from club_member where actorid=%d", g_actors[actor_index].id );
//	if ( mysql_query( myGame, szSQL ) )
//	{
//		printf( "Query failed (%s)\n", mysql_error( myGame ) );
//		write_gamelog( "%s", szSQL );
//		return -1;
//	}
//
//	db_escape( (const char *)g_actors[actor_index].player_attr.name, actorName, 0 );
//	sprintf( szSQL, "insert into club_member(actorid,clubid,name,shape,gender,aclass,level,clubclass,online) values( '%d','%d','%s','%d','%d','%d','%d','%d','%d')", \
//		g_actors[actor_index].id, g_club[club_index].m_clubid, actorName, g_actors[actor_index].player_attr.shape, \
//		g_actors[actor_index].player_attr.gender, g_actors[actor_index].aclass, \
//		g_actors[actor_index].player_attr.level, CLUBPLACE_NORMAL, 1 );
//	if ( mysql_query( myGame, szSQL ) )
//	{
//		printf( "Query failed (%s)\n", mysql_error( myGame ) );
//		write_gamelog( "%s", szSQL );
//		return -1;
//	}
//	g_club[club_index].m_membercount++;
//	g_actors[actor_index].club_index = club_index;
//	system_cmd( g_actors[actor_index].id, SYSTEMCMD_CLUB_ENTER, g_club[club_index].m_clubid, 0 );
//	sprintf( szMsg, "%s�����˼���", g_actors[actor_index].player_attr.name );
//	actor_notify( target_index, 0, (short)strlen( szMsg ), szMsg );
//	return 0;
//}
//
//int club_proc_openinvitedlg( int actor_index, int actorid )
//{
//	int club_level;
//	short club_index;
//	char tmpbuf[2048];
//	int tmpsize;
//	char *ptr, *ptrsize;
//	char fail_reason = 0;
//
//	if ( actor_index < 0 || actor_index >= g_maxactornum )
//		return -1;
//
//	club_index = g_actors[actor_index].club_index;
//	// û�м����
//	if ( club_index <= 0 || club_index >= MAX_CLUB_COUNT )
//		return -1;
//	// û��Ȩ�޵�
//	if ( club_place_right( club_index, g_actors[actor_index].club_place, RIGHT_INVITE_MEMBER ) == -1 )
//	{
//		club_send_result( actor_index, CLUBMSG_POWER_NONE );
//		return -1;
//	}
//
//	club_level = g_club[club_index].m_level;
//	// ����̫��,�Ӳ�����
//	if ( g_club[club_index].m_membercount >= club_get_member_max( club_index ) )
//	{
//		club_send_result( actor_index, CLUBMSG_MEMBER_FULL );
//		return -1;
//	}
//
//	ptr = tmpbuf;
//	tmpsize = 0;
//	ptr += sizeof(short);
//	*(short *)ptr = CMDS_OPENINVITEDLG;		ptr += sizeof(short); tmpsize += sizeof(short);
//	ptrsize = ptr;								ptr += sizeof(short); tmpsize += sizeof(short);
//
//	*(char *)ptr = fail_reason;	ptr += sizeof(char); tmpsize += sizeof(char);
//	*(short *)ptrsize = (short)tmpsize - sizeof(short)* 2;
//	*(unsigned short *)tmpbuf = tmpsize;
//
//	sendtoqueue( actor_index, tmpbuf, tmpsize + sizeof(short) );
//	write_gamelog( "[size:%d][<-CMDS_OPENINVITEDLG][%d]", tmpsize, g_actors[actor_index].id );
//	return 0;
//}
//
//
//// clubclass < 0��ʱ���ʾ����
//int club_memberproc( int actor_index, int actorid, char clubclass )
//{
//	int		tmpi;
//	char	szSQL[1024];
//	MYSQL_RES	*res;
//	MYSQL_ROW	row;
//	int right = 0;
//	char place = 0;
//	char actorname[64];
//	int target_index;
//	int ret;
//
//	short club_index;
//	if ( actor_index < 0 || actor_index >= g_maxactornum )
//		return -1;
//	club_index = g_actors[actor_index].club_index;
//	// û�м����
//	if ( club_index <= 0 || club_index >= MAX_CLUB_COUNT )
//		return -1;
//	target_index = actors_getindex_withid( actorid );
//	// �����ݿ����ҵ������Ա
//	sprintf( szSQL, "select clubclass , name from club_member where actorid='%d' and clubid='%d'", actorid, g_club[club_index].m_clubid );
//	if ( mysql_query( myGame, szSQL ) )
//	{
//		printf( "Query failed (%s)\n", mysql_error( myGame ) );
//		write_gamelog( "%s", szSQL );
//		return -1;
//	}
//	res = mysql_store_result( myGame );
//
//	if ( (row = mysql_fetch_row( res )) )
//	{
//		if ( row[0] )
//			place = (char)atoi( row[0] );
//		if ( row[1] )
//			strcpy( actorname, row[1] );
//	}
//	mysql_free_result( res );
//	// �ж��Ƿ����峤��ֻ���峤��ְλ����Ȩ�������������˵�Ȩ�޳��⣩
//	if ( clubclass > 0 )
//	{
//		if ( place == CLUBPLACE_LEADER )
//		{
//			club_send_result( actor_index, CLUBMSG_POWER_NONE );
//			return 0;
//		}
//		if ( clubclass == CLUBPLACE_BUSINESSMAN || (place == CLUBPLACE_BUSINESSMAN && clubclass == CLUBPLACE_NORMAL) )
//		{ // ��������ˣ��ж�Ȩ�ޣ�
//			if ( g_actors[actor_index].club_place != CLUBPLACE_LEADER &&
//				club_place_right( club_index, g_actors[actor_index].club_place, RIGHT_BUSINESSMAN ) == -1 )
//			{
//				club_send_result( actor_index, CLUBMSG_POWER_NONE );
//				return 0;
//			}
//		}
//		else
//		{ // ����ְλ
//			if ( g_actors[actor_index].club_place != CLUBPLACE_LEADER )
//			{
//				club_send_result( actor_index, CLUBMSG_POWER_NONE );
//				return 0;
//			}
//		}
//	}
//
//	if ( g_actors[actor_index].id == actorid )
//	{
//		club_send_result( actor_index, CLUBMSG_PROC_MYSELF );
//		return 0;
//	}
//
//	if ( clubclass < 0 )
//	{
//		if ( club_place_right( club_index, g_actors[actor_index].club_place, RIGHT_KICK_MEMBER ) == -1 )
//		{
//			club_send_result( actor_index, CLUBMSG_POWER_NONE );
//			return 0;
//		}
//		if ( clubmelee_getstate( ) == CLUBMELEE_STATE_ENTER ||
//			clubmelee_getstate( ) == CLUBMELEE_STATE_FIGHT )
//		{
//			char szMsg[128];
//			sprintf( szMsg, "��������ڼ��޷�����!" );
//			actor_notify( actor_index, 0, (short)strlen( szMsg ), szMsg );
//			return 0;
//		}
//		if ( place > g_actors[actor_index].club_place )
//		{
//			club_send_result( actor_index, CLUBMSG_POWER_NONE );
//			return 0;
//		}
//		if ( place > 1 )
//		{
//			ACTOR_NORMAL_NOTIFY_RETURN( actor_index, "���Ƚ�����Ա��ְ", -1 );
//		}
//
//		// ɾ��һ����Ա���������
//		// ����ֻ��ɾ����ͨ����Ա,��������Ҫ�Ƚ�ְ�ſ���
//		sprintf( szSQL, "delete from club_member where actorid='%d' and clubid='%d' and clubclass='1'", actorid, g_club[club_index].m_clubid );
//		if ( mysql_query( myGame, szSQL ) )
//		{
//			printf( "Query failed (%s)\n", mysql_error( myGame ) );
//			write_gamelog( "%s", szSQL );
//			return -1;
//		}
//		if ( mysql_affected_rows( myGame ) <= 0 )
//			return -1;
//		if ( target_index >= 0 && target_index < g_maxactornum )
//		{
//			for ( tmpi = 0; tmpi < MAX_MEMBER_COUNT; tmpi++ )
//			{
//				if ( g_club[club_index].m_online_member[tmpi] == target_index )
//				{
//					g_club[club_index].m_online_member[tmpi] = -1;
//					break;
//				}
//			}
//		}
//		// ��ʧ1����Ա&w
//		g_club[club_index].m_membercount--;
//
//		club_add_event( g_club[club_index].m_clubid, actorid, actorname, 0, 0, CLUBEVENT_LEAVE );
//		system_cmd( actorid, SYSTEMCMD_CLUB_LEAVE, g_club[club_index].m_clubid, 1 );
//		ACTOR_NORMAL_NOTIFY( actor_index, "�����ɹ�" );
//		club_send_result( actor_index, CLUBMSG_PROC_OK );
//
//		// begin �������ʱ�Ĺ��� [11/3/2009 yxx]
//		{
//			char szMsg[256];
//			int szLen;
//			sprintf( szMsg, "��%s������%s�������˼��壡", actorname, g_actors[actor_index].player_attr.name );
//			szLen = (int)strlen( szMsg );
//			actors_talk( actor_index, CHANNEL_TYPE_ORG, szMsg, szLen, 0 );
//		}
//		// end [11/3/2009 yxx]
//
//		return 0;
//	}
//
//	ret = club_change_class( club_index, actorid, clubclass );
//	if ( ret == -2 )
//	{
//		// û�и����ְλ��
//		club_send_result( actor_index, CLUBMSG_PROC_FULL );
//		return -1;
//	}
//	else if ( ret == 0 )
//	{
//		ACTOR_NORMAL_NOTIFY( actor_index, "ְλ�����ɹ�" );
//		club_memberlist( actor_index, 0 );
//	}
//
//	club_send_result( actor_index, CLUBMSG_PROC_OK );
//
//	return 0;
//}
//
//// ȡ�ü����¼�
//int club_get_event( int actor_index, short pageno )
//{
//	MYSQL_RES		*res;
//	MYSQL_ROW		row;
//	char	szSQL[1024];
//
//	char tmpbuf[2048];
//	int tmpsize;
//	char *ptr, *ptrsize;
//
//	short begin = pageno*CLUB_EVENT_PAGECOUNT;
//	//	short count = 0;
//
//	short club_index;
//
//	if ( actor_index < 0 || actor_index >= g_maxactornum )
//		return -1;
//	club_index = g_actors[actor_index].club_index;
//	if ( club_index <= 0 || g_actors[actor_index].club_place <= 0 )
//		return -1;
//
//	// ������Ϣ���Լ�
//	ptr = tmpbuf;
//	tmpsize = 0;
//	ptr += sizeof(short);
//	*(short *)ptr = CMDS_CLUBEVENT;	ptr += sizeof(short); tmpsize += sizeof(short);
//	ptrsize = ptr;					ptr += sizeof(short); tmpsize += sizeof(short);
//
//	sprintf( szSQL, "select actorid,name,otherid,othername,event,happentime from club_event where clubid='%d' order by happentime desc limit %d,%d", g_club[club_index].m_clubid, begin, CLUB_EVENT_PAGECOUNT );
//	if ( mysql_query( myGame, szSQL ) )
//	{
//		printf( "Query failed (%s)\n", mysql_error( myGame ) );
//		write_gamelog( "%s", szSQL );
//		return -1;
//	}
//	res = mysql_store_result( myGame );
//	while ( (row = mysql_fetch_row( res )) )
//	{
//		*(int*)ptr = atoi( row[0] ); ptr += sizeof(int); tmpsize += sizeof(int);
//		memcpy( ptr, row[1], NAME_SIZE*sizeof(char) ); ptr += NAME_SIZE*sizeof(char); tmpsize += NAME_SIZE*sizeof(char);
//		*(int*)ptr = atoi( row[2] ); ptr += sizeof(int); tmpsize += sizeof(int);
//		memcpy( ptr, row[3], NAME_SIZE*sizeof(char) ); ptr += NAME_SIZE*sizeof(char); tmpsize += NAME_SIZE*sizeof(char);
//		*ptr = (char)atoi( row[4] ); ptr += sizeof(char); tmpsize += sizeof(char);
//		*(int*)ptr = atoi( row[5] ); ptr += sizeof(int); tmpsize += sizeof(int);
//	}
//	mysql_free_result( res );
//
//	*(int *)ptr = -1;		ptr += sizeof(int); tmpsize += sizeof(int);
//
//	*(short *)ptrsize = (short)tmpsize - sizeof(short)* 2;
//	*(unsigned short *)tmpbuf = tmpsize;
//
//	sendtoqueue( actor_index, tmpbuf, tmpsize + sizeof(short) );
//	write_gamelog( "[size:%d][<-CMDS_CLUBEVENT][%d]", tmpsize, g_actors[actor_index].id );
//	return 0;
//}
//
//int club_invitememeber( int actor_index, int actorID )
//{
//	int target_index;
//	char tmpbuf[2048];
//	int tmpsize;
//	char *ptr, *ptrsize;
//	short club_index;
//	char szMsg[256] = { 0 };
//
//	if ( actor_index < 0 || actor_index >= g_maxactornum )
//		return -1;
//	if ( g_actors[actor_index].id == actorID )
//		return -1;
//
//	if ( g_frame_count - g_actors[actor_index].last_invite_frame > 0 &&
//		g_frame_count - g_actors[actor_index].last_invite_frame < 90 )
//		return -1;
//
//	g_actors[actor_index].last_invite_frame = g_frame_count;
//
//	club_index = g_actors[actor_index].club_index;
//
//	if ( club_place_right( club_index, g_actors[actor_index].club_place, RIGHT_INVITE_MEMBER ) < 0 )
//	{
//		club_send_result( actor_index, CLUBMSG_POWER_NONE );
//		return -1;
//	}
//
//	if ( g_club[club_index].m_membercount >= club_get_member_max( club_index ) )
//	{
//		club_send_result( actor_index, CLUBMSG_MEMBER_FULL );
//		return -1;
//	}
//
//	target_index = actors_getindex_withid( actorID );
//	if ( target_index < 0 )
//		return -1;
//	if ( g_actors[target_index].player_attr.level < 20 )
//	{
//		sprintf( szMsg, "�Է��ȼ�����" );
//		actor_notify( actor_index, 0, (short)strlen( szMsg ), szMsg );
//		return -1;
//	}
//	if ( g_actors[target_index].club_index > 0 && g_actors[target_index].club_place > 0 )
//	{
//		club_send_result( actor_index, CLUBMSG_HAS_CLUB );
//		return -1;
//	}
//
//	ptr = tmpbuf;
//	tmpsize = 0;
//	ptr += sizeof(short);
//	*(short *)ptr = CMDS_ASKINVITEMEMBER;		ptr += sizeof(short); tmpsize += sizeof(short);
//	ptrsize = ptr;								ptr += sizeof(short); tmpsize += sizeof(short);
//
//	*(int *)ptr = g_actors[actor_index].id;	ptr += sizeof(int); tmpsize += sizeof(int);
//	memcpy( ptr, g_actors[actor_index].player_attr.name, NAME_SIZE ); ptr += NAME_SIZE; tmpsize += NAME_SIZE;
//	*(short *)ptr = g_club[club_index].m_clubid; ptr += sizeof(short); tmpsize += sizeof(short);
//	memcpy( ptr, g_club[g_actors[actor_index].club_index].m_clubname, NAME_SIZE ); ptr += NAME_SIZE; tmpsize += NAME_SIZE;
//
//	*(short *)ptrsize = (short)tmpsize - sizeof(short)* 2;
//	*(unsigned short *)tmpbuf = tmpsize;
//
//	sendtoqueue( target_index, tmpbuf, tmpsize + sizeof(short) );
//	write_gamelog( "[size:%d][<-CMDS_ASKINVITEMEMBER][%d]", tmpsize, g_actors[actor_index].id );
//	return 0;
//}
//
//// ������߼�����1���ӵļ��ִ��-
//void club_logic( )
//{
//	static int time_stamp;
//
//	int tmpi, tmpj;
//	for ( tmpi = 0; tmpi < MAX_CLUB_COUNT; tmpi++ )
//	{
//		if ( g_club[tmpi].m_clubid > 0 )
//		{
//			club_check_current_week( tmpi );
//		}
//
//	}
//}
//
//
//int club_place_right( short club_index, char place, int right )
//{
//	if ( place == CLUBPLACE_LEADER )
//		return 0;
//	else if ( place <= 1 )
//		return -1;
//	else if ( place == CLUBPLACE_FLEADER )
//	{
//		if ( g_club[club_index].m_fleaderight & right )
//			return 0;
//	}
//	else if ( place == CLUBPLACE_DIRECTOR )
//	{
//		if ( g_club[club_index].m_directoright & right )
//			return 0;
//	}
//	else if ( place == CLUBPLACE_MINISTER )
//	{
//		if ( g_club[club_index].m_ministeright & right )
//			return 0;
//	}
//	else if ( place == CLUBPLACE_SEC )
//	{
//		if ( g_club[club_index].m_secright & right )
//			return 0;
//	}
//	else
//		return -1;
//
//	return -1;
//}
//
//
//// �򿪼���Ĳֿ�
//int club_open_storage( int actor_index, char type )
//{
//	int tmpi;
//
//	char tmpbuf[2048];
//	int tmpsize;
//	char *ptr, *ptrsize;
//	int club_index;
//
//	if ( actor_index < 0 || actor_index >= g_maxactornum )
//		return -1;
//
//	club_index = g_actors[actor_index].club_index;
//
//	if ( club_index <= 0 || club_index >= MAX_CLUB_COUNT )
//		return -1;
//	// ���ֿ����ݴ��浽�ֲ�����
//	ptr = tmpbuf;
//	tmpsize = 0;
//	ptr += sizeof(short);
//	*(short *)ptr = CMDS_OPENCLUBSTORAGE;		ptr += sizeof(short); tmpsize += sizeof(short);
//	ptrsize = ptr;								ptr += sizeof(short); tmpsize += sizeof(short);
//
//	*(short *)ptr = g_club[club_index].m_clubid;	ptr += sizeof(short); tmpsize += sizeof(short);			// ����id
//	*(char *)ptr = g_club[club_index].m_level;	ptr += sizeof(char); tmpsize += sizeof(char);		// �ֿ⼶��
//	*(char *)ptr = type;						ptr += sizeof(char); tmpsize += sizeof(char);		// ���� 0�ǼĴ棬1��ȡ��
//	if ( type != 0 )
//	{
//		// �����ȡ�أ����زֿ��������Ʒ�б�
//		for ( tmpi = 0; tmpi < CLUB_STORAGE_PAGECOUNT * g_club[club_index].m_level; tmpi++ )
//		{
//			*(int *)ptr = g_club[club_index].m_storage[tmpi].m_kind;	ptr += sizeof(int);	tmpsize += sizeof(int);
//			*(short *)ptr = g_club[club_index].m_storage[tmpi].m_num;	ptr += sizeof(short);	tmpsize += sizeof(short);
//		}
//	}
//
//	*(short *)ptrsize = (short)tmpsize - sizeof(short)* 2;
//	*(unsigned short *)tmpbuf = tmpsize;
//
//	sendtoqueue( actor_index, tmpbuf, tmpsize + sizeof(short) );
//	write_gamelog( "[size:%d][<-CMDS_OPENCLUBSTORAGE][%d]", tmpsize, g_actors[actor_index].id );
//
//	return 0;
//}
//
//// �򿪼��������
//int club_open_bank( int actor_index, char type )
//{
//	char tmpbuf[2048];
//	int tmpsize;
//	char *ptr, *ptrsize;
//	short club_index;
//
//	if ( actor_index < 0 || actor_index >= g_maxactornum )
//		return -1;
//
//	club_index = g_actors[actor_index].club_index;
//
//	if ( club_index <= 0 || club_index >= MAX_CLUB_COUNT )
//		return -1;
//	if ( g_club[club_index].m_clubid <= 0 )
//		return -1;
//	// ����ɫ�ƶ����͵����򻺳�
//	ptr = tmpbuf;
//	tmpsize = 0;
//	ptr += sizeof(short);
//	*(short *)ptr = CMDS_OPENCLUBBANK;				ptr += sizeof(short); tmpsize += sizeof(short);
//	ptrsize = ptr;								ptr += sizeof(short); tmpsize += sizeof(short);
//
//	*(short *)ptr = g_club[club_index].m_clubid;	ptr += sizeof(short); tmpsize += sizeof(short);		// ����id
//	*(char *)ptr = type;					ptr += sizeof(char); tmpsize += sizeof(char);		// ���� 0�Ǵ棬1��ȡ
//	*(int *)ptr = g_club[club_index].m_money;	ptr += sizeof(int); tmpsize += sizeof(int);		// ����ĵ�ǰ�ʽ���	
//
//	*(short *)ptrsize = (short)tmpsize - sizeof(short)* 2;
//	*(unsigned short *)tmpbuf = tmpsize;
//
//	sendtoqueue( actor_index, tmpbuf, tmpsize + sizeof(short) );
//	write_gamelog( "[size:%d][<-CMDS_OPENCLUBBANK][%d]", tmpsize, g_actors[actor_index].id );
//
//	return 0;
//}
//
//// �Ĵ����Ĳֿ�
//int club_storagein( int actor_index, short clubid, short item_sel, short item_num )
//{
//	char tmpbuf[1024];
//	int tmpsize;
//
//	char *ptr, *ptrsize;
//
//	int tmpi;
//	int firstemptypos, emptypos;
//	short club_index;
//
//	short itemtype;
//
//	if ( actor_index < 0 || actor_index >= g_maxactornum )
//		return -1;
//
//	// ս���в�����
//	if ( g_actors[actor_index].stat == ACTOR_STAT_FIGHT )
//		return -1;
//
//	club_index = club_getindex( clubid );
//	if ( club_index <= 0 || club_index >= MAX_CLUB_COUNT )
//		return -1;
//
//	if ( item_sel < 0 || item_sel >= CLUB_STORAGE_PAGECOUNT * g_club[club_index].m_level )
//		return -1;
//
//	// ������Ʒ�����������Ƿ���ȷ
//	if ( item_num > g_actors[actor_index].item[item_sel].m_num || item_num <= 0 )
//		return -1;
//	if ( g_actors[actor_index].item[item_sel].m_kind <= 0 || g_actors[actor_index].item[item_sel].m_num <= 0 )
//		return -1;
//	if ( g_actors[actor_index].item[item_sel].m_binded )
//	{
//		char szMsg[128] = { 0 };
//		sprintf( szMsg, "��������ҽ�����Ʒ���ܷ�������" );
//		actor_notify( actor_index, 0, (short)strlen( szMsg ), szMsg );
//		return -1;
//	}
//	if ( g_actors[actor_index].item[item_sel].m_locked )
//	{
//		char szMsg[128] = { 0 };
//		sprintf( szMsg, "������Ʒ���ܷ�������" );
//		actor_notify( actor_index, 0, (short)strlen( szMsg ), szMsg );
//		return -1;
//	}
//	itemtype = item_gettype( g_actors[actor_index].item[item_sel].m_kind );
//	if ( itemtype >= ITEM_TYPE_QUEST_NORMAL && itemtype < ITEM_TYPE_QUEST_NORMAL + 10 )
//	{
//		char szMsg[128] = { 0 };
//		sprintf( szMsg, "����Ʒ���ܷ�������" );
//		actor_notify( actor_index, 0, (short)strlen( szMsg ), szMsg );
//		return -1;
//	}
//	// ���ɶ����ĵ��߲��ܷ���
//	if ( item_undiscardable( actor_index, item_sel ) )
//	{
//		char szMsg[128] = { 0 };
//		sprintf( szMsg, "����Ʒ���ܷ�������" );
//		actor_notify( actor_index, 0, (short)strlen( szMsg ), szMsg );
//		return -1;
//	}
//	// Ҳ�����������
//	if ( item_isquest_item( actor_index, item_sel ) )
//		return -1;
//
//	// Ѱ�Ҳֿ�Ŀո�
//	firstemptypos = -1;
//	emptypos = -1;
//	for ( tmpi = 0; tmpi < CLUB_STORAGE_PAGECOUNT * g_club[club_index].m_level; tmpi++ )
//	{
//		if ( g_club[club_index].m_storage[tmpi].m_kind <= 0 )
//		{
//			if ( firstemptypos < 0 )
//			{
//				firstemptypos = tmpi;
//			}
//			continue;
//		}
//		if ( g_club[club_index].m_storage[tmpi].m_kind == g_actors[actor_index].item[item_sel].m_kind )
//		{
//			// ����������Ʒ�����Ҳ��������ص���Ŀ
//			if ( g_club[club_index].m_storage[tmpi].m_num + item_num <= item_get_overlap( g_actors[actor_index].item[item_sel].m_kind ) )
//			{
//				emptypos = tmpi;
//				break;
//			}
//		}
//	}
//	if ( emptypos < 0 )
//		emptypos = firstemptypos;
//	if ( emptypos < 0 )
//	{
//		goto STORAGEIN_SENDNOW;
//	}
//
//	// ������Ʒ������Ʒ
//	g_actors[actor_index].item[item_sel].m_num -= item_num;
//	// ���б�Ҫ������Ʒ��Ϣ������ֿ�
//	if ( g_club[club_index].m_storage[emptypos].m_kind <= 0 )
//	{
//		// ������Ʒ��Դ����յ�ʱ����Ҫ��ID.
//		memcpy( &g_club[club_index].m_storage[emptypos], &g_actors[actor_index].item[item_sel], sizeof(Item) );
//		g_club[club_index].m_storage[emptypos].m_num = item_num;
//		if ( g_actors[actor_index].item[item_sel].m_num > 0 )
//		{
//			// �°�����
//			club_newstorage_db( club_index, emptypos, 1 );
//		}
//		else
//		{
//			club_newstorage_db( club_index, emptypos, 0 );
//			// ɾ����Ʒ��
//			item_deletebox( actor_index, item_sel, 0 );
//			g_actors[actor_index].item[item_sel].m_id = 0;
//			g_actors[actor_index].item[item_sel].m_kind = 0;
//		}
//		write_gamelog( "�ֿ�%d��λ��%d����Ŀ��%d", club_index, emptypos, item_num );
//	}
//	else
//	{
//		if ( g_actors[actor_index].item[item_sel].m_num <= 0 )
//		{
//			// ɾ����Ʒ��
//			item_deletebox( actor_index, item_sel, 0 );
//			g_actors[actor_index].item[item_sel].m_id = 0;
//			g_actors[actor_index].item[item_sel].m_kind = 0;
//		}
//		// ���Ӱ����������Ʒ
//		g_club[club_index].m_storage[emptypos].m_num += item_num;
//	}
//
//	if ( itemtype == ITEM_TYPE_ROCK )
//		item_reset_burden( actor_index );
//
//STORAGEIN_SENDNOW:
//	// ���ͽ�ɫ�Ĵ�����Ʒ
//	ptr = tmpbuf;
//	tmpsize = 0;
//	ptr += sizeof(short);
//	*(short *)ptr = CMDS_OPCLUBSTORAGE;			ptr += sizeof(short); tmpsize += sizeof(short);
//	ptrsize = ptr;								ptr += sizeof(short); tmpsize += sizeof(short);
//
//	*(char *)ptr = 0;					ptr += sizeof(char); tmpsize += sizeof(char);		//  ���ͣ�0�Ĵ� 1ȡ��
//	*(short *)ptr = emptypos;					ptr += sizeof(short); tmpsize += sizeof(short);		// �Ǹ��ֿ��λ��
//	*(short *)ptr = item_sel;					ptr += sizeof(short); tmpsize += sizeof(short);		// �Ǹ����ӵ���Ʒ��
//	*(short *)ptr = item_num;					ptr += sizeof(short); tmpsize += sizeof(short);	// ��Ʒ����
//
//	*(short *)ptrsize = (short)tmpsize - sizeof(short)* 2;
//	*(unsigned short *)tmpbuf = tmpsize;
//
//	sendtoqueue( actor_index, tmpbuf, tmpsize + sizeof(short) );
//	write_gamelog( "[size:%d][<-CMDS_OPCLUBSTORAGE][%d]", tmpsize, g_actors[actor_index].id );
//	return 0;
//}
//
//// ȡ������Ĳֿ�
//int club_storageout( int actor_index, short clubid, short item_sel, short item_num )
//{
//	char tmpbuf[1024];
//	int tmpsize;
//
//	char *ptr, *ptrsize;
//
//	short club_index;
//	short itemtype;
//
//	int tmpi;
//	int firstemptypos, emptypos;
//
//	if ( actor_index < 0 || actor_index >= g_maxactornum )
//		return -1;
//
//	// ս���в�����
//	if ( g_actors[actor_index].stat == ACTOR_STAT_FIGHT )
//		return -1;
//
//	club_index = club_getindex( clubid );
//	if ( club_index <= 0 || club_index >= MAX_CLUB_COUNT )
//		return -1;
//
//	if ( item_sel < 0 || item_sel >= CLUB_STORAGE_PAGECOUNT * g_club[club_index].m_level )
//		return -1;
//
//	// ������Ʒ�����������Ƿ���ȷ
//	if ( item_num > g_club[club_index].m_storage[item_sel].m_num || item_num <= 0 )
//		return -1;
//	if ( g_club[club_index].m_storage[item_sel].m_kind <= 0 || g_club[club_index].m_storage[item_sel].m_num <= 0 )
//		return -1;
//
//	itemtype = item_gettype( g_club[club_index].m_storage[item_sel].m_kind );
//
//	// Ѱ����Ʒ���Ŀո�
//	firstemptypos = -1;
//	emptypos = -1;
//	for ( tmpi = 0; tmpi < MAX_ACTOR_ITEMNUM; tmpi++ )
//	{
//		if ( g_actors[actor_index].item[tmpi].m_kind <= 0 )
//		{
//			if ( firstemptypos < 0 )
//			{
//				firstemptypos = tmpi;
//			}
//			continue;
//		}
//		if ( g_actors[actor_index].item[tmpi].m_kind == g_club[club_index].m_storage[item_sel].m_kind )
//		{
//			// ����������Ʒ�����Ҳ��������ص���Ŀ
//			if ( g_actors[actor_index].item[tmpi].m_num + item_num <= item_get_overlap( g_actors[actor_index].item[tmpi].m_kind ) )
//			{
//				emptypos = tmpi;
//				break;
//			}
//		}
//	}
//	if ( emptypos < 0 )
//		emptypos = firstemptypos;
//	if ( emptypos < 0 )		// �޷�Я��������Ʒ
//	{
//		goto STORAGEOUT_SENDNOW;
//	}
//
//	// ���ٲֿ����Ʒ
//	g_club[club_index].m_storage[item_sel].m_num -= item_num;
//	// ���б�Ҫ������Ʒ��Ϣ������
//	if ( g_actors[actor_index].item[emptypos].m_kind <= 0 )
//	{
//		// ��Ʒ������û�����
//		memcpy( &g_actors[actor_index].item[emptypos], &g_club[club_index].m_storage[item_sel], sizeof(Item) );
//
//		// ����Ʒ��
//		item_newbox( actor_index, emptypos, 0 );
//		if ( g_club[club_index].m_storage[item_sel].m_num <= 0 )
//		{
//			g_club[club_index].m_storage[item_sel].m_id = 0;
//			g_club[club_index].m_storage[item_sel].m_kind = 0;
//			club_deletestorage_db( club_index, item_sel );
//		}
//		g_actors[actor_index].item[emptypos].m_num = item_num;
//	}
//	else
//	{
//		// �ֿ������ˣ��������Ʒ��Ϣ
//		if ( g_club[club_index].m_storage[item_sel].m_num <= 0 )
//		{
//			// ɾ��������
//			club_deletestorage_db( club_index, item_sel );
//			g_club[club_index].m_storage[item_sel].m_id = 0;
//			g_club[club_index].m_storage[item_sel].m_kind = 0;
//		}
//		// ������Ʒ���������Ʒ
//		g_actors[actor_index].item[emptypos].m_num += item_num;
//	}
//	if ( itemtype == ITEM_TYPE_ROCK )
//		item_reset_burden( actor_index );
//
//STORAGEOUT_SENDNOW:
//	// ���ͽ�ɫȡ������Ʒ
//	ptr = tmpbuf;
//	tmpsize = 0;
//	ptr += sizeof(short);
//	*(short *)ptr = CMDS_OPCLUBSTORAGE;			ptr += sizeof(short); tmpsize += sizeof(short);
//	ptrsize = ptr;								ptr += sizeof(short); tmpsize += sizeof(short);
//
//	*(char *)ptr = 1;					ptr += sizeof(char); tmpsize += sizeof(char);		//  ���ͣ�0�Ĵ� 1ȡ��
//	*(short *)ptr = item_sel;					ptr += sizeof(short); tmpsize += sizeof(short);		// �Ǹ�����λ��
//	*(short *)ptr = emptypos;					ptr += sizeof(short); tmpsize += sizeof(short);		// �Ǹ����ӵ���Ʒ��
//	*(short *)ptr = item_num;					ptr += sizeof(short); tmpsize += sizeof(short);	// ��Ʒ����
//	*(short *)ptr = item_gettype( g_actors[actor_index].item[emptypos].m_kind );	ptr += sizeof(short); tmpsize += sizeof(short);	// ȡ����Ʒ����Ҫ�����Ʒ����
//	*(short *)ptr = item_getsituation( g_actors[actor_index].item[emptypos].m_kind ); ptr += sizeof(short); tmpsize += sizeof(short); // ȡ����Ʒ����Ҫ�����Ʒʹ��״̬
//	*(char *)ptr = item_geteffobject( g_actors[actor_index].item[emptypos].m_kind ); ptr += sizeof(char); tmpsize += sizeof(char); // ȡ����Ʒ����Ҫ�����Ʒ�����ö���
//
//	*(short *)ptrsize = (short)tmpsize - sizeof(short)* 2;
//	*(unsigned short *)tmpbuf = tmpsize;
//
//	sendtoqueue( actor_index, tmpbuf, tmpsize + sizeof(short) );
//	write_gamelog( "[size:%d][<-CMDS_OPCLUBSTORAGE][%d]", tmpsize, g_actors[actor_index].id );
//	return 0;
//}
//
//
//// ����һ������ֿ����ݿ��¼
//int club_newstorage_db( short club_index, int item_offset, char fNewid )
//{
//	char	szSQL[2048];
//	short clubid;
//	char bigint[21];
//
//	int tmpi;
//	MYSQL_RES		*res;
//	MYSQL_ROW		row;
//
//	Item *pItem;
//
//	if ( club_index <= 0 || club_index >= MAX_CLUB_COUNT )
//		return -1;
//	clubid = g_club[club_index].m_clubid;
//	if ( clubid <= 0 )
//		return -1;
//
//	pItem = &g_club[club_index].m_storage[item_offset];
//	if ( pItem->m_kind <= 0 )
//		return -1;
//
//	for ( tmpi = 0; tmpi < 10; tmpi++ )
//	{
//		// 0:���ı�id�� 1:����id
//		if ( fNewid )
//		{
//			pItem->m_id = g_maxitemid;
//			g_maxitemid++;
//		}
//
//		lltoa( pItem->m_id, bigint, 10 );
//		sprintf( szSQL, "insert into club_storage ( itemid,clubid,offset,kind,num,binded,life_time,"
//			"ability1,value1,ability2,value2,ability3,value3,ability4,value4,ability5,value5,ability6,value6,ability7,value7,ability8,value8,ability9,value9,ability10,value10, "
//			"base_attr1, base_attr1_value,"
//			"base_attr2, base_attr2_value,"
//			"base_attr3, base_attr3_value,"
//			"base_attr4, base_attr4_value,"
//			"stone_attr1, stone_attr1_value,"
//			"stone_attr2, stone_attr2_value,"
//			"stone_attr3, stone_attr3_value,"
//			"stone_attr4, stone_attr4_value,"
//			"countermark_attr1,countermark_attr1_value,"
//			"countermark_attr2,countermark_attr2_value,"
//			"countermark_attr3,countermark_attr3_value,"
//			"countermark_attr4,countermark_attr4_value )"
//			" values('%s','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d'"
//			" %d,%d,%d,%d,%d,%d,%d,%d,"
//			" %d,%d,%d,%d,%d,%d,%d,%d,"
//			" %d,%d,%d,%d,%d,%d,%d,%d )",
//
//			bigint, clubid, item_offset, pItem->m_kind, \
//			pItem->m_num, pItem->m_binded, pItem->m_life_time, \
//			pItem->m_ability[0], pItem->m_value[0], \
//			pItem->m_ability[1], pItem->m_value[1], \
//			pItem->m_ability[2], pItem->m_value[2], \
//			pItem->m_ability[3], pItem->m_value[3], \
//			pItem->m_ability[4], pItem->m_value[4], \
//			pItem->m_ability[5], pItem->m_value[5], \
//			pItem->m_ability[6], pItem->m_value[6], \
//			pItem->m_ability[7], pItem->m_value[7], \
//			pItem->m_ability[8], pItem->m_value[8], \
//			pItem->m_ability[9], pItem->m_value[9],
//
//			pItem->m_baseattr[0], pItem->m_baseattr_value[0],
//			pItem->m_baseattr[1], pItem->m_baseattr_value[1],
//			pItem->m_baseattr[2], pItem->m_baseattr_value[2],
//			pItem->m_baseattr[3], pItem->m_baseattr_value[3],
//
//			pItem->m_stoneattr[0], pItem->m_stoneattr_value[0],
//			pItem->m_stoneattr[1], pItem->m_stoneattr_value[1],
//			pItem->m_stoneattr[2], pItem->m_stoneattr_value[2],
//			pItem->m_stoneattr[3], pItem->m_stoneattr_value[3],
//
//			pItem->m_countermarkattr[0], pItem->m_countermarkattr_value[0],
//			pItem->m_countermarkattr[1], pItem->m_countermarkattr_value[1],
//			pItem->m_countermarkattr[2], pItem->m_countermarkattr_value[2],
//			pItem->m_countermarkattr[3], pItem->m_countermarkattr_value[3]
//			);
//		if ( mysql_query( myGame, szSQL ) )
//		{
//			write_gamelog( "Query failed (%s)\n", mysql_error( myGame ) );
//			write_gamelog( "[%d]%s", tmpi, szSQL );
//
//			// ����úõ���һ��
//			lltoa( pItem->m_id, bigint, 10 );
//			sprintf( szSQL, "select clubid,offset,kind,num from club_storage where itemid='%s'", bigint );
//			if ( !mysql_query( myGame, szSQL ) )
//			{
//				res = mysql_store_result( myGame );
//				if ( (row = mysql_fetch_row( res )) )
//				{
//					write_gamelog( "clubid:%s offset:%s kind:%s num:%s", row[0], row[1], row[2], row[3] );
//				}
//				mysql_free_result( res );
//			}
//			continue;
//			//			return -1;
//		}
//		break;
//	}
//	return 0;
//}
//
//int club_deletestorage_db( short club_index, int item_offset )
//{
//	char szSQL[1024];
//	char bigint[21];
//
//	Item *pItem;
//
//	if ( club_index <= 0 || club_index >= MAX_CLUB_COUNT )
//		return -1;
//
//	pItem = &g_club[club_index].m_storage[item_offset];
//
//	lltoa( pItem->m_id, bigint, 10 );
//	sprintf( szSQL, "delete from club_storage where itemid='%s'", bigint );
//	if ( mysql_query( myGame, szSQL ) )
//	{
//		printf( "Query failed (%s)\n", mysql_error( myGame ) );
//		write_gamelog( "%s", szSQL );
//		return -1;
//	}
//	return 0;
//}
//
//
//int club_storage_load( short club_index )
//{
//	MYSQL_RES		*res;
//	MYSQL_ROW		row;
//	char	szSQL[1024];
//	int count, tmpi;
//	Item *pItem;
//
//	int clubid;
//	if ( club_index <= 0 || club_index >= MAX_CLUB_COUNT )
//		return -1;
//	clubid = g_club[club_index].m_clubid;
//	if ( clubid <= 0 )
//		return -1;
//
//	memset( g_club[club_index].m_storage, 0, sizeof(Item)*CLUB_STORAGE_PAGECOUNT*MAX_CLUBLEVEL );
//
//	sprintf( szSQL, "select itemid,offset,kind,num,binded,life_time,wear,wear_max,oil,oil_max,ability1,value1,\
//										ability2,value2,ability3,value3,ability4,value4,ability5,value5,ability6,value6,ability7,value7,\
//															ability8,value8,ability9,value9,ability10,value10, "
//															"base_attr1, base_attr1_value,"
//															"base_attr2, base_attr2_value,"
//															"base_attr3, base_attr3_value,"
//															"base_attr4, base_attr4_value,"
//															"stone_attr1, stone_attr1_value,"
//															"stone_attr2, stone_attr2_value,"
//															"stone_attr3, stone_attr3_value,"
//															"stone_attr4, stone_attr4_value,"
//															"countermark_attr1, countermark_attr1_value,"
//															"countermark_attr2, countermark_attr2_value,"
//															"countermark_attr3, countermark_attr3_value,"
//															"countermark_attr4, countermark_attr4_value "
//															" from club_storage where clubid='%d'", clubid );
//	if ( mysql_query( myGame, szSQL ) )
//	{
//		printf( "Query failed (%s)\n", mysql_error( myGame ) );
//		write_gamelog( "%s", szSQL );
//		return -1;
//	}
//	res = mysql_store_result( myGame );
//
//	count = 0;
//	while ( (row = mysql_fetch_row( res )) )
//	{
//		pItem = g_club[club_index].m_storage;
//		tmpi = atoi( row[1] );
//		pItem[tmpi].m_id = atoll( row[0] );
//		pItem[tmpi].m_kind = atoi( row[2] );
//		pItem[tmpi].m_num = atoi( row[3] );
//		pItem[tmpi].m_binded = atoi( row[4] );
//		pItem[tmpi].m_life_time = atoi( row[5] );
//		pItem[tmpi].m_wear = atoi( row[6] );
//		pItem[tmpi].m_wear_max = atoi( row[7] );
//		pItem[tmpi].m_oil = atoi( row[8] );
//		pItem[tmpi].m_oil_max = atoi( row[9] );
//		pItem[tmpi].m_ability[0] = atoi( row[10] );
//		pItem[tmpi].m_value[0] = atoi( row[11] );
//		pItem[tmpi].m_ability[1] = atoi( row[12] );
//		pItem[tmpi].m_value[1] = atoi( row[13] );
//		pItem[tmpi].m_ability[2] = atoi( row[14] );
//		pItem[tmpi].m_value[2] = atoi( row[15] );
//		pItem[tmpi].m_ability[3] = atoi( row[16] );
//		pItem[tmpi].m_value[3] = atoi( row[17] );
//		pItem[tmpi].m_ability[4] = atoi( row[18] );
//		pItem[tmpi].m_value[4] = atoi( row[19] );
//		pItem[tmpi].m_ability[5] = atoi( row[20] );
//		pItem[tmpi].m_value[5] = atoi( row[21] );
//		pItem[tmpi].m_ability[6] = atoi( row[22] );
//		pItem[tmpi].m_value[6] = atoi( row[23] );
//		pItem[tmpi].m_ability[7] = atoi( row[24] );
//		pItem[tmpi].m_value[7] = atoi( row[25] );
//		pItem[tmpi].m_ability[8] = atoi( row[26] );
//		pItem[tmpi].m_value[8] = atoi( row[27] );
//		pItem[tmpi].m_ability[9] = atoi( row[28] );
//		pItem[tmpi].m_value[9] = atoi( row[29] );
//
//		pItem[tmpi].m_baseattr[0] = atoi( row[30] );
//		pItem[tmpi].m_baseattr_value[0] = atoi( row[31] );
//		pItem[tmpi].m_baseattr[1] = atoi( row[32] );
//		pItem[tmpi].m_baseattr_value[1] = atoi( row[33] );
//		pItem[tmpi].m_baseattr[2] = atoi( row[34] );
//		pItem[tmpi].m_baseattr_value[2] = atoi( row[35] );
//		pItem[tmpi].m_baseattr[3] = atoi( row[36] );
//		pItem[tmpi].m_baseattr_value[3] = atoi( row[37] );
//
//		pItem[tmpi].m_stoneattr[0] = atoi( row[38] );
//		pItem[tmpi].m_stoneattr_value[0] = atoi( row[39] );
//		pItem[tmpi].m_stoneattr[1] = atoi( row[40] );
//		pItem[tmpi].m_stoneattr_value[1] = atoi( row[41] );
//		pItem[tmpi].m_stoneattr[2] = atoi( row[42] );
//		pItem[tmpi].m_stoneattr_value[2] = atoi( row[43] );
//		pItem[tmpi].m_stoneattr[3] = atoi( row[44] );
//		pItem[tmpi].m_stoneattr_value[3] = atoi( row[45] );
//
//		pItem[tmpi].m_countermarkattr[0] = atoi( row[46] );
//		pItem[tmpi].m_countermarkattr_value[0] = atoi( row[47] );
//		pItem[tmpi].m_countermarkattr[1] = atoi( row[48] );
//		pItem[tmpi].m_countermarkattr_value[1] = atoi( row[49] );
//		pItem[tmpi].m_countermarkattr[2] = atoi( row[50] );
//		pItem[tmpi].m_countermarkattr_value[2] = atoi( row[51] );
//		pItem[tmpi].m_countermarkattr[3] = atoi( row[52] );
//		pItem[tmpi].m_countermarkattr_value[3] = atoi( row[53] );
//
//		count++;
//	}
//	mysql_free_result( res );
//
//	return count;
//}
//
//// �������д��
//int club_bankin( int actor_index, short clubid, int money )
//{
//	char tmpbuf[1024];
//	int tmpsize;
//
//	char *ptr, *ptrsize;
//
//	short club_index;
//
//	if ( actor_index < 0 || actor_index >= g_maxactornum )
//		return -1;
//
//	club_index = club_getindex( clubid );
//	if ( club_index <= 0 || club_index >= MAX_CLUB_COUNT )
//		return -1;
//
//	// ս���в�����
//	if ( g_actors[actor_index].stat == ACTOR_STAT_FIGHT )
//		return -1;
//
//	// �����Ǯ�����Ƿ���ȷ
//	if ( money > g_actors[actor_index].money || money <= 0 )
//		return -1;
//
//	// �������ϵĽ�Ǯ
//	g_actors[actor_index].money -= money;
//	// �������е�Ǯ
//	g_club[club_index].m_money += money;
//
//	// ���ͽ�ɫ�Ѿ���Ǯ
//	ptr = tmpbuf;
//	tmpsize = 0;
//	ptr += sizeof(short);
//	*(short *)ptr = CMDS_OPCLUBBANK;			ptr += sizeof(short); tmpsize += sizeof(short);
//	ptrsize = ptr;								ptr += sizeof(short); tmpsize += sizeof(short);
//
//	*(char *)ptr = 0;					ptr += sizeof(char); tmpsize += sizeof(char);		//  ���ͣ�0�Ĵ� 1ȡ��
//	*(int *)ptr = money;					ptr += sizeof(int); tmpsize += sizeof(int);	// Ǯ��
//
//	*(short *)ptrsize = (short)tmpsize - sizeof(short)* 2;
//	*(unsigned short *)tmpbuf = tmpsize;
//
//	sendtoqueue( actor_index, tmpbuf, tmpsize + sizeof(short) );
//	write_gamelog( "[size:%d][<-CMDS_OPCLUBBANK][%d]", tmpsize, g_actors[actor_index].id );
//	return 0;
//}
//
//// ��������ȡ��
//int club_bankout( int actor_index, short clubid, int money )
//{
//	char tmpbuf[1024];
//	int tmpsize;
//
//	char *ptr, *ptrsize;
//
//	short club_index;
//
//	if ( actor_index < 0 || actor_index >= g_maxactornum )
//		return -1;
//
//	club_index = club_getindex( clubid );
//	if ( club_index <= 0 || club_index >= MAX_CLUB_COUNT )
//		return -1;
//
//	// ս���в�����
//	if ( g_actors[actor_index].stat == ACTOR_STAT_FIGHT )
//		return -1;
//
//	// �����Ǯ�����Ƿ���ȷ
//	if ( money > g_club[club_index].m_money || money <= 0 )
//		return -1;
//
//	// �������е�Ǯ
//	g_club[club_index].m_money -= money;
//	// �������ϵ�Ǯ
//	g_actors[actor_index].money += money;
//
//	// ���ͽ�ɫȡ������Ʒ
//	ptr = tmpbuf;
//	tmpsize = 0;
//	ptr += sizeof(short);
//	*(short *)ptr = CMDS_OPCLUBBANK;			ptr += sizeof(short); tmpsize += sizeof(short);
//	ptrsize = ptr;								ptr += sizeof(short); tmpsize += sizeof(short);
//
//	*(char *)ptr = 1;					ptr += sizeof(char); tmpsize += sizeof(char);		//  ���ͣ�0�Ĵ� 1ȡ��
//	*(int *)ptr = money;					ptr += sizeof(int); tmpsize += sizeof(int);	// Ǯ��
//
//	*(short *)ptrsize = (short)tmpsize - sizeof(short)* 2;
//	*(unsigned short *)tmpbuf = tmpsize;
//
//	sendtoqueue( actor_index, tmpbuf, tmpsize + sizeof(short) );
//	write_gamelog( "[size:%d][<-CMDS_OPCLUBBANK][%d]", tmpsize, g_actors[actor_index].id );
//	return 0;
//}
//// ��������
//int club_upgrade( int actor_index )
//{
//	int club_index;
//	ACTOR_CHECK_INDEX( actor_index );
//	//if( g_actors[actor_index].club_place != CLUBPLACE_LEADER )
//	//	return CLUB_NOTLEADER;
//	//if( club_checkplace( actor_index ) == 0 )
//	//	return CLUB_NOTOWNER;
//	club_index = g_actors[actor_index].club_index;
//	CLUB_CHECK_INDEX( club_index );
//	if ( club_place_right( club_index, g_actors[actor_index].club_place, RIGHT_CLUB_BUILD ) == -1 )
//		return CLUB_POWER_NONE;
//	if ( club_index <= 0 || club_index >= MAX_CLUB_COUNT )
//		return CLUB_NOCLUB;
//	if ( g_club[club_index].m_level >= MAX_CLUBLEVEL )
//		return CLUB_MAX_LEVEL;
//	if ( club_is_stop( club_index ) )
//		return CLUB_STOP;
//	if ( g_club[club_index].m_curbuild_develop != ClubBuildNormal ||
//		g_club[club_index].m_isclub_upgrade )
//		return CLUB_UPGRADE_ING; // ����������
//	if ( !club_canupgrade_withbuild( club_index ) )
//		return CLUB_UPGRADE_BUILD_LIMIT; // �����ȼ�������
//
//	// ����ʽ�
//	if ( g_club[club_index].m_money <
//		g_clubupgrade[g_club[club_index].m_level].need_money )
//		return CLUB_MONEY;
//	// �������
//	if ( g_club[club_index].m_repute <
//		g_clubupgrade[g_club[club_index].m_level].need_repute )
//		return CLUB_REPUTE;
//	// ��鷱��
//	if ( g_club[club_index].m_boom <
//		g_clubupgrade[g_club[club_index].m_level].need_boom )
//		return CLUB_BOOM;
//
//	club_set_money( club_index, -g_clubupgrade[g_club[club_index].m_level].need_money );
//	club_set_boom( club_index, -g_clubupgrade[g_club[club_index].m_level].need_boom );
//	club_set_repute( club_index, -g_clubupgrade[g_club[club_index].m_level].need_repute );
//
//	// ��ʼ����������
//	g_club[club_index].m_isclub_upgrade = 1;
//	g_club[club_index].m_club_upgrade_exp = 0;
//
//
//	g_club[club_index].m_post_type[0] = 1;
//	g_club[club_index].m_post_frame[0] = 0;
//
//	club_send_buildinfo( actor_index );
//	// begin֪ͨ������Ա����
//	{
//		char szMsg[256] = { 0 };
//		sprintf( szMsg, "[%s]�����˼����ģ���������裡����һ��Ϊ��Ŭ����", g_actors[actor_index].player_attr.name );
//		clubmelee_clubtalk( g_club[club_index].m_clubid, szMsg );
//	}
//	// end
//	return 0;
//}
//
//
//int club_getquestdonetimes( short club_index, short questtype_offset )
//{
//	int thisday;
//	int i;
//	CLUB_CHECK_INDEX( club_index );
//	thisday = ((int)time( NULL ) - (int)timezone) / 86400;
//	if ( g_club[club_index].m_fdate != thisday )
//	{
//		// ����δˢ��,ˢ�����е���ʹ����
//		for ( i = 0; i < CLUB_QUESTTYPE_NUM; i++ )
//			g_club[club_index].m_questdonetimes_perday[i] = 0;
//	}
//	if ( questtype_offset < 0 || questtype_offset >= CLUB_QUESTTYPE_NUM )
//		return -1;
//	return g_club[club_index].m_questdonetimes_perday[questtype_offset];
//}
//
//int club_setquestdonetimes( short club_index, short questtype_offset, int times )
//{
//	CLUB_CHECK_INDEX( club_index );
//	if ( questtype_offset < 0 || questtype_offset >= CLUB_QUESTTYPE_NUM )
//		return -1;
//	g_club[club_index].m_questdonetimes_perday[questtype_offset] = times;
//	return 0;
//}
//
//int club_update_membercontribute( int actor_index )
//{
//	char szSQL[2048];
//	ACTOR_CHECK_INDEX( actor_index );
//	sprintf( szSQL, "update club_member set total_contribute='%d',"
//		"current_contribute='%d' where actorid='%d' ",
//		g_actors[actor_index].club_total_contribute,
//		g_actors[actor_index].club_current_contribute,
//		g_actors[actor_index].id );
//	if ( mysql_query( myGame, szSQL ) )
//	{
//		printf( "Query failed (%s)\n", mysql_error( myGame ) );
//		write_gamelog( "%s", szSQL );
//		return -1;
//	}
//	return 0;
//}
//
//
//int club_add_boom_repute( int club_index, int boom, int repute )
//{
//	CLUB_CHECK_INDEX( club_index );
//	if ( g_club[club_index].m_boom < 0 )
//		g_club[club_index].m_boom = 0;
//	if ( g_club[club_index].m_repute < 0 )
//		g_club[club_index].m_repute = 0;
//
//	if ( boom != 0 )
//		g_club[club_index].m_boom += boom;
//	if ( repute != 0 )
//		g_club[club_index].m_repute += repute;
//	club_info_update( club_index );
//	return 0;
//}
//
//int club_add_money( int club_index, int money )
//{
//	CLUB_CHECK_INDEX( club_index );
//	g_club[club_index].m_money += money;
//	if ( g_club[club_index].m_money < 0 )
//		g_club[club_index].m_money = 0;
//
//	club_info_update( club_index );
//	return 0;
//}
//
//int club_gm( int actor_index, int type, int value, int clubid )
//{
//	int club_index = 0;
//	if ( clubid <= 0 )
//		club_index = g_actors[actor_index].club_index;
//	else
//	{
//		club_index = club_getindex( clubid );
//	}
//	CLUB_CHECK_INDEX( club_index );
//	if ( type == 1 )
//		club_set_money( club_index, value );
//	else if ( type == 2 )
//		club_set_boom( club_index, value );
//	else if ( type == 3 )
//		club_set_repute( club_index, value );
//	else if ( type == 4 )
//		club_set_res( club_index, value );
//	else if ( type == 5 )
//		club_add_clubupgrade_exp( club_index, value );	// ��ӵ�ǰ��������Ľ����
//	else if ( type == 6 )
//		club_add_curbuild_exp( club_index, value );		// ��ӵ�ǰ���������Ľ����
//	else if ( type == 7 )
//		club_add_curskill_exp( club_index, value );
//	else if ( type == 8 )
//		club_change_member_contribute( actor_index, value );
//	else if ( type == 9 )
//		club_logic_hourly( );
//	else if ( type == 10 )
//		g_club[club_index].m_level = value;
//	return 0;
//}
//
//int club_getallclubnum( )
//{
//	int tmpi;
//	int rt_count = 0;
//	for ( tmpi = 0; tmpi < MAX_CLUB_COUNT; tmpi++ )
//	{
//		if ( g_club[tmpi].m_clubid > 0 )
//			rt_count += 1;
//	}
//
//	return rt_count;
//}
//
//int club_getallclubnum_without_isbroken( )
//{
//	int tmpi;
//	int rt_count = 0;
//	for ( tmpi = 0; tmpi < MAX_CLUB_COUNT; tmpi++ )
//	{
//		if ( g_club[tmpi].m_clubid > 0 )
//		{
//			rt_count += 1;
//		}
//	}
//	return rt_count;
//}
//
//int club_getonlinemembernum( short club_index )
//{
//	int rt = 0;
//	int tmpi;
//	CLUB_CHECK_INDEX( club_index );
//
//	for ( tmpi = 0; tmpi < MAX_MEMBER_COUNT; tmpi++ )
//	{
//		if ( g_club[club_index].m_online_member[tmpi] >= 0 )
//		{
//			rt += 1;
//		}
//	}
//
//	return rt;
//}
//
//int club_member_inmap( short club_index, int mapid )
//{
//	int rt = 0;
//	int tmpi;
//	CLUB_CHECK_INDEX( club_index );
//
//	for ( tmpi = 0; tmpi < MAX_MEMBER_COUNT; tmpi++ )
//	{
//		if ( g_club[club_index].m_online_member[tmpi] >= 0 )
//		{
//			int tmp_index = g_club[club_index].m_online_member[tmpi];
//			if ( g_actors[tmp_index].mapid == mapid )
//				rt += 1;
//		}
//	}
//
//	return rt;
//}
//
//int club_reset_member_wcontribute( int club_index )
//{
//	return 0;
//}
//
//int club_check_current_week( int club_index )
//{
//	int thisweek;
//	char szSQL[2048];
//	if ( club_index <= 0 || club_index >= MAX_CLUB_COUNT )
//		return -1;
//	if ( g_club[club_index].m_clubid <= 0 )
//		return -1;
//	thisweek = ((int)time( NULL ) - (int)timezone + 259200) / 604800;
//	if ( g_club[club_index].m_current_week != thisweek )
//	{
//		sprintf( szSQL, "update club_member set week_contribute='0' where clubid=%d",
//			g_club[club_index].m_clubid );
//		if ( mysql_query( myGame, szSQL ) )
//		{
//			printf( "Query failed (%s)\n", mysql_error( myGame ) );
//			write_gamelog( "%s", szSQL );
//			return -1;
//		}
//
//		g_club[club_index].m_current_week = thisweek;
//		sprintf( szSQL, "update club set current_week=%d where clubid=%d",
//			thisweek, g_club[club_index].m_clubid );
//		if ( mysql_query( myGame, szSQL ) )
//		{
//			printf( "Query failed (%s)\n", mysql_error( myGame ) );
//			write_gamelog( "%s", szSQL );
//			return -1;
//		}
//	}
//	return 0;
//}
//
//
//int club_broken_logic( )
//{
//	return 0;
//}
//
//int club_check_leader_online( int club_index )
//{
//	MYSQL_RES		*res;
//	MYSQL_ROW		row;
//	char	szSQL[1024];
//	int leader_lasttime = 0;
//	int lower_lasttime = 0;
//	int new_leaderid = 0;
//
//	if ( club_index <= 0 || club_index >= MAX_CLUB_COUNT )
//		return -1;
//	lower_lasttime = (int)time( NULL ) - (3600 * 24 * 7);
//	// ȡ���峤���һ�ε�½ʱ��
//	sprintf( szSQL, "select lasttime from club_member where clubid='%d' and clubclass=50",
//		g_club[club_index].m_clubid );
//	if ( mysql_query( myGame, szSQL ) )
//	{
//		printf( "Query failed (%s)\n", mysql_error( myGame ) );
//		write_gamelog( "%s", szSQL );
//		return -1;
//	}
//	res = mysql_store_result( myGame );
//	if ( (row = mysql_fetch_row( res )) )
//	{
//		leader_lasttime = atoi( row[0] );
//	}
//	mysql_free_result( res );
//
//	if ( leader_lasttime < lower_lasttime )
//	{
//		new_leaderid = 0;
//		sprintf( szSQL,
//			"select actorid from club_member "
//			" where clubid='%d' and level>39 "
//			"and clubclass>0 and clubclass <50 and lasttime>%d "
//			"ORDER by clubclass desc limit 1", g_club[club_index].m_clubid,
//			lower_lasttime );
//		if ( mysql_query( myGame, szSQL ) )
//		{
//			printf( "Query failed (%s)\n", mysql_error( myGame ) );
//			write_gamelog( "%s", szSQL );
//			return -1;
//		}
//		res = mysql_store_result( myGame );
//		if ( (row = mysql_fetch_row( res )) )
//		{
//			new_leaderid = atoi( row[0] );
//		}
//		mysql_free_result( res );
//		if ( new_leaderid > 0 )
//		{
//			write_gamelog( "replace club_leader" );
//			sprintf( szSQL,
//				"update club_member set clubclass=1 where clubid=%d and clubclass=50 "
//				, g_club[club_index].m_clubid );
//			if ( mysql_query( myGame, szSQL ) )
//			{
//				printf( "Query failed (%s)\n", mysql_error( myGame ) );
//				write_gamelog( "%s", szSQL );
//				return -1;
//			}
//			club_change_class( club_index, new_leaderid, CLUBPLACE_LEADER );
//		}
//		else
//		{
//			club_delete( club_index );
//			return -2;
//		}
//	}
//	return 0;
//}
//
//int club_reset_broken( )
//{
//	//int tmpi,tmpj;
//	//for( tmpi = 0; tmpi < MAX_CLUB_COUNT; tmpi ++ )
//	//{
//	//if( g_club[tmpi].m_clubid <= 0 )
//	//continue;
//	//g_club[tmpi].m_isbroken = 0;
//	//for( tmpj = 0; tmpj < 7; tmpj ++ )
//	//{
//	//g_club[tmpi].m_week_online[tmpj] = 0;
//	//}
//	//g_club[tmpi].m_broken_days = 0;
//	//g_club[tmpi].m_calc_broken_time = (int)time(NULL);
//	//memset( g_club[tmpi].m_today_online_actors, 0, sizeof(int)*MAX_MEMBER_COUNT );
//	//}
//	return 0;
//}
//int club_allowenter_clubmap( int actor_index )
//{
//	int tmpi;
//	int tmp_index;
//	if ( actor_index < 0 || actor_index >= g_maxactornum )
//		return -1;
//	if ( g_actors[actor_index].team_member[0] >= 0 )
//	{
//		for ( tmpi = 0; tmpi < 5; tmpi++ )
//		{
//			if ( g_actors[actor_index].team_member[tmpi] >= 0 )
//			{
//				tmp_index = g_actors[actor_index].team_member[tmpi];
//				if ( g_actors[actor_index].club_index != g_actors[tmp_index].club_index )
//					//ACTOR_NORMAL_NOTIFY_RETURN( actor_index, "�������з������ĳ�Ա", -1 );
//					return -2;
//			}
//		}
//	}
//	return 0;
//}
//
//
//
//*/
//// ���貿��
//int club_get_money( int club_index )
//{
//	CLUB_CHECK_INDEX( club_index );
//	return g_club[club_index].m_money;
//}
//int club_get_boom( int club_index )
//{
//	CLUB_CHECK_INDEX( club_index );
//	return g_club[club_index].m_boom;
//}
//int club_set_money( int club_index, int value )
//{
//	CLUB_CHECK_INDEX( club_index );
//	g_club[club_index].m_money += value;
//	if ( club_is_stop( club_index ) )
//	{//  ����ʽ����ˣ�֪ͨ���,ֹͣ�з���
//
//	}
//	if ( g_club[club_index].m_money < 1 )
//	{// ��ģ�½�
//		club_level_fall( club_index );
//	}
//	if ( g_club[club_index].m_money > club_get_money_most( club_index ) )
//	{ // ��������
//		g_club[club_index].m_money = club_get_money_most( club_index );
//	}
//	club_update( club_index );
//	return 0;
//}
//int club_set_boom( int club_index, int value )
//{
//	CLUB_CHECK_INDEX( club_index );
//	g_club[club_index].m_boom += value;
//	if ( g_club[club_index].m_boom > 5000 )
//	{
//		g_club[club_index].m_boom = 5000;
//	}
//	club_update( club_index );
//	return 0;
//}
//int club_set_repute( int club_index, int value )
//{
//	CLUB_CHECK_INDEX( club_index );
//	g_club[club_index].m_repute += value;
//	if ( g_club[club_index].m_repute < 1 )
//	{// ��ģ�½�
//		club_level_fall( club_index );
//	}
//	if ( g_club[club_index].m_repute > 5000 )
//	{
//		g_club[club_index].m_repute = 5000;
//	}
//	club_update( club_index );
//	return 0;
//}
//int club_set_res( int club_index, int value )
//{
//	CLUB_CHECK_INDEX( club_index );
//	g_club[club_index].m_res_point += value;
//	if ( g_club[club_index].m_res_point > club_get_maxres( club_index ) )
//	{
//		g_club[club_index].m_res_point = club_get_maxres( club_index );
//	}
//	else if ( g_club[club_index].m_res_point < 0 )
//	{
//		g_club[club_index].m_res_point = 0;
//	}
//	club_update( club_index );
//	return 0;
//}
//int club_level_fall( int club_index )
//{
//	CLUB_CHECK_INDEX( club_index );
//	g_club[club_index].m_level -= 1;
//	if ( g_club[club_index].m_level == 0 )
//	{ // ��ɢ	
//		// begin֪ͨ������Ա����
//		{
//			char szMsg[256] = { 0 };
//			sprintf( szMsg, "��������ά���ʽ�̫�ͣ���������ɢ��" );
//			//clubmelee_clubtalk( g_club[club_index].m_clubid, szMsg );
//		}
//		// end
//		club_delete( club_index );
//	}
//	else
//	{
//		g_club[club_index].m_money += 50000000;
//		g_club[club_index].m_repute += 500;
//		club_update( club_index );
//		// begin֪ͨ������Ա����
//		{
//			char szMsg[256] = { 0 };
//			sprintf( szMsg, "��������ά���ʽ�̫�͵�ԭ�򣬵����˵ȼ��Ľ��ͣ�" );
//			//clubmelee_clubtalk( g_club[club_index].m_clubid, szMsg );
//			MYSQL_RES		*res;
//			MYSQL_ROW		row;
//			char	szSQL[1024];
//			// �����ɰ���������Ϣ
//			sprintf( szSQL, "select actorid from club_member where clubid='%d' and place=%d", g_club[club_index].m_clubid, CLUBPLACE_LEADER );
//			if ( mysql_query( myGame, szSQL ) )
//			{
//				printf( "Query failed (%s)\n", mysql_error( myGame ) );
//				write_gamelog( "%s", szSQL );
//				return -1;
//			}
//			res = mysql_store_result( myGame );
//			while ( (row = mysql_fetch_row( res )) )
//			{
//				mail_send( -157, atoi( row[0] ), "��������ά���ʽ�̫�͵�ԭ�򣬵����˵ȼ��Ľ��ͣ�", 0, 0, 0, 0, 0, 0 );
//			}
//			club_add_event( g_club[club_index].m_clubid, 0, 0, 0, 0, CLUBEVENT_CLUB_BACKOUT );
//		}
//		// end
//	}
//	return 0;
//}
//int club_is_stop( int club_index )
//{
//	CLUB_CHECK_INDEX( club_index );
//	if ( g_club[club_index].m_money < club_get_money_least( club_index ) )
//		return 1;
//	return 0;
//}
//// ��ȡÿСʱά������
//int club_get_hourly_mainte_money( int club_index )
//{
//	int money = 0;
//	CLUB_CHECK_INDEX( club_index );
//	if ( club_is_stop( club_index ) )
//	{ // ������ڵ�����
//		money = g_club[club_index].m_level * 10000;// ����ά������
//	}
//	else
//	{
//		money = g_club[club_index].m_level * 10000 +
//			g_clubbuild_hourlymoney[Club_burse][g_club[club_index].m_build_level[Club_burse]] +
//			g_clubbuild_hourlymoney[Club_room][g_club[club_index].m_build_level[Club_room]] +
//			g_clubbuild_hourlymoney[Club_drug][g_club[club_index].m_build_level[Club_drug]];
//	}
//
//
//	return	-money*24;
//}
//// ��ȡ�ʽ����
//int club_get_money_least( int club_index )
//{
//	int money_least = 0;
//	int money;
//	CLUB_CHECK_INDEX( club_index );
//	money = g_club[club_index].m_level * 10000 +
//		g_clubbuild_hourlymoney[Club_burse][g_club[club_index].m_build_level[Club_burse]] +
//		g_clubbuild_hourlymoney[Club_room][g_club[club_index].m_build_level[Club_room]] +
//		g_clubbuild_hourlymoney[Club_drug][g_club[club_index].m_build_level[Club_drug]];
//	money_least = 48 * money;
//	return money_least;
//}
//// ��ȡ�ʽ�����
//int club_get_money_most( int club_index )
//{
//	CLUB_CHECK_INDEX( club_index );
//	if ( g_club[club_index].m_build_level[Club_burse] < 0 ||
//		g_club[club_index].m_build_level[Club_burse] > MAX_CLUB_BUILDLEVEL )
//		return 0;
//	return g_maxclubmoneymost[g_club[club_index].m_build_level[Club_burse]];
//}
//// ��ȡ����Ա
//int club_get_member_max( int club_index )
//{
//	CLUB_CHECK_INDEX( club_index );
//	if ( g_club[club_index].m_build_level[Club_room] < 0 ||
//		g_club[club_index].m_build_level[Club_room] > MAX_CLUB_BUILDLEVEL )
//		return 0;
//	return g_maxclubmember[g_club[club_index].m_build_level[Club_room]];
//}
//// ��ȡÿСʱ��������Դ��
//int club_get_hourly_makeres( int club_index )
//{
//	//CLUB_CHECK_INDEX( club_index );
//	//if ( g_club[club_index].m_build_level[Club_depot] < 0 ||
//	//	g_club[club_index].m_build_level[Club_depot] > MAX_CLUB_BUILDLEVEL )
//	//	return 0;
//	//if ( club_is_stop( club_index ) )
//	//{
//	//	return 0;
//	//}
//	//if ( g_club[club_index].m_res_point >= club_get_maxres( club_index ) )
//	//{// ��Դ����
//	//	return 0;
//	//}
//	//return g_club_hourly_makeres[g_club[club_index].m_build_level[Club_depot]];
//	return 0;
//}
//// ��ȡ��Դ����
//int club_get_maxres( int club_index )
//{
//	//CLUB_CHECK_INDEX( club_index );
//	//if ( g_club[club_index].m_build_level[Club_depot] < 0 ||
//	//	g_club[club_index].m_build_level[Club_depot] > MAX_CLUB_BUILDLEVEL )
//	//	return 0;
//	//return g_club_maxres[g_club[club_index].m_build_level[Club_depot]];
//	return 0;
//}
//// ��ȡ���̽�������
//int club_get_business_award_least( int club_index )
//{
//	CLUB_CHECK_INDEX( club_index );
//	if ( g_club[club_index].m_build_level[Club_burse] < 0 ||
//		g_club[club_index].m_build_level[Club_burse] > MAX_CLUB_BUILDLEVEL )
//		return 0;
//	return g_club_business_award_least[g_club[club_index].m_build_level[Club_burse]];
//}
//// ��ȡ�о���
//int club_get_studypower( int club_index )
//{
//	//CLUB_CHECK_INDEX( club_index );
//	//if ( g_club[club_index].m_build_level[Club_tech] < 0 ||
//	//	g_club[club_index].m_build_level[Club_tech] > MAX_CLUB_BUILDLEVEL )
//	//	return 0;
//	//if ( club_is_stop( club_index ) )
//	//	return 0;
//	//return g_club_studypower[g_club[club_index].m_build_level[Club_tech]];
//	return 0;
//}
//// ��ȡҩ��ˢҩ������
//int club_get_brush_drugcount( int club_index )
//{
//	CLUB_CHECK_INDEX( club_index );
//	if ( g_club[club_index].m_build_level[Club_drug] < 0 ||
//		g_club[club_index].m_build_level[Club_drug] > MAX_CLUB_BUILDLEVEL )
//		return 0;
//	if ( club_is_stop( club_index ) )
//		return 0;
//	return g_club_brush_drugcount[g_club[club_index].m_build_level[Club_drug]];
//}
//// ��ȡҩ��ˢҩ������-������ʾ
//int club_get_brush_drugcount_show( int club_index )
//{
//	CLUB_CHECK_INDEX( club_index );
//	if ( g_club[club_index].m_build_level[Club_drug] < 0 ||
//		g_club[club_index].m_build_level[Club_drug] > MAX_CLUB_BUILDLEVEL )
//		return 0;
//	return g_club_brush_drugcount[g_club[club_index].m_build_level[Club_drug]];
//}
//// ��ȡ����������
//int club_get_repute_value( int club_index )
//{
//	int repute_value = 0;
//	CLUB_CHECK_INDEX( club_index );
//	repute_value = g_club[club_index].m_online_count - (g_club[club_index].m_level * 2 + 1);
//	return repute_value;
//}
//// ��ȡ�����������轨���
//int club_get_build_upgradeexp( int club_index, int build )
//{
//	int buildlevel;
//	CLUB_CHECK_INDEX( club_index );
//	if ( build <= ClubBuildNormal || build >= ClubBuildMax )
//		return -1;
//	buildlevel = g_club[club_index].m_build_level[build];
//	if ( buildlevel < 0 || buildlevel > MAX_CLUB_BUILDLEVEL )
//		return -1;
//	return g_clubbuildupgrade[build][buildlevel].progress;
//}
//// ��ȡ�������������ʽ�
//int club_get_build_upgrademoney( int club_index, int build )
//{
//	int buildlevel;
//	CLUB_CHECK_INDEX( club_index );
//	if ( build <= ClubBuildNormal || ClubBuildNormal >= ClubBuildMax )
//		return -1;
//	buildlevel = g_club[club_index].m_build_level[build];
//	if ( buildlevel < 0 || buildlevel > MAX_CLUB_BUILDLEVEL )
//		return -1;
//	g_clubbuildupgrade[build][buildlevel].need_money;
//	return g_clubbuildupgrade[build][buildlevel].need_money;
//}
//// ��ȡ����������������
//int club_get_build_upgraderepute( int club_index, int build )
//{
//	int buildlevel;
//	CLUB_CHECK_INDEX( club_index );
//	if ( build <= ClubBuildNormal || ClubBuildNormal >= ClubBuildMax )
//		return -1;
//	buildlevel = g_club[club_index].m_build_level[build];
//	if ( buildlevel < 0 || buildlevel > MAX_CLUB_BUILDLEVEL )
//		return -1;
//	return g_clubbuildupgrade[build][buildlevel].need_repute;
//}
//// ��ȡ�����������跱��
//int club_get_build_upgradeboom( int club_index, int build )
//{
//	int buildlevel;
//	int boom = 0;
//	CLUB_CHECK_INDEX( club_index );
//	if ( build <= ClubBuildNormal || ClubBuildNormal >= ClubBuildMax )
//		return -1;
//	buildlevel = g_club[club_index].m_build_level[build];
//	if ( buildlevel < 0 || buildlevel > MAX_CLUB_BUILDLEVEL )
//		return -1;
//	return g_clubbuildupgrade[build][buildlevel].need_boom;
//}
//
//// ��ӵ�ǰ��������Ľ����
//int club_add_clubupgrade_exp( int club_index, int value )
//{
//	CLUB_CHECK_INDEX( club_index );
//	if ( g_club[club_index].m_isclub_upgrade )
//	{
//		g_club[club_index].m_club_upgrade_exp += value;
//		club_upgrade_issuccess( club_index );
//		club_update( club_index );
//	}
//
//	return 0;
//}
//// ��ӵ�ǰ���������Ľ����
//int club_add_curbuild_exp( int club_index, int value )
//{
//	CLUB_CHECK_INDEX( club_index );
//	if ( g_club[club_index].m_curbuild_develop > ClubBuildNormal &&
//		g_club[club_index].m_curbuild_develop < ClubBuildMax )
//	{
//		g_club[club_index].m_curbuild_exp += value;
//		club_build_upgrade_issuccess( club_index );
//		club_update( club_index );
//	}
//	return 0;
//}
//
//// ���彨���ܷ���������
//int club_canupgrade_withbuild( int club_index )
//{
//	int tmpi;
//	int flag = 0;
//	int needlevel = 0;
//	CLUB_CHECK_INDEX( club_index );
//	needlevel = 4 * g_club[club_index].m_level;
//	for ( tmpi = Club_burse; tmpi < ClubBuildMax; tmpi++ )
//	{
//		if ( g_club[club_index].m_build_level[tmpi] >= needlevel )
//			flag += 1;
//	}
//	if ( flag >= 2 )
//	{
//		return 1;
//	}
//
//	return 0;
//}
//// ���������Ƿ�ɹ�
//int club_upgrade_issuccess( int club_index )
//{
//	CLUB_CHECK_INDEX( club_index );
//
//	if ( g_club[club_index].m_club_upgrade_exp >= g_clubupgrade[g_club[club_index].m_level].progress )
//	{	// ����ȹ���
//		g_club[club_index].m_level += 1;
//		g_club[club_index].m_club_upgrade_exp = 0; // ��λ
//		g_club[club_index].m_isclub_upgrade = 0;   // ��λ
//		club_update( club_index );
//		club_add_event( g_club[club_index].m_clubid, 0, "", 0, 0, CLUBEVENT_CLUB_UPGRADE );
//		// begin֪ͨ������Ա����
//		{
//			char szMsg[256] = { 0 };
//			sprintf( szMsg, "���ǵļ��徭����ҵĲ�иŬ���Ѿ��ɹ�������ģ��Ŀǰ�ȼ���%d������������ȫ������ԱŬ���Ľ��!���ǵļ���δ������ã�", g_club[club_index].m_level );
//			//clubmelee_clubtalk( g_club[club_index].m_clubid, szMsg );
//		}
//		// end
//		return 1;
//	}
//
//	return 0;
//}
//
//// ����������Ϣ
//int club_upgrade_info( int actor_index, short buildid )
//{
//	if ( actor_index < 0 || actor_index >= g_maxactornum )
//		return -1;
//	int club_index = g_actors[actor_index].club.m_club_index;
//	CLUB_CHECK_INDEX( club_index );
//
//	SLK_NetS_ClubUpgradeInfo pValue = { 0 };
//	pValue.m_buildlevel[0] = g_club[club_index].m_level;
//	pValue.m_buildlevel[Club_burse] = g_club[club_index].m_build_level[Club_burse];
//	pValue.m_buildlevel[Club_drug] = g_club[club_index].m_build_level[Club_drug];
//	pValue.m_buildlevel[Club_room] = g_club[club_index].m_build_level[Club_room];
//	pValue.m_needmoney[0] = g_clubupgrade[g_club[club_index].m_level].need_money;
//	pValue.m_needmoney[Club_burse] = club_get_build_upgrademoney( club_index, Club_burse );
//	pValue.m_needmoney[Club_drug] = club_get_build_upgrademoney( club_index, Club_drug );
//	pValue.m_needmoney[Club_room] = club_get_build_upgrademoney( club_index, Club_room );
//	pValue.m_clubmoney = g_club[club_index].m_money;
//	pValue.m_leastmoney = club_get_money_least( club_index );
//	pValue.m_maxmoney = club_get_money_most( club_index );
//	pValue.m_buildid = (char)buildid;
//	netsend_clubupgradeinfo_S( actor_index, SENDTYPE_ACTOR, &pValue );
//	return 0;
//}
//
//// ��������
//int club_upgrade( int actor_index )
//{
//	ACTOR_CHECK_INDEX( actor_index );
//	short club_index = g_actors[actor_index].club.m_club_index;
//	CLUB_CHECK_INDEX( club_index );
//	int clubid = g_club[club_index].m_clubid;
//	if ( g_actors[actor_index].club.m_place < CLUBPLACE_FLEADER )
//	{
//		actor_notify( actor_index, 0, -1, "��û�����Ȩ��" );
//		return -1;
//	}
//	
//	if ( g_club[club_index].m_level >= MAX_CLUBLEVEL || g_club[club_index].m_level < 0 )
//	{
//		actor_notify( actor_index, 0, -1, "�Ѿ������ȼ�" );
//		return -1;
//	}
//	if ( g_club[club_index].m_build_level[Club_burse] + g_club[club_index].m_build_level[Club_drug] + g_club[club_index].m_build_level[Club_room]
//		< g_club[club_index].m_level * 5 )
//	{
//		char szMsg[256] = { 0 };
//		sprintf( szMsg, "�������������ȼ��ܺ���ﵽ&g%d��&w", g_club[club_index].m_level * 5 );
//		actor_notify( actor_index, 0, -1, szMsg );
//		return -1;
//	}
//
//	// ����ʽ�
//	if ( g_club[club_index].m_money < g_clubupgrade[g_club[club_index].m_level].need_money )
//	{
//		actor_notify( actor_index, 0, -1, "�����ʽ�����֧��" );
//		return -1;
//	}
//	int minmoney = club_get_money_least( club_index ) + g_clubupgrade[g_club[club_index].m_level].need_money;
//	if ( g_club[club_index].m_money < minmoney )
//	{
//		char szMsg[256] = { 0 };
//		sprintf( szMsg, "�����ʽ����ٴﵽ %d", minmoney );
//		actor_notify( actor_index, 0, -1, szMsg );
//		return -1;
//	}
//	club_set_money( club_index, -(g_clubupgrade[g_club[club_index].m_level].need_money) );
//	// ��ʼ����
//	g_club[club_index].m_level += 1;
//	club_update( club_index );
//	club_add_event( g_club[club_index].m_clubid, 0, "", g_club[club_index].m_level, "����", CLUBEVENT_CLUB_UPGRADE );
//	club_upgrade_info( actor_index, 0 );
//	return 0;
//}
//
//// ���彨������
//int club_build_upgrade( int actor_index, int build )
//{
//	ACTOR_CHECK_INDEX( actor_index );
//	short club_index = g_actors[actor_index].club.m_club_index;
//	CLUB_CHECK_INDEX( club_index );
//	int clubid = g_club[club_index].m_clubid;
//	if ( g_actors[actor_index].club.m_place < CLUBPLACE_FLEADER )
//	{
//		actor_notify( actor_index, 0, -1, "��û�����Ȩ��" );
//		return -1;
//	}
//	//if ( club_is_stop( club_index ) )
//	//	return CLUB_STOP;
//	if ( build <= ClubBuildNormal || build >= ClubBuildMax )
//		return -1;
//	if ( g_club[club_index].m_build_level[build] >= MAX_CLUB_BUILDLEVEL ||
//		g_club[club_index].m_build_level[build] < 0 )
//	{
//		actor_notify( actor_index, 0, -1, "�Ѿ������ȼ�" );
//		return -1;
//	}
//	if ( g_club[club_index].m_build_level[build] >= g_club[club_index].m_level * 3 )
//	{
//		actor_notify( actor_index, 0, -1, "��Ҫ����������" );
//		return -1;
//	}
//
//	// ����ʽ�
//	if ( g_club[club_index].m_money < g_clubbuildupgrade[build][g_club[club_index].m_build_level[build]].need_money )
//	{
//		actor_notify( actor_index, 0, -1, "�����ʽ�����֧��" );
//		return -1;
//	}
//	int minmoney = club_get_money_least( club_index ) + g_clubbuildupgrade[build][g_club[club_index].m_build_level[build]].need_money;
//	if ( g_club[club_index].m_money < minmoney )
//	{
//		char szMsg[256] = { 0 };
//		sprintf( szMsg, "�����ʽ����ٴﵽ %d", minmoney );
//		actor_notify( actor_index, 0, -1, szMsg );
//		return -1;
//	}
//	club_set_money( club_index, -(g_clubbuildupgrade[build][g_club[club_index].m_build_level[build]].need_money) );
//
//	// ��ʼ�������彨����
//	g_club[club_index].m_build_level[build] += 1;
//	club_update( club_index );
//	club_add_event( g_club[club_index].m_clubid, 0, "", g_club[club_index].m_build_level[build], club_getbuildname( build ), CLUBEVENT_BUILD_UPGRADE );
//	club_upgrade_info( actor_index, build );
//	return 0;
//}
//// ���彨�������Ƿ�ɹ�
//int club_build_upgrade_issuccess( int club_index )
//{
//	int build;
//	int buildlevel;
//	CLUB_CHECK_INDEX( club_index );
//	if ( g_club[club_index].m_curbuild_develop <= ClubBuildNormal ||
//		g_club[club_index].m_curbuild_develop >= ClubBuildMax )
//		return -1;
//	build = g_club[club_index].m_curbuild_develop;
//	buildlevel = g_club[club_index].m_build_level[build];
//	if ( g_club[club_index].m_curbuild_exp >= g_clubbuildupgrade[build][buildlevel].progress )
//	{
//		g_club[club_index].m_build_level[build] += 1;
//		if ( g_club[club_index].m_build_level[build] > MAX_CLUB_BUILDLEVEL )
//		{
//			g_club[club_index].m_build_level[build] = MAX_CLUB_BUILDLEVEL;
//		}
//		g_club[club_index].m_curbuild_develop = ClubBuildNormal;
//		g_club[club_index].m_curbuild_exp = 0;
//		club_update( club_index );
//		club_add_event( g_club[club_index].m_clubid, 0, "", 0, 0, CLUBEVENT_BUILD_UPGRADE );
//		// begin֪ͨ������Ա����
//		{
//			char szMsg[256] = { 0 };
//			sprintf( szMsg, "������ҵ�Ŭ�������彨����%s���Ĺ�ģ�Ѿ��ɹ�������Ŀǰ�ȼ�%d������������ȫ������ԱŬ���Ľ��!", g_szClubBuildName[build], g_club[club_index].m_build_level[build] );
//			//clubmelee_clubtalk( g_club[club_index].m_clubid, szMsg );
//		}
//		// end
//	}
//
//	return 0;
//}
//
//int club_degrade( int actor_index )
//{
//	int club_index;
//	if ( actor_index < 0 || actor_index >= g_maxactornum )
//		return -1;
//	club_index = g_actors[actor_index].club.m_club_index;
//	CLUB_CHECK_INDEX( club_index );
//	//if ( club_place_right( club_index, g_actors[actor_index].club_place, RIGHT_CLUB_BUILD ) == -1 )
//	//	return CLUB_POWER_NONE;
//	if ( g_club[club_index].m_level > 1 )
//	{ // ����	
//		if ( g_club[club_index].m_isclub_upgrade )
//		{
//			ACTOR_NORMAL_NOTIFY_RETURN( actor_index, "�������������ģ�У��޷����!", -1 );
//		}
//		g_club[club_index].m_level -= 1;
//		ACTOR_NORMAL_NOTIFY( actor_index, "����ɹ�����ģ����һ��!" );
//		club_update( club_index );
//	}
//	else
//	{ // �����ٽ�����
//		ACTOR_NORMAL_NOTIFY_RETURN( actor_index, "�����Ѿ�����͹�ģ", -1 );
//	}
//	club_send_buildinfo( actor_index );
//	club_add_event( g_club[club_index].m_clubid, 0, "", 0, 0, CLUBEVENT_CLUB_BACKOUT );
//	// begin֪ͨ������Ա����
//	{
//		char szMsg[256] = { 0 };
//		sprintf( szMsg, "[%s]�����˼���Ĺ�ģ��", g_actors[actor_index].pattr[0]->name );
//		//clubmelee_clubtalk( g_club[club_index].m_clubid, szMsg );
//	}
//	// end
//	return 0;
//}
//int club_build_degrade( int actor_index, int build )
//{
//	int club_index;
//	if ( actor_index < 0 || actor_index >= g_maxactornum )
//		return -1;
//	club_index = g_actors[actor_index].club.m_club_index;
//	CLUB_CHECK_INDEX( club_index );
//	//if ( club_place_right( club_index, g_actors[actor_index].club_place, RIGHT_CLUB_BUILD ) == -1 )
//	//	return CLUB_POWER_NONE;
//	if ( build <= ClubBuildNormal || build >= ClubBuildMax )
//		return -1;
//	if ( g_club[club_index].m_build_level[build] > 1 )
//	{ // ����	
//		if ( g_club[club_index].m_curbuild_develop == build )
//		{
//			ACTOR_NORMAL_NOTIFY_RETURN( actor_index, "�ý������������ģ�У��޷����!", -1 );
//		}
//		g_club[club_index].m_build_level[build] -= 1;
//		ACTOR_NORMAL_NOTIFY( actor_index, "����ɹ�����ģ����һ��!" );
//		club_update( club_index );
//	}
//	else
//	{ // �����ٽ�����
//		ACTOR_NORMAL_NOTIFY_RETURN( actor_index, "�ý����Ѿ�����͹�ģ!", -1 );
//	}
//	club_send_buildinfo( actor_index );
//	club_add_event( g_club[club_index].m_clubid, 0, "", 0, 0, CLUBEVENT_BUILD_BACKOUT );
//	// begin֪ͨ������Ա����
//	{
//		char szMsg[256] = { 0 };
//		sprintf( szMsg, "[%s]����˽�����%s�������¹�ģ����һ����", g_szClubBuildName[build], g_actors[actor_index].pattr[0]->name );
//		//clubmelee_clubtalk( g_club[club_index].m_clubid, szMsg );
//	}
//	// end
//	return 0;
//}
//
//int club_upgrade_stop( int actor_index )
//{
//	int club_index;
//	if ( actor_index < 0 || actor_index >= g_maxactornum )
//		return -1;
//	club_index = g_actors[actor_index].club.m_club_index;
//	CLUB_CHECK_INDEX( club_index );
//	//if ( club_place_right( club_index, g_actors[actor_index].club_place, RIGHT_CLUB_BUILD ) == -1 )
//	//	return CLUB_POWER_NONE;
//	if ( !g_club[club_index].m_isclub_upgrade )
//		return -1;
//
//	g_club[club_index].m_isclub_upgrade = 0;
//	g_club[club_index].m_club_upgrade_exp = 0;
//	ACTOR_NORMAL_NOTIFY( actor_index, "�����ģ��������ֹ!" );
//	club_update( club_index );
//	club_send_buildinfo( actor_index );
//	club_add_event( g_club[club_index].m_clubid, 0, "", 0, 0, CLUBEVENT_CLUB_STOP );
//	// begin֪ͨ������Ա����
//	{
//		char szMsg[256] = { 0 };
//		sprintf( szMsg, "[%s]��ֹ�˼����ģ��������", g_actors[actor_index].pattr[0]->name );
//		//clubmelee_clubtalk( g_club[club_index].m_clubid, szMsg );
//	}
//	// end
//	return 0;
//}
//int club_build_upgrade_stop( int actor_index, int build )
//{
//	int club_index;
//	if ( actor_index < 0 || actor_index >= g_maxactornum )
//		return -1;
//	club_index = g_actors[actor_index].club.m_club_index;
//	CLUB_CHECK_INDEX( club_index );
//	//if ( club_place_right( club_index, g_actors[actor_index].club.m_place, RIGHT_CLUB_BUILD ) == -1 )
//	//	return CLUB_POWER_NONE;
//	if ( build <= ClubBuildNormal || build >= ClubBuildMax )
//		return -1;
//	if ( g_club[club_index].m_curbuild_develop != build )
//		return 0;
//
//	g_club[club_index].m_curbuild_develop = ClubBuildNormal;
//	g_club[club_index].m_curbuild_exp = 0;
//	ACTOR_NORMAL_NOTIFY( actor_index, "�ý�����ģ��������ֹ!" );
//	club_update( club_index );
//	club_send_buildinfo( actor_index );
//	club_add_event( g_club[club_index].m_clubid, 0, "", 0, 0, CLUBEVENT_BUILD_STOP );
//	// begin֪ͨ������Ա����
//	{
//		char szMsg[256] = { 0 };
//		sprintf( szMsg, "[%s]������ֹ������壨%s���Ĺ�ģ�ˣ�", g_szClubBuildName[build], g_actors[actor_index].pattr[0]->name );
//		//clubmelee_clubtalk( g_club[club_index].m_clubid, szMsg );
//	}
//	// end
//	return 0;
//}
//
//int club_send_buildinfo( int actor_index )
//{
//	//char tmpbuf[2048];
//	//int tmpsize;
//	//char *ptr, *ptrsize;
//	//int club_index;
//	//int tmpi;
//	//if ( actor_index < 0 || actor_index >= g_maxactornum )
//	//	return -1;
//	//club_index = g_actors[actor_index].club_index;
//	//CLUB_CHECK_INDEX( club_index );
//
//	//ptr = tmpbuf;
//	//tmpsize = 0;
//	//ptr += sizeof(short);
//	//*(short *)ptr = CMDS_CLUB_BUILD_INFO;	ptr += sizeof(short);	tmpsize += sizeof(short);
//	//ptrsize = ptr;						ptr += sizeof(short);	tmpsize += sizeof(short);
//	////���ȼ�
//	//*(short *)ptr = g_club[club_index].m_build_level[Club_burse]; ptr += sizeof(short); tmpsize += sizeof(short);
//	////ס���ȼ�
//	//*(short *)ptr = g_club[club_index].m_build_level[Club_room]; ptr += sizeof(short); tmpsize += sizeof(short);
//	////ҩ���ȼ�	
//	//*(short *)ptr = g_club[club_index].m_build_level[Club_drug]; ptr += sizeof(short); tmpsize += sizeof(short);
//	////�ֿ�ȼ�	
//	//*(short *)ptr = g_club[club_index].m_build_level[Club_depot]; ptr += sizeof(short); tmpsize += sizeof(short);
//	////�о����ȼ�	
//	//*(short *)ptr = g_club[club_index].m_build_level[Club_tech]; ptr += sizeof(short); tmpsize += sizeof(short);
//
//	////Ŀǰ�ʽ�
//	//*(int *)ptr = g_club[club_index].m_money; ptr += sizeof(int); tmpsize += sizeof(int);
//	////����ʽ�
//	//*(int *)ptr = club_get_money_most( club_index ); ptr += sizeof(int); tmpsize += sizeof(int);
//	////Ŀǰ��Ա	
//	//*(short *)ptr = g_club[club_index].m_membercount; ptr += sizeof(short); tmpsize += sizeof(short);
//	////����Ա	
//	//*(short *)ptr = club_get_member_max( club_index ); ptr += sizeof(short); tmpsize += sizeof(short);
//	////ҩƷ������	
//	//*(short *)ptr = club_get_brush_drugcount_show( club_index ); ptr += sizeof(short); tmpsize += sizeof(short);
//	////Ŀǰ��Դ
//	//*(int *)ptr = g_club[club_index].m_res_point; ptr += sizeof(int); tmpsize += sizeof(int);
//	////�����Դ
//	//*(int *)ptr = club_get_maxres( club_index ); ptr += sizeof(int); tmpsize += sizeof(int);
//	////����о���	
//	//*(int *)ptr = club_get_studypower_show( club_index ); ptr += sizeof(int); tmpsize += sizeof(int);
//	////��ǰά����	
//	//*(int *)ptr = club_get_hourly_mainte_money( club_index ); ptr += sizeof(int); tmpsize += sizeof(int);
//
//	//if ( g_club[club_index].m_isclub_upgrade )
//	//{ // ����������
//	//	//��ǰ����
//	//	*(int *)ptr = CLUB_MAINUPGRADE_ING; ptr += sizeof(int); tmpsize += sizeof(int);
//	//	//�������	
//	//	*(int *)ptr = g_club[club_index].m_club_upgrade_exp; ptr += sizeof(int); tmpsize += sizeof(int);
//	//}
//	//else
//	//{
//	//	//��ǰ����
//	//	*(int *)ptr = g_club[club_index].m_curbuild_develop; ptr += sizeof(int); tmpsize += sizeof(int);
//	//	//�������	
//	//	*(int *)ptr = g_club[club_index].m_curbuild_exp; ptr += sizeof(int); tmpsize += sizeof(int);
//	//}
//
//
//	////�����ģ����
//	//*(int *)ptr = g_clubupgrade[g_club[club_index].m_level].progress; ptr += sizeof(int); tmpsize += sizeof(int);
//	////�����ģ����	
//	//*(int *)ptr = g_clubupgrade[g_club[club_index].m_level].need_money; ptr += sizeof(int); tmpsize += sizeof(int);
//	////�����ģ����	
//	//*(int *)ptr = g_clubupgrade[g_club[club_index].m_level].need_boom ; ptr += sizeof(int); tmpsize += sizeof(int);
//	////�����ģ����	
//	//*(int *)ptr = g_clubupgrade[g_club[club_index].m_level].need_repute; ptr += sizeof(int); tmpsize += sizeof(int);
//
//	//for ( tmpi = Club_burse; tmpi < ClubBuildMax; tmpi++ )
//	//{
//	//	//�轨�����	
//	//	*(int *)ptr = club_get_build_upgradeexp( club_index, tmpi ); ptr += sizeof(int); tmpsize += sizeof(int);
//	//	//������ʽ�	
//	//	*(int *)ptr = club_get_build_upgrademoney( club_index, tmpi ); ptr += sizeof(int); tmpsize += sizeof(int);
//	//	//����ķ���
//	//	*(int *)ptr = club_get_build_upgradeboom( club_index, tmpi ); ptr += sizeof(int); tmpsize += sizeof(int);
//	//	//���������	
//	//	*(int *)ptr = club_get_build_upgraderepute( club_index, tmpi ); ptr += sizeof(int); tmpsize += sizeof(int);
//
//	//}
//
//	//*(short *)ptrsize = (short)tmpsize - sizeof(short)* 2;
//	//*(unsigned short *)tmpbuf = tmpsize;
//	//sendtoqueue( actor_index, tmpbuf, tmpsize + sizeof(short) );
//	//write_gamelog( "[size:%d][<-CMDS_CLUB_BUILD_INFO][%d]", tmpsize, g_actors[actor_index].id );
//	return 0;
//}
//
//// �Ƿ�������
//int club_actor_isbusinessman( int actor_index )
//{
//	if ( actor_index < 0 || actor_index >= g_maxactornum )
//		return 0;
//	if ( g_actors[actor_index].club.m_place == CLUBPLACE_LEADER )
//	{
//		return 1;
//	}
//	return 0;
//}
//
//
//// ���ҩƷ������
//int club_random_drugcount( int club_index, int totalcount )
//{
//	//int tmpi = 0;
//	//int index = 0;
//	//CLUB_CHECK_INDEX( club_index );
//	//for ( tmpi = 0; tmpi < 12; tmpi++ )
//	//{
//	//	index = rand( ) % CLUB_SHOP_ITEM_COUNT;
//	//	g_club[club_index].m_drugcount[index] += (totalcount<10 ? totalcount : 10);
//	//	totalcount -= 10;
//	//	if ( totalcount <= 0 )
//	//		break;
//	//}
//	//// �洢����ҩ��
//	//club_shop_update( club_index );
//	return 0;
//}
//// �������ҩ����Ʒ����ĸ���
//int club_drugcount_total( int club_index )
//{
//	//int tmpi;
//	int kindcount = 0;
//	/*CLUB_CHECK_INDEX( club_index );
//	for ( tmpi = 0; tmpi < CLUB_SHOP_ITEM_COUNT; tmpi++ )
//	{
//		if ( g_club[club_index].m_drugcount[tmpi] > 0 )
//			kindcount += 1;
//	}*/
//	return kindcount;
//}
//// �������ҩ����Ʒ����ĸ���
//int club_drugcount_withkind( int club_index, int kind )
//{
//	//int tmpi;
//	int kindcount = 0;
//	/*CLUB_CHECK_INDEX( club_index );
//	for ( tmpi = 0; tmpi < CLUB_SHOP_ITEM_COUNT; tmpi++ )
//	{
//		if ( g_club_drugkind[tmpi] == kind )
//		{
//			kindcount = g_club[club_index].m_drugcount[tmpi];
//			break;
//		}
//	}*/
//	return kindcount;
//}
//
//
//// ���ü���ҩ����Ʒ����
//int club_set_drugcount_withkind( int club_index, int kind, int value )
//{
//	//int tmpi;
//	//CLUB_CHECK_INDEX( club_index );
//	//for ( tmpi = 0; tmpi < CLUB_SHOP_ITEM_COUNT; tmpi++ )
//	//{
//	//	if ( g_club_drugkind[tmpi] == kind )
//	//	{
//	//		g_club[club_index].m_drugcount[tmpi] += value;
//	//		if ( g_club[club_index].m_drugcount[tmpi] < 0 )
//	//			g_club[club_index].m_drugcount[tmpi] = 0;
//	//		break;
//	//	}
//	//}
//	//// �洢����ҩ��
//	//club_shop_update( club_index );
//	return 0;
//}
//
//
//int club_actorget_bill_value( int actor_index, int *value, int *max_value )
//{
//	int offset;
//	ACTOR_CHECK_INDEX( actor_index );
//	if ( !value || !max_value )
//		return -1;
//	for ( offset = 0; offset < MAX_ACTOR_ITEMNUM; offset++ )
//	{
//		if ( g_actors[actor_index].item[offset].m_itemid == g_actors[actor_index].quest.other_data[QUESTINDEX_BUSINESSMAN] )
//		{// �ҵ������Ʒ
//			*value = _item_get_ability_value( &g_actors[actor_index].item[offset], ITEM_ABILITY_BILL_VALUE );
//			*max_value = _item_get_ability_value( &g_actors[actor_index].item[offset], ITEM_ABILITY_BILL_MAXVALUE );
//			break;
//		}
//	}
//	return 0;
//}
//int club_actorset_bill_value( int actor_index, int value, int max_value )
//{
//	int offset;
//	ACTOR_CHECK_INDEX( actor_index );
//	for ( offset = 0; offset < MAX_ACTOR_ITEMNUM; offset++ )
//	{
//		if ( g_actors[actor_index].item[offset].m_itemid == g_actors[actor_index].quest.other_data[QUESTINDEX_BUSINESSMAN] )
//		{// �ҵ������Ʒ
//			_item_set_ability_value( &g_actors[actor_index].item[offset], ITEM_ABILITY_BILL_VALUE, value );
//			_item_set_ability_value( &g_actors[actor_index].item[offset], ITEM_ABILITY_BILL_MAXVALUE, max_value );
//			item_info( actor_index, offset );
//			break;
//		}
//	}
//	return 0;
//}
//// �Ƿ��������̵�
//extern int g_shopcount;
//int club_issellshop( int shopid )
//{
//	int shopindex = 0;
//	if ( shopid <= 0 || shopid >= g_shopcount )
//		return 0;
//	for ( shopindex = 0; shopindex < CLUB_BUSINESS_SHOP_COUNT; shopindex++ )
//	{
//		if ( g_business_sell[shopindex].shopid == shopid )
//			return 1;
//	}
//	return 0;
//}
//// ��ȡ����̵꣬�����Ʒ�ļ۸�
//int club_getsellprice( int shopid, int itemkind )
//{
//	int sellprice = 0;
//	int shopindex = 0;
//	int itemindex = 0;
//	for ( shopindex = 0; shopindex < CLUB_BUSINESS_SHOP_COUNT; shopindex++ )
//	{
//		if ( g_business_sell[shopindex].shopid == shopid )
//		{ // �ҵ�����̵�
//			for ( itemindex = 0; itemindex < CLUB_BUSINESS_ITEM_COUNT; itemindex++ )
//			{
//				if ( g_business_sell[shopindex].itemkind[itemindex] == itemkind )
//				{ // �ҵ������Ʒ
//					sellprice = g_business_sell[shopindex].sellprice[itemindex];
//					break;
//				}
//			}
//			break;
//		}
//	}
//	return sellprice;
//}
//int club_randomsellshop_init( )
//{
//	int index;
//	MYSQL_RES *res;
//	MYSQL_ROW row;
//	char szSQL[1024];
//	// ���ظ���ȡ5��shopid
//	sprintf( szSQL, "SELECT DISTINCT(shopid) FROM business_sell LIMIT  %d;", CLUB_BUSINESS_SHOP_COUNT );
//	if ( mysql_query( myData, szSQL ) )
//	{
//		printf( "Query failed (%s)\n", mysql_error( myData ) );
//		write_gamelog( "%s", szSQL );
//		return -1;
//	}
//	res = mysql_store_result( myData );
//	index = 0;
//	while ( row = mysql_fetch_row( res ) )
//	{
//		if ( index >= CLUB_BUSINESS_SHOP_COUNT )
//			break;
//
//		g_business_sell[index].shopid = atoi( row[0] );
//		index += 1;
//	}
//	mysql_free_result( res );
//
//	club_shopsellprice_init( g_business_sell[0].shopid );
//	club_shopsellprice_init( g_business_sell[1].shopid );
//	club_shopsellprice_init( g_business_sell[2].shopid );
//	club_shopsellprice_init( g_business_sell[3].shopid );
//	club_shopsellprice_init( g_business_sell[4].shopid );
//	return 0;
//}
//
//int club_shopsellprice_init( int shopid )
//{
//	int index;
//	int shopindex = 0;
//	int price = 0;
//	int value = 0;
//	MYSQL_RES *res;
//	MYSQL_ROW row;
//	char szSQL[1024];
//	// �ҵ�shop��Ӧ��index
//	for ( shopindex = 0; shopindex < CLUB_BUSINESS_SHOP_COUNT; shopindex++ )
//	{
//		if ( g_business_sell[shopindex].shopid == shopid )
//			break;
//	}
//	if ( shopindex >= CLUB_BUSINESS_SHOP_COUNT )
//		return -1;
//	memset( g_business_sell[shopindex].itemkind, 0, sizeof(int)*CLUB_BUSINESS_ITEM_COUNT );
//	memset( g_business_sell[shopindex].sellprice, 0, sizeof(int)*CLUB_BUSINESS_ITEM_COUNT );
//	memset( g_business_sell[shopindex].price, 0, sizeof(int)*CLUB_BUSINESS_ITEM_COUNT );
//	memset( g_business_sell[shopindex].value, 0, sizeof(int)*CLUB_BUSINESS_ITEM_COUNT );
//	//��ȡ����/
//	sprintf( szSQL, "SELECT itemkind, price, value FROM business_sell WHERE shopid=%d", shopid );
//	if ( mysql_query( myData, szSQL ) )
//	{
//		printf( "Query failed (%s)\n", mysql_error( myData ) );
//		write_gamelog( "%s", szSQL );
//		return -1;
//	}
//	index = 0;
//	res = mysql_store_result( myData );
//	while ( row = mysql_fetch_row( res ) )
//	{
//		if ( index >= CLUB_BUSINESS_ITEM_COUNT )
//			break;
//
//		g_business_sell[shopindex].itemkind[index] = atoi( row[0] );
//		g_business_sell[shopindex].price[index] = atoi( row[1] );
//		g_business_sell[shopindex].value[index] = atoi( row[2] );
//		price = g_business_sell[shopindex].price[index];
//		value = g_business_sell[shopindex].value[index];
//		if ( rand( ) % 2 == 0 )
//		{ // value%����
//			value = price*value / 100;
//			value = value>0 ? value : 1;
//			price = price - rand( ) % value;
//		}
//		else
//		{
//			value = price*value / 100;
//			value = value>0 ? value : 1;
//			price = price + rand( ) % value;
//		}
//		g_business_sell[shopindex].sellprice[index] = price;
//
//		index += 1;
//	}
//	mysql_free_result( res );
//	return 0;
//}
//// ����۸�
//int club_randomsellprice( int shopid )
//{
//	int index;
//	int shopindex = 0;
//	int price = 0;
//	int value = 0;
//	// �ҵ�shop��Ӧ��index
//	for ( shopindex = 0; shopindex < CLUB_BUSINESS_SHOP_COUNT; shopindex++ )
//	{
//		if ( g_business_sell[shopindex].shopid == shopid )
//			break;
//	}
//	if ( shopindex >= CLUB_BUSINESS_SHOP_COUNT )
//		return -1;
//
//	memset( g_business_sell[shopindex].sellprice, 0, sizeof(int)*CLUB_BUSINESS_ITEM_COUNT );
//
//	for ( index = 0; index < CLUB_BUSINESS_ITEM_COUNT; index++ )
//	{
//		price = g_business_sell[shopindex].price[index];
//		value = g_business_sell[shopindex].value[index];
//		if ( rand( ) % 2 == 0 )
//		{ // value%����
//			value = price*value / 100;
//			value = value>0 ? value : 1;
//			price = price - rand( ) % value;
//		}
//		else
//		{
//			value = price*value / 100;
//			value = value>0 ? value : 1;
//			price = price + rand( ) % value;
//		}
//		g_business_sell[shopindex].sellprice[index] = price;
//	}
//
//	return 0;
//}
