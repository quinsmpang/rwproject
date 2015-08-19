#ifdef WIN32
#include <winsock2.h>
#include <mswsock.h>
#endif
#include <stdio.h>
#include <string.h>

#include <mysql.h>
#include "struct_config.h"
#include "netcmd_config.h"
#include "actordata_config.h"
#include "utils.h"

//#define _NETPROCESSAUTO

extern MYSQL *myData;
extern SNetcmdConfig *g_netscmd;
extern int g_netscmd_count;
extern SNetcmdConfig *g_netccmd;
extern int g_netccmd_count;

// 任务目标的分组信息
extern SStructConfigGroup *g_struct;
extern int g_struct_count;

int actordata_getdatatype2( short datatype, char *ptr, char *lkset )
{
	char ispoint = 0;
	int size = 0;
	if( datatype > 100 )
	{
		ispoint = 1;
		datatype -= 100;
	}
	if( datatype < 0 || datatype >= MAX_ACTORDATA_COUNT )
		return NULL;
	*ptr = 0;
	if( lkset )
		strcpy( lkset, "c_data_stringset" );
	return size;
}

int struct_getdatatype2( short datatype, char *ptr, char *lkset )
{
	int size = 0;
	if( ptr == NULL )
		return NULL;

	if( datatype <= -100 )
	{
		return actordata_getdatatype2( (-datatype)-100, ptr, lkset );
	}

	switch( datatype )
	{
	case DATATYPE_CHAR:
		strcpy( ptr, "char" );
		if( lkset )
			strcpy( lkset, "SByte" );
		size = 1;
		break;
	case DATATYPE_SHORT:
		strcpy( ptr, "short" );
		if( lkset )
			strcpy( lkset, "Short" );
		size = 2;
		break;
	case DATATYPE_INT:
		strcpy( ptr, "int" );
		if( lkset )
			strcpy( lkset, "Int" );
		size = 4;
		break;
	case DATATYPE_INT64:
		strcpy( ptr, "i64" );
		if( lkset )
			strcpy( lkset, "Long" );
		size = 8;
		break;
	case DATATYPE_FLOAT:
		strcpy( ptr, "float" );
		if( lkset )
			strcpy( lkset, "Float" );
		size = 4;
		break;
	case DATATYPE_DOUBLE:
		strcpy( ptr, "double" );
		if( lkset )
			strcpy( lkset, "Double" );
		size = 8;
		break;
	case DATATYPE_UNCHAR:
		strcpy( ptr, "unsigned char" );
		if( lkset )
			strcpy( lkset, "Byte" );
		size = 1;
		break;
	case DATATYPE_UNSHORT:
		strcpy( ptr, "unsigned short" );
		if( lkset )
			strcpy( lkset, "UShort" );
		size = 2;
		break;
	case DATATYPE_UNINT:
		strcpy( ptr, "unsigned int" );
		if( lkset )
			strcpy( lkset, "UInt" );
		size = 4;
		break;
	case DATATYPE_BIN:
		strcpy( ptr, "char" );
		if( lkset )
			strcpy( lkset, "Byte" );
		size = 1;
		break;
	default:
		if( datatype < 0 || datatype >= g_struct_count )
			return NULL;
		sprintf( ptr, "CLK_%s", g_struct[datatype].m_name );
		if( lkset )
			strcpy( lkset, "String" );
		size = 0;
		break;
	}
	return size;
}

