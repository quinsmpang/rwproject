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
	printf( "------------------------------ 竞技场机器人生成器 ------------------------------" );
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

	printf( "正在连接数据库\n" );
	if ( db_init( host, user, pass, database ) < 0 )
	{
		printf( "数据库连接失败\n" );
		return 0;
	}
	
	/* 检查数据库里是否有数据，有就清空在创建 */
	if ( match_check() > 0 ) 
	{
		printf( "检测到竞技场已经有数据是否清空（y/n）?" );
		scanf( "%s", &clear );
		if ( strcmp( clear, "y" ) == 0 )
		{
			match_delete( database, "match_list" );
			match_delete( database, "match_attr" );
			match_delete( database, "match_item" );
			match_delete( database, "match_skill" );
			match_delete( database, "match_partner" );
			printf( "清空成功\n\n" );
		}
	}
	
	printf( "请输入要生成机器人数量:" );
	scanf( "%d", &playercount );
	printf( "\n" );
	/* 创建 */
	printf( "生成开始\n" );
	match_create( playercount );
	printf( "生成完毕\n" );
	db_closedata();
	return 0;
}