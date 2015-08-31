#ifdef WIN32
#include <winsock2.h>
#include <mswsock.h>
#endif
#include <stdio.h>
#include <string.h>

#include <mysql.h>
#include "struct_config.h"
#include "actordata_config.h"
#include "utils.h"

extern MYSQL *myData;

#define MAX_ACTORDATA_COUNT	2
// 任务目标的分组信息
SStructConfigGroup g_actordata_struct[MAX_ACTORDATA_COUNT];
const char *g_actordata_table[] = { "actor","actor_attr","item","npc","quest","npclocal","friend" };
const char *g_actordata_table_word[] = { "Actor","ActorAttr","Item","Npc","Quest","NpcCopy","Friend" };
const char *g_actordata_table_upper[] = { "ACTOR","ACTORATTR","ITEM","NPC","QUEST","NPCCOPY","FRIEND" };
const int g_actordata_type[] =        {    0,       0,         2,     0,    0,      2,        2 };	// 2表示拥有回调函数

char *actordata_getdatatype( short datatype, char *ptr, char *lkset )
{
	char ispoint = 0;
	if( datatype > 100 )
	{
		ispoint = 1;
		datatype -= 100;
	}
	if( datatype < 0 || datatype >= MAX_ACTORDATA_COUNT )
		return NULL;
	if( ispoint )
		sprintf( ptr, "%s *", g_actordata_table_word[datatype] );
	else
		sprintf( ptr, "%s", g_actordata_table_word[datatype] );
	if( lkset )
		strcpy( lkset, "LKSET_MEM" );
	return ptr;
}


char *sql_getdatatype( short datatype, char *ptr, char *lkset, int datasize )
{
	if( ptr == NULL )
		return NULL;

	switch( datatype )
	{
	case DATATYPE_CHAR:
	case DATATYPE_UNCHAR:
	case DATATYPE_BIN:
		if( datasize > 1 )
		{
			sprintf( ptr, "varbinary(%d)", datasize );
			if( lkset )
				strcpy( lkset, "NOT NULL DEFAULT ''" );
		}
		else
		{
			strcpy( ptr, "tinyint(4)" );
			if( lkset )
				strcpy( lkset, "NOT NULL DEFAULT 0" );
		}
		break;
	case DATATYPE_SHORT:
	case DATATYPE_UNSHORT:
		strcpy( ptr, "smallint(6)" );
		if( lkset )
			strcpy( lkset, "NOT NULL DEFAULT 0" );
		break;
	case DATATYPE_INT:
	case DATATYPE_UNINT:
		strcpy( ptr, "int(11)" );
		if( lkset )
			strcpy( lkset, "NOT NULL DEFAULT 0" );
		break;
	case DATATYPE_INT64:
		strcpy( ptr, "bigint(20)" );
		if( lkset )
			strcpy( lkset, "NOT NULL DEFAULT 0" );
		break;
	case DATATYPE_FLOAT:
	case DATATYPE_DOUBLE:
		strcpy( ptr, "double" );
		if( lkset )
			strcpy( lkset, "NOT NULL DEFAULT 0" );
		break;
	default:
		strcpy( ptr, "int(11)" );
		if( lkset )
			strcpy( lkset, "NOT NULL DEFAULT 0" );
		break;
	}
	return ptr;
}