int struct_create_h2()
{
	int tmpi;
	char tmpstr[256];
	char szFile[256];
	char szPath[256];
	FILE *fpbase;

	strcpy( szPath, CLIENT_CREATE_PATH );
	struct_reset();

	sprintf( szFile, "%s/z_client_struct_auto.lua", szPath );
	fpbase = fopen( szFile, "wb" );
	if( fpbase == NULL )
		return -1;
	fprintf( fpbase, "--client_struct_auto.lua\r\n\r\n");

	fprintf( fpbase, "CMDS_BASE = 0;\r\n" );
	fprintf( fpbase, "CMDC_BASE = 0;\r\n" );
	fprintf( fpbase, "\r\n" );

	for( tmpi = 0; tmpi < g_netscmd_count; tmpi++ )
	{
		if( g_netscmd[tmpi].m_cmdstr[0] )
		{
			strcpy( tmpstr, g_netscmd[tmpi].m_cmdstr );
			u_str_toupper( tmpstr );
			if( g_netscmd[tmpi].m_cmdid >= 0 )
				fprintf( fpbase, "CMDS_%s = CMDS_BASE+%d;\r\n", tmpstr, g_netscmd[tmpi].m_cmdid );
			else
				fprintf( fpbase, "CMDS_%s = CMDS_BASE%d;\r\n", tmpstr, g_netscmd[tmpi].m_cmdid );
		}
	}
	fprintf( fpbase, "\r\n" );

	for( tmpi = 0; tmpi < g_netccmd_count; tmpi++ )
	{
		if( g_netccmd[tmpi].m_cmdid < 0 )
			continue;
		if( g_netccmd[tmpi].m_cmdstr[0] )
		{
			strcpy( tmpstr, g_netccmd[tmpi].m_cmdstr );
			u_str_toupper( tmpstr );
			if( g_netccmd[tmpi].m_cmdid >= 0 )
				fprintf( fpbase, "CMDC_%s = CMDC_BASE+%d;\r\n", tmpstr, g_netccmd[tmpi].m_cmdid );
			else
				fprintf( fpbase, "CMDC_%s = CMDC_BASE%d;\r\n", tmpstr, g_netccmd[tmpi].m_cmdid );
		}
	}
	fprintf( fpbase, "\r\n" );
	fclose( fpbase );
	return 0;
}

void struct_create_structsend2()
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

	int struct_size;

	strcpy( szPath, CLIENT_CREATE_PATH );
	sprintf( szFile, "%s/z_client_structsend_auto.lua", szPath );

	fp = fopen( szFile, "wb" );
	if( fp == NULL )
		return;

	fprintf( fp, "--client_structsend_auto.lua\r\n\r\n");
	for( structid = 0; structid < g_struct_count; structid++ )
	{
		if( g_struct[structid].m_name[0] && g_struct[structid].m_struct_type == 2 )
		{
			fprintf( fp, "function struct_%s_send( buf, sendValue )\r\n", g_struct[structid].m_name );
			for( tmpj = 0; tmpj < g_struct[structid].m_count; tmpj++ )
			{
				struct_size =  struct_getdatatype2( g_struct[structid].m_sconf[tmpj].data_type, tmpstr, lkset );
				//if( struct_size == 0 )
				//{
				//	fclose( fp );
				//	return;
				//}
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
							fprintf( fp, "	for tmpi=1,%d,1 do\r\n", g_struct[structid].m_sconf[tmpj].data_count );
							fprintf( fp, "		struct_%s_send( buf, sendValue.%s%s[tmpi] );\r\n", g_struct[substructid].m_name, szFix, g_struct[structid].m_sconf[tmpj].m_name );
							fprintf( fp, "	end\r\n" );
						}
						else if( g_struct[structid].m_sconf[tmpj].data_count == -1 )
						{
							if( tmpj > 0 )
							{
								if( g_struct[structid].m_sconf[tmpj].data_size == 1 )
								{
									fprintf( fp, "	struct_%s_send( buf, sendValue.%s%s );\r\n", g_struct[substructid].m_name, szFix, g_struct[structid].m_sconf[tmpj].m_name );
								}
								else
								{
									fprintf( fp, "	for tmpi=1,sendValue.%s%s do\r\n", szFixPre, g_struct[structid].m_sconf[tmpj-1].m_name );
									fprintf( fp, "		struct_%s_send( buf, sendValue.%s%s[tmpi] );\r\n", g_struct[substructid].m_name, szFix, g_struct[structid].m_sconf[tmpj].m_name );
									fprintf( fp, "	end\r\n" );
								}
							}
						}
						else
						{
							fprintf( fp, "	struct_%s_send( buf, sendValue.%s%s );\r\n", g_struct[substructid].m_name, szFix, g_struct[structid].m_sconf[tmpj].m_name );
						}
					}
				}
				else
				{
					if( g_struct[structid].m_sconf[tmpj].data_count > 1 )
					{
						if( g_struct[structid].m_sconf[tmpj].data_type == DATATYPE_CHAR )
						{
							fprintf( fp, "	buf:WriteStringWithLength( sendValue.%s%s );\r\n", szFix, g_struct[structid].m_sconf[tmpj].m_name );
						}
						else
						{
							fprintf( fp, "	for tmpi=1,%d,1 do\r\n", g_struct[structid].m_sconf[tmpj].data_count );
							fprintf( fp, "		buf:Write%s( sendValue.%s%s[tmpi] );\r\n", lkset, szFix, g_struct[structid].m_sconf[tmpj].m_name );
							fprintf( fp, "	end\r\n" );
						}
					}
					else if( g_struct[structid].m_sconf[tmpj].data_count == -1 )
					{
						if( tmpj > 0 )
						{
							if( g_struct[structid].m_sconf[tmpj].data_type == DATATYPE_CHAR )
							{
								fprintf( fp, "	buf:WriteStringWithLength( sendValue.%s%s );\r\n", szFix, g_struct[structid].m_sconf[tmpj].m_name );
							}
							else
							{
								fprintf( fp, "	for tmpi=1,sendValue.%s%s,1 do\r\n", szFixPre, g_struct[structid].m_sconf[tmpj-1].m_name );
								fprintf( fp, "		buf:Write%s( sendValue.%s%s[tmpi] );\r\n", lkset, szFix, g_struct[structid].m_sconf[tmpj].m_name );
								fprintf( fp, "	end\r\n" );
							}

						}
					}
					else
					{
						fprintf( fp, "	buf:Write%s( sendValue.%s%s );\r\n", lkset, szFix, g_struct[structid].m_sconf[tmpj].m_name );
					}
				}
			}
			fprintf( fp, "end\r\n" );
			fprintf( fp, "\r\n" );
		}
	}
	fclose( fp );
}

