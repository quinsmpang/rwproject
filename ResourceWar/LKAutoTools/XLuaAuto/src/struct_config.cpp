#ifdef WIN32
#include <winsock2.h>
#include <mswsock.h>
#endif
#include <stdio.h>
#include <string.h>

#include <mysql.h>
#include "struct_config.h"
#include "utils.h"


//#define _NETPROCESSAUTO

extern MYSQL *myData;

// 任务目标的分组信息
SStructConfig *g_struct = NULL;
int g_struct_count = 0;

int struct_reset( char *ptab )
{
	free( g_struct );
	g_struct = NULL;

	struct_init( ptab );
	return 0;
}

int struct_init( char *ptab )
{
	MYSQL_RES		*res;
	MYSQL_ROW		row;
	char	szSQL[1024];

	int index;
	int tmpobjid;

	char *ptr;

	// 确定目标组的个数（虚个数，以最大的一个ID确定内存大小），并且分配structid的内存
	g_struct_count = 0;

	sprintf( szSQL, "select max(structid) from %s", ptab );
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

	g_struct = (SStructConfig *)malloc( sizeof(SStructConfig)*g_struct_count );
	memset( g_struct, 0, sizeof(SStructConfig)*g_struct_count );

	// 确定每个目标组的个数
	sprintf( szSQL, "select * from %s order by structid", ptab );
	if( mysql_query( myData, szSQL ) )
	{
		printf( "Query failed (%s)\n", mysql_error(myData) );
		write_gamelog( "%s", szSQL );
		return -1;
	}
	res = mysql_store_result( myData );
	while( ( row = mysql_fetch_row( res ) ) )
	{
		index = 0;
		tmpobjid = atoi( row[index++] );
		if( tmpobjid < 0 || tmpobjid >= g_struct_count )
		{
			mysql_free_result( res );
			return -1;
		}
		strncpy( g_struct[tmpobjid].m_name, row[index++], 64 );
		strncpy( g_struct[tmpobjid].m_context, row[index++], 128 );
		g_struct[tmpobjid].m_limit_count = (char)atoi( row[index++] );
		g_struct[tmpobjid].m_arg_count = 0;
		g_struct[tmpobjid].m_rtn_count = 0;
		for( int tmpi = 0; tmpi < 16; tmpi++ )
		{
			g_struct[tmpobjid].m_type[tmpi] = (char)atoi( row[index++] );
			if( g_struct[tmpobjid].m_type[tmpi] == 0 )
				break;
			strncpy( g_struct[tmpobjid].m_arg[tmpi], row[index++], 32 );
			for( ptr = g_struct[tmpobjid].m_arg[tmpi]; *ptr != 0; ptr++ )
			{
				if( *ptr == '|' )
				{
					strcpy( g_struct[tmpobjid].m_desc[tmpi], ptr+1 );
					*ptr = 0;
					break;
				}
			}
			if( g_struct[tmpobjid].m_type[tmpi] < 0 )
				g_struct[tmpobjid].m_arg_count++;
			else if( g_struct[tmpobjid].m_type[tmpi] > 0 )
				g_struct[tmpobjid].m_rtn_count++;
		}
	}
	mysql_free_result( res );

	return 0;
}

