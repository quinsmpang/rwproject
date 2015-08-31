#ifdef WIN32
#include <winsock2.h>
#include <windows.h>
#endif
#include <mysql.h>
#include "db.h"
#include "actor.h"
#include "auto_actor_hero.h"
extern MYSQL *myGame;

int actor_hero_save_auto( HeroAttr *pHeroAttr, char *pTab, FILE *fp )
{
	char	szSQL[8192];
	char reconnect_flag = 0;
	char szText_name[MAX_PATH]={0};
	char sz64_experience[21]={0};
RE_HEROATTR_UPDATE:
	sprintf( szSQL, "REPLACE INTO %s (`actorid`,`offset`,`herokind`,`name`,`shape`,`level`,`experience`,`feats`,`official`,`mastership`,`health`,`grade`,`state`,`attack`,`defance`,`agility`,`hp`,`skill0`,`skill1`,`skill2`,`skill3`) Values('%d','%d','%d','%s','%d','%d','%s','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d')",pTab,pHeroAttr->m_actorid,pHeroAttr->m_offset,pHeroAttr->m_herokind,db_escape((const char *)pHeroAttr->m_name,szText_name,0),pHeroAttr->m_shape,pHeroAttr->m_level,lltoa(pHeroAttr->m_experience,sz64_experience,10 ),pHeroAttr->m_feats,pHeroAttr->m_official,pHeroAttr->m_mastership,pHeroAttr->m_health,pHeroAttr->m_grade,pHeroAttr->m_state,pHeroAttr->m_attack,pHeroAttr->m_defance,pHeroAttr->m_agility,pHeroAttr->m_hp,pHeroAttr->m_skill[0],pHeroAttr->m_skill[1],pHeroAttr->m_skill[2],pHeroAttr->m_skill[3]);
	if( fp )
	{
		fprintf( fp, "%s;\n", szSQL );
	}
	else if( mysql_query( myGame, szSQL ) )
	{
		printf( "Query failed (%s)\n", mysql_error(myGame) );
		write_gamelog( "%s", szSQL );
		if( reconnect_flag )
			return -1;
		if( mysql_ping(myGame) != 0 )
		{
			db_reconnect_game();
			reconnect_flag = 1;
			goto RE_HEROATTR_UPDATE;
		}
		return -1;
	}
	return 0;
}

int actor_hero_load_auto( int actor_index, LPCB_GETHEROATTR pCB_GetHeroAttr, int actorid, char *pTab )
{
	MYSQL_RES		*res;
	MYSQL_ROW		row;
	char	szSQL[8192];
	int index;
	HeroAttr *pHeroAttr;

	sprintf( szSQL, "select `actorid`,`offset`,`herokind`,`name`,`shape`,`level`,`experience`,`feats`,`official`,`mastership`,`health`,`grade`,`state`,`attack`,`defance`,`agility`,`hp`,`skill0`,`skill1`,`skill2`,`skill3` from %s where actorid='%d'", pTab,actorid );
	if( mysql_query( myGame, szSQL ) )
	{
		printf( "Query failed (%s)\n", mysql_error(myGame) );
		write_gamelog( "%s", szSQL );
		if( mysql_ping(myGame) != 0 )
			db_reconnect_game();
		return -1;
	}
	res = mysql_store_result( myGame );

	while( ( row = mysql_fetch_row( res ) ) )
	{
		index = 0;
		pHeroAttr = pCB_GetHeroAttr( actor_index, atoi(row[1]) );
		if( pHeroAttr == NULL )
			continue;
		pHeroAttr->m_actorid = atoi(row[index++]);
		pHeroAttr->m_offset = atoi(row[index++]);
		pHeroAttr->m_herokind = atoi(row[index++]);
		memcpy( pHeroAttr->m_name, row[index++], 22 ); pHeroAttr->m_name[21]=0;
		pHeroAttr->m_shape = atoi(row[index++]);
		pHeroAttr->m_level = atoi(row[index++]);
		pHeroAttr->m_experience = atoll(row[index++]);
		pHeroAttr->m_feats = atoi(row[index++]);
		pHeroAttr->m_official = atoi(row[index++]);
		pHeroAttr->m_mastership = atoi(row[index++]);
		pHeroAttr->m_health = atoi(row[index++]);
		pHeroAttr->m_grade = atoi(row[index++]);
		pHeroAttr->m_state = atoi(row[index++]);
		pHeroAttr->m_attack = atoi(row[index++]);
		pHeroAttr->m_defance = atoi(row[index++]);
		pHeroAttr->m_agility = atoi(row[index++]);
		pHeroAttr->m_hp = atoi(row[index++]);
		pHeroAttr->m_skill[0] = atoi(row[index++]);
		pHeroAttr->m_skill[1] = atoi(row[index++]);
		pHeroAttr->m_skill[2] = atoi(row[index++]);
		pHeroAttr->m_skill[3] = atoi(row[index++]);
	}
	mysql_free_result( res );
	return 0;
}