void struct_print_send( FILE *fp, int structid );
void struct_print_send( FILE *fp, int structid )
{
	int tmpj;
	char tmpstr[256];
	char lkset[256];
	char szFix[8];
	char szFixPre[8];
	int substructid;
	if( structid < 0 )
	{
		fprintf( fp, "number" );
		return;
	}
	for( tmpj = 0; tmpj < g_struct[structid].m_count; tmpj++ )
	{
		struct_getdatatype2( g_struct[structid].m_sconf[tmpj].data_type, tmpstr, lkset );
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
					fprintf( fp, "%s%s={", szFix, g_struct[structid].m_sconf[tmpj].m_name );
					struct_print_send( fp, g_struct[structid].m_sconf[tmpj].data_type );
					fprintf( fp, "[%d]},", g_struct[structid].m_sconf[tmpj].data_count );
				}
				else if( g_struct[structid].m_sconf[tmpj].data_count == -1 )
				{
					if( tmpj > 0 )
					{
						if( g_struct[structid].m_sconf[tmpj].data_size == 1 )
						{
							fprintf( fp, "%s%s={", szFix, g_struct[structid].m_sconf[tmpj].m_name );
							struct_print_send( fp, g_struct[structid].m_sconf[tmpj].data_type );
							fprintf( fp, "},", szFixPre, g_struct[structid].m_sconf[tmpj-1].m_name );
						}
						else
						{
							fprintf( fp, "%s%s={", szFix, g_struct[structid].m_sconf[tmpj].m_name );
							struct_print_send( fp, g_struct[structid].m_sconf[tmpj].data_type );
							fprintf( fp, "[%s%s]},", szFixPre, g_struct[structid].m_sconf[tmpj-1].m_name );
						}
					}
				}
				else
				{
					fprintf( fp, "%s%s={", szFix, g_struct[structid].m_sconf[tmpj].m_name );
					struct_print_send( fp, g_struct[structid].m_sconf[tmpj].data_type );
					fprintf( fp, "},", szFixPre, g_struct[structid].m_sconf[tmpj-1].m_name );
				}
			}
		}
		else
		{
			if( g_struct[structid].m_sconf[tmpj].data_count > 1 )
			{
				if( g_struct[structid].m_sconf[tmpj].data_type == DATATYPE_CHAR )
				{
					fprintf( fp, "%s%s=\"[%d]\",", szFix, g_struct[structid].m_sconf[tmpj].m_name, g_struct[structid].m_sconf[tmpj].data_count );
				}
				else
				{
					fprintf( fp, "%s%s={[%d]},", szFix, g_struct[structid].m_sconf[tmpj].m_name, g_struct[structid].m_sconf[tmpj].data_count );
				}
			}
			else if( g_struct[structid].m_sconf[tmpj].data_count == -1 )
			{
				if( tmpj > 0 )
				{
					if( g_struct[structid].m_sconf[tmpj].data_type == DATATYPE_CHAR )
					{
						fprintf( fp, "%s%s=\"[%s%s]\",", szFix, g_struct[structid].m_sconf[tmpj].m_name, szFixPre, g_struct[structid].m_sconf[tmpj-1].m_name );
					}
					else
					{
						fprintf( fp, "%s%s={}[%s%s],", szFix, g_struct[structid].m_sconf[tmpj].m_name, szFixPre, g_struct[structid].m_sconf[tmpj-1].m_name );
					}

				}
			}
			else
			{
				fprintf( fp, "%s%s=0,", szFix, g_struct[structid].m_sconf[tmpj].m_name );
			}
		}
	}
}

