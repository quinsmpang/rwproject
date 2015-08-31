#ifdef WIN32
#include <winsock2.h>
#include <mswsock.h>
#endif
#include <stdio.h>
#include <string.h>

#include <mysql.h>
#include "struct_config.h"
#include "netcmd_config.h"
#include "utils.h"
#include "actordata_config.h"

extern MYSQL *myData;
extern SNetcmdConfig *g_netscmd;
extern int g_netscmd_count;
extern SNetcmdConfig *g_netccmd;
extern int g_netccmd_count;

// 任务目标的分组信息
SStructConfigGroup *g_struct = NULL;
int g_struct_count = 0;

int struct_reset()
{
	int tmpi;

	for( tmpi = 0; tmpi < g_struct_count; tmpi++ )
	{
		if( g_struct[tmpi].m_sconf )
			free( g_struct[tmpi].m_sconf );
	}
	free( g_struct );
	g_struct = NULL;

	struct_init();
	return 0;
}

int struct_init()
{
	MYSQL_RES		*res;
	MYSQL_ROW		row;
	char	szSQL[1024];

	int tmpgroup;
	int tmpobjid;

	netcmd_init();

	// 确定目标组的个数（虚个数，以最大的一个ID确定内存大小），并且分配structid的内存
	g_struct_count = 0;

	sprintf( szSQL, "select max(structid) from net_struct" );
	if( mysql_query( myData, szSQL ) )
	{
		printf( "Query failed (%s)\n", mysql_error(myData) );
		write_gamelog( "%s", szSQL );
		return -1;
	}
	res = mysql_store_result( myData );
	if( ( row = mysql_fetch_row( res ) ) )
	{
		if( row[0] )
			g_struct_count = atoi( row[0] )+1;
		else
			g_struct_count = 0;
	}
	else
	{
		g_struct_count = 0;
		mysql_free_result( res );
		return -1;
	}
	mysql_free_result( res );

	g_struct = (SStructConfigGroup *)malloc( sizeof(SStructConfigGroup)*g_struct_count );
	memset( g_struct, 0, sizeof(SStructConfigGroup)*g_struct_count );

	// 确定每个目标组的个数
	sprintf( szSQL, "select structid,name,context,struct_type from net_struct order by structid" );
	if( mysql_query( myData, szSQL ) )
	{
		printf( "Query failed (%s)\n", mysql_error(myData) );
		write_gamelog( "%s", szSQL );
		return -1;
	}
	res = mysql_store_result( myData );
	while( ( row = mysql_fetch_row( res ) ) )
	{
		tmpgroup = atoi( row[0] );
		if( tmpgroup < 0 || tmpgroup >= g_struct_count )
		{
			mysql_free_result( res );
			return -1;
		}
		strncpy( g_struct[tmpgroup].m_name, row[1], 32 );
		strncpy( g_struct[tmpgroup].m_context, row[2], 128 );
		g_struct[tmpgroup].m_struct_type = atoi( row[3] );
	}
	mysql_free_result( res );

	// 确定每个目标组的个数
	sprintf( szSQL, "select structid,count(*) from net_struct_conf group by structid" );
	if( mysql_query( myData, szSQL ) )
	{
		printf( "Query failed (%s)\n", mysql_error(myData) );
		write_gamelog( "%s", szSQL );
		return -1;
	}
	res = mysql_store_result( myData );
	while( ( row = mysql_fetch_row( res ) ) )
	{
		tmpgroup = atoi( row[0] );
		if( tmpgroup < 0 || tmpgroup >= g_struct_count )
		{
			continue;
		}
		g_struct[tmpgroup].m_count = atoi( row[1] );
		g_struct[tmpgroup].m_sconf = (SStructConfig *)malloc( sizeof(SStructConfig)*g_struct[tmpgroup].m_count );
		memset( g_struct[tmpgroup].m_sconf, 0, sizeof(SStructConfig)*g_struct[tmpgroup].m_count );

		g_struct[tmpgroup].m_count = 0;
	}
	mysql_free_result( res );

	sprintf( szSQL, "select dataid,structid,name,data_type,data_count,data_size,context,notsave,kind,m_fix from net_struct_conf order by structid,`offset`" );
	if( mysql_query( myData, szSQL ) )
	{
		printf( "Query failed (%s)\n", mysql_error(myData) );
		write_gamelog( "%s", szSQL );
		return -1;
	}
	res = mysql_store_result( myData );

	while( ( row = mysql_fetch_row( res ) ) )
	{
		tmpobjid = atoi( row[0] );
		tmpgroup = atoi( row[1] );
		if( tmpgroup < 0 || tmpgroup >= g_struct_count )
			continue;
		g_struct[tmpgroup].m_sconf[g_struct[tmpgroup].m_count].m_dataid = tmpobjid;
		strncpy( g_struct[tmpgroup].m_sconf[g_struct[tmpgroup].m_count].m_name, row[2], 32 );
		g_struct[tmpgroup].m_sconf[g_struct[tmpgroup].m_count].data_type = atoi( row[3] );
		g_struct[tmpgroup].m_sconf[g_struct[tmpgroup].m_count].data_count = atoi( row[4] );
		g_struct[tmpgroup].m_sconf[g_struct[tmpgroup].m_count].data_size = atoi( row[5] );
		strncpy( g_struct[tmpgroup].m_sconf[g_struct[tmpgroup].m_count].m_context, row[6], 128 );
		g_struct[tmpgroup].m_sconf[g_struct[tmpgroup].m_count].m_notsave = atoi( row[7] );
		g_struct[tmpgroup].m_sconf[g_struct[tmpgroup].m_count].m_kind = atoi( row[8] );
		g_struct[tmpgroup].m_sconf[g_struct[tmpgroup].m_count].m_fix = atoi( row[9] );

		if( g_struct[tmpgroup].m_sconf[g_struct[tmpgroup].m_count].data_count == 0 )
			g_struct[tmpgroup].m_sconf[g_struct[tmpgroup].m_count].data_size = 1;
		else if( g_struct[tmpgroup].m_sconf[g_struct[tmpgroup].m_count].data_count > 0 )
			g_struct[tmpgroup].m_sconf[g_struct[tmpgroup].m_count].data_size = g_struct[tmpgroup].m_sconf[g_struct[tmpgroup].m_count].data_count;
		else if( g_struct[tmpgroup].m_sconf[g_struct[tmpgroup].m_count].data_size < 0 )
			g_struct[tmpgroup].m_sconf[g_struct[tmpgroup].m_count].data_size = 1;

		g_struct[tmpgroup].m_count++;
	}
	mysql_free_result( res );
	return 0;
}

int struct_add( char *struct_name, char *struct_context )
{
	int tmpi;
	char	szSQL[1024];
	for( tmpi = 0; tmpi < g_struct_count; tmpi++ )
	{
		if( strcmp( g_struct[tmpi].m_name, struct_name ) == 0 )
		{
			return -1;
		}
	}

	sprintf( szSQL, "insert into net_struct (structid,name,context) values('%d','%s','%s')", g_struct_count,struct_name,struct_context );
	if( mysql_query( myData, szSQL ) )
	{
		printf( "Query failed (%s)\n", mysql_error(myData) );
		write_gamelog( "%s", szSQL );
		return -1;
	}
	struct_reset();
	return 0;

}

int struct_del( char *struct_name )
{
	int tmpi;
	char	szSQL[1024];
	int structid = -1;
	for( tmpi = 0; tmpi < g_struct_count; tmpi++ )
	{
		if( strcmp( g_struct[tmpi].m_name, struct_name ) == 0 )
		{
			structid = tmpi;
			break;
		}
	}
	if( structid < 0 )
		return -1;

	sprintf( szSQL, "delete from net_struct where structid='%d'", structid );
	if( mysql_query( myData, szSQL ) )
	{
		printf( "Query failed (%s)\n", mysql_error(myData) );
		write_gamelog( "%s", szSQL );
		return -1;
	}

	sprintf( szSQL, "delete from net_struct_conf where structid='%d'", structid );
	if( mysql_query( myData, szSQL ) )
	{
		printf( "Query failed (%s)\n", mysql_error(myData) );
		write_gamelog( "%s", szSQL );
		return -1;
	}

	struct_reset();
	return 0;

}

