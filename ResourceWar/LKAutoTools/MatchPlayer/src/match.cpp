#ifdef WIN32
#include <winsock2.h>
#include <mswsock.h>
#endif
#include <stdio.h>
#include <string.h>
#include <mysql.h>

#include "utils.h"
#include "db.h"
#include "match.h"
#include "randname.h"
extern MYSQL *myData;

int random( int min, int max )
{
	int value = (max - min + 1);
	if ( value <= 0 )
		value = 1;
	return min + rand() % value;
}

void ruffle( int *list, int count )
{
	for ( int tmpi =  0; tmpi < count; tmpi++ )
	{
		int index = rand()%count;
		int swap = list[index];
		swap = list[tmpi];
		list[tmpi] = list[index];
		list[index] = swap;
	}
}

/* 检查生成表里是否有数据 */
int match_check()
{
	MYSQL_RES *res;
	MYSQL_ROW row;
	char szSQL[256];
	int matchcount = 0;

	sprintf( szSQL, "select count(*) from match_list;" );
	if( mysql_query( myData, szSQL ) )
	{
		printf( "Query failed (%s)\n", mysql_error(myData) );
		write_gamelog( "%s", szSQL );
		return -1;
	}
	res = mysql_store_result( myData );
	if ( row = mysql_fetch_row( res ) )
	{
		if ( row[0] )
			matchcount = atoi( row[0] );
	}
	else
	{
		mysql_free_result( res );
		return 0;
	}
	mysql_free_result( res );
	return matchcount;
}

/* 清空数据表 */
int match_delete( char * database, char *szTable )
{
	char szSQL[256];
	sprintf( szSQL, "TRUNCATE `%s`.`%s`", database, szTable );
	if( mysql_query( myData, szSQL ) )
	{
		printf( "Query failed (%s)\n", mysql_error(myData) );
		write_gamelog( "%s", szSQL );
		return -1;
	}

	return 0;
}


/* 竞技场角色列表创建 */
int match_list_insert( Actor *pActor )
{
	char szSQL[2048];
	char tmpstr[256];
	int member = 0;
	/* 主角 */
	sprintf( szSQL, "REPLACE INTO match_list VALUES( '%d','%d','%d','%s','%d','%d'", pActor->m_actorid, pActor->m_rank, pActor->m_shape, pActor->m_name, pActor->m_level, pActor->m_combatpower );
	/* 伙伴 */
	for( member = 0; member < 4; member++ )
	{
		sprintf( tmpstr, ",'%d','%d','%d'", pActor->m_partner[member].m_partnerkind, pActor->m_partner[member].m_level, pActor->m_partner[member].m_partnercolor );
		strcat( szSQL, tmpstr );
	}
	for( ; member < 4; member++ )
	{
		sprintf( tmpstr, ",'0','0','0'" );
		strcat( szSQL, tmpstr );
	}
	strcat( szSQL, ");" );
	if( mysql_query( myData, szSQL ) )
	{
		printf( "Query failed (%s) [%s](%d)\n", mysql_error(myData), __FUNCTION__, __LINE__ );
		write_gamelog( "%s", szSQL );
		return -1;
	}
	return 0;
}


/* 竞技场角色属性创建 */
int match_attr_insert( Actor *pActor )
{
	char	szSQL[8192];
	char szText_name[MAX_PATH]={};
	sprintf( szSQL, "REPLACE INTO match_attr (`actorid`,`aclass`,`name`,`shape`,`level`,`experience`,`body`,`skill_closeflag`,`switchtime`,`baseshape`,`professional_level0`,`professional_level1`,`professional_level2`,`professional_level3`) "
					"Values('%d','%d','%s','%d','%d','%s','%d','%d','%d','%d','%d','%d','%d','%d')",
					pActor->m_actorid, 
					pActor->m_aclass, 
					db_escape((const char *)pActor->m_name, szText_name, 16),
					pActor->m_shape,
					pActor->m_level,
					0,
					0,
					pActor->m_skill_closeflag,
					0,
					0,
					pActor->m_professional_level[0],
					pActor->m_professional_level[1],
					pActor->m_professional_level[2],
					pActor->m_professional_level[3]);
	if( mysql_query( myData, szSQL ) )
	{
		printf( "Query failed (%s)\n", mysql_error(myData) );
		write_gamelog( "%s", szSQL );
		return -1;
	}
	return 0;
}