void struct_create_netsend2()
{
	int tmpi;
	FILE *fp;
	int structid;
	char tmpstr[256];
	char lkset[256];
	char szFile[256];
	char szPath[256];
	int struct_size;

	SNetcmdConfig *pnetcmd;
	int netcmd_count;

	strcpy( szPath, CLIENT_CREATE_PATH );
	pnetcmd = g_netccmd;
	netcmd_count = g_netccmd_count;
	sprintf( szFile, "%s/z_client_netsend_auto.lua", szPath );

	fp = fopen( szFile, "wb" );
	if( fp == NULL )
		return;

	fprintf( fp, "--client_netsend_auto.lua\r\n\r\n");

	for( tmpi = 0; tmpi < netcmd_count; tmpi++ )
	{
		if( pnetcmd[tmpi].m_cmdid < 0 )
			continue;
		structid = pnetcmd[tmpi].m_structid;
		if( structid < 0 || g_struct[structid].m_name[0] )
		{
			fprintf( fp, "-- ");
			struct_print_send( fp, structid );
			fprintf( fp, "\r\n");

			fprintf( fp, "function netsend_%s_C( sendValue )\r\n", pnetcmd[tmpi].m_cmdstr );
			fprintf( fp, "	local buf = ByteBuffer.New();\r\n" );

			strcpy( tmpstr, pnetcmd[tmpi].m_cmdstr );
			u_str_toupper( tmpstr );
			fprintf( fp, "	buf:WriteShort( CMDC_%s );\r\n", tmpstr );

			if( structid < 0 )
			{
				struct_size = struct_getdatatype2( structid, tmpstr, lkset );
				if( struct_size )
				{
					fprintf( fp, "	buf:Write%s( sendValue );\r\n", lkset );
				}
			}
			else
			{
				fprintf( fp, "	struct_%s_send( buf, sendValue );\r\n", g_struct[structid].m_name );
			}

			strcpy( tmpstr, pnetcmd[tmpi].m_cmdstr );
			u_str_toupper( tmpstr );
#ifdef COCOS2D
			fprintf( fp, "	SendMessage(buf);\r\n" );
#else
			fprintf( fp, "	fruit.networkManager:SendMessage(buf);\r\n" );
#endif
			fprintf( fp, "end\r\n" );
			fprintf( fp, "\r\n" );
		}
	}
	fclose( fp );
}