int struct_adddata( char *name, short type, short count, short size, char *context, char *struct_name )
{
	int tmpi;
	char	szSQL[1024];
	int structid = -1;
	for( tmpi = 0; tmpi < g_struct_count; tmpi++ )
	{
		if( strcmp( g_struct[tmpi].m_name, struct_name ) == 0 )
		{
			structid = tmpi;
			break;
		}
	}
	if( structid < 0 )
		return -1;

	sprintf( szSQL, "insert into net_struct_conf (structid,name,data_type,data_count,data_size,context) values('%d','%s','%d','%d','%d','%s')", structid,name,type,count,size,context );
	if( mysql_query( myData, szSQL ) )
	{
		printf( "Query failed (%s)\n", mysql_error(myData) );
		write_gamelog( "%s", szSQL );
		return -1;
	}

	struct_reset();
	return 0;
}

int struct_deldata( char *name, char *struct_name )
{
	int tmpi;
	char	szSQL[1024];
	int structid = -1;
	int dataid = -1;
	for( tmpi = 0; tmpi < g_struct_count; tmpi++ )
	{
		if( strcmp( g_struct[tmpi].m_name, struct_name ) == 0 )
		{
			structid = tmpi;
			break;
		}
	}
	if( structid < 0 )
		return -1;

	for( tmpi = 0; tmpi < g_struct[structid].m_count; tmpi++ )
	{
		if( strcmp( g_struct[structid].m_sconf[tmpi].m_name, name ) == 0 )
		{
			dataid = g_struct[structid].m_sconf[tmpi].m_dataid;
			break;
		}
	}
	if( dataid < 0 )
		return -1;

	sprintf( szSQL, "delete from net_struct_conf where dataid='%d'", dataid );
	if( mysql_query( myData, szSQL ) )
	{
		printf( "Query failed (%s)\n", mysql_error(myData) );
		write_gamelog( "%s", szSQL );
		return -1;
	}

	struct_reset();
	return 0;
}

char *struct_getdatatype( short datatype, char *ptr, char *lkset, char type )
{
	if( ptr == NULL )
		return NULL;

	if( datatype <= -100 )
	{
		return actordata_getdatatype( (-datatype)-100, ptr, lkset );
	}

	switch( datatype )
	{
	case DATATYPE_CHAR:
	case DATATYPE_BIN:
		strcpy( ptr, "char" );
		if( lkset )
			strcpy( lkset, "LKSET_BYTE" );
		break;
	case DATATYPE_SHORT:
		strcpy( ptr, "short" );
		if( lkset )
			strcpy( lkset, "LKSET_WORD" );
		break;
	case DATATYPE_INT:
		strcpy( ptr, "int" );
		if( lkset )
			strcpy( lkset, "LKSET_DWORD" );
		break;
	case DATATYPE_INT64:
		strcpy( ptr, "i64" );
		if( lkset )
			strcpy( lkset, "LKSET_LONG" );
		break;
	case DATATYPE_FLOAT:
		strcpy( ptr, "float" );
		if( lkset )
			strcpy( lkset, "LKSET_FLOAT" );
		break;
	case DATATYPE_DOUBLE:
		strcpy( ptr, "double" );
		if( lkset )
			strcpy( lkset, "LKSET_DOUBLE" );
		break;
	case DATATYPE_UNCHAR:
		strcpy( ptr, "unsigned char" );
		if( lkset )
			strcpy( lkset, "LKSET_BYTE" );
		break;
	case DATATYPE_UNSHORT:
		strcpy( ptr, "unsigned short" );
		if( lkset )
			strcpy( lkset, "LKSET_WORD" );
		break;
	case DATATYPE_UNINT:
		strcpy( ptr, "unsigned int" );
		if( lkset )
			strcpy( lkset, "LKSET_DWORD" );
		break;
	default:
		if( datatype < 0 || datatype >= g_struct_count )
			return NULL;
		if( type == 0 )
			sprintf( ptr, "SLK_%s", g_struct[datatype].m_name );
		else
			sprintf( ptr, "CLK_%s", g_struct[datatype].m_name );
		if( lkset )
			strcpy( lkset, "LKSET_MEM" );
		break;
	}
	return ptr;
}

