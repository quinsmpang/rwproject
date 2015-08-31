#include <stdio.h>
#include "db.h"
#include "match.h"
#include "randname.h"

int main( int argc, char *argv[] )
{
	char host[64]={0};
	char user[64]={0};
	char pass[64]={0};
	char database[64]={0};
	char clear[16] = {0};
	int playercount = 0;
	srand( (unsigned)time( NULL ) );
	randname_init();
	printf( "------------------------------ ������������������ ------------------------------" );
	printf( "host:" );
	scanf( "%s", &host );
	printf( "\n" );

	printf( "user:" );
	scanf( "%s", &user );
	printf( "\n" );

	printf( "password:" );
	scanf( "%s", &pass );
	printf( "\n" );

	printf( "database:" );
	scanf( "%s", &database );
	printf( "\n" );

	printf( "�����������ݿ�\n" );
	if ( db_init( host, user, pass, database ) < 0 )
	{
		printf( "���ݿ�����ʧ��\n" );
		return 0;
	}
	
	/* ������ݿ����Ƿ������ݣ��о�����ڴ��� */
	if ( match_check() > 0 ) 
	{
		printf( "��⵽�������Ѿ��������Ƿ���գ�y/n��?" );
		scanf( "%s", &clear );
		if ( strcmp( clear, "y" ) == 0 )
		{
			match_delete( database, "match_list" );
			match_delete( database, "match_attr" );
			match_delete( database, "match_item" );
			match_delete( database, "match_skill" );
			match_delete( database, "match_partner" );
			printf( "��ճɹ�\n\n" );
		}
	}
	
	printf( "������Ҫ���ɻ���������:" );
	scanf( "%d", &playercount );
	printf( "\n" );
	/* ���� */
	printf( "���ɿ�ʼ\n" );
	match_create( playercount );
	printf( "�������\n" );
	db_closedata();
	return 0;
}