void struct_create_structrecv2()
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
	int struct_size;

	strcpy( szPath, CLIENT_CREATE_PATH );
	sprintf( szFile, "%s/z_client_structrecv_auto.lua", szPath );

	fp = fopen( szFile, "wb" );
	if( fp == NULL )
		return;

	fprintf( fp, "--client_structrecv_auto.lua\r\n\r\n");
	for( structid = 0; structid < g_struct_count; structid++ )
	{
		if( g_struct[structid].m_name[0] && g_struct[structid].m_struct_type == 1 )
		{
			fprintf( fp, "function struct_%s_recv( buffer )\r\n", g_struct[structid].m_name );
			fprintf( fp, "	local recvValue = {};\r\n");			

			for( tmpj = 0; tmpj < g_struct[structid].m_count; tmpj++ )
			{
				struct_size = struct_getdatatype2( g_struct[structid].m_sconf[tmpj].data_type, tmpstr, lkset );
/*
				if( struct_size == 0 )
				{
					fclose( fp );
					return;
				}
*/
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
							fprintf( fp, "	recvValue.%s%s = {};\r\n", szFix, g_struct[structid].m_sconf[tmpj].m_name );
							fprintf( fp, "	for tmpi=1,%d,1 do\r\n", g_struct[structid].m_sconf[tmpj].data_count );
							fprintf( fp, "		local tmpValue={};\r\n" );
							fprintf( fp, "		tmpValue = struct_%s_recv( buffer );\r\n", g_struct[substructid].m_name );
							fprintf( fp, "		table.insert( recvValue.%s%s, tmpValue );\r\n", szFix, g_struct[structid].m_sconf[tmpj].m_name );
							fprintf( fp, "	end\r\n" );
						}
						else if( g_struct[structid].m_sconf[tmpj].data_count == -1 )
						{
							if( tmpj > 0 )
							{
								if( g_struct[structid].m_sconf[tmpj].data_size == 1 )
								{
									fprintf( fp, "	recvValue.%s%s = struct_%s_recv( buffer );\r\n", szFix, g_struct[structid].m_sconf[tmpj].m_name, g_struct[substructid].m_name );
								}
								else
								{
									fprintf( fp, "	recvValue.%s%s = {};\r\n", szFix, g_struct[structid].m_sconf[tmpj].m_name );
									fprintf( fp, "	for tmpi=1,recvValue.%s%s,1 do\r\n", szFixPre, g_struct[structid].m_sconf[tmpj-1].m_name );
									fprintf( fp, "		local tmpValue={};\r\n" );
									fprintf( fp, "		tmpValue = struct_%s_recv( buffer );\r\n", g_struct[substructid].m_name );
									fprintf( fp, "		table.insert( recvValue.%s%s, tmpValue );\r\n", szFix, g_struct[structid].m_sconf[tmpj].m_name );
									fprintf( fp, "	end\r\n" );
								}
							}
						}
						else
						{
							fprintf( fp, "	recvValue.%s%s = struct_%s_recv( buffer );\r\n", szFix, g_struct[structid].m_sconf[tmpj].m_name, g_struct[substructid].m_name );
						}
					}
				}
				else
				{
					if( g_struct[structid].m_sconf[tmpj].data_count > 1 )
					{
						if( g_struct[structid].m_sconf[tmpj].data_type == DATATYPE_CHAR )
						{
							fprintf( fp, "	recvValue.%s%s = buffer:ReadStringWithLen( %d );\r\n", szFix, g_struct[structid].m_sconf[tmpj].m_name, g_struct[structid].m_sconf[tmpj].data_count );
						}
						else
						{
							fprintf( fp, "	recvValue.%s%s={};\r\n", szFix, g_struct[structid].m_sconf[tmpj].m_name );
							fprintf( fp, "	for tmpi=1,%d,1 do\r\n", g_struct[structid].m_sconf[tmpj].data_count );
							fprintf( fp, "		recvValue.%s%s[tmpi] = buffer:Read%s();\r\n", szFix, g_struct[structid].m_sconf[tmpj].m_name, lkset );
							fprintf( fp, "	end\r\n" );
						}
					}
					else if( g_struct[structid].m_sconf[tmpj].data_count == -1 )
					{
						if( tmpj > 0 )
						{
							if( g_struct[structid].m_sconf[tmpj].data_type == DATATYPE_CHAR )
							{
								fprintf( fp, "	recvValue.%s%s = buffer:ReadStringWithLen( recvValue.%s%s );\r\n", szFix, g_struct[structid].m_sconf[tmpj].m_name, szFixPre, g_struct[structid].m_sconf[tmpj-1].m_name );
							}
							else
							{
								fprintf( fp, "	recvValue.%s%s={};\r\n", szFix, g_struct[structid].m_sconf[tmpj].m_name );
								fprintf( fp, "	for tmpi=1,recvValue.%s%s,1 do\r\n", szFixPre, g_struct[structid].m_sconf[tmpj - 1].m_name );
								fprintf( fp, "		recvValue.%s%s[tmpi] = buffer:Read%s();\r\n", szFix, g_struct[structid].m_sconf[tmpj].m_name, lkset );
								fprintf( fp, "	end\r\n" );
							}
						}
					}
					else
					{
						fprintf( fp, "	recvValue.%s%s = buffer:Read%s();\r\n", szFix, g_struct[structid].m_sconf[tmpj].m_name, lkset );
					}
				}
			}
			fprintf( fp, "	return recvValue;\r\n");
			fprintf( fp, "end\r\n" );
			fprintf( fp, "\r\n" );
		}
	}
	fclose( fp );
}