int struct_create_h( char type )
{
	int tmpi,tmpj;
	char tmpstr[256];
	char szFile[256];
	char szPath[256];
	char szFix[8];
	FILE *fp;
	FILE *fpbase;
	FILE *fpsend;
	FILE *fprecv;

	if( type == 0 )
	{
		strcpy( szPath, SERVER_CREATE_PATH );
		struct_reset();

		sprintf( szFile, "%s/include/server_struct_auto.h", szPath );
		fpbase = fopen( szFile, "wb" );
		if( fpbase == NULL )
			return -1;

		sprintf( szFile, "%s/include/server_structsend_auto.h", szPath );
		fpsend = fopen( szFile, "wb" );
		if( fpsend == NULL )
			return -1;

		sprintf( szFile, "%s/include/server_structrecv_auto.h", szPath );
		fprecv = fopen( szFile, "wb" );
		if( fprecv == NULL )
			return -1;
	}
	else
	{
		strcpy( szPath, CLIENT_CREATE_PATH );
		struct_reset();

		sprintf( szFile, "%s/include/client_struct_auto.h", szPath );
		fpbase = fopen( szFile, "wb" );
		if( fpbase == NULL )
			return -1;

		sprintf( szFile, "%s/include/client_structsend_auto.h", szPath );
		fpsend = fopen( szFile, "wb" );
		if( fpsend == NULL )
			return -1;

		sprintf( szFile, "%s/include/client_structrecv_auto.h", szPath );
		fprecv = fopen( szFile, "wb" );
		if( fprecv == NULL )
			return -1;
	}

	if( type )
	{
		fp = fpsend;
		fpsend = fprecv;
		fprecv = fp;
	}

	fprintf( fpbase, "#ifndef __STRUCT_AUTO%d_H\r\n", type );
	fprintf( fpbase, "#define __STRUCT_AUTO%d_H\r\n", type );
	fprintf( fpbase, "\r\n" );
	fprintf( fpbase, "#include \"utils.h\"\r\n", type );

	fprintf( fpsend, "#ifndef __STRUCTSEND%d_AUTO_H\r\n", type );
	fprintf( fpsend, "#define __STRUCTSEND%d_AUTO_H\r\n", type );
	if( type == 0 )
		fprintf( fpsend, "#include \"server_struct_auto.h\"\r\n", type );
	else
		fprintf( fpsend, "#include \"client_struct_auto.h\"\r\n", type );
	fprintf( fpsend, "\r\n" );

	fprintf( fprecv, "#ifndef __STRUCTRECV%d_AUTO_H\r\n", type );
	fprintf( fprecv, "#define __STRUCTRECV%d_AUTO_H\r\n", type );
	if( type == 0 )
		fprintf( fprecv, "#include \"server_struct_auto.h\"\r\n", type );
	else
		fprintf( fprecv, "#include \"client_struct_auto.h\"\r\n", type );
	fprintf( fprecv, "\r\n" );

	fprintf( fpbase, "#define CMDS_BASE	0\r\n" );
	fprintf( fpbase, "#define CMDC_BASE	0\r\n" );
	fprintf( fpbase, "\r\n" );

	for( tmpi = 0; tmpi < g_netscmd_count; tmpi++ )
	{
		if( g_netscmd[tmpi].m_cmdstr[0] )
		{
			strcpy( tmpstr, g_netscmd[tmpi].m_cmdstr );
			u_str_toupper( tmpstr );
			if( g_netscmd[tmpi].m_cmdid >= 0 )
				fprintf( fpbase, "#define CMDS_%s	(CMDS_BASE+%d)\r\n", tmpstr, g_netscmd[tmpi].m_cmdid );
			else
				fprintf( fpbase, "#define CMDS_%s	(CMDS_BASE%d)\r\n", tmpstr, g_netscmd[tmpi].m_cmdid );
		}
	}

	fprintf( fpbase, "\r\n" );

	for( tmpi = 0; tmpi < g_netccmd_count; tmpi++ )
	{
		if( type && g_netccmd[tmpi].m_cmdid < 0 )
			continue;
		if( g_netccmd[tmpi].m_cmdstr[0] )
		{
			strcpy( tmpstr, g_netccmd[tmpi].m_cmdstr );
			u_str_toupper( tmpstr );
			if( g_netccmd[tmpi].m_cmdid >= 0 )
				fprintf( fpbase, "#define CMDC_%s	(CMDC_BASE+%d)\r\n", tmpstr, g_netccmd[tmpi].m_cmdid );
			else
				fprintf( fpbase, "#define CMDC_%s	(CMDC_BASE%d)\r\n", tmpstr, g_netccmd[tmpi].m_cmdid );
		}
	}

	for( tmpi = 0; tmpi < g_struct_count; tmpi++ )
	{
		if( type && g_struct[tmpi].m_struct_type == 3 )
			continue;
		if( g_struct[tmpi].m_name[0] )
		{
			if( g_struct[tmpi].m_struct_type == 1 )
				fp = fpsend;
			else if( g_struct[tmpi].m_struct_type == 2 )
				fp = fprecv;
			else
				fp = fpbase;
			if( type == 0 )
				fprintf( fp, "struct _slk_%s {\r\n", g_struct[tmpi].m_name );
			else
				fprintf( fp, "struct _clk_%s {\r\n", g_struct[tmpi].m_name );
			for( tmpj = 0; tmpj < g_struct[tmpi].m_count; tmpj++ )
			{
				if( struct_getdatatype( g_struct[tmpi].m_sconf[tmpj].data_type, tmpstr, NULL, type ) == NULL )
				{
					fclose( fp );
					return -1;
				}

				if( g_struct[tmpi].m_sconf[tmpj].m_fix )
					strcpy( szFix, "m_" );
				else
					szFix[0] = 0;

				if( g_struct[tmpi].m_sconf[tmpj].data_size > 1 )
					fprintf( fp, "\t%s %s%s[%d];\t//%s\r\n", tmpstr, szFix, g_struct[tmpi].m_sconf[tmpj].m_name, g_struct[tmpi].m_sconf[tmpj].data_size, g_struct[tmpi].m_sconf[tmpj].m_context );
				else
					fprintf( fp, "\t%s %s%s;\t//%s\r\n", tmpstr, szFix, g_struct[tmpi].m_sconf[tmpj].m_name, g_struct[tmpi].m_sconf[tmpj].m_context );
			}
			fprintf( fp, "};\r\n" );
			if( type == 0 )
				fprintf( fp, "typedef struct _slk_%s SLK_%s;\t//%s\r\n", g_struct[tmpi].m_name, g_struct[tmpi].m_name, g_struct[tmpi].m_context );
			else
				fprintf( fp, "typedef struct _clk_%s CLK_%s;\t//%s\r\n", g_struct[tmpi].m_name, g_struct[tmpi].m_name, g_struct[tmpi].m_context );
			fprintf( fp, "\r\n" );
		}
	}
	for( tmpi = 0; tmpi < g_struct_count; tmpi++ )
	{
		if( type && g_struct[tmpi].m_struct_type == 3 )
			continue;
		if( g_struct[tmpi].m_name[0] )
		{
			if( g_struct[tmpi].m_struct_type == 1 )
			{
				fp = fpsend;
				if( type == 0 )
					fprintf( fp, "int struct_%s_send( char **pptr, int *psize, SLK_%s *pValue );\r\n", g_struct[tmpi].m_name, g_struct[tmpi].m_name );
				else
					fprintf( fp, "int struct_%s_recv( char **pptr, int *psize, CLK_%s *pValue );\r\n", g_struct[tmpi].m_name, g_struct[tmpi].m_name );
			}
			else if( g_struct[tmpi].m_struct_type == 2 )
			{
				fp = fprecv;
				if( type == 0 )
					fprintf( fp, "int struct_%s_recv( char **pptr, int *psize, SLK_%s *pValue );\r\n", g_struct[tmpi].m_name, g_struct[tmpi].m_name );
				else
					fprintf( fp, "int struct_%s_send( char **pptr, int *psize, CLK_%s *pValue );\r\n", g_struct[tmpi].m_name, g_struct[tmpi].m_name );
			}
			else if( g_struct[tmpi].m_struct_type == 3 )
			{
				fp = fpbase;
				if( type == 0 )
				{
					fprintf( fp, "int struct_%s_send( char **pptr, int *psize, SLK_%s *pValue );\r\n", g_struct[tmpi].m_name, g_struct[tmpi].m_name );
					fprintf( fp, "int struct_%s_recv( char **pptr, int *psize, SLK_%s *pValue );\r\n", g_struct[tmpi].m_name, g_struct[tmpi].m_name );
				}
				else
				{
					fprintf( fp, "int struct_%s_send( char **pptr, int *psize, CLK_%s *pValue );\r\n", g_struct[tmpi].m_name, g_struct[tmpi].m_name );
					fprintf( fp, "int struct_%s_recv( char **pptr, int *psize, CLK_%s *pValue );\r\n", g_struct[tmpi].m_name, g_struct[tmpi].m_name );
				}
			}
			else
				continue;
		}
	}
	fprintf( fpbase, "\r\n#endif\r\n" );
	fprintf( fpsend, "\r\n#endif\r\n" );
	fprintf( fprecv, "\r\n#endif\r\n" );
	fclose( fpbase );
	fclose( fpsend );
	fclose( fprecv );
	return 0;
}

