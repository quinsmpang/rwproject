#ifdef WIN32
#include <winsock2.h>
#include <mswsock.h>
#endif
#include <stdio.h>
#include <string.h>

#include <mysql.h>
#include "struct_config.h"
#include "utils.h"

extern MYSQL *myData;

void DeleteTabCopy( char *szTmpStr, const char *prow )
{
	if( prow == NULL )
	{
		szTmpStr[0] = 0;
		return;
	}
	int len = strlen( prow );
	int tmpi;
	int tmpj = 0;
	for( tmpi = 0; tmpi <= len; tmpi++ )
	{
		if( prow[tmpi] != '\t' )
		{
			szTmpStr[tmpj] = prow[tmpi];
			tmpj++;
		}
	}
}

int tabcreate()
{
	MYSQL_RES		*res;
	MYSQL_ROW		row;
	char	szSQL[1024];
	char szTmpStr[2048];

	char tab_new[MAX_TABCOUNT][256];
	int tabcount_new = 0;
	int tmpi_new = 0;

	FInfo field_new[MAX_FIELDCOUNT];
	int fcount_new = 0;

	char szFile[MAX_PATH];
	FILE *fp;

	sprintf( szSQL, "show tables" );
	if( mysql_query( myData, szSQL ) )
	{
		printf( "Query failed (%s)\n", mysql_error(myData) );
		return -1;
	}
	res = mysql_store_result( myData );
	while( row = mysql_fetch_row( res ) )
	{
		strcpy( tab_new[tabcount_new], row[0] );
		tabcount_new++;
		if( tabcount_new >= MAX_TABCOUNT )
		{
			mysql_free_result( res );
			return -1;
		}
	}
	mysql_free_result( res );

	for( tmpi_new = 0; tmpi_new < tabcount_new; tmpi_new++ )
	{
		sprintf( szFile, "%s/%s.txt", SCUI_CREATE_PATH, tab_new[tmpi_new] );
		fp = fopen( szFile, "wb" );
		fcount_new = 0;
		sprintf( szSQL, "desc %s", tab_new[tmpi_new] );
		if( mysql_query( myData, szSQL ) )
		{
			printf( "Query failed (%s)\n", mysql_error(myData) );
			fclose( fp );
			return -1;
		}
		res = mysql_store_result( myData );
		while( row = mysql_fetch_row( res ) )
		{
			strncpy( field_new[fcount_new].m_field, row[0], 32 );
			strncpy( field_new[fcount_new].m_type, row[1], 32 );
			strncpy( field_new[fcount_new].m_null, row[2], 8 );
			strncpy( field_new[fcount_new].m_key, row[3], 8 );
			if( row[4] == NULL )
			{
				field_new[fcount_new].m_hasdef = 0;
			}
			else
			{
				field_new[fcount_new].m_hasdef = 1;
				strncpy( field_new[fcount_new].m_default, row[4], 24 );
			}
			strncpy( field_new[fcount_new].m_extra, row[5], 24 );
			fcount_new++;
			if( fcount_new >= MAX_FIELDCOUNT )
			{
				mysql_free_result( res );
				fclose( fp );
				return -1;
			}
		}
		mysql_free_result( res );
		for( int tmpi = 0; tmpi < fcount_new; tmpi++ )
		{
			if( tmpi == 0 )
				fprintf( fp, "%s", field_new[tmpi].m_field );
			else
				fprintf( fp, "\t%s", field_new[tmpi].m_field );
		}
		fprintf( fp, "\r\n" );
		sprintf( szSQL, "select * from %s order by `%s`", tab_new[tmpi_new], field_new[0].m_field );
		if( mysql_query( myData, szSQL ) )
		{
			printf( "Query failed (%s)\n", mysql_error(myData) );
			fclose( fp );
			return -1;
		}
		res = mysql_store_result( myData );
		while( row = mysql_fetch_row( res ) )
		{
			for( int tmpi = 0; tmpi < fcount_new; tmpi++ )
			{
				DeleteTabCopy( szTmpStr, row[tmpi] );
				if( tmpi == 0 )
				{
					if( row[tmpi] )
						fprintf( fp, "%s", szTmpStr );
				}
				else
				{
					if( row[tmpi] )
						fprintf( fp, "\t%s", szTmpStr );
					else
						fprintf( fp, "\t" );
				}
			}
			fprintf( fp, "\r\n" );
		}
		mysql_free_result( res );
		fclose( fp );
	}

	return 0;
}
