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

extern MYSQL *myData;
extern SNetcmdConfig *g_netscmd;
extern int g_netscmd_count;
extern SNetcmdConfig *g_netccmd;
extern int g_netccmd_count;

// 任务目标的分组信息
extern SStructConfigGroup *g_struct;
extern int g_struct_count;

void struct_create_structrecv_client()
{
	int tmpj;
	char str[256];
	char szFile[256];
	char szPath[256];
	FILE *fp;
	int structid;
	int substructid;

	strcpy( szPath, CLIENT_CREATE_PATH );

	sprintf( szFile, "%s/src/client_structsend_auto.c", szPath );
	fp = fopen( szFile, "wb" );
	if( fp == NULL )
		return;

	fprintf( fp, "#include <string.h>\r\n" );
	fprintf( fp, "#include \"client_structsend_auto.h\"\r\n" );
	fprintf( fp, "#include \"actor_send.h\"\r\n" );
	fprintf( fp, "\r\n" );

	for( structid = 0; structid < g_struct_count; structid++ )
	{
		if( g_struct[structid].m_name[0] && g_struct[structid].m_struct_type == 1 )
		{
			fprintf( fp, "int struct_%s( char **pptr, int *psize, SLK_%s *pValue )\r\n", g_struct[structid].m_name, g_struct[structid].m_name );
			fprintf( fp, "{\r\n" );
			fprintf( fp, "	int tmpi = 0;\r\n");
			fprintf( fp, "\r\n");
			for( tmpj = 0; tmpj < g_struct[structid].m_count; tmpj++ )
			{
				if( struct_getdatatype( g_struct[structid].m_sconf[tmpj].data_type, str, NULL ) == NULL )
				{
					fclose( fp );
					return;
				}
				if( g_struct[structid].m_sconf[tmpj].data_type >= 0 )
				{
					substructid = g_struct[structid].m_sconf[tmpj].data_type;
					if( g_struct[substructid].m_name[0] )
					{
						if( g_struct[structid].m_sconf[tmpj].data_count > 1 )
						{
							fprintf( fp, "	for( tmpi = 0; tmpi < %d; tmpi++ )\r\n	{\r\n", g_struct[structid].m_sconf[tmpj].data_count );
							fprintf( fp, "		struct_%s( pptr, psize, &pValue->m_%s[tmpi] );\r\n	}\r\n", g_struct[substructid].m_name, g_struct[structid].m_sconf[tmpj].m_name );
						}
						else if( g_struct[structid].m_sconf[tmpj].data_count == -1 )
						{
							if( tmpj > 0 )
							{
								fprintf( fp, "	for( tmpi = 0; tmpi < pValue->m_%s; tmpi++ )\r\n	{\r\n", g_struct[structid].m_sconf[tmpj-1].m_name );
								fprintf( fp, "		struct_%s( pptr, psize, &pValue->m_%s[tmpi] );\r\n	}\r\n", g_struct[substructid].m_name, g_struct[structid].m_sconf[tmpj].m_name );
							}
						}
						else
						{
							fprintf( fp, "	struct_%s( pptr, psize, &pValue->m_%s );\r\n", g_struct[substructid].m_name, g_struct[structid].m_sconf[tmpj].m_name );
						}
					}
				}
				else
				{
					if( g_struct[structid].m_sconf[tmpj].data_count > 1 )
					{
						fprintf( fp, "	memcpy( (*pptr), pValue->m_%s, %d*sizeof(%s) );	(*pptr)+=%d*sizeof(%s); (*psize)+=%d*sizeof(%s);\r\n", g_struct[structid].m_sconf[tmpj].m_name, g_struct[structid].m_sconf[tmpj].data_count, str, g_struct[structid].m_sconf[tmpj].data_count, str, g_struct[structid].m_sconf[tmpj].data_count, str );
					}
					else if( g_struct[structid].m_sconf[tmpj].data_count == -1 )
					{
						if( tmpj > 0 )
							fprintf( fp, "	memcpy( (*pptr), pValue->m_%s, pValue->m_%s*sizeof(%s) );	(*pptr)+=pValue->m_%s*sizeof(%s); (*psize)+=pValue->m_%s*sizeof(%s);\r\n", g_struct[structid].m_sconf[tmpj].m_name, g_struct[structid].m_sconf[tmpj-1].m_name, str, g_struct[structid].m_sconf[tmpj-1].m_name, str, g_struct[structid].m_sconf[tmpj-1].m_name, str );
					}
					else
					{
						fprintf( fp, "	*(%s *)(*pptr) = pValue->m_%s; (*pptr)+=sizeof(%s); (*psize)+=sizeof(%s);\r\n", str, g_struct[structid].m_sconf[tmpj].m_name, str, str );
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

void struct_create_netsend()
{
	int tmpi;
	FILE *fp;
	int structid;
	char str[256];
	char szFile[256];
	char szPath[256];

	int type = 0;
	strcpy( szPath, CLIENT_CREATE_PATH );

	sprintf( szFile, "%s/include/client_netsend_auto.h", szPath );
	fp = fopen( szFile, "wb" );
	if( fp == NULL )
		return;

	fprintf( fp, "#ifndef __NETSEND_AUTO_H\r\n" );
	fprintf( fp, "#define __NETSEND_AUTO_H\r\n" );
	fprintf( fp, "\r\n" );
	fprintf( fp, "#include \"client_structsend_auto.h\"\r\n" );
	for( tmpi = 0; tmpi < g_netscmd_count; tmpi++ )
	{
		structid = g_netscmd[tmpi].m_structid;
		if( g_struct[structid].m_name[0] )
		{
			fprintf( fp, "int netsend_%s_C( int actor_index, char send_type, SLK_%s *pValue );\r\n", g_netscmd[tmpi].m_cmdstr, g_struct[structid].m_name );
		}
	}
	fprintf( fp, "\r\n#endif\r\n" );
	fclose( fp );

	sprintf( szFile, "%s/src/client_netsend_auto.c", szPath );
	fp = fopen( szFile, "wb" );
	if( fp == NULL )
		return;

	fprintf( fp, "#include \"client_struct_auto.h\"\r\n" );
	fprintf( fp, "#include \"client_netsend_auto.h\"\r\n" );
	fprintf( fp, "#include \"actor_send.h\"\r\n" );

	if( type == 0 )
		fprintf( fp, "\r\n\r\nextern int g_maxactornum;\r\n" );

	fprintf( fp, "\r\n");

	for( tmpi = 0; tmpi < g_netscmd_count; tmpi++ )
	{
		structid = g_netscmd[tmpi].m_structid;
		if( g_struct[structid].m_name[0] )
		{
			if( type == 0 )
			{
				fprintf( fp, "int netsend_%s_C( int actor_index, char send_type, SLK_%s *pValue )\r\n", g_netscmd[tmpi].m_cmdstr, g_struct[structid].m_name );
				fprintf( fp, "{\r\n" );
				fprintf( fp, "	char tmpbuf[2048];\r\n");
				fprintf( fp, "	int tmpsize;\r\n");
				fprintf( fp, "	char *ptr, *ptrsize;\r\n");
				fprintf( fp, "	char *ptrsubdata;\r\n");
				fprintf( fp, "\r\n");
				fprintf( fp, "	if( actor_index < 0 || actor_index >= g_maxactornum )\r\n");
				fprintf( fp, "		return -1;\r\n");
				fprintf( fp, "\r\n");
				fprintf( fp, "	ptr = tmpbuf;\r\n");
				fprintf( fp, "	tmpsize = 0;\r\n");
				fprintf( fp, "	ptr+=sizeof(short);\r\n");
				fprintf( fp, "	ptrsubdata = ptr;\r\n");

				strcpy( str, g_netscmd[tmpi].m_cmdstr );
				u_str_toupper( str );

				fprintf( fp, "	*(short *)ptr = CMDS_%s; ptr+=sizeof(short); tmpsize+=sizeof(short);\r\n", str );
				fprintf( fp, "	ptrsize = ptr;	ptr+=sizeof(short);tmpsize+=sizeof(short);\r\n");
				fprintf( fp, "\r\n");

				fprintf( fp, "	struct_%s( &ptr, &tmpsize, pValue );\r\n\r\n", g_struct[structid].m_name );

				fprintf( fp, "	*(short *)ptrsize = tmpsize - (int)sizeof(short)*2;\r\n");
				fprintf( fp, "	*(unsigned short *)tmpbuf = tmpsize;\r\n");
				fprintf( fp, "\r\n");
				fprintf( fp, "	actor_senddata( actor_index, send_type, tmpbuf, tmpsize );\r\n");
				fprintf( fp, "	return 0;\r\n");
				fprintf( fp, "}\r\n" );
				fprintf( fp, "\r\n" );
			}
			else
			{
				fprintf( fp, "int netsend_%s_C( SLK_%s *pValue )\r\n", g_netscmd[tmpi].m_cmdstr, g_struct[structid].m_name );
				fprintf( fp, "{\r\n" );
				fprintf( fp, "	char tmpbuf[2048];\r\n");
				fprintf( fp, "	int tmpsize;\r\n");
				fprintf( fp, "	char *ptr;\r\n");
				fprintf( fp, "\r\n");
				fprintf( fp, "	ptr = tmpbuf;\r\n");
				fprintf( fp, "	tmpsize = 0;\r\n");
				fprintf( fp, "\r\n");
				fprintf( fp, "	struct_%s( &ptr, &tmpsize, pValue );\r\n\r\n", g_struct[structid].m_name );

				strcpy( str, g_netscmd[tmpi].m_cmdstr );
				u_str_toupper( str );

				fprintf( fp, "	rawsend( tmpbuf, tmpsize, CMDC_%s );\r\n", str );
				fprintf( fp, "	return 0;\r\n");
				fprintf( fp, "}\r\n" );
				fprintf( fp, "\r\n" );
			}
		}
	}
	fclose( fp );
}

void struct_create_structrecv()
{
	int tmpj;
	char str[256];
	char szFile[256];
	char szPath[256];
	FILE *fp;
	int structid;
	int substructid;

	strcpy( szPath, CLIENT_CREATE_PATH );

	sprintf( szFile, "%s/src/client_structrecv_auto.c", szPath );
	fp = fopen( szFile, "wb" );
	if( fp == NULL )
		return;

	fprintf( fp, "#include <string.h>\r\n" );
	fprintf( fp, "#include \"client_structrecv_auto.h\"\r\n" );
	fprintf( fp, "#include \"actor_send.h\"\r\n" );
	fprintf( fp, "\r\n" );

	for( structid = 0; structid < g_struct_count; structid++ )
	{
		if( g_struct[structid].m_name[0] && g_struct[structid].m_struct_type == 2 )
		{
			fprintf( fp, "int struct_%s( char **pptr, int *psize, SLK_%s *pValue )\r\n", g_struct[structid].m_name, g_struct[structid].m_name );
			fprintf( fp, "{\r\n" );
			fprintf( fp, "	int tmpi = 0;\r\n");
			fprintf( fp, "\r\n");
			for( tmpj = 0; tmpj < g_struct[structid].m_count; tmpj++ )
			{
				if( struct_getdatatype( g_struct[structid].m_sconf[tmpj].data_type, str, NULL ) == NULL )
				{
					fclose( fp );
					return;
				}
				if( g_struct[structid].m_sconf[tmpj].data_type >= 0 )
				{
					substructid = g_struct[structid].m_sconf[tmpj].data_type;
					if( g_struct[substructid].m_name[0] )
					{
						if( g_struct[structid].m_sconf[tmpj].data_count > 1 )
						{
							fprintf( fp, "	for( tmpi = 0; tmpi < %d; tmpi++ )\r\n	{\r\n", g_struct[structid].m_sconf[tmpj].data_count );
							fprintf( fp, "		struct_%s( pptr, psize, &pValue->m_%s[tmpi] );\r\n	}\r\n", g_struct[substructid].m_name, g_struct[structid].m_sconf[tmpj].m_name );
						}
						else if( g_struct[structid].m_sconf[tmpj].data_count == -1 )
						{
							if( tmpj > 0 )
							{
								fprintf( fp, "	for( tmpi = 0; tmpi < pValue->m_%s; tmpi++ )\r\n	{\r\n", g_struct[structid].m_sconf[tmpj-1].m_name );
								fprintf( fp, "		struct_%s( pptr, psize, &pValue->m_%s[tmpi] );\r\n	}\r\n", g_struct[substructid].m_name, g_struct[structid].m_sconf[tmpj].m_name );
							}
						}
						else
						{
							fprintf( fp, "	struct_%s( pptr, psize, &pValue->m_%s );\r\n", g_struct[substructid].m_name, g_struct[structid].m_sconf[tmpj].m_name );
						}
					}
				}
				else
				{
					if( g_struct[structid].m_sconf[tmpj].data_count > 1 )
					{
						fprintf( fp, "	memcpy( pValue->m_%s, (*pptr), %d*sizeof(%s) );	(*pptr)+=%d*sizeof(%s); (*psize)-=%d*sizeof(%s);\r\n", g_struct[structid].m_sconf[tmpj].m_name, g_struct[structid].m_sconf[tmpj].data_count, str, g_struct[structid].m_sconf[tmpj].data_count, str, g_struct[structid].m_sconf[tmpj].data_count, str );
					}
					else if( g_struct[structid].m_sconf[tmpj].data_count == -1 )
					{
						if( tmpj > 0 )
							fprintf( fp, "	memcpy( pValue->m_%s, (*pptr), pValue->m_%s*sizeof(%s) );	(*pptr)+=pValue->m_%s*sizeof(%s); (*psize)-=pValue->m_%s*sizeof(%s);\r\n", g_struct[structid].m_sconf[tmpj].m_name, g_struct[structid].m_sconf[tmpj-1].m_name, str, g_struct[structid].m_sconf[tmpj-1].m_name, str, g_struct[structid].m_sconf[tmpj-1].m_name, str );
					}
					else
					{
						fprintf( fp, "	pValue->m_%s = *(%s *)(*pptr); (*pptr)+=sizeof(%s); (*psize)-=sizeof(%s);\r\n", g_struct[structid].m_sconf[tmpj].m_name, str, str, str );
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

void struct_create_netrecv()
{
	int tmpi;
	FILE *fp;
	int structid;
	char str[256];
	char szFile[256];
	char szPath[256];

	strcpy( szPath, CLIENT_CREATE_PATH );

	sprintf( szFile, "%s/include/client_netrecv_auto.h", szPath );
	fp = fopen( szFile, "wb" );
	if( fp == NULL )
		return;

	fprintf( fp, "#ifndef __NETRECV_AUTO_H\r\n" );
	fprintf( fp, "#define __NETRECV_AUTO_H\r\n" );
	fprintf( fp, "\r\n" );
	fprintf( fp, "#include \"client_structrecv_auto.h\"\r\n" );

	for( tmpi = 0; tmpi < g_netccmd_count; tmpi++ )
	{
		structid = g_netccmd[tmpi].m_structid;
		if( g_struct[structid].m_name[0] )
		{
			fprintf( fp, "int netrecv_%s( int client_index, char *data, int size );\r\n", g_netccmd[tmpi].m_cmdstr );
		}
	}

	fprintf( fp, "\r\n" );

	for( tmpi = 0; tmpi < g_netccmd_count; tmpi++ )
	{
		structid = g_netccmd[tmpi].m_structid;
		if( g_struct[structid].m_name[0] && g_netccmd[tmpi].m_target_type == 1 )
		{
			fprintf( fp, "int netrecv_wqueue_%s( int client_index, char *data, int size, int exec_code );\r\n", g_netccmd[tmpi].m_cmdstr );
		}
	}

	fprintf( fp, "\r\n#endif\r\n" );
	fclose( fp );

	sprintf( szFile, "%s/src/client_netrecv_auto.c", szPath );
	fp = fopen( szFile, "wb" );
	if( fp == NULL )
		return;

	fprintf( fp, "#include \"client_struct_auto.h\"\r\n" );
	fprintf( fp, "#include \"client_process_auto.h\"\r\n" );
	fprintf( fp, "#include \"client_netrecv_auto.h\"\r\n" );
	fprintf( fp, "#include \"actor_send.h\"\r\n" );
	fprintf( fp, "#include \"client.h\"\r\n" );
	fprintf( fp, "#include \"wqueue.h\"\r\n" );
	fprintf( fp, "#include \"utils.h\"\r\n" );
	fprintf( fp, "extern int g_mysql_wait;\r\n" );
	fprintf( fp, "\r\n" );

	for( tmpi = 0; tmpi < g_netccmd_count; tmpi++ )
	{
		structid = g_netccmd[tmpi].m_structid;
		if( g_struct[structid].m_name[0] )
		{
			fprintf( fp, "int netrecv_%s( int client_index, char *data, int size )\r\n", g_netccmd[tmpi].m_cmdstr );
			fprintf( fp, "{\r\n" );
			fprintf( fp, "	SLK_%s Value;\r\n", g_struct[structid].m_name );
			fprintf( fp, "	int tmpsize = size;\r\n");
			fprintf( fp, "	char *ptr = data;\r\n");
			fprintf( fp, "\r\n");

			fprintf( fp, "	struct_%s( &ptr, &tmpsize, &Value );\r\n\r\n", g_struct[structid].m_name );
			fprintf( fp, "	proc_%s_C( client_index, &Value );\r\n", g_netccmd[tmpi].m_cmdstr );

			fprintf( fp, "\r\n");
			fprintf( fp, "	return 0;\r\n" );
			fprintf( fp, "}\r\n" );
			fprintf( fp, "\r\n" );
		}
	}

	for( tmpi = 0; tmpi < g_netccmd_count; tmpi++ )
	{
		structid = g_netccmd[tmpi].m_structid;
		if( g_struct[structid].m_name[0] && g_netccmd[tmpi].m_target_type == 1 )
		{
			strcpy( str, g_netccmd[tmpi].m_cmdstr );
			u_str_toupper( str );

			fprintf( fp, "int netrecv_wqueue_%s( int client_index, char *data, int size, int exec_code )\r\n", g_netccmd[tmpi].m_cmdstr );
			fprintf( fp, "{\r\n" );
			fprintf( fp, "	client_setwait( client_index, 1 );\r\n" );
			fprintf( fp, "	if( g_mysql_wait )\r\n" );
			fprintf( fp, "	{\r\n" );
			fprintf( fp, "		if( add_wqueue( client_index, WCMD_%s, data, size, exec_code, 0 ) < 0 )\r\n", str );
			fprintf( fp, "		{\r\n" );
			fprintf( fp, "			write_gamelog( \"add_wqueue full\" );\r\n" );
			fprintf( fp, "			net_closesock_direct( client_index );\r\n" );
			fprintf( fp, "			return -1;\r\n" );
			fprintf( fp, "		}\r\n" );
			fprintf( fp, "		return 0;\r\n" );
			fprintf( fp, "	}\r\n" );
			fprintf( fp, "	else\r\n" );
			fprintf( fp, "	{\r\n" );
			fprintf( fp, "		return netrecv_%s( client_index, data, size );\r\n", g_netccmd[tmpi].m_cmdstr );
			fprintf( fp, "	}\r\n" );
			fprintf( fp, "}\r\n" );
			fprintf( fp, "\r\n" );
		}
	}

	fprintf( fp, "int proc_command_C( int client_index, short cmd, char *ptr, int tmpsize, int exec_code )\r\n" );
	fprintf( fp, "{\r\n" );
	fprintf( fp, "	switch( cmd )\r\n" );
	fprintf( fp, "	{\r\n" );
	for( tmpi = 0; tmpi < g_netccmd_count; tmpi++ )
	{
		structid = g_netccmd[tmpi].m_structid;
		if( g_struct[structid].m_name[0] )
		{
			strcpy( str, g_netccmd[tmpi].m_cmdstr );
			u_str_toupper( str );

			fprintf( fp, "	case CMDC_%s:\r\n", str );
			if( g_netccmd[tmpi].m_target_type == 0 )
				fprintf( fp, "		netrecv_%s( client_index, ptr, tmpsize );\r\n", g_netccmd[tmpi].m_cmdstr );
			else if( g_netccmd[tmpi].m_target_type == 1 )
				fprintf( fp, "		netrecv_wqueue_%s( client_index, ptr, tmpsize, exec_code );\r\n", g_netccmd[tmpi].m_cmdstr );
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

void struct_create_process()
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

	strcpy( szPath, CLIENT_CREATE_PATH );

	sprintf( szFile, "%s/include/client_process_auto.h", szPath );
	fp = fopen( szFile, "wb" );
	if( fp == NULL )
		return;

	fprintf( fp, "#ifndef __PROCESS_AUTO_H\r\n" );
	fprintf( fp, "#define __PROCESS_AUTO_H\r\n" );
	fprintf( fp, "#include \"client_structrecv_auto.h\"\r\n" );
	fprintf( fp, "\r\n" );
	fprintf( fp, "int proc_command_C( int client_index, short cmd, char *ptr, int tmpsize, int exec_code );\r\n" );

	for( tmpi = 0; tmpi < g_netccmd_count; tmpi++ )
	{
		structid = g_netccmd[tmpi].m_structid;
		if( g_struct[structid].m_name[0] )
		{
			fprintf( fp, "void proc_%s_C( int client_index, SLK_%s *pValue );\r\n", g_netccmd[tmpi].m_cmdstr, g_struct[structid].m_name );
		}
	}
	fprintf( fp, "\r\n#endif\r\n" );
	fclose( fp );

	sprintf( szFile, "%s/src/client_process_auto.c", szPath );
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
		fprintf( fp, "#include \"client_process_auto.h\"\r\n" );
		fprintf( fp, "#include \"actor_send.h\"\r\n" );
		fprintf( fp, "\r\n" );
	}

	for( tmpi = 0; tmpi < g_netccmd_count; tmpi++ )
	{
		structid = g_netccmd[tmpi].m_structid;
		if( g_struct[structid].m_name[0] )
		{
			has = 0;
			if( linebuf )
			{
				sprintf( tmpstr, "void proc_%s_C(", g_netccmd[tmpi].m_cmdstr );
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

			fprintf( fp, "void proc_%s_C( int client_index, SLK_%s *pValue )\r\n", g_netccmd[tmpi].m_cmdstr, g_struct[structid].m_name );
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