void struct_create_structsend( char type )
{
	int tmpj;
	char tmpstr[256];
	char lkset[256];
	char szFile[256];
	char szPath[256];
	char szFix[8];
	char szFixPre[8];
	FILE *fp;
	int structid;
	int substructid;

	if( type == 0 )
	{
		strcpy( szPath, SERVER_CREATE_PATH );
		sprintf( szFile, "%s/src/server_structsend_auto.cpp", szPath );
		sprintf( szFile, "%s/src/server_structsend_auto.cpp", szPath );
	}
	else
	{
		strcpy( szPath, CLIENT_CREATE_PATH );
		sprintf( szFile, "%s/src/client_structsend_auto.cpp", szPath );
		sprintf( szFile, "%s/src/client_structsend_auto.cpp", szPath );
	}

	fp = fopen( szFile, "wb" );
	if( fp == NULL )
		return;

	fprintf( fp, "#include <string.h>\r\n" );
	if( type == 0 )
	{
		fprintf( fp, "#include \"server_structsend_auto.h\"\r\n" );
		fprintf( fp, "#include \"actor_send.h\"\r\n" );
	}
	else
	{
		fprintf( fp, "#include \"client_structsend_auto.h\"\r\n" );
	}
	fprintf( fp, "\r\n" );

	for( structid = 0; structid < g_struct_count; structid++ )
	{
		if( ( g_struct[structid].m_name[0] && g_struct[structid].m_struct_type == 1+type ) || ( type == 0 && g_struct[structid].m_struct_type == 3 ) )
		{
			if( type == 0 )
				fprintf( fp, "int struct_%s_send( char **pptr, int *psize, SLK_%s *pValue )\r\n", g_struct[structid].m_name, g_struct[structid].m_name );
			else
				fprintf( fp, "int struct_%s_send( char **pptr, int *psize, CLK_%s *pValue )\r\n", g_struct[structid].m_name, g_struct[structid].m_name );
			fprintf( fp, "{\r\n" );
			fprintf( fp, "	int tmpi = 0;\r\n");
			fprintf( fp, "\r\n");
			for( tmpj = 0; tmpj < g_struct[structid].m_count; tmpj++ )
			{
				if( struct_getdatatype( g_struct[structid].m_sconf[tmpj].data_type, tmpstr, lkset, type ) == NULL )
				{
					fclose( fp );
					return;
				}
				if( g_struct[structid].m_sconf[tmpj].m_fix )
					strcpy( szFix, "m_" );
				else
					szFix[0] = 0;
				if( tmpj > 0 && g_struct[structid].m_sconf[tmpj-1].m_fix )
					strcpy( szFixPre, "m_" );
				else
					szFixPre[0] = 0;
				if( g_struct[structid].m_sconf[tmpj].data_type >= 0 )
				{
					substructid = g_struct[structid].m_sconf[tmpj].data_type;
					if( g_struct[substructid].m_name[0] )
					{
						if( g_struct[structid].m_sconf[tmpj].data_count > 1 )
						{
							fprintf( fp, "	for( tmpi = 0; tmpi < %d; tmpi++ )\r\n	{\r\n", g_struct[structid].m_sconf[tmpj].data_count );
							fprintf( fp, "		struct_%s_send( pptr, psize, &pValue->%s%s[tmpi] );\r\n	}\r\n", g_struct[substructid].m_name, szFix, g_struct[structid].m_sconf[tmpj].m_name );
						}
						else if( g_struct[structid].m_sconf[tmpj].data_count == -1 )
						{
							if( tmpj > 0 )
							{
								if( g_struct[structid].m_sconf[tmpj].data_size == 1 )
								{
									fprintf( fp, "	struct_%s_send( pptr, psize, &pValue->%s%s );\r\n", g_struct[substructid].m_name, szFix, g_struct[structid].m_sconf[tmpj].m_name );
								}
								else
								{
									fprintf( fp, "	for( tmpi = 0; tmpi < pValue->%s%s; tmpi++ )\r\n	{\r\n", szFixPre, g_struct[structid].m_sconf[tmpj-1].m_name );
									fprintf( fp, "		struct_%s_send( pptr, psize, &pValue->%s%s[tmpi] );\r\n	}\r\n", g_struct[substructid].m_name, szFix, g_struct[structid].m_sconf[tmpj].m_name );
								}
							}
						}
						else
						{
							fprintf( fp, "	struct_%s_send( pptr, psize, &pValue->%s%s );\r\n", g_struct[substructid].m_name, szFix, g_struct[structid].m_sconf[tmpj].m_name );
						}
					}
				}
				else
				{
					if( g_struct[structid].m_sconf[tmpj].data_count > 1 )
					{
						fprintf( fp, "	LKSET_MEM_SEND( (*pptr), pValue->%s%s, %d*sizeof(%s), (*psize) );\r\n", szFix, g_struct[structid].m_sconf[tmpj].m_name, g_struct[structid].m_sconf[tmpj].data_count, tmpstr );
						//fprintf( fp, "	memcpy( (*pptr), pValue->m_%s, %d*sizeof(%s) );	(*pptr)+=%d*sizeof(%s); (*psize)+=%d*sizeof(%s);\r\n", g_struct[structid].m_sconf[tmpj].m_name, g_struct[structid].m_sconf[tmpj].data_count, tmpstr, g_struct[structid].m_sconf[tmpj].data_count, tmpstr, g_struct[structid].m_sconf[tmpj].data_count, tmpstr );
					}
					else if( g_struct[structid].m_sconf[tmpj].data_count == -1 )
					{
						if( tmpj > 0 )
						{
							fprintf( fp, "	if( pValue->%s%s > 0 && pValue->%s%s <= %d )\r\n", szFixPre, g_struct[structid].m_sconf[tmpj-1].m_name, szFixPre, g_struct[structid].m_sconf[tmpj-1].m_name, g_struct[structid].m_sconf[tmpj].data_size );
							fprintf( fp, "		LKSET_MEM_SEND( (*pptr), pValue->%s%s, pValue->%s%s*sizeof(%s), (*psize) );\r\n", szFix, g_struct[structid].m_sconf[tmpj].m_name, szFixPre, g_struct[structid].m_sconf[tmpj-1].m_name, tmpstr );
							//fprintf( fp, "		memcpy( (*pptr), pValue->m_%s, pValue->m_%s*sizeof(%s) );\r\n", g_struct[structid].m_sconf[tmpj].m_name, g_struct[structid].m_sconf[tmpj-1].m_name, tmpstr, g_struct[structid].m_sconf[tmpj-1].m_name, tmpstr, g_struct[structid].m_sconf[tmpj-1].m_name, tmpstr );
							//fprintf( fp, "	(*pptr)+=pValue->m_%s*sizeof(%s); (*psize)+=pValue->m_%s*sizeof(%s);\r\n", g_struct[structid].m_sconf[tmpj-1].m_name, tmpstr, g_struct[structid].m_sconf[tmpj-1].m_name, tmpstr );
						}
					}
					else
					{
						fprintf( fp, "	%s_SEND( (*pptr), &pValue->%s%s, (*psize) );\r\n", lkset, szFix, g_struct[structid].m_sconf[tmpj].m_name );
						//fprintf( fp, "	*(%s *)(*pptr) = pValue->m_%s; (*pptr)+=sizeof(%s); (*psize)+=sizeof(%s);\r\n", tmpstr, g_struct[structid].m_sconf[tmpj].m_name, tmpstr, tmpstr );
					}
				}
			}
			fprintf( fp, "	return 0;\r\n");
			fprintf( fp, "}\r\n" );
			fprintf( fp, "\r\n" );
		}
	}
	fclose( fp );
}