void struct_create_netrecv2()
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

	strcpy( szPath, CLIENT_CREATE_PATH );
	pnetcmd = g_netscmd;
	netcmd_count = g_netscmd_count;

	sprintf( szFile, "%s/z_client_netrecv_auto.lua", szPath );
	fp = fopen( szFile, "wb" );
	if( fp == NULL )
		return;
	fprintf( fp, "--client_netrecv_auto.lua\r\n\r\n");

	for( tmpi = 0; tmpi < netcmd_count; tmpi++ )
	{
		structid = pnetcmd[tmpi].m_structid;
		if( structid < 0 || g_struct[structid].m_name[0] )
		{
			fprintf( fp, "function netrecv_%s_C( buffer )\r\n", pnetcmd[tmpi].m_cmdstr );

			if( structid < 0 )
			{
				if( struct_getdatatype2( structid, tmpstr, lkset ) )
				{
					fprintf( fp, "	local recvValue = buffer:Read%s();\r\n", lkset );
				}
			}
			else
			{
				fprintf( fp, "	local recvValue = struct_%s_recv( buffer );\r\n", g_struct[structid].m_name );
			}
			fprintf( fp, "	proc_%s_C( recvValue );\r\n", pnetcmd[tmpi].m_cmdstr );

			fprintf( fp, "end\r\n" );
			fprintf( fp, "\r\n" );
		}
	}

	fprintf( fp, "Proc_Command = {\r\n" );
	for( tmpi = 0; tmpi < netcmd_count; tmpi++ )
	{
		structid = pnetcmd[tmpi].m_structid;
		if( structid < 0 || g_struct[structid].m_name[0] )
		{
			strcpy( tmpstr, pnetcmd[tmpi].m_cmdstr );
			u_str_toupper( tmpstr );

			if( pnetcmd[tmpi].m_nolua )
				fprintf( fp, "	--[CMDS_%s]=netrecv_%s_C;\r\n", tmpstr, pnetcmd[tmpi].m_cmdstr );
			else
				fprintf( fp, "	[CMDS_%s]=netrecv_%s_C;\r\n", tmpstr, pnetcmd[tmpi].m_cmdstr );
		}
	}
	fprintf( fp, "}\r\n" );
	fprintf( fp, "\r\n" );

	fprintf( fp, "function in_proc_command_C( cmd, buffer )\r\n" );
	fprintf( fp, "	if Proc_Command[cmd] ~= nil then\r\n" );
	fprintf( fp, "		Proc_Command[cmd]( buffer );\r\n" );
	fprintf( fp, "		return 1;\r\n" );
	fprintf( fp, "	end\r\n" );
	fprintf( fp, "	return 0;\r\n" );
	fprintf( fp, "end\r\n" );
	fclose( fp );
}