int actordata_init()
{
	MYSQL_RES		*res;
	MYSQL_ROW		row;
	char	szSQL[1024];

	int tmpgroup;
	int tmpobjid;

	memset( g_actordata_struct, 0, sizeof(SStructConfigGroup)*MAX_ACTORDATA_COUNT );


	for( tmpgroup = 0; tmpgroup < MAX_ACTORDATA_COUNT; tmpgroup++ )
	{
		// 确定每个目标组的个数
		sprintf( szSQL, "select count(*) from st_%s_conf", g_actordata_table[tmpgroup] );
		if( mysql_query( myData, szSQL ) )
		{
			printf( "Query failed (%s)\n", mysql_error(myData) );
			write_gamelog( "%s", szSQL );
			return -1;
		}
		res = mysql_store_result( myData );
		while( ( row = mysql_fetch_row( res ) ) )
		{
			g_actordata_struct[tmpgroup].m_count = atoi( row[0] );
			g_actordata_struct[tmpgroup].m_sconf = (SStructConfig *)malloc( sizeof(SStructConfig)*g_actordata_struct[tmpgroup].m_count );
			memset( g_actordata_struct[tmpgroup].m_sconf, 0, sizeof(SStructConfig)*g_actordata_struct[tmpgroup].m_count );

			g_actordata_struct[tmpgroup].m_count = 0;
		}
		mysql_free_result( res );

		// 开始导入数据
		sprintf( szSQL, "select dataid,name,data_type,data_count,data_size,context,notsave,kind,m_fix from st_%s_conf order by `offset`", g_actordata_table[tmpgroup] );
		if( mysql_query( myData, szSQL ) )
		{
			printf( "Query failed (%s)\n", mysql_error(myData) );
			write_gamelog( "%s", szSQL );
			return -1;
		}
		res = mysql_store_result( myData );

		g_actordata_struct[tmpgroup].m_count = 0;
		strcpy( g_actordata_struct[tmpgroup].m_name, g_actordata_table[tmpgroup] );
		g_actordata_struct[tmpgroup].m_structid = -1;
		g_actordata_struct[tmpgroup].m_struct_type = -1;

		while( ( row = mysql_fetch_row( res ) ) )
		{
			tmpobjid = atoi( row[0] );

			g_actordata_struct[tmpgroup].m_sconf[g_actordata_struct[tmpgroup].m_count].m_dataid = tmpobjid;
			strncpy( g_actordata_struct[tmpgroup].m_sconf[g_actordata_struct[tmpgroup].m_count].m_name, row[1], 32 );
			g_actordata_struct[tmpgroup].m_sconf[g_actordata_struct[tmpgroup].m_count].data_type = atoi( row[2] );
			g_actordata_struct[tmpgroup].m_sconf[g_actordata_struct[tmpgroup].m_count].data_count = atoi( row[3] );
			g_actordata_struct[tmpgroup].m_sconf[g_actordata_struct[tmpgroup].m_count].data_size = atoi( row[4] );
			strncpy( g_actordata_struct[tmpgroup].m_sconf[g_actordata_struct[tmpgroup].m_count].m_context, row[5], 128 );
			g_actordata_struct[tmpgroup].m_sconf[g_actordata_struct[tmpgroup].m_count].m_notsave = atoi( row[6] );
			g_actordata_struct[tmpgroup].m_sconf[g_actordata_struct[tmpgroup].m_count].m_kind = atoi( row[7] );
			g_actordata_struct[tmpgroup].m_sconf[g_actordata_struct[tmpgroup].m_count].m_fix = atoi( row[8] );

			if( g_actordata_struct[tmpgroup].m_sconf[g_actordata_struct[tmpgroup].m_count].data_count == 0 )
				g_actordata_struct[tmpgroup].m_sconf[g_actordata_struct[tmpgroup].m_count].data_size = 1;
			else if( g_actordata_struct[tmpgroup].m_sconf[g_actordata_struct[tmpgroup].m_count].data_count > 0 )
				g_actordata_struct[tmpgroup].m_sconf[g_actordata_struct[tmpgroup].m_count].data_size = g_actordata_struct[tmpgroup].m_sconf[g_actordata_struct[tmpgroup].m_count].data_count;
			else if( g_actordata_struct[tmpgroup].m_sconf[g_actordata_struct[tmpgroup].m_count].data_size < 0 )
				g_actordata_struct[tmpgroup].m_sconf[g_actordata_struct[tmpgroup].m_count].data_size = 1;

			g_actordata_struct[tmpgroup].m_count++;
		}
		mysql_free_result( res );
	}
	return 0;
}