void struct_create_netsend( char type )
{
	int tmpi;
	FILE *fp;
	int structid;
	char tmpstr[256];
	char lkset[256];
	char szFile[256];
	char szPath[256];

	SNetcmdConfig *pnetcmd;
	int netcmd_count;

	if( type == 0 )
	{
		strcpy( szPath, SERVER_CREATE_PATH );
		pnetcmd = g_netscmd;
		netcmd_count = g_netscmd_count;
		sprintf( szFile, "%s/include/server_netsend_auto.h", szPath );
	}
	else
	{
		strcpy( szPath, CLIENT_CREATE_PATH );
		pnetcmd = g_netccmd;
		netcmd_count = g_netccmd_count;
		sprintf( szFile, "%s/include/client_netsend_auto.h", szPath );
	}

	fp = fopen( szFile, "wb" );
	if( fp == NULL )
		return;

	fprintf( fp, "#ifndef __NETSEND_AUTO_H%d\r\n", type );
	fprintf( fp, "#define __NETSEND_AUTO_H%d\r\n", type );
	fprintf( fp, "\r\n" );
	if( type == 0 )
		fprintf( fp, "#include \"server_structsend_auto.h\"\r\n" );
	else
		fprintf( fp, "#include \"client_structsend_auto.h\"\r\n" );

recreate_client_command_h:
	for( tmpi = 0; tmpi < netcmd_count; tmpi++ )
	{
		if( type && pnetcmd[tmpi].m_cmdid < 0 )
			continue;
		if( type == 0 && pnetcmd[tmpi].m_cmdid >= 0 && pnetcmd == g_netccmd )
			continue;
		structid = pnetcmd[tmpi].m_structid;
		if( structid < 0 )
		{
			if( struct_getdatatype( structid, tmpstr, NULL, type ) )
			{
				if( type == 0 )
				{
					if( pnetcmd[tmpi].m_target_type == 10 )
						fprintf( fp, "int netsend_%s_S( char *databuf, int size, %s *pValue );\r\n", pnetcmd[tmpi].m_cmdstr, tmpstr );
					else
						fprintf( fp, "int netsend_%s_S( int actor_index, char send_type, %s *pValue );\r\n", pnetcmd[tmpi].m_cmdstr, tmpstr );
				}
				else
					fprintf( fp, "int netsend_%s_C( %s *pValue );\r\n", pnetcmd[tmpi].m_cmdstr, tmpstr );
			}
		}
		else if( g_struct[structid].m_name[0] )
		{
			if( type == 0 )
			{
				if( pnetcmd[tmpi].m_target_type == 10 )
					fprintf( fp, "int netsend_%s_S( char *databuf, int size, SLK_%s *pValue );\r\n", pnetcmd[tmpi].m_cmdstr, g_struct[structid].m_name );
				else
					fprintf( fp, "int netsend_%s_S( int actor_index, char send_type, SLK_%s *pValue );\r\n", pnetcmd[tmpi].m_cmdstr, g_struct[structid].m_name );
			}
			else
			{
				fprintf( fp, "int netsend_%s_C( CLK_%s *pValue );\r\n", pnetcmd[tmpi].m_cmdstr, g_struct[structid].m_name );
			}
		}
	}
	if( type == 0 && pnetcmd == g_netscmd )
	{
		pnetcmd = g_netccmd;
		netcmd_count = g_netccmd_count;
		fprintf( fp, "\r\n" );
		goto recreate_client_command_h;
	}

	fprintf( fp, "\r\n#endif\r\n" );
	fclose( fp );


	if( type == 0 )
	{
		strcpy( szPath, SERVER_CREATE_PATH );
		pnetcmd = g_netscmd;
		netcmd_count = g_netscmd_count;
		sprintf( szFile, "%s/src/server_netsend_auto.cpp", szPath );
	}
	else
	{
		strcpy( szPath, CLIENT_CREATE_PATH );
		pnetcmd = g_netccmd;
		netcmd_count = g_netccmd_count;
		sprintf( szFile, "%s/src/client_netsend_auto.cpp", szPath );
	}
	fp = fopen( szFile, "wb" );
	if( fp == NULL )
		return;

	if( type == 0 )
	{
		fprintf( fp, "#include \"server_struct_auto.h\"\r\n" );
		fprintf( fp, "#include \"server_netsend_auto.h\"\r\n" );
	}
	else
	{
		fprintf( fp, "#include \"client_struct_auto.h\"\r\n" );
		fprintf( fp, "#include \"client_netsend_auto.h\"\r\n" );
	}
	if( type == 0 )
	{
#ifdef _NETPROCESSAUTO
		fprintf( fp, "#include \"netfunc.h\"\r\n" );
#endif
		fprintf( fp, "#include \"actor_send.h\"\r\n" );
		fprintf( fp, "#include \"fight.h\"\r\n" );
		fprintf( fp, "\r\n\r\nextern int g_maxactornum;\r\n" );
	}
	else
	{
		fprintf( fp, "\r\nint rawsend( char *data, int size, short cmd );\r\n" );
	}
	fprintf( fp, "\r\n");

recreate_client_command_cpp:
	for( tmpi = 0; tmpi < netcmd_count; tmpi++ )
	{
		if( type && pnetcmd[tmpi].m_cmdid < 0 )
			continue;
		if( type == 0 && pnetcmd[tmpi].m_cmdid >= 0 && pnetcmd == g_netccmd )
			continue;
		structid = pnetcmd[tmpi].m_structid;
		if( structid < 0 || g_struct[structid].m_name[0] )
		{
			if( type == 0 )
			{
				if( structid < 0 )
				{
					if( struct_getdatatype( structid, tmpstr, NULL, type ) )
					{
						if( pnetcmd[tmpi].m_target_type == 10 )
							fprintf( fp, "int netsend_%s_S( char *databuf, int size, %s *pValue )\r\n", pnetcmd[tmpi].m_cmdstr, tmpstr );
						else
							fprintf( fp, "int netsend_%s_S( int actor_index, char send_type, %s *pValue )\r\n", pnetcmd[tmpi].m_cmdstr, tmpstr );
					}
				}
				else
				{
					if( pnetcmd[tmpi].m_target_type == 10 )
						fprintf( fp, "int netsend_%s_S( char *databuf, int size, SLK_%s *pValue )\r\n", pnetcmd[tmpi].m_cmdstr, g_struct[structid].m_name );
					else
						fprintf( fp, "int netsend_%s_S( int actor_index, char send_type, SLK_%s *pValue )\r\n", pnetcmd[tmpi].m_cmdstr, g_struct[structid].m_name );
				}
				fprintf( fp, "{\r\n" );
				fprintf( fp, "	char tmpbuf[2048];\r\n");
				fprintf( fp, "	int tmpsize;\r\n");
				fprintf( fp, "	char *ptrsubdata;\r\n");

				strcpy( tmpstr, pnetcmd[tmpi].m_cmdstr );
				u_str_toupper( tmpstr );

				if( pnetcmd == g_netccmd )
				{
					fprintf( fp, "	char *ptr;\r\n");
					fprintf( fp, "	short cmd=CMDC_%s;\r\n",tmpstr);
					fprintf( fp, "\r\n");
					if( pnetcmd[tmpi].m_target_type != 10 )
					{
						fprintf( fp, "	if( actor_index >= 0 )\r\n");
						fprintf( fp, "		return -1;\r\n");
					}
				}
				else
				{
					fprintf( fp, "	char *ptr, *ptrsize;\r\n");
					fprintf( fp, "	short cmd=CMDS_%s;\r\n",tmpstr);
					fprintf( fp, "\r\n");
					if( pnetcmd[tmpi].m_target_type != 10 )
					{
//						fprintf( fp, "	if( actor_index < 0 || actor_index >= g_maxactornum )\r\n");
						fprintf( fp, "	if( actor_index < 0 )\r\n");
						fprintf( fp, "		return -1;\r\n");
					}
				}
				fprintf( fp, "\r\n");
				fprintf( fp, "	ptr = tmpbuf;\r\n");
				fprintf( fp, "	tmpsize = 0;\r\n");
				if( pnetcmd[tmpi].m_target_type != 10 )
					fprintf( fp, "	ptr+=sizeof(short);\r\n");
				fprintf( fp, "	ptrsubdata = ptr;\r\n");

				if( pnetcmd == g_netccmd )
				{
					//fprintf( fp, "	LKSET_WORD_SEND( ptr, &cmd, tmpsize );\r\n" );
					fprintf( fp, "	*(short *)ptr = CMDC_%s; ptr+=sizeof(short); tmpsize+=sizeof(short);\r\n", tmpstr );
				}
				else
				{
					//fprintf( fp, "	LKSET_WORD_SEND( ptr, &cmd, tmpsize );\r\n" );
					fprintf( fp, "	*(short *)ptr = CMDS_%s; ptr+=sizeof(short); tmpsize+=sizeof(short);\r\n", tmpstr );
					fprintf( fp, "	ptrsize = ptr;	ptr+=sizeof(short);tmpsize+=sizeof(short);\r\n");
				}
				fprintf( fp, "\r\n");

				if( structid < 0 )
				{
					if( struct_getdatatype( structid, tmpstr, lkset, type ) )
					{
						fprintf( fp, "	%s_SEND( ptr, pValue, tmpsize );\r\n\r\n", lkset );
						//fprintf( fp, "	*(%s *)ptr = *pValue; ptr+=sizeof(%s); tmpsize+=sizeof(%s);\r\n\r\n", tmpstr, tmpstr, tmpstr );
					}
				}
				else
				{
					fprintf( fp, "	struct_%s_send( &ptr, &tmpsize, pValue );\r\n\r\n", g_struct[structid].m_name );
				}

				if( pnetcmd == g_netccmd )
				{
					fprintf( fp, "	*(unsigned short *)tmpbuf = tmpsize;\r\n");
					
					fprintf( fp, "\r\n");
					fprintf( fp, "	readtoqueue( actor_index, tmpbuf, tmpsize+sizeof(short), 0 );\r\n");
					fprintf( fp, "	return 0;\r\n");
					fprintf( fp, "}\r\n" );
					fprintf( fp, "\r\n" );
				}
				else
				{
					fprintf( fp, "	*(short *)ptrsize = tmpsize - (int)sizeof(short)*2;\r\n");
					if( pnetcmd[tmpi].m_target_type == 10 )
					{
						fprintf( fp, "	if( tmpsize > size )\r\n");
						fprintf( fp, "		return 0;\r\n");
						fprintf( fp, "\r\n");
						fprintf( fp, "	memcpy( databuf, tmpbuf, tmpsize );\r\n");
						fprintf( fp, "	return tmpsize;\r\n");
					}
					else
					{
						fprintf( fp, "	*(unsigned short *)tmpbuf = tmpsize;\r\n");
						fprintf( fp, "\r\n");
						fprintf( fp, "	actor_senddata( actor_index, send_type, tmpbuf, tmpsize );\r\n");
						if ( structid == 18 )
						{
							fprintf( fp, "	fight_record_create_save( actor_index, tmpbuf, tmpsize );\r\n");
						}
						else if( structid == 22 )
						{
							fprintf( fp, "	fight_record_process_save( actor_index, send_type, tmpbuf, tmpsize );\r\n");
						}
						fprintf( fp, "	return 0;\r\n");
					}
					fprintf( fp, "}\r\n" );
					fprintf( fp, "\r\n" );
				}
			}
			else
			{
				if( structid < 0 )
				{
					if( struct_getdatatype( structid, tmpstr, NULL, type ) )
					{
						fprintf( fp, "int netsend_%s_C( %s *pValue )\r\n", pnetcmd[tmpi].m_cmdstr, tmpstr );
					}
				}
				else
				{
					fprintf( fp, "int netsend_%s_C( CLK_%s *pValue )\r\n", pnetcmd[tmpi].m_cmdstr, g_struct[structid].m_name );
				}
				fprintf( fp, "{\r\n" );
				fprintf( fp, "	char tmpbuf[2048];\r\n");
				fprintf( fp, "	int tmpsize;\r\n");
				fprintf( fp, "	char *ptr;\r\n");
				fprintf( fp, "\r\n");
				fprintf( fp, "	ptr = tmpbuf;\r\n");
				fprintf( fp, "	tmpsize = 0;\r\n");
				fprintf( fp, "\r\n");

				if( structid < 0 )
				{
					if( struct_getdatatype( structid, tmpstr, lkset, type ) )
					{
						fprintf( fp, "	%s_SEND( ptr, pValue, tmpsize );\r\n\r\n", lkset );
						//fprintf( fp, "	*(%s *)ptr = *pValue; ptr+=sizeof(%s); tmpsize+=sizeof(%s);\r\n\r\n", tmpstr, tmpstr, tmpstr );
					}
				}
				else
				{
					fprintf( fp, "	struct_%s_send( &ptr, &tmpsize, pValue );\r\n\r\n", g_struct[structid].m_name );
				}

				strcpy( tmpstr, pnetcmd[tmpi].m_cmdstr );
				u_str_toupper( tmpstr );

				fprintf( fp, "	rawsend( tmpbuf, tmpsize, CMDC_%s );\r\n", tmpstr );
				fprintf( fp, "	return 0;\r\n");
				fprintf( fp, "}\r\n" );
				fprintf( fp, "\r\n" );
			}
		}
	}
	if( type == 0 && pnetcmd == g_netscmd )
	{
		pnetcmd = g_netccmd;
		netcmd_count = g_netccmd_count;
		fprintf( fp, "\r\n" );
		goto recreate_client_command_cpp;
	}
	fclose( fp );
}

void struct_create_structrecv( char type )
{
	int tmpj;
	char tmpstr[256];
	char lkset[256];
	char szFile[256];
	char szPath[256];
	char szFix[8];
	char szFixPre[8];
	FILE *fp;
	int structid;
	int substructid;

	if( type == 0 )
	{
		strcpy( szPath, SERVER_CREATE_PATH );
		sprintf( szFile, "%s/src/server_structrecv_auto.cpp", szPath );
	}
	else
	{
		strcpy( szPath, CLIENT_CREATE_PATH );
		sprintf( szFile, "%s/src/client_structrecv_auto.cpp", szPath );
	}

	fp = fopen( szFile, "wb" );
	if( fp == NULL )
		return;

	fprintf( fp, "#include <string.h>\r\n" );
	if( type == 0 )
	{
		fprintf( fp, "#include \"server_structrecv_auto.h\"\r\n" );
		fprintf( fp, "#include \"actor_send.h\"\r\n" );
	}
	else
	{
		fprintf( fp, "#include \"client_structrecv_auto.h\"\r\n" );
	}
	fprintf( fp, "\r\n" );

	for( structid = 0; structid < g_struct_count; structid++ )
	{
		if( ( g_struct[structid].m_name[0] && g_struct[structid].m_struct_type == 2-type ) || ( type == 0 && g_struct[structid].m_struct_type == 3 ) )
		{
			if( type == 0 )
				fprintf( fp, "int struct_%s_recv( char **pptr, int *psize, SLK_%s *pValue )\r\n", g_struct[structid].m_name, g_struct[structid].m_name );
			else
				fprintf( fp, "int struct_%s_recv( char **pptr, int *psize, CLK_%s *pValue )\r\n", g_struct[structid].m_name, g_struct[structid].m_name );
			fprintf( fp, "{\r\n" );
			fprintf( fp, "	int tmpi = 0;\r\n");
			fprintf( fp, "\r\n");
			for( tmpj = 0; tmpj < g_struct[structid].m_count; tmpj++ )
			{
				if( struct_getdatatype( g_struct[structid].m_sconf[tmpj].data_type, tmpstr, lkset, type ) == NULL )
				{
					fclose( fp );
					return;
				}
				if( g_struct[structid].m_sconf[tmpj].m_fix )
					strcpy( szFix, "m_" );
				else
					szFix[0] = 0;
				if( tmpj > 0 && g_struct[structid].m_sconf[tmpj-1].m_fix )
					strcpy( szFixPre, "m_" );
				else
					szFixPre[0] = 0;
				
				if( g_struct[structid].m_sconf[tmpj].data_type >= 0 )
				{
					substructid = g_struct[structid].m_sconf[tmpj].data_type;
					if( g_struct[substructid].m_name[0] )
					{
						if( g_struct[structid].m_sconf[tmpj].data_count > 1 )
						{
							fprintf( fp, "	for( tmpi = 0; tmpi < %d; tmpi++ )\r\n	{\r\n", g_struct[structid].m_sconf[tmpj].data_count );
							fprintf( fp, "		struct_%s_recv( pptr, psize, &pValue->%s%s[tmpi] );\r\n	}\r\n", g_struct[substructid].m_name, szFix, g_struct[structid].m_sconf[tmpj].m_name );
						}
						else if( g_struct[structid].m_sconf[tmpj].data_count == -1 )
						{
							if( tmpj > 0 )
							{
								if( g_struct[structid].m_sconf[tmpj].data_size == 1 )
								{
									fprintf( fp, "	struct_%s_recv( pptr, psize, &pValue->%s%s );\r\n", g_struct[substructid].m_name, szFix, g_struct[structid].m_sconf[tmpj].m_name );
								}
								else
								{
									fprintf( fp, "	for( tmpi = 0; tmpi < pValue->%s%s; tmpi++ )\r\n	{\r\n", szFixPre, g_struct[structid].m_sconf[tmpj-1].m_name );
									fprintf( fp, "		struct_%s_recv( pptr, psize, &pValue->%s%s[tmpi] );\r\n	}\r\n", g_struct[substructid].m_name, szFix, g_struct[structid].m_sconf[tmpj].m_name );
								}
							}
						}
						else
						{
							fprintf( fp, "	struct_%s_recv( pptr, psize, &pValue->%s%s );\r\n", g_struct[substructid].m_name, szFix, g_struct[structid].m_sconf[tmpj].m_name );
						}
					}
				}
				else
				{
					if( g_struct[structid].m_sconf[tmpj].data_count > 1 )
					{
						fprintf( fp, "	LKSET_MEM_RECV( pValue->%s%s, (*pptr), %d*sizeof(%s), (*psize) );\r\n", szFix, g_struct[structid].m_sconf[tmpj].m_name, g_struct[structid].m_sconf[tmpj].data_count, tmpstr );
						//fprintf( fp, "	memcpy( pValue->m_%s, (*pptr), %d*sizeof(%s) );	(*pptr)+=%d*sizeof(%s); (*psize)-=%d*sizeof(%s);\r\n", g_struct[structid].m_sconf[tmpj].m_name, g_struct[structid].m_sconf[tmpj].data_count, tmpstr, g_struct[structid].m_sconf[tmpj].data_count, tmpstr, g_struct[structid].m_sconf[tmpj].data_count, tmpstr );
					}
					else if( g_struct[structid].m_sconf[tmpj].data_count == -1 )
					{
						if( tmpj > 0 )
						{
							fprintf( fp, "	if( pValue->%s%s < 0 || pValue->%s%s > (*psize)*(int)sizeof(%s) )\r\n		return -1;\r\n", szFixPre, g_struct[structid].m_sconf[tmpj-1].m_name, szFixPre, g_struct[structid].m_sconf[tmpj-1].m_name, tmpstr );
							fprintf( fp, "	LKSET_MEM_RECV( pValue->%s%s, (*pptr), pValue->%s%s*sizeof(%s), (*psize) );\r\n", szFix, g_struct[structid].m_sconf[tmpj].m_name, szFixPre, g_struct[structid].m_sconf[tmpj-1].m_name, tmpstr );
							//fprintf( fp, "	memcpy( pValue->m_%s, (*pptr), pValue->m_%s*sizeof(%s) );	(*pptr)+=pValue->m_%s*sizeof(%s); (*psize)-=pValue->m_%s*sizeof(%s);\r\n", g_struct[structid].m_sconf[tmpj].m_name, g_struct[structid].m_sconf[tmpj-1].m_name, tmpstr, g_struct[structid].m_sconf[tmpj-1].m_name, tmpstr, g_struct[structid].m_sconf[tmpj-1].m_name, tmpstr );
						}
					}
					else
					{
						fprintf( fp, "	%s_RECV( &pValue->%s%s, (*pptr), (*psize) );\r\n", lkset, szFix, g_struct[structid].m_sconf[tmpj].m_name );
						//fprintf( fp, "	pValue->m_%s = *(%s *)(*pptr); (*pptr)+=sizeof(%s); (*psize)-=sizeof(%s);\r\n", g_struct[structid].m_sconf[tmpj].m_name, tmpstr, tmpstr, tmpstr );
					}
				}
			}
			fprintf( fp, "	return 0;\r\n");
			fprintf( fp, "}\r\n" );
			fprintf( fp, "\r\n" );
		}
	}
	fclose( fp );
}