/* 插入竞技场角色伙伴 */
int match_partner_insert(  Actor *pActor  )
{
	char szSQL[2048] = { 0 };
	char reconnect_flag = 0;
	char sz64_partnerid[21] = { 0 };
	
	for ( int tmpi = 0; tmpi < 4; tmpi++ )
	{
		if ( pActor->m_partner[tmpi].m_partnerid <= 0 )
			continue;
		sprintf( szSQL, "REPLACE INTO match_partner (`partnerid`,`actorid`,`teamoffset`,`partnerkind`,`color`,`experience`,`level`,`main_skill1`,`main_skill2`,`link_skill1`,`link_skill2`,`link_skill3`,`link_skill4`,`link_skill5`,`skillexp`,`skill_closeflag`) "
			"Values('%s','%d','%d','%d','%d','%s','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d')",
			lltoa( pActor->m_partner[tmpi].m_partnerid, sz64_partnerid, 10 ),
			pActor->m_actorid,
			tmpi+1,
			pActor->m_partner[tmpi].m_partnerkind,
			pActor->m_partner[tmpi].m_partnercolor,
			0,
			pActor->m_partner[tmpi].m_level,
			0,
			0,
			0,
			0,
			0,
			0,
			0,
			0,
			0);
		if ( mysql_query( myData, szSQL ) )
		{
			printf( "Query failed (%s)\n", mysql_error( myData ) );
			write_gamelog( "%s", szSQL );
			return -1;
		}
	}
	return 0;
}