int actordata_create()
{
	int tmpgroup;
	int tmpi;
	char tmpstr[256];
	char szFile[256];
	char szPath[256];
	char szFix[8];
	char tmpfield[256];
	char **linebuf = NULL;
	int linecount;
	FILE *fp;

	strcpy( szPath, SERVER_CREATE_PATH );
	for( tmpgroup = 0; tmpgroup < MAX_ACTORDATA_COUNT; tmpgroup++ )
	{
		sprintf( szFile, "%s/include/auto_%s.h", szPath, g_actordata_table[tmpgroup] );

		fp = fopen( szFile, "rb" );
		if( fp != NULL )
		{
			linebuf = u_load_file( fp, &linecount );
			fclose( fp );
		}

		fp = fopen( szFile, "wb" );
		if( fp == NULL )
			return -1;

		if( linebuf == NULL )
		{
			fprintf( fp, "#ifndef __%s_AUTO_H\r\n", g_actordata_table_upper[tmpgroup] );
			fprintf( fp, "#define __%s_AUTO_H\r\n", g_actordata_table_upper[tmpgroup] );
			fprintf( fp, "\r\n" );

			fprintf( fp, "struct _%s {\r\n", g_actordata_table[tmpgroup] );
			fprintf( fp, "//--Automatically generated\r\n" );
		}
		else
		{
			for( tmpi = 0; tmpi < linecount; tmpi++ )
			{
				fprintf( fp, "%s\n", linebuf[tmpi] );
				if( u_frontcmp( linebuf[tmpi], "//--Automatically generated" ) )
					break;
			}
		}
		for( tmpi = 0; tmpi < g_actordata_struct[tmpgroup].m_count; tmpi++ )
		{
			if( struct_getdatatype( g_actordata_struct[tmpgroup].m_sconf[tmpi].data_type, tmpstr, NULL, 0 ) == NULL )
			{
				fclose( fp );
				return -1;
			}

			if( g_actordata_struct[tmpgroup].m_sconf[tmpi].m_fix )
				strcpy( szFix, "m_" );
			else
				szFix[0] = 0;
			if( g_actordata_struct[tmpgroup].m_sconf[tmpi].data_size > 1 )
				fprintf( fp, "\t\t\t\t%s %s%s[%d];\t//%s\r\n", tmpstr, szFix, g_actordata_struct[tmpgroup].m_sconf[tmpi].m_name, g_actordata_struct[tmpgroup].m_sconf[tmpi].data_size, g_actordata_struct[tmpgroup].m_sconf[tmpi].m_context );
			else
				fprintf( fp, "\t\t\t\t%s %s%s;\t//%s\r\n", tmpstr, szFix, g_actordata_struct[tmpgroup].m_sconf[tmpi].m_name, g_actordata_struct[tmpgroup].m_sconf[tmpi].m_context );
		}
		fprintf( fp, "};\r\n" );
		fprintf( fp, "typedef struct _%s %s;\r\n", g_actordata_table[tmpgroup], g_actordata_table_word[tmpgroup] );
		fprintf( fp, "\r\n" );

		if( g_actordata_type[tmpgroup] == 2 )
		{
			tmpfield[0] = 0;
			for( tmpi = 0; tmpi < g_actordata_struct[tmpgroup].m_count; tmpi++ )
			{
				if( g_actordata_struct[tmpgroup].m_sconf[tmpi].m_kind == 2 )
				{
					strcpy( tmpfield, g_actordata_struct[tmpgroup].m_sconf[tmpi].m_name );
					break;
				}
			}
			fprintf( fp, "typedef %s * ( *LPCB_GET%s)( int actor_index, int offset );\r\n", g_actordata_table_word[tmpgroup], g_actordata_table_upper[tmpgroup] );
			fprintf( fp, "int %s_load_auto( int actor_index, LPCB_GET%s pCB_Get%s, int %s, char *pTab );\r\n", g_actordata_table[tmpgroup], g_actordata_table_upper[tmpgroup], g_actordata_table_word[tmpgroup], tmpfield );
		}
		else
		{
			fprintf( fp, "int %s_load_auto( %s *p%s, char *pTab );\r\n", g_actordata_table[tmpgroup], g_actordata_table_word[tmpgroup], g_actordata_table_word[tmpgroup] );
		}
		fprintf( fp, "int %s_save_auto( %s *p%s, char *pTab, FILE *fp );\r\n", g_actordata_table[tmpgroup], g_actordata_table_word[tmpgroup], g_actordata_table_word[tmpgroup] );

		fprintf( fp, "\r\n#endif\r\n" );

		fclose( fp );

		if( linebuf )
		{
			u_free_vec( linebuf );
			linebuf = NULL;
			linecount = 0;
		}
	}

	return actorsave_create();
}