void struct_create_netrecv( char type )
{
	int tmpi;
	FILE *fp;
	int structid;
	char tmpstr[256];
	char lkset[256];
	char szFile[256];
	char szPath[256];

	SNetcmdConfig *pnetcmd;
	int netcmd_count;

	if( type == 0 )
	{
		strcpy( szPath, SERVER_CREATE_PATH );
		pnetcmd = g_netccmd;
		netcmd_count = g_netccmd_count;
		sprintf( szFile, "%s/include/server_netrecv_auto.h", szPath );
	}
	else
	{
		strcpy( szPath, CLIENT_CREATE_PATH );
		pnetcmd = g_netscmd;
		netcmd_count = g_netscmd_count;
		sprintf( szFile, "%s/include/client_netrecv_auto.h", szPath );
	}

	fp = fopen( szFile, "wb" );
	if( fp == NULL )
		return;

	fprintf( fp, "#ifndef __NETRECV_AUTO_H%d\r\n", type );
	fprintf( fp, "#define __NETRECV_AUTO_H%d\r\n", type );
	fprintf( fp, "\r\n" );
	if( type == 0 )
		fprintf( fp, "#include \"server_structrecv_auto.h\"\r\n" );
	else
		fprintf( fp, "#include \"client_structrecv_auto.h\"\r\n" );

	for( tmpi = 0; tmpi < netcmd_count; tmpi++ )
	{
		structid = pnetcmd[tmpi].m_structid;
		if( structid < 0 || g_struct[structid].m_name[0] )
		{
			if( type == 0 )
				fprintf( fp, "int netrecv_%s_S( int client_index, char *data, int size );\r\n", pnetcmd[tmpi].m_cmdstr );
			else
				fprintf( fp, "int netrecv_%s_C( char *data, int size );\r\n", pnetcmd[tmpi].m_cmdstr );
		}
	}

	fprintf( fp, "\r\n" );

	if( type == 0 )
	{
		for( tmpi = 0; tmpi < netcmd_count; tmpi++ )
		{
			structid = pnetcmd[tmpi].m_structid;
			if( ( structid < 0 || g_struct[structid].m_name[0] ) && pnetcmd[tmpi].m_target_type == 1 )
			{
				fprintf( fp, "int netrecv_wqueue_%s_S( int client_index, char *data, int size, int exec_code );\r\n", pnetcmd[tmpi].m_cmdstr );
			}
		}
	}

	fprintf( fp, "\r\n#endif\r\n" );
	fclose( fp );

	if( type == 0 )
		sprintf( szFile, "%s/src/server_netrecv_auto.cpp", szPath );
	else
		sprintf( szFile, "%s/src/client_netrecv_auto.cpp", szPath );
	fp = fopen( szFile, "wb" );
	if( fp == NULL )
		return;

	if( type == 0 )
	{
		fprintf( fp, "#include \"server_struct_auto.h\"\r\n" );
		fprintf( fp, "#include \"server_process_auto.h\"\r\n" );
		fprintf( fp, "#include \"server_netrecv_auto.h\"\r\n" );
	}
	else
	{
		fprintf( fp, "#include \"client_struct_auto.h\"\r\n" );
		fprintf( fp, "#include \"client_process_auto.h\"\r\n" );
		fprintf( fp, "#include \"client_netrecv_auto.h\"\r\n" );
	}
	if( type == 0 )
	{
		fprintf( fp, "#include \"actor_send.h\"\r\n" );
#ifdef _NETPROCESSAUTO
		fprintf( fp, "#include \"client.h\"\r\n" );
		fprintf( fp, "#include \"wqueue.h\"\r\n" );
#endif
		fprintf( fp, "#include \"utils.h\"\r\n" );
		fprintf( fp, "extern int g_mysql_wait;\r\n" );
	}
	fprintf( fp, "\r\n" );

	for( tmpi = 0; tmpi < netcmd_count; tmpi++ )
	{
		structid = pnetcmd[tmpi].m_structid;
		if( structid < 0 || g_struct[structid].m_name[0] )
		{
			if( type == 0 )
				fprintf( fp, "int netrecv_%s_S( int client_index, char *data, int size )\r\n", pnetcmd[tmpi].m_cmdstr );
			else
				fprintf( fp, "int netrecv_%s_C( char *data, int size )\r\n", pnetcmd[tmpi].m_cmdstr );
			fprintf( fp, "{\r\n" );
			if( structid < 0 )
			{
				if( struct_getdatatype( structid, tmpstr, NULL, type ) )
				{
					fprintf( fp, "	%s Value = 0;\r\n", tmpstr );
				}
			}
			else
			{
				if( type == 0 )
					fprintf( fp, "	SLK_%s Value = {};\r\n", g_struct[structid].m_name );
				else
					fprintf( fp, "	CLK_%s Value = {};\r\n", g_struct[structid].m_name );
			}
			fprintf( fp, "	int tmpsize = size;\r\n");
			fprintf( fp, "	char *ptr = data;\r\n");
			fprintf( fp, "\r\n");

			if( structid < 0 )
			{
				if( struct_getdatatype( structid, tmpstr, lkset, type ) )
				{
					fprintf( fp, "	%s_RECV( &Value, ptr, tmpsize );\r\n\r\n", lkset );
					//fprintf( fp, "	Value = *(%s *)ptr; ptr+=sizeof(%s); tmpsize-=sizeof(%s);\r\n\r\n", tmpstr, tmpstr, tmpstr );
				}
			}
			else
			{
				fprintf( fp, "	struct_%s_recv( &ptr, &tmpsize, &Value );\r\n\r\n", g_struct[structid].m_name );
			}
			if( type == 0 )
				fprintf( fp, "	proc_%s_S( client_index, &Value );\r\n", pnetcmd[tmpi].m_cmdstr );
			else
				fprintf( fp, "	proc_%s_C( &Value );\r\n", pnetcmd[tmpi].m_cmdstr );

			fprintf( fp, "\r\n");
			fprintf( fp, "	return 0;\r\n" );
			fprintf( fp, "}\r\n" );
			fprintf( fp, "\r\n" );
		}
	}

	if( type == 0 )
	{
		for( tmpi = 0; tmpi < netcmd_count; tmpi++ )
		{
			structid = pnetcmd[tmpi].m_structid;
			if( ( structid < 0 || g_struct[structid].m_name[0] ) && pnetcmd[tmpi].m_target_type == 1 )
			{
				strcpy( tmpstr, pnetcmd[tmpi].m_cmdstr );
				u_str_toupper( tmpstr );

				fprintf( fp, "int netrecv_wqueue_%s_S( int client_index, char *data, int size, int exec_code )\r\n", pnetcmd[tmpi].m_cmdstr );
				fprintf( fp, "{\r\n" );
				fprintf( fp, "	client_setwait( client_index, 1 );\r\n" );
				fprintf( fp, "	if( g_mysql_wait )\r\n" );
				fprintf( fp, "	{\r\n" );
				fprintf( fp, "		if( add_wqueue( client_index, WCMD_%s, data, size, exec_code, 0 ) < 0 )\r\n", tmpstr );
				fprintf( fp, "		{\r\n" );
				fprintf( fp, "			write_gamelog( \"add_wqueue full\" );\r\n" );
				fprintf( fp, "			net_closesock_direct( client_index );\r\n" );
				fprintf( fp, "			return -1;\r\n" );
				fprintf( fp, "		}\r\n" );
				fprintf( fp, "		return 0;\r\n" );
				fprintf( fp, "	}\r\n" );
				fprintf( fp, "	else\r\n" );
				fprintf( fp, "	{\r\n" );
				fprintf( fp, "		return netrecv_%s_S( client_index, data, size );\r\n", pnetcmd[tmpi].m_cmdstr );
				fprintf( fp, "	}\r\n" );
				fprintf( fp, "}\r\n" );
				fprintf( fp, "\r\n" );
			}
		}
	}

	if( type == 0 )
		fprintf( fp, "int proc_command_S( int client_index, short cmd, char *ptr, int tmpsize, int exec_code )\r\n" );
	else
		fprintf( fp, "int proc_command_C( short cmd, char *ptr, int tmpsize )\r\n" );
	fprintf( fp, "{\r\n" );
	fprintf( fp, "	switch( cmd )\r\n" );
	fprintf( fp, "	{\r\n" );
	for( tmpi = 0; tmpi < netcmd_count; tmpi++ )
	{
		structid = pnetcmd[tmpi].m_structid;
		if( structid < 0 || g_struct[structid].m_name[0] )
		{
			strcpy( tmpstr, pnetcmd[tmpi].m_cmdstr );
			u_str_toupper( tmpstr );

			if( type == 0 )
			{
				fprintf( fp, "	case CMDC_%s:\r\n", tmpstr );
				if( pnetcmd[tmpi].m_target_type == 0 )
					fprintf( fp, "		netrecv_%s_S( client_index, ptr, tmpsize );\r\n", pnetcmd[tmpi].m_cmdstr );
				else if( pnetcmd[tmpi].m_target_type == 1 )
					fprintf( fp, "		netrecv_wqueue_%s_S( client_index, ptr, tmpsize, exec_code );\r\n", pnetcmd[tmpi].m_cmdstr );
			}
			else
			{
				fprintf( fp, "	case CMDS_%s:\r\n", tmpstr );
				fprintf( fp, "		netrecv_%s_C( ptr, tmpsize );\r\n", pnetcmd[tmpi].m_cmdstr );
			}
			fprintf( fp, "		break;\r\n" );
		}
	}
	fprintf( fp, "	default:\r\n" );
	fprintf( fp, "		return -1;\r\n" );
	fprintf( fp, "	}\r\n" );
	fprintf( fp, "	return 0;\r\n" );
	fprintf( fp, "}\r\n" );
	fprintf( fp, "\r\n" );
	fclose( fp );
}

