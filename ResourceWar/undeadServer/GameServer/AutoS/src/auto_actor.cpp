#ifdef WIN32
#include <winsock2.h>
#include <windows.h>
#endif
#include <mysql.h>
#include "db.h"
#include "actor.h"
#include "auto_actor.h"
extern MYSQL *myGame;

int actor_save_auto( Actor *pActor, char *pTab, FILE *fp )
{
	char	szSQL[8192];
	char reconnect_flag = 0;
	char szText_lastip[MAX_PATH]={0};
	char szText_name[MAX_PATH]={0};
	char sz64_experience[21]={0};
	char szText_chat_mask_list[sizeof(int)*64*2]={0};
RE_ACTOR_UPDATE:
	sprintf( szSQL, "REPLACE INTO %s (`actorid`,`lastip`,`createtime`,`forbidtime`,`name`,`aclass`,`shape`,`level`,`experience`,`leadership`,`token`,`total_charge`,`itemext`,`chat_mask_list`) Values('%d','%s','%d','%d','%s','%d','%d','%d','%s','%d','%d','%d','%d','%s')",pTab,pActor->actorid,db_escape((const char *)pActor->lastip,szText_lastip,0),pActor->createtime,pActor->forbidtime,db_escape((const char *)pActor->name,szText_name,0),pActor->aclass,pActor->shape,pActor->level,lltoa(pActor->experience,sz64_experience,10 ),pActor->leadership,pActor->token,pActor->total_charge,pActor->itemext,db_escape((const char *)pActor->chat_mask_list,szText_chat_mask_list,sizeof(int)*64));
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
			goto RE_ACTOR_UPDATE;
		}
		return -1;
	}
	return 0;
}

int actor_load_auto( Actor *pActor, char *pTab )
{
	MYSQL_RES		*res;
	MYSQL_ROW		row;
	char	szSQL[8192];
	int index;

	sprintf( szSQL, "select `actorid`,`admin`,`lastip`,`createtime`,`forbidtime`,`name`,`aclass`,`shape`,`level`,`experience`,`leadership`,`token`,`total_charge`,`itemext`,`chat_mask_list` from %s where actorid='%d'", pTab, pActor->actorid );
	if( mysql_query( myGame, szSQL ) )
	{
		printf( "Query failed (%s)\n", mysql_error(myGame) );
		write_gamelog( "%s", szSQL );
		if( mysql_ping(myGame) != 0 )
			db_reconnect_game();
		return -1;
	}
	res = mysql_store_result( myGame );

	if( ( row = mysql_fetch_row( res ) ) )
	{
		index = 0;
		pActor->actorid = atoi(row[index++]);
		pActor->admin = atoi(row[index++]);
		memcpy( pActor->lastip, row[index++], 16 ); pActor->lastip[15]=0;
		pActor->createtime = atoi(row[index++]);
		pActor->forbidtime = atoi(row[index++]);
		memcpy( pActor->name, row[index++], 22 ); pActor->name[21]=0;
		pActor->aclass = atoi(row[index++]);
		pActor->shape = atoi(row[index++]);
		pActor->level = atoi(row[index++]);
		pActor->experience = atoll(row[index++]);
		pActor->leadership = atoi(row[index++]);
		pActor->token = atoi(row[index++]);
		pActor->total_charge = atoi(row[index++]);
		pActor->itemext = atoi(row[index++]);
		memcpy( pActor->chat_mask_list, row[index++], sizeof(int)*64 );
	}
	else
	{
		mysql_free_result( res );
		return -1;
	}
	mysql_free_result( res );
	return 0;
}