int actorsave_create()
{
	int tmpgroup;
	int tmpi;
	char tmpstr[256];
	char szFile[256];
	char szPath[256];
	char szFix[8];
	int index;
	char tmpfield[256];

	FILE *fp;

	strcpy( szPath, SERVER_CREATE_PATH );
	for( tmpgroup = 0; tmpgroup < MAX_ACTORDATA_COUNT; tmpgroup++ )
	{
		sprintf( szFile, "%s/src/auto_%s.cpp", szPath, g_actordata_table[tmpgroup] );

		fp = fopen( szFile, "wb" );
		if( fp == NULL )
			return -1;

		fprintf( fp, "#ifdef WIN32\r\n" );
		fprintf( fp, "#include <winsock2.h>\r\n" );
		fprintf( fp, "#include <windows.h>\r\n" );
		fprintf( fp, "#endif\r\n" );
		fprintf( fp, "#include <mysql.h>\r\n" );
		fprintf( fp, "#include \"db.h\"\r\n" );
		fprintf( fp, "#include \"actor.h\"\r\n" );
		fprintf( fp, "#include \"auto_%s.h\"\r\n", g_actordata_table[tmpgroup] );
		fprintf( fp, "extern MYSQL *myGame;\r\n\r\n" );

		fprintf( fp, "int %s_save_auto( %s *p%s, char *pTab, FILE *fp )\r\n", g_actordata_table[tmpgroup], g_actordata_table_word[tmpgroup], g_actordata_table_word[tmpgroup] );
		fprintf( fp, "{\r\n" );

		fprintf( fp, "	char	szSQL[8192];\r\n" );
		fprintf( fp, "	char reconnect_flag = 0;\r\n" );

		for( tmpi = 0; tmpi < g_actordata_struct[tmpgroup].m_count; tmpi++ )
		{
			if( g_actordata_struct[tmpgroup].m_sconf[tmpi].m_notsave > 0 || g_actordata_struct[tmpgroup].m_sconf[tmpi].data_type <= -100 || g_actordata_struct[tmpgroup].m_sconf[tmpi].data_type >= 0 )
				continue;
			if ( g_actordata_struct[tmpgroup].m_sconf[tmpi].data_type == DATATYPE_INT64 )
			{
				fprintf( fp, "	char sz64_%s[21]={};\r\n", g_actordata_struct[tmpgroup].m_sconf[tmpi].m_name );
			}
			else if ( g_actordata_struct[tmpgroup].m_sconf[tmpi].data_size > 1 && g_actordata_struct[tmpgroup].m_sconf[tmpi].data_type == DATATYPE_CHAR )
			{
				fprintf( fp, "	char szText_%s[MAX_PATH]={};\r\n", g_actordata_struct[tmpgroup].m_sconf[tmpi].m_name );
			}
			else if ( g_actordata_struct[tmpgroup].m_sconf[tmpi].data_size > 1 && g_actordata_struct[tmpgroup].m_sconf[tmpi].data_type == DATATYPE_BIN )
			{
				struct_getdatatype( g_actordata_struct[tmpgroup].m_sconf[tmpi].data_count, tmpstr, NULL, 0 );
				fprintf( fp, "	char szText_%s[sizeof(%s)*%d+1]={0};\r\n", g_actordata_struct[tmpgroup].m_sconf[tmpi].m_name, tmpstr, g_actordata_struct[tmpgroup].m_sconf[tmpi].data_size );
			}
		}

		fprintf( fp, "RE_%s_UPDATE:\r\n", g_actordata_table_upper[tmpgroup] );

		fprintf( fp, "	sprintf( szSQL, \"REPLACE INTO %%s (" );
		for( tmpi = 0; tmpi < g_actordata_struct[tmpgroup].m_count; tmpi++ )
		{
			if( g_actordata_struct[tmpgroup].m_sconf[tmpi].m_notsave > 0 || g_actordata_struct[tmpgroup].m_sconf[tmpi].data_type <= -100 || g_actordata_struct[tmpgroup].m_sconf[tmpi].data_type >= 0 )
				continue;
			if( tmpi > 0 )
				fprintf( fp, "," );
			if ( g_actordata_struct[tmpgroup].m_sconf[tmpi].data_size > 1 && g_actordata_struct[tmpgroup].m_sconf[tmpi].data_type != DATATYPE_CHAR )
			{
				for( index = 0; index < g_actordata_struct[tmpgroup].m_sconf[tmpi].data_size; index++ )
				{
					if( index > 0 )
						fprintf( fp, "," );
					fprintf( fp, "`%s%d`",  g_actordata_struct[tmpgroup].m_sconf[tmpi].m_name, index );
				}
			}
			else
			{
				fprintf( fp, "`%s`",  g_actordata_struct[tmpgroup].m_sconf[tmpi].m_name );
			}
		}
		fprintf( fp, ") Values(" );
		for( tmpi = 0; tmpi < g_actordata_struct[tmpgroup].m_count; tmpi++ )
		{
			if( g_actordata_struct[tmpgroup].m_sconf[tmpi].m_notsave > 0 || g_actordata_struct[tmpgroup].m_sconf[tmpi].data_type <= -100 || g_actordata_struct[tmpgroup].m_sconf[tmpi].data_type >= 0 )
				continue;
			if( tmpi > 0 )
				fprintf( fp, "," );

			if ( g_actordata_struct[tmpgroup].m_sconf[tmpi].data_size > 1 && g_actordata_struct[tmpgroup].m_sconf[tmpi].data_type != DATATYPE_CHAR )
			{
				for( index = 0; index < g_actordata_struct[tmpgroup].m_sconf[tmpi].data_size; index++ )
				{
					if( index > 0 )
						fprintf( fp, "," );
					fprintf( fp, "'%%d'" );
				}
			}
			else if ( (g_actordata_struct[tmpgroup].m_sconf[tmpi].data_size > 1 && g_actordata_struct[tmpgroup].m_sconf[tmpi].data_type == DATATYPE_CHAR) || g_actordata_struct[tmpgroup].m_sconf[tmpi].data_type == DATATYPE_INT64 )
			{
				fprintf( fp, "'%%s'" );
			}
			else if ( g_actordata_struct[tmpgroup].m_sconf[tmpi].data_size > 1 && g_actordata_struct[tmpgroup].m_sconf[tmpi].data_type == DATATYPE_BIN )
			{
				fprintf( fp, "'%%s'" );
			}
			else
			{
				fprintf( fp, "'%%d'" );
			}
		}
		fprintf( fp, ")\",pTab," );
		for( tmpi = 0; tmpi < g_actordata_struct[tmpgroup].m_count; tmpi++ )
		{
			if( g_actordata_struct[tmpgroup].m_sconf[tmpi].m_notsave > 0 || g_actordata_struct[tmpgroup].m_sconf[tmpi].data_type <= -100 || g_actordata_struct[tmpgroup].m_sconf[tmpi].data_type >= 0 )
				continue;
			if( struct_getdatatype( g_actordata_struct[tmpgroup].m_sconf[tmpi].data_type, tmpstr, NULL, 0 ) == NULL )
			{
				fclose( fp );
				return -1;
			}

			if( g_actordata_struct[tmpgroup].m_sconf[tmpi].m_fix )
				strcpy( szFix, "m_" );
			else
				szFix[0] = 0;

			if( tmpi > 0 )
				fprintf( fp, "," );

			if ( g_actordata_struct[tmpgroup].m_sconf[tmpi].data_size > 1 && g_actordata_struct[tmpgroup].m_sconf[tmpi].data_type != DATATYPE_CHAR )
			{
				for( index = 0; index < g_actordata_struct[tmpgroup].m_sconf[tmpi].data_size; index++ )
				{
					if( index > 0 )
						fprintf( fp, "," );
					fprintf( fp, "p%s->%s%s[%d]", g_actordata_table_word[tmpgroup], szFix, g_actordata_struct[tmpgroup].m_sconf[tmpi].m_name, index );
				}
			}
			else if ( g_actordata_struct[tmpgroup].m_sconf[tmpi].data_type == DATATYPE_INT64 )
			{
				fprintf( fp, "lltoa(p%s->%s%s,sz64_%s,10 )", g_actordata_table_word[tmpgroup], szFix, g_actordata_struct[tmpgroup].m_sconf[tmpi].m_name, g_actordata_struct[tmpgroup].m_sconf[tmpi].m_name );
			}
			else if ( g_actordata_struct[tmpgroup].m_sconf[tmpi].data_size > 1 && g_actordata_struct[tmpgroup].m_sconf[tmpi].data_type == DATATYPE_CHAR )
			{
				fprintf( fp, "db_escape((const char *)p%s->%s%s,szText_%s,%d)", g_actordata_table_word[tmpgroup], szFix, g_actordata_struct[tmpgroup].m_sconf[tmpi].m_name, g_actordata_struct[tmpgroup].m_sconf[tmpi].m_name, g_actordata_struct[tmpgroup].m_sconf[tmpi].data_size );
			}
			else if ( g_actordata_struct[tmpgroup].m_sconf[tmpi].data_size > 1 && g_actordata_struct[tmpgroup].m_sconf[tmpi].data_type == DATATYPE_BIN )
			{
				fprintf( fp, "db_escape((const char *)p%s->%s%s,szText_%s,%d)", g_actordata_table_word[tmpgroup], szFix, g_actordata_struct[tmpgroup].m_sconf[tmpi].m_name, g_actordata_struct[tmpgroup].m_sconf[tmpi].m_name, g_actordata_struct[tmpgroup].m_sconf[tmpi].data_size );
			}
			else
			{
				fprintf( fp, "p%s->%s%s", g_actordata_table_word[tmpgroup], szFix, g_actordata_struct[tmpgroup].m_sconf[tmpi].m_name );
			}
		}
		fprintf( fp, ");\r\n" );
		fprintf( fp, "	if( fp )\r\n" );
		fprintf( fp, "	{\r\n" );
		fprintf( fp, "		fprintf( fp, \"%%s;\\n\", szSQL );\r\n" );
		fprintf( fp, "	}\r\n" );
		fprintf( fp, "	else if( mysql_query( myGame, szSQL ) )\r\n" );
		fprintf( fp, "	{\r\n" );
		fprintf( fp, "		printf( \"Query failed (%%s)\\n\", mysql_error(myGame) );\r\n" );
		fprintf( fp, "		write_gamelog( \"%%s\", szSQL );\r\n" );
		fprintf( fp, "		if( reconnect_flag )\r\n" );
		fprintf( fp, "			return -1;\r\n" );
		fprintf( fp, "		if( mysql_ping(myGame) != 0 )\r\n" );
		fprintf( fp, "		{\r\n" );
		fprintf( fp, "			db_reconnect_game();\r\n" );
		fprintf( fp, "			reconnect_flag = 1;\r\n" );
		fprintf( fp, "			goto RE_%s_UPDATE;\r\n", g_actordata_table_upper[tmpgroup] );
		fprintf( fp, "		}\r\n" );
		fprintf( fp, "		return -1;\r\n" );
		fprintf( fp, "	}\r\n" );
		fprintf( fp, "	return 0;\r\n" );
		fprintf( fp, "}\r\n" );
		fprintf( fp, "\r\n" );

		if( g_actordata_type[tmpgroup] == 2 )
		{
			tmpfield[0] = 0;
			for( tmpi = 0; tmpi < g_actordata_struct[tmpgroup].m_count; tmpi++ )
			{
				if( g_actordata_struct[tmpgroup].m_sconf[tmpi].m_kind == 2 )
				{
					strcpy( tmpfield, g_actordata_struct[tmpgroup].m_sconf[tmpi].m_name );
					break;
				}
			}
			fprintf( fp, "int %s_load_auto( int actor_index, LPCB_GET%s pCB_Get%s, int %s, char *pTab )\r\n", g_actordata_table[tmpgroup], g_actordata_table_upper[tmpgroup], g_actordata_table_word[tmpgroup], tmpfield );
		}
		else
		{
			fprintf( fp, "int %s_load_auto( %s *p%s, char *pTab )\r\n", g_actordata_table[tmpgroup], g_actordata_table_word[tmpgroup], g_actordata_table_word[tmpgroup] );
		}
		fprintf( fp, "{\r\n");
		fprintf( fp, "	MYSQL_RES		*res;\r\n");
		fprintf( fp, "	MYSQL_ROW		row;\r\n");
		fprintf( fp, "	char	szSQL[8192];\r\n");
		fprintf( fp, "	int index;\r\n");
		if( g_actordata_type[tmpgroup] == 2 )
		{
			fprintf( fp, "	%s *p%s;\r\n", g_actordata_table_word[tmpgroup], g_actordata_table_word[tmpgroup] );
		}
		fprintf( fp, "\r\n");
		fprintf( fp, "	sprintf( szSQL, \"select ");
		for( tmpi = 0; tmpi < g_actordata_struct[tmpgroup].m_count; tmpi++ )
		{
			if( g_actordata_struct[tmpgroup].m_sconf[tmpi].m_notsave == 1 || g_actordata_struct[tmpgroup].m_sconf[tmpi].m_notsave == 3 || g_actordata_struct[tmpgroup].m_sconf[tmpi].data_type <= -100 || g_actordata_struct[tmpgroup].m_sconf[tmpi].data_type >= 0 )
				continue;
			if( tmpi > 0 )
				fprintf( fp, "," );
			if ( g_actordata_struct[tmpgroup].m_sconf[tmpi].data_size > 1 && g_actordata_struct[tmpgroup].m_sconf[tmpi].data_type != DATATYPE_CHAR )
			{
				for( index = 0; index < g_actordata_struct[tmpgroup].m_sconf[tmpi].data_size; index++ )
				{
					if( index > 0 )
						fprintf( fp, "," );
					fprintf( fp, "`%s%d`",  g_actordata_struct[tmpgroup].m_sconf[tmpi].m_name, index );
				}
			}
			else
			{
				fprintf( fp, "`%s`",  g_actordata_struct[tmpgroup].m_sconf[tmpi].m_name );
			}
		}
		if( g_actordata_struct[tmpgroup].m_sconf[0].m_fix )
			strcpy( szFix, "m_" );
		else
			szFix[0] = 0;
		fprintf( fp, " from %%s " );
		if( g_actordata_type[tmpgroup] == 2 )
		{
			fprintf( fp, "where %s='%%d'", tmpfield );
			fprintf( fp, "\", pTab" );
			fprintf( fp, ",%s );\r\n", tmpfield );
		}
		else
		{
			index = 0;
			for( tmpi = 0; tmpi < g_actordata_struct[tmpgroup].m_count; tmpi++ )
			{
				if( g_actordata_struct[tmpgroup].m_sconf[tmpi].m_kind == 1 )
				{
					if( index == 0 )
					{
						fprintf( fp, "where %s='%%d'", g_actordata_struct[tmpgroup].m_sconf[tmpi].m_name );
					}
					else
					{
						fprintf( fp, " and %s='%%d'", g_actordata_struct[tmpgroup].m_sconf[tmpi].m_name );
					}
					index++;
				}
			}
			fprintf( fp, "\", pTab" );
			index = 0;
			for( tmpi = 0; tmpi < g_actordata_struct[tmpgroup].m_count; tmpi++ )
			{
				if( g_actordata_struct[tmpgroup].m_sconf[tmpi].m_kind == 1 )
				{
					fprintf( fp, ", p%s->%s%s", g_actordata_table_word[tmpgroup], szFix, g_actordata_struct[tmpgroup].m_sconf[tmpi].m_name );
					index++;
				}
			}
			fprintf( fp, " );\r\n" );
		}

		fprintf( fp, "	if( mysql_query( myGame, szSQL ) )\r\n" );
		fprintf( fp, "	{\r\n" );
		fprintf( fp, "		printf( \"Query failed (%%s)\\n\", mysql_error(myGame) );\r\n" );
		fprintf( fp, "		write_gamelog( \"%%s\", szSQL );\r\n" );
		fprintf( fp, "		if( mysql_ping(myGame) != 0 )\r\n" );
		fprintf( fp, "			db_reconnect_game();\r\n" );
		fprintf( fp, "		return -1;\r\n" );
		fprintf( fp, "	}\r\n" );
		fprintf( fp, "	res = mysql_store_result( myGame );\r\n" );
		fprintf( fp, "\r\n" );
		if( g_actordata_type[tmpgroup] == 2 )
		{
			fprintf( fp, "	while( ( row = mysql_fetch_row( res ) ) )\r\n" );
			fprintf( fp, "	{\r\n" );
			fprintf( fp, "		index = 0;\r\n" );
			index = 0;
			for( tmpi = 0; tmpi < g_actordata_struct[tmpgroup].m_count; tmpi++ )
			{
				if( g_actordata_struct[tmpgroup].m_sconf[tmpi].m_kind == 3 )
				{
					index = tmpi;
					break;
				}
			}
			fprintf( fp, "		p%s = pCB_Get%s( actor_index, atoi(row[%d]) );\r\n", g_actordata_table_word[tmpgroup], g_actordata_table_word[tmpgroup], index );
			fprintf( fp, "		if( p%s == NULL )\r\n", g_actordata_table_word[tmpgroup] );
			fprintf( fp, "			continue;\r\n" );
		}
		else
		{
			fprintf( fp, "	if( ( row = mysql_fetch_row( res ) ) )\r\n" );
			fprintf( fp, "	{\r\n" );
			fprintf( fp, "		index = 0;\r\n" );
		}

		for( tmpi = 0; tmpi < g_actordata_struct[tmpgroup].m_count; tmpi++ )
		{
			if( g_actordata_struct[tmpgroup].m_sconf[tmpi].m_notsave == 1 || g_actordata_struct[tmpgroup].m_sconf[tmpi].m_notsave == 3 || g_actordata_struct[tmpgroup].m_sconf[tmpi].data_type <= -100 || g_actordata_struct[tmpgroup].m_sconf[tmpi].data_type >= 0 )
				continue;
			if( struct_getdatatype( g_actordata_struct[tmpgroup].m_sconf[tmpi].data_type, tmpstr, NULL, 0 ) == NULL )
			{
				fclose( fp );
				return -1;
			}

			if( g_actordata_struct[tmpgroup].m_sconf[tmpi].m_fix )
				strcpy( szFix, "m_" );
			else
				szFix[0] = 0;

			if ( g_actordata_struct[tmpgroup].m_sconf[tmpi].data_size > 1 && g_actordata_struct[tmpgroup].m_sconf[tmpi].data_type != DATATYPE_CHAR )
			{
				for( index = 0; index < g_actordata_struct[tmpgroup].m_sconf[tmpi].data_size; index++ )
				{
					fprintf( fp, "		p%s->%s%s[%d] = atoi(row[index++]);\r\n", g_actordata_table_word[tmpgroup], szFix, g_actordata_struct[tmpgroup].m_sconf[tmpi].m_name, index );
				}
			}
			else if ( g_actordata_struct[tmpgroup].m_sconf[tmpi].data_size > 1 && g_actordata_struct[tmpgroup].m_sconf[tmpi].data_type == DATATYPE_CHAR )
			{
				fprintf( fp, "		memcpy( p%s->%s%s, row[index++], %d ); p%s->%s%s[%d]=0;\r\n", g_actordata_table_word[tmpgroup], szFix, g_actordata_struct[tmpgroup].m_sconf[tmpi].m_name, g_actordata_struct[tmpgroup].m_sconf[tmpi].data_size, g_actordata_table_word[tmpgroup], szFix, g_actordata_struct[tmpgroup].m_sconf[tmpi].m_name, g_actordata_struct[tmpgroup].m_sconf[tmpi].data_size-1 );
			}
			else if ( g_actordata_struct[tmpgroup].m_sconf[tmpi].data_size > 1 && g_actordata_struct[tmpgroup].m_sconf[tmpi].data_type == DATATYPE_BIN )
			{
				fprintf( fp, "		memcpy( p%s->%s%s, row[index++], %d ); p%s->%s%s[%d]=0;\r\n", g_actordata_table_word[tmpgroup], szFix, g_actordata_struct[tmpgroup].m_sconf[tmpi].m_name, g_actordata_struct[tmpgroup].m_sconf[tmpi].data_size, g_actordata_table_word[tmpgroup], szFix, g_actordata_struct[tmpgroup].m_sconf[tmpi].m_name, g_actordata_struct[tmpgroup].m_sconf[tmpi].data_size - 1 );
			}
			else if ( g_actordata_struct[tmpgroup].m_sconf[tmpi].data_type == DATATYPE_INT64 )
			{
				fprintf( fp, "		p%s->%s%s = atoll(row[index++]);\r\n", g_actordata_table_word[tmpgroup], szFix, g_actordata_struct[tmpgroup].m_sconf[tmpi].m_name );
			}
			else
			{
				fprintf( fp, "		p%s->%s%s = atoi(row[index++]);\r\n", g_actordata_table_word[tmpgroup], szFix, g_actordata_struct[tmpgroup].m_sconf[tmpi].m_name );
			}
		}
		fprintf( fp, "	}\r\n" );
		if( g_actordata_type[tmpgroup] == 2 )
		{
		}
		else
		{
			fprintf( fp, "	else\r\n" );
			fprintf( fp, "	{\r\n" );
			fprintf( fp, "		mysql_free_result( res );\r\n" );
			fprintf( fp, "		return -1;\r\n" );
			fprintf( fp, "	}\r\n" );
		}
		fprintf( fp, "	mysql_free_result( res );\r\n" );
		fprintf( fp, "	return 0;\r\n" );
		fprintf( fp, "}\r\n" );

		fclose( fp );
	}

	return actorsql_create();
}