void struct_create_process( char type )
{
	int tmpi,tmpj;
	FILE *fp;
	int structid;

	char tmpstr[256];
	char szFile[256];
	char szPath[256];
	char **linebuf = NULL;
	int linecount;
	char has;

	SNetcmdConfig *pnetcmd;
	int netcmd_count;

	if( type == 0 )
	{
		strcpy( szPath, SERVER_CREATE_PATH );
		pnetcmd = g_netccmd;
		netcmd_count = g_netccmd_count;
		sprintf( szFile, "%s/include/server_process_auto.h", szPath );
	}
	else
	{
		strcpy( szPath, CLIENT_CREATE_PATH );
		pnetcmd = g_netscmd;
		netcmd_count = g_netscmd_count;
		sprintf( szFile, "%s/include/client_process_auto.h", szPath );
	}

	fp = fopen( szFile, "wb" );
	if( fp == NULL )
		return;

	fprintf( fp, "#ifndef __PROCESS_AUTO_H%d\r\n", type );
	fprintf( fp, "#define __PROCESS_AUTO_H%d\r\n", type );
	if( type == 0 )
		fprintf( fp, "#include \"server_structrecv_auto.h\"\r\n" );
	else
		fprintf( fp, "#include \"client_structrecv_auto.h\"\r\n" );
	fprintf( fp, "\r\n" );
	if( type == 0 )
		fprintf( fp, "int proc_command_S( int client_index, short cmd, char *ptr, int tmpsize, int exec_code );\r\n" );
	else
		fprintf( fp, "int proc_command_C( short cmd, char *ptr, int tmpsize );\r\n" );

	for( tmpi = 0; tmpi < netcmd_count; tmpi++ )
	{
		structid = pnetcmd[tmpi].m_structid;
		if( structid < 0 )
		{
			if( struct_getdatatype( structid, tmpstr, NULL, type ) )
			{
				if( type == 0 )
					fprintf( fp, "void proc_%s_S( int client_index, %s *pValue );\r\n", pnetcmd[tmpi].m_cmdstr, tmpstr );
				else
					fprintf( fp, "void proc_%s_C( %s *pValue );\r\n", pnetcmd[tmpi].m_cmdstr, tmpstr );
			}
		}
		else if( g_struct[structid].m_name[0] )
		{
			if( type == 0 )
				fprintf( fp, "void proc_%s_S( int client_index, SLK_%s *pValue );\r\n", pnetcmd[tmpi].m_cmdstr, g_struct[structid].m_name );
			else
				fprintf( fp, "void proc_%s_C( CLK_%s *pValue );\r\n", pnetcmd[tmpi].m_cmdstr, g_struct[structid].m_name );
		}
	}
	fprintf( fp, "\r\n#endif\r\n" );
	fclose( fp );

	if( type == 0 )
		sprintf( szFile, "%s/src/server_process_auto.cpp", szPath );
	else
		sprintf( szFile, "%s/src/client_process_auto.cpp", szPath );
	fp = fopen( szFile, "rb" );
	if( fp != NULL )
	{
		linebuf = u_load_file( fp, &linecount );
		fclose( fp );
	}

	fp = fopen( szFile, "ab+" );
	if( fp == NULL )
		return;

	if( linebuf == NULL )
	{
		if( type == 0 )
		{
			fprintf( fp, "#include \"server_process_auto.h\"\r\n" );
			fprintf( fp, "#include \"actor_send.h\"\r\n" );
		}
		else
		{
			fprintf( fp, "#include \"client_process_auto.h\"\r\n" );
			fprintf( fp, "#include \"client_netsend_auto.h\"\r\n" );
		}
		fprintf( fp, "\r\n" );
	}

	for( tmpi = 0; tmpi < netcmd_count; tmpi++ )
	{
		structid = pnetcmd[tmpi].m_structid;
		if( structid < 0 || g_struct[structid].m_name[0] )
		{
			has = 0;
			if( linebuf )
			{
				if( type == 0 )
					sprintf( tmpstr, "void proc_%s_S(", pnetcmd[tmpi].m_cmdstr );
				else
					sprintf( tmpstr, "void proc_%s_C(", pnetcmd[tmpi].m_cmdstr );
				for( tmpj = 0; tmpj < linecount; tmpj++ )
				{
					if( u_frontcmp( linebuf[tmpj], tmpstr )  )
					{
						has = 1;
						break;
					}
				}
			}
			if( has )
				continue;

			if( structid < 0 )
			{
				if( struct_getdatatype( structid, tmpstr, NULL, type ) )
				{
					if( type == 0 )
						fprintf( fp, "void proc_%s_S( int client_index, %s *pValue )\r\n", pnetcmd[tmpi].m_cmdstr, tmpstr );
					else
						fprintf( fp, "void proc_%s_C( %s *pValue )\r\n", pnetcmd[tmpi].m_cmdstr, tmpstr );
				}
			}
			else
			{
				if( type == 0 )
					fprintf( fp, "void proc_%s_S( int client_index, SLK_%s *pValue )\r\n", pnetcmd[tmpi].m_cmdstr, g_struct[structid].m_name );
				else
					fprintf( fp, "void proc_%s_C( CLK_%s *pValue )\r\n", pnetcmd[tmpi].m_cmdstr, g_struct[structid].m_name );
			}
			fprintf( fp, "{\r\n" );
			fprintf( fp, "	// process.\r\n");
			fprintf( fp, "}\r\n" );
			fprintf( fp, "\r\n" );
		}
	}
	fclose( fp );
	
	if( linebuf )
		u_free_vec( linebuf );
}

int struct_create_c( char type )
{
	struct_reset();
	struct_create_structsend( type );
	struct_create_netsend( type );

	struct_create_structrecv( type );
	struct_create_netrecv( type );

	struct_create_process( type );
	return 0;
}

int struct_create()
{
	struct_create_h( 0 );
	struct_create_h( 1 );
	struct_create_c( 0 );
	struct_create_c( 1 );
	return 0;
}