void struct_print( FILE *fp, int structid );
void struct_print( FILE *fp, int structid )
{
	int tmpj;
	char tmpstr[256];
	char lkset[256];
	char szFix[8];
	char szFixPre[8];
	int substructid;
	for( tmpj = 0; tmpj < g_struct[structid].m_count; tmpj++ )
	{
		struct_getdatatype2( g_struct[structid].m_sconf[tmpj].data_type, tmpstr, lkset );
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
					fprintf( fp, "%s%s={", szFix, g_struct[structid].m_sconf[tmpj].m_name );
					struct_print( fp, g_struct[structid].m_sconf[tmpj].data_type );
					fprintf( fp, "[%d]},", g_struct[structid].m_sconf[tmpj].data_count );
				}
				else if( g_struct[structid].m_sconf[tmpj].data_count == -1 )
				{
					if( tmpj > 0 )
					{
						if( g_struct[structid].m_sconf[tmpj].data_size == 1 )
						{
							fprintf( fp, "%s%s={", szFix, g_struct[structid].m_sconf[tmpj].m_name );
							struct_print( fp, g_struct[structid].m_sconf[tmpj].data_type );
							fprintf( fp, "},", szFixPre, g_struct[structid].m_sconf[tmpj-1].m_name );
						}
						else
						{
							fprintf( fp, "%s%s={", szFix, g_struct[structid].m_sconf[tmpj].m_name );
							struct_print( fp, g_struct[structid].m_sconf[tmpj].data_type );
							fprintf( fp, "[%s%s]},", szFixPre, g_struct[structid].m_sconf[tmpj-1].m_name );
						}
					}
				}
				else
				{
					fprintf( fp, "%s%s={", szFix, g_struct[structid].m_sconf[tmpj].m_name );
					struct_print( fp, g_struct[structid].m_sconf[tmpj].data_type );
					fprintf( fp, "},", szFixPre, g_struct[structid].m_sconf[tmpj-1].m_name );
				}
			}
		}
		else
		{
			if( g_struct[structid].m_sconf[tmpj].data_count > 1 )
			{
				if( g_struct[structid].m_sconf[tmpj].data_type == DATATYPE_CHAR )
				{
					fprintf( fp, "%s%s=\"[%d]\",", szFix, g_struct[structid].m_sconf[tmpj].m_name, g_struct[structid].m_sconf[tmpj].data_count );
				}
				else
				{
					fprintf( fp, "%s%s={[%d]},", szFix, g_struct[structid].m_sconf[tmpj].m_name, g_struct[structid].m_sconf[tmpj].data_count );
				}
			}
			else if( g_struct[structid].m_sconf[tmpj].data_count == -1 )
			{
				if( tmpj > 0 )
				{
					if( g_struct[structid].m_sconf[tmpj].data_type == DATATYPE_CHAR )
					{
						fprintf( fp, "%s%s=\"[%s%s]\",", szFix, g_struct[structid].m_sconf[tmpj].m_name, szFixPre, g_struct[structid].m_sconf[tmpj-1].m_name );
					}
					else
					{
						fprintf( fp, "%s%s={}[%s%s],", szFix, g_struct[structid].m_sconf[tmpj].m_name, szFixPre, g_struct[structid].m_sconf[tmpj-1].m_name );
					}

				}
			}
			else
			{
				fprintf( fp, "%s%s=0,", szFix, g_struct[structid].m_sconf[tmpj].m_name );
			}
		}
	}
}

void struct_create_process2()
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

	strcpy( szPath, CLIENT_CREATE_PATH );
	pnetcmd = g_netscmd;
	netcmd_count = g_netscmd_count;

	sprintf( szFile, "%s/z_client_process_auto.lua", szPath );
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
		fprintf( fp, "--client_process_auto.lua\r\n\r\n");
	}

	for( tmpi = 0; tmpi < netcmd_count; tmpi++ )
	{
		structid = pnetcmd[tmpi].m_structid;
		if( structid < 0 || g_struct[structid].m_name[0] )
		{
			has = 0;
			if( linebuf )
			{
				sprintf( tmpstr, "function proc_%s_C(", pnetcmd[tmpi].m_cmdstr );
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
				if( struct_getdatatype2( structid, tmpstr, NULL ) )
				{
					fprintf( fp, "function proc_%s_C( recvValue )\r\n", pnetcmd[tmpi].m_cmdstr );
				}
			}
			else
			{
////////////////////////
				fprintf( fp, "-- ");
				struct_print( fp, structid );
				fprintf( fp, "\r\n");
///////////////
				fprintf( fp, "function proc_%s_C( recvValue )\r\n", pnetcmd[tmpi].m_cmdstr );
			}
			fprintf( fp, "	-- process.\r\n");
			fprintf( fp, "	-- EventProtocol.addEventListener( \"proc_%s_C\", function( recvValue ) end )\r\n", pnetcmd[tmpi].m_cmdstr );
			fprintf( fp, "	EventProtocol.dispatchEvent( \"proc_%s_C\", recvValue );\r\n", pnetcmd[tmpi].m_cmdstr );
			fprintf( fp, "end\r\n" );
			fprintf( fp, "\r\n" );
		}
	}
	fclose( fp );
	
	if( linebuf )
		u_free_vec( linebuf );
}

int struct_create_c2()
{
	struct_create_structsend2();
	struct_create_netsend2();

	struct_create_structrecv2();
	struct_create_netrecv2();

	struct_create_process2();
	return 0;
}

int struct_create2()
{
	struct_create_h2();
	struct_create_c2();
	return 0;
}