char *struct_getdatatype( short datatype, char *ptr, char *lkset )
{
	if( ptr == NULL )
		return NULL;

	switch( datatype )
	{
	case DATATYPE_CHAR:
	case DATATYPE_BIN:
		strcpy( ptr, "char " );
		if( lkset )
			strcpy( lkset, "lua_tointeger" );
		break;
	case DATATYPE_SHORT:
		strcpy( ptr, "short " );
		if( lkset )
			strcpy( lkset, "lua_tointeger" );
		break;
	case DATATYPE_INT:
		strcpy( ptr, "int " );
		if( lkset )
			strcpy( lkset, "lua_tointeger" );
		break;
	case DATATYPE_INT64:
		strcpy( ptr, "i64 " );
		if( lkset )
			strcpy( lkset, "lua_tonumber" );
		break;
	case DATATYPE_FLOAT:
		strcpy( ptr, "float " );
		if( lkset )
			strcpy( lkset, "lua_tonumber" );
		break;
	case DATATYPE_DOUBLE:
		strcpy( ptr, "double " );
		if( lkset )
			strcpy( lkset, "lua_tonumber" );
		break;
	case DATATYPE_CHARPTR:
		strcpy( ptr, "char *" );
		if( lkset )
			strcpy( lkset, "lua_tostring" );
		break;
	case DATATYPE_VOIDPTR:
		strcpy( ptr, "void *" );
		if( lkset )
			strcpy( lkset, "lua_touserdata" );
		break;
	case DATATYPE_UNCHAR:
		strcpy( ptr, "unsigned char " );
		if( lkset )
			strcpy( lkset, "lua_tointeger" );
		break;
	case DATATYPE_UNSHORT:
		strcpy( ptr, "unsigned short " );
		if( lkset )
			strcpy( lkset, "lua_tointeger" );
		break;
	case DATATYPE_UNINT:
		strcpy( ptr, "unsigned int " );
		if( lkset )
			strcpy( lkset, "lua_tointeger" );
		break;
	case -(DATATYPE_CHAR):
		strcpy( ptr, "char " );
		if( lkset )
			strcpy( lkset, "lua_pushinteger" );
		break;
	case -(DATATYPE_SHORT):
		strcpy( ptr, "short " );
		if( lkset )
			strcpy( lkset, "lua_pushinteger" );
		break;
	case -(DATATYPE_INT):
		strcpy( ptr, "int " );
		if( lkset )
			strcpy( lkset, "lua_pushinteger" );
		break;
	case -(DATATYPE_INT64):
		strcpy( ptr, "i64 " );
		if( lkset )
			strcpy( lkset, "lua_pushnumber" );
		break;
	case -(DATATYPE_FLOAT):
		strcpy( ptr, "float " );
		if( lkset )
			strcpy( lkset, "lua_pushnumber" );
		break;
	case -(DATATYPE_DOUBLE):
		strcpy( ptr, "double " );
		if( lkset )
			strcpy( lkset, "lua_pushnumber" );
		break;
	case -(DATATYPE_CHARPTR):
		strcpy( ptr, "char *" );
		if( lkset )
			strcpy( lkset, "lua_pushstring" );
		break;
	case -(DATATYPE_VOIDPTR):
		strcpy( ptr, "void *" );
		if( lkset )
			strcpy( lkset, "lua_pushlightuserdata" );
		break;
	case -(DATATYPE_UNCHAR):
		strcpy( ptr, "unsigned char " );
		if( lkset )
			strcpy( lkset, "lua_pushinteger" );
		break;
	case -(DATATYPE_UNSHORT):
		strcpy( ptr, "unsigned short " );
		if( lkset )
			strcpy( lkset, "lua_pushinteger" );
		break;
	case -(DATATYPE_UNINT):
		strcpy( ptr, "unsigned int " );
		if( lkset )
			strcpy( lkset, "lua_pushinteger" );
		break;
	}
	return ptr;
}

int struct_create_h_srv()
{
	int tmpi, tmpj;
	char tmpstr[256];
	char szFile[256];
	char szPath[256];

	FILE *fp;

	strcpy( szPath, SCUI_IN_CREATE_PATH );
	struct_reset( "scsrv_in_struct" );

	sprintf( szFile, "%s/include/script_auto.h", szPath );
	fp = fopen( szFile, "wb" );
	if( fp == NULL )
		return -1;

	fprintf( fp, "#ifndef __SCRIPT_auto_H\r\n" );
	fprintf( fp, "#define __SCRIPT_auto_H\r\n" );
	fprintf( fp, "\r\n" );
	fprintf( fp, "void lua_func_register();\r\n" );
	fprintf( fp, "void lua_func_function();\r\n" );
	fprintf( fp, "\r\n" );

	for( tmpi = 0; tmpi < g_struct_count; tmpi++ )
	{
		if( g_struct[tmpi].m_name[0] )
		{
			fprintf( fp, "int sc_%s(", g_struct[tmpi].m_name );
			for( tmpj = 0; tmpj < g_struct[tmpi].m_arg_count+g_struct[tmpi].m_rtn_count; tmpj++ )
			{
				if( g_struct[tmpi].m_type[tmpj] >= 0 )
					continue;
				if( tmpj > 0 )
					fprintf( fp, ", " );
				else
					fprintf( fp, " " );
				fprintf( fp, "%s%s", struct_getdatatype( g_struct[tmpi].m_type[tmpj], tmpstr, NULL ), g_struct[tmpi].m_arg[tmpj] );
			}
			if( tmpj > 0 )
				fprintf( fp, " " );
			fprintf( fp, ");\r\n" );
		}
	}
	fprintf( fp, "\r\n#endif\r\n" );
	fclose( fp );
	return 0;
}