int actorsql_create()
{
	int tmpgroup;
	int tmpi;
	char tmpstr[256];
	char szFile[256];
	char szPath[256];
	char lkset[256];
	int index;

	FILE *fp;

	strcpy( szPath, SERVER_CREATE_PATH );
	for( tmpgroup = 0; tmpgroup < MAX_ACTORDATA_COUNT; tmpgroup++ )
	{
		sprintf( szFile, "%s/auto_%s.sql", szPath, g_actordata_table[tmpgroup] );

		fp = fopen( szFile, "wb" );
		if( fp == NULL )
			return -1;

		fprintf( fp, "CREATE TABLE `%s` (\r\n", g_actordata_table[tmpgroup] );
		for( tmpi = 0; tmpi < g_actordata_struct[tmpgroup].m_count; tmpi++ )
		{
			if( g_actordata_struct[tmpgroup].m_sconf[tmpi].m_notsave == 1 || g_actordata_struct[tmpgroup].m_sconf[tmpi].data_type <= -100 || g_actordata_struct[tmpgroup].m_sconf[tmpi].data_type >= 0 )
				continue;
			if( sql_getdatatype( g_actordata_struct[tmpgroup].m_sconf[tmpi].data_type, tmpstr, lkset, g_actordata_struct[tmpgroup].m_sconf[tmpi].data_size ) == NULL )
			{
				fclose( fp );
				return -1;
			}
			
			if ( g_actordata_struct[tmpgroup].m_sconf[tmpi].data_size > 1 && g_actordata_struct[tmpgroup].m_sconf[tmpi].data_type != DATATYPE_CHAR )
			{
				for( index = 0; index < g_actordata_struct[tmpgroup].m_sconf[tmpi].data_size; index++ )
				{
					fprintf( fp, "  `%s%d` %s %s COMMENT '%s',\r\n", g_actordata_struct[tmpgroup].m_sconf[tmpi].m_name, index, tmpstr, lkset, g_actordata_struct[tmpgroup].m_sconf[tmpi].m_context );
				}
			}
			else
			{
				fprintf( fp, "  `%s` %s %s COMMENT '%s',\r\n", g_actordata_struct[tmpgroup].m_sconf[tmpi].m_name, tmpstr, lkset, g_actordata_struct[tmpgroup].m_sconf[tmpi].m_context );
			}
		}
		index = 0;
		for( tmpi = 0; tmpi < g_actordata_struct[tmpgroup].m_count; tmpi++ )
		{
			if( g_actordata_struct[tmpgroup].m_sconf[tmpi].m_kind == 1 )
			{
				if( index == 0 )
				{
					fprintf( fp, "  PRIMARY KEY (`%s`", g_actordata_struct[tmpgroup].m_sconf[tmpi].m_name );
				}
				else
				{
					fprintf( fp, ",`%s`", g_actordata_struct[tmpgroup].m_sconf[tmpi].m_name );
				}
				index++;
			}
		}
		if( index > 0 )
			fprintf( fp, ")\r\n" );

		index = 0;
		for( tmpi = 0; tmpi < g_actordata_struct[tmpgroup].m_count; tmpi++ )
		{
			if( g_actordata_struct[tmpgroup].m_sconf[tmpi].m_kind == 2 )
			{
				if( index == 0 )
				{
					fprintf( fp, "  KEY (`%s`", g_actordata_struct[tmpgroup].m_sconf[tmpi].m_name );
				}
				else
				{
					fprintf( fp, ",`%s`", g_actordata_struct[tmpgroup].m_sconf[tmpi].m_name );
				}
				index++;
			}
		}
		if( index > 0 )
			fprintf( fp, ")\r\n" );

		fprintf( fp, ") ENGINE=MyISAM;\r\n" );

		fclose( fp );
	}

	return 0;
}