/* 竞技场角色列表创建 */
int match_create( int robotcount )
{
	int partnerid = 1;
	for ( int tmpi = 0; tmpi < robotcount; tmpi++ )
	{
		char szName[16]={0};
		Actor actor = {0};

		/* 角色信息 */
		actor.m_rank = tmpi + 1;
		actor.m_actorid =  MATCH_ACTORID_OFFSET + tmpi;

		/* 角色等级 */
		if ( tmpi <= robotcount*1/5 )
			actor.m_level = random( 23, 25 );
		else if ( tmpi <= robotcount*2/5 )
			actor.m_level = random( 20, 22 );
		else if ( tmpi <= robotcount*3/5 )
			actor.m_level = random( 18, 19 );
		else if ( tmpi <= robotcount*4/5 )
			actor.m_level = random( 16, 17 );
		else if ( tmpi <= robotcount*5/5 )
			actor.m_level = random( 15, 16 );
		
		actor.m_combatpower += actor.m_level * 10;

		/* 角色名称 */
		randname_get( actor.m_name );
		
		/* 形象和类型 */
		static int s_shape[2] = { 313, 314 };
		actor.m_aclass  = random( 0, 1 );
		actor.m_shape = s_shape[actor.m_aclass];

		/* 技能 */
		actor.m_skill_closeflag = 1;

		/* 奥义 */
		if ( actor.m_level >= 23 )
		{
			actor.m_professional_level[0] = random( actor.m_level, actor.m_level+30 );
			actor.m_professional_level[1] = random( actor.m_level, actor.m_level+30 );
			actor.m_professional_level[2] = random( actor.m_level, actor.m_level+30 );
			actor.m_professional_level[3] = random( actor.m_level, actor.m_level+30 );
		}
		else if ( actor.m_level >= 19 )
		{
			actor.m_professional_level[0] = random( actor.m_level, actor.m_level+20 );
			actor.m_professional_level[1] = random( actor.m_level, actor.m_level+20 );
			actor.m_professional_level[2] = random( actor.m_level, actor.m_level+20 );
			actor.m_professional_level[3] = random( actor.m_level, actor.m_level+20 );
		}
		else
		{
			actor.m_professional_level[0] = random( actor.m_level, actor.m_level+10 );
			actor.m_professional_level[1] = random( actor.m_level, actor.m_level+10 );
			actor.m_professional_level[2] = random( actor.m_level, actor.m_level+10 );
			actor.m_professional_level[3] = random( actor.m_level, actor.m_level+10 );	
		}

		actor.m_combatpower += (actor.m_professional_level[0]+actor.m_professional_level[2]+actor.m_professional_level[2]+actor.m_professional_level[3]);
		/* 伙伴 蓝 紫2个级别段 */
		static int s_partnerkindlist1[] = { 1,53,12,11,10,9,7,34,64,3,2 };
		static int s_partnerkindlist2[] = { 5,8,15,17,22,23,25,35,43,50 };
		static int s_partnerkindlist3[] = { 14,13,16,18,19,21,48 };
		
		/* 根据三个等级段，第一等级段固定2个绿色+1个鸣人1个井野， 第二等级段2个蓝色+1个鸣人+1个井野， 第三等级段2个紫色 +1个鸣人+1个井野 */
		char colortype = 1;
		if ( actor.m_level >= 23 )
			colortype = 3;
		else if ( actor.m_level >= 19 )
			colortype = 2;
		else
			colortype = 1;

		int partnerkindlist[4] = { 0,0,0,0 };
		int partnercolorlist[4] = { 0,0,0,0 };
		int index = 0;
		if( colortype == 1 )
		{
			partnerkindlist[0] = 40;partnercolorlist[0] = 1;
			partnerkindlist[1] = 33;partnercolorlist[1] = 2;
			ruffle( s_partnerkindlist1, sizeof(s_partnerkindlist1)/sizeof(s_partnerkindlist1[0]) );
			partnerkindlist[2] = s_partnerkindlist1[0]; partnercolorlist[2] = 1;
			partnerkindlist[3] = s_partnerkindlist1[1];partnercolorlist[3] = 1;
		}
		else if( colortype == 2 )
		{
			partnerkindlist[0] = 40;partnercolorlist[0] = 1;
			partnerkindlist[1] = 33;partnercolorlist[1] = 2;
			ruffle( s_partnerkindlist2, sizeof(s_partnerkindlist2)/sizeof(s_partnerkindlist2[0]) );
			partnerkindlist[2] = s_partnerkindlist2[0]; partnercolorlist[2] = 2;
			partnerkindlist[3] = s_partnerkindlist2[1];partnercolorlist[3] = 2;
		}
		else if( colortype == 3 )
		{
			partnerkindlist[0] = 40;partnercolorlist[0] = 1;
			partnerkindlist[1] = 33;partnercolorlist[1] = 2;
			ruffle( s_partnerkindlist3, sizeof(s_partnerkindlist3)/sizeof(s_partnerkindlist3[0]) );
			partnerkindlist[2] = s_partnerkindlist3[0]; partnercolorlist[2] = 3;
			partnerkindlist[3] = s_partnerkindlist3[1];partnercolorlist[3] = 3;
		}
		
		int sortindex[4] = { 0,1,2,3 };
		ruffle( sortindex, 4 );
		for ( int member = 0; member < 4; member++ )
		{
			actor.m_partner[member].m_partnerid = partnerid;
			actor.m_partner[member].m_partnerkind = partnerkindlist[sortindex[member]];
			actor.m_partner[member].m_partnercolor = partnercolorlist[sortindex[member]];
			actor.m_partner[member].m_level = random( actor.m_level-2, actor.m_level );
			actor.m_partner[member].m_skillexp = actor.m_partner[member].m_level+10;
			partnerid += 1;
		}
		
		actor.m_combatpower += 2000;
		/* 插入竞技场角色列表 */
		match_list_insert( &actor );

		/* 插入竞技场角色属性 */
		match_attr_insert( &actor );

		/* 插入竞技场角色技能 */

		/* 插入竞技场角色伙伴 */
		match_partner_insert( &actor );

		/* 插入竞技场角色道具 */

		char szMsg[16]={0};
		sprintf( szMsg, "%d", tmpi );
		puts( szMsg );
	}
	return 0;
}