void struct_create_srvreg()
{
	int tmpi,tmpj;
	FILE *fp;

	char lkset[256];
	char tmpstr[256];
	char szFile[256];
	char szPath[256];
	char **linebuf = NULL;
	int linecount;
	char has;
	int isbegin;

	struct_reset( "scsrv_c_struct" );

	strcpy( szPath, SCUI_IN_CREATE_PATH );
	sprintf( szFile, "%s/src/script_reg_auto.cpp", szPath );

	fp = fopen( szFile, "rb" );
	if( fp != NULL )
	{
		linebuf = u_load_file( fp, &linecount );
		fclose( fp );
	}

	fp = fopen( szFile, "wb" );
	if( fp == NULL )
		return;

	if( linebuf == NULL )
	{
		fprintf( fp, "extern \"C\" {\r\n" );
		fprintf( fp, "	#include \"lua.h\"\r\n" );
		fprintf( fp, "	#include \"lualib.h\"\r\n" );
		fprintf( fp, "	#include \"lauxlib.h\"\r\n" );
		fprintf( fp, "}\r\n" );

		fprintf( fp, "#include \"script.h\"\r\n" );
		fprintf( fp, "#include \"define.h\"\r\n" );
		fprintf( fp, "#include \"thread.h\"\r\n" );
		fprintf( fp, "#include \"utils.h\"\r\n" );
		fprintf( fp, "\r\n" );
		fprintf( fp, "extern lua_State* servL;\r\n" );
		fprintf( fp, "//--Automatically define function\r\n" );
	}
	else
	{
		for( tmpi = 0; tmpi < linecount; tmpi++ )
		{
			fprintf( fp, "%s\n", linebuf[tmpi] );
			if( u_frontcmp( linebuf[tmpi], "//--Automatically define function" ) )
				break;
		}
	}
	for( tmpi = 0; tmpi < g_struct_count; tmpi++ )
	{
		if( g_struct[tmpi].m_name[0] )
		{
			fprintf( fp, "static int lua_%s( lua_State *servL );\r\n", g_struct[tmpi].m_name );
		}
	}
	fprintf( fp, "\r\n" );
	fprintf( fp, "void lua_func_register()\r\n{\r\n" );
	for( tmpi = 0; tmpi < g_struct_count; tmpi++ )
	{
		if( g_struct[tmpi].m_name[0] )
		{
			fprintf( fp, "	lua_register(servL, \"%s\", lua_%s);\r\n", g_struct[tmpi].m_name, g_struct[tmpi].m_name );
		}
	}
	fprintf( fp, "}\r\n" );
	fprintf( fp, "\r\n" );

	if( linebuf == NULL )
	{
		fprintf( fp, "//--Automatically main function\r\n" );
	}
	else
	{
		isbegin = 0;
		for( tmpi = 0; tmpi < linecount; tmpi++ )
		{
			if( isbegin == 0 && u_frontcmp( linebuf[tmpi], "//--Automatically main function" ) )
				isbegin = tmpi+1;
			if( isbegin == 0 )
				continue;
			fprintf( fp, "%s\n", linebuf[tmpi] );
		}
	}

	for( tmpi = 0; tmpi < g_struct_count; tmpi++ )
	{
		if( g_struct[tmpi].m_name[0] )
		{
			has = 0;
			if( linebuf )
			{
				sprintf( tmpstr, "static int lua_%s( lua_State *servL )", g_struct[tmpi].m_name );
				for( tmpj = isbegin; tmpj < linecount; tmpj++ )
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
			fprintf( fp, "static int lua_%s( lua_State *servL )\r\n{\r\n", g_struct[tmpi].m_name );

			fprintf( fp, "	int num = lua_gettop(servL);\r\n" );
			if( g_struct[tmpi].m_limit_count > 0 )
				fprintf( fp, "	if ( num < %d )\r\n", g_struct[tmpi].m_limit_count );
			else
				fprintf( fp, "	if ( num != %d )\r\n", g_struct[tmpi].m_arg_count );

			fprintf( fp, "	{\r\n" );
			fprintf( fp, "		char szErrorMsg[128];\r\n" );
			fprintf( fp, "		sprintf( szErrorMsg, \"Incorrect argument to function '%%s'\", __FUNCTION__ );\r\n" );
			fprintf( fp, "		lua_pushstring( servL, szErrorMsg );\r\n" );
			fprintf( fp, "		lua_error( servL );\r\n" );
			fprintf( fp, "		return 0;\r\n" );
			fprintf( fp, "	}\r\n" );

			for( tmpj = 0; tmpj < g_struct[tmpi].m_arg_count+g_struct[tmpi].m_rtn_count; tmpj++ )
			{
				if( g_struct[tmpi].m_type[tmpj] >= 0 )
					continue;
				struct_getdatatype( g_struct[tmpi].m_type[tmpj], tmpstr, lkset );
				if( g_struct[tmpi].m_limit_count > 0 && tmpj >= g_struct[tmpi].m_limit_count )
				{
					fprintf( fp, "	%s%s = 0; if( num > %d ) %s = (%s)%s( servL, %d );\r\n", tmpstr, g_struct[tmpi].m_arg[tmpj], tmpj, g_struct[tmpi].m_arg[tmpj], tmpstr, lkset, tmpj+1 );
				}
				else
				{
					fprintf( fp, "	%s%s = (%s)%s( servL, %d );\r\n", tmpstr, g_struct[tmpi].m_arg[tmpj], tmpstr, lkset, tmpj+1 );
				}
			}
			fprintf( fp, "	//--Process script\r\n" );
			for( tmpj = 0; tmpj < g_struct[tmpi].m_arg_count+g_struct[tmpi].m_rtn_count; tmpj++ )
			{
				if( g_struct[tmpi].m_type[tmpj] <= 0 )
					continue;
				struct_getdatatype( g_struct[tmpi].m_type[tmpj], tmpstr, lkset );
				fprintf( fp, "	%s%s = 0;\r\n", tmpstr, g_struct[tmpi].m_arg[tmpj] );
			}
			for( tmpj = 0; tmpj < g_struct[tmpi].m_arg_count+g_struct[tmpi].m_rtn_count; tmpj++ )
			{
				if( g_struct[tmpi].m_type[tmpj] <= 0 )
					continue;
				struct_getdatatype( g_struct[tmpi].m_type[tmpj], tmpstr, lkset );
				fprintf( fp, "	%s( servL, %s );\r\n", lkset, g_struct[tmpi].m_arg[tmpj] );
			}
			fprintf( fp, "	return %d;\r\n", g_struct[tmpi].m_rtn_count );
			fprintf( fp, "}\r\n" );
			fprintf( fp, "\r\n" );
		}
	}
	fclose( fp );
	
	if( linebuf )
		u_free_vec( linebuf );
}

void struct_create_srv()
{
	int tmpi,tmpj;
	FILE *fp;

	char lkset[256];
	char tmpstr[256];
	char szFile[256];
	char szPath[256];

	struct_reset( "scsrv_in_struct" );

	strcpy( szPath, SCUI_IN_CREATE_PATH );
	sprintf( szFile, "%s/src/script_auto.cpp", szPath );

	fp = fopen( szFile, "wb" );
	if( fp == NULL )
		return;

	fprintf( fp, "extern \"C\" {\r\n" );
	fprintf( fp, "	#include \"lua.h\"\r\n" );
	fprintf( fp, "	#include \"lualib.h\"\r\n" );
	fprintf( fp, "	#include \"lauxlib.h\"\r\n" );
	fprintf( fp, "}\r\n" );
	fprintf( fp, "\r\n" );
	fprintf( fp, "#include \"script.h\"\r\n" );
	fprintf( fp, "#include \"define.h\"\r\n" );
	fprintf( fp, "#include \"thread.h\"\r\n" );
	fprintf( fp, "#include \"utils.h\"\r\n" );

	fprintf( fp, "\r\n" );
	fprintf( fp, "extern lua_State* servL;\r\n" );
	fprintf( fp, "//--Automatically define function\r\n" );
	fprintf( fp, "\r\n" );

	for( tmpi = 0; tmpi < g_struct_count; tmpi++ )
	{
		if( g_struct[tmpi].m_name[0] )
		{
			fprintf( fp, "static int _%s_ref;\r\n", g_struct[tmpi].m_name );
		}
	}
	fprintf( fp, "\r\n" );
	fprintf( fp, "void lua_func_function()\r\n{\r\n" );
	for( tmpi = 0; tmpi < g_struct_count; tmpi++ )
	{
		if( g_struct[tmpi].m_name[0] )
		{
			fprintf( fp, "	lua_getglobal(servL, \"IN_%s\");\r\n", g_struct[tmpi].m_name );
			fprintf( fp, "	_%s_ref = luaL_ref( servL, LUA_REGISTRYINDEX );\r\n", g_struct[tmpi].m_name );
		}
	}
	fprintf( fp, "}\r\n" );
	fprintf( fp, "\r\n" );

	for( tmpi = 0; tmpi < g_struct_count; tmpi++ )
	{
		if( g_struct[tmpi].m_name[0] )
		{
			fprintf( fp, "int sc_%s(", g_struct[tmpi].m_name );
			for( tmpj = 0; tmpj < g_struct[tmpi].m_arg_count; tmpj++ )
			{
				if( g_struct[tmpi].m_type[tmpj] >= 0 )
					continue;				
				if( tmpj > 0 )
					fprintf( fp, ", " );
				else
					fprintf( fp, " " );
				fprintf( fp, "%s%s", struct_getdatatype( g_struct[tmpi].m_type[tmpj], tmpstr, NULL ), g_struct[tmpi].m_arg[tmpj] );
			}
			if( tmpj > 0 )
				fprintf( fp, " " );

			for ( tmpj = 0; tmpj < g_struct[tmpi].m_rtn_count; tmpj++ )
			{
				if( g_struct[tmpi].m_type[tmpj] <= 0 )
					continue;	
				if( tmpj > 0 )
					fprintf( fp, ", " );
				else
					fprintf( fp, " " );
				fprintf( fp, "%s*%s", struct_getdatatype( g_struct[tmpi].m_type[tmpj], tmpstr, NULL ), g_struct[tmpi].m_arg[tmpj] );
			}
			if( tmpj > 0 )
				fprintf( fp, " " );
			fprintf( fp, ")\r\n{\r\n" );

			fprintf( fp, "	int nResult = 0;\r\n" );
			fprintf( fp, "	lua_rawgeti( servL, LUA_REGISTRYINDEX, _%s_ref );\r\n", g_struct[tmpi].m_name );

			for( tmpj = 0; tmpj < g_struct[tmpi].m_arg_count; tmpj++ )
			{
				if( g_struct[tmpi].m_type[tmpj] >= 0 )
					continue;
				struct_getdatatype( -g_struct[tmpi].m_type[tmpj], tmpstr, lkset );
				fprintf( fp, "	%s( servL, %s );\r\n", lkset, g_struct[tmpi].m_arg[tmpj] );
			}
			if ( g_struct[tmpi].m_rtn_count > 1 )
			{
				fprintf( fp, "	lua_call( servL, %d, %d );\r\n", g_struct[tmpi].m_arg_count, g_struct[tmpi].m_rtn_count );
				for ( tmpj = 0; tmpj < g_struct[tmpi].m_rtn_count; tmpj++ )
				{
					fprintf( fp, "	*%s = (int)lua_tonumber( servL, %d );\r\n", g_struct[tmpi].m_arg[tmpj], -(tmpj+1) );
					fprintf( fp, "	lua_pop( servL, %d );\r\n", g_struct[tmpi].m_rtn_count );
				}
				fprintf( fp, "	return nResult;\r\n" );
			}
			else
			{
				fprintf( fp, "	lua_call( servL, %d, 1 );\r\n", g_struct[tmpi].m_arg_count );
				fprintf( fp, "	nResult = (int)lua_tonumber( servL, -1 );\r\n" );
				fprintf( fp, "	lua_pop( servL, 1 );\r\n" );
				fprintf( fp, "	return nResult;\r\n" );
			}
			fprintf( fp, "}\r\n" );
			fprintf( fp, "\r\n" );
		}
	}

	fclose( fp );
}

int struct_create()
{
	struct_create_h_srv();
	struct_create_srvreg();
	struct_create_srv();
	return 0;
}
