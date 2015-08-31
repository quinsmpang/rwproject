#ifdef WIN32
#include <winsock2.h>
#include <windows.h>
#else
#include<signal.h>
#include<syslog.h>
#include<unistd.h>
#include<fcntl.h>
#include<time.h>
#include<errno.h>
#include<string.h>
#include<sys/stat.h>
#include<sys/file.h>
#include<sys/ioctl.h>
#include<stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#endif
#include "netserver.h"

#include "mytimer.h"
#include "netfunc.h"
#include "gameproc.h"
#include "utils.h"
#include "define.h"
#include "server_netsend_auto.h"

const char *GetRootPath();

extern SConfig g_Config;
int fps_limit = 30;

char g_szWorkPath[MAX_PATH];

// 设置当前服务器状态，用于鉴定当前服务器状态
int serv_setstat( int stat )
{
	char szStatFile[MAX_PATH];
	sprintf( szStatFile, "%s/.stat", GetRootPath() );

	FILE *fp;
	fp = fopen( szStatFile, "wb" );
	if( fp == NULL )
		return -1;

	fprintf( fp, "%d\r\n", stat );
	fclose( fp );
	return 0;
}

#ifndef WIN32
#define LOCKFILE "nj.pid"
#define LOCKMODE (S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH)

int lockfile( int ff )
{
	struct flock fl;
	fl.l_type = F_WRLCK;
	fl.l_start = 0;
	fl.l_whence = SEEK_SET;
	fl.l_len = 0;
	return ( fcntl(ff,F_SETLK,&fl) );
}

void already_running()
{
	int fd;
	char buf[16];
	char szLockFile[MAX_PATH];

	sprintf(szLockFile, "%s/%s", GetRootPath(), LOCKFILE );

	fd = open( szLockFile,O_RDWR|O_CREAT,LOCKMODE );
	if( fd < 0 )
	{
		printf( "already running.[1]\n" );
		exit(1);
	}

	if( lockfile(fd) < 0 )
	{
		if( errno == EACCES || errno == EAGAIN )
		{
			close( fd );
			printf( "already running.[2]\n" );
			exit(1);
		}
		printf( "already running.[3]\n" );
		exit(1);
	}
	ftruncate( fd, 0 );

	sprintf( buf,"%d", getpid() );
	write( fd, buf, strlen(buf) );
}

int getmypid()
{
	int fd;
	int size;
	char buf[16];
	char szLockFile[MAX_PATH];

	sprintf(szLockFile, "%s/%s", GetRootPath(), LOCKFILE );

	fd = open( szLockFile,O_RDWR|O_CREAT,LOCKMODE );
	if( fd < 0 )
		return -1;

	if( lockfile(fd) < 0 )
	{
		if( errno == EACCES || errno == EAGAIN )
		{
			size = read( fd, buf, 16 );
			buf[size] = 0;
			close(fd);
			return atoi(buf);
		}
		size = read( fd, buf, 16 );
		buf[size] = 0;
		close(fd);
		return atoi(buf);
	}
	return -1;
}

#endif

int configprocess( const char *pSign, const char *pValue )
{
	if( strcmp( pSign, "SERVER_HOST" ) == 0 )
	{
		g_Config.ipaddress = inet_addr( pValue );
	}
	else if( strcmp( pSign, "SERVER_PORT" ) == 0 )
	{
		g_Config.port = atoi(pValue);
	}
	else if( strcmp( pSign, "USERSRV_HOST" ) == 0 )
	{
		strcpy( g_Config.user_host, pValue );
		g_Config.user_ipaddress = inet_addr( pValue );
	}
	else if( strcmp( pSign, "USERSRV_PORT" ) == 0 )
	{
		g_Config.user_port = atoi(pValue);
	}
	else if( strcmp( pSign, "DSRV_HOST" ) == 0 )
	{
		g_Config.ds_ipaddress = inet_addr( pValue );
	}
	else if( strcmp( pSign, "DSRV_PORT" ) == 0 )
	{
		g_Config.ds_port = atoi(pValue);
	}
	else if( strcmp( pSign, "DATADB_HOST" ) == 0 )
	{
		strcpy( g_Config.datadb_host, pValue );
	}
	else if( strcmp( pSign, "DATADB_USER" ) == 0 )
	{
		strcpy( g_Config.datadb_user, pValue );
	}
	else if( strcmp( pSign, "DATADB_PASS" ) == 0 )
	{
		strcpy( g_Config.datadb_pass, pValue );
	}
	else if( strcmp( pSign, "DATADB_DATABASE" ) == 0 )
	{
		strcpy( g_Config.datadb_database, pValue );
	}
	else if( strcmp( pSign, "GAMEDB_HOST" ) == 0 )
	{
		strcpy( g_Config.gamedb_host, pValue );
	}
	else if( strcmp( pSign, "GAMEDB_USER" ) == 0 )
	{
		strcpy( g_Config.gamedb_user, pValue );
	}
	else if( strcmp( pSign, "GAMEDB_PASS" ) == 0 )
	{
		strcpy( g_Config.gamedb_pass, pValue );
	}
	else if( strcmp( pSign, "GAMEDB_DATABASE" ) == 0 )
	{
		strcpy( g_Config.gamedb_database, pValue );
	}
	else if( strcmp( pSign, "LOGDB_HOST" ) == 0 )
	{
		strcpy( g_Config.logdb_host, pValue );
	}
	else if( strcmp( pSign, "LOGDB_USER" ) == 0 )
	{
		strcpy( g_Config.logdb_user, pValue );
	}
	else if( strcmp( pSign, "LOGDB_PASS" ) == 0 )
	{
		strcpy( g_Config.logdb_pass, pValue );
	}
	else if( strcmp( pSign, "LOGDB_DATABASE" ) == 0 )
	{
		strcpy( g_Config.logdb_database, pValue );
	}
	else if( strcmp( pSign, "MAX_CONNECTION" ) == 0 )
	{
#ifdef WIN32
		g_Config.max_connection = atoi(pValue);
#else
		g_Config.max_connection = atoi(pValue);
#endif
	}
	else if ( strcmp( pSign, "MAX_CITYCOUNT" ) == 0 )
	{
		g_Config.max_citycount = atoi( pValue );
	}
	else if ( strcmp( pSign, "MAX_CLUBCOUNT" ) == 0 )
	{
		g_Config.max_clubcount = atoi( pValue );
	}
	else if( strcmp( pSign, "MIN_VER1" ) == 0 )
	{
		g_Config.minver.ss.m1 = atoi(pValue);
	}
	else if( strcmp( pSign, "MIN_VER2" ) == 0 )
	{
		g_Config.minver.ss.m2 = atoi(pValue);
	}
	else if( strcmp( pSign, "MIN_VER3" ) == 0 )
	{
		g_Config.minver.ss.m3 = atoi(pValue);
	}
	else if( strcmp( pSign, "AREA_CODE" ) == 0 )
	{
		g_Config.area_code = atoi(pValue);
	}
	else if( strcmp( pSign, "SERVER_CODE" ) == 0 )
	{
		g_Config.server_code = atoi(pValue);
	}
	else if( strcmp( pSign, "SERVER_NAME" ) == 0 )
	{
		strcpy( g_Config.server_name, pValue );
	}
	else if ( strcmp( pSign, "SERVER_START_DATE" ) == 0 )
	{
		g_Config.server_start_date = atoi( pValue );
	}
	else if ( strcmp( pSign, "SERVER_EXIT_WEEK" ) == 0 )
	{
		g_Config.server_exit_week = atoi( pValue );
	}
	else if ( strcmp( pSign, "SERVER_EXIT_HOUR" ) == 0 )
	{
		g_Config.server_exit_hour = atoi( pValue );
	}
	else if ( strcmp( pSign, "SERVER_EXIT_MIN" ) == 0 )
	{
		g_Config.server_exit_min = atoi( pValue );
	}
	else if ( strcmp( pSign, "SERVER_PLAT" ) == 0 )
	{
		g_Config.server_plat = atoi( pValue );
	}
	else if ( strcmp( pSign, "NOPAY" ) == 0 )
	{
		g_Config.nopay = atoi( pValue );
	}

	return 0;
}

// 计时器发送指令
void target_incrementor()
{
	char ch = 0;

	netsend_timer_S( -2, 0, &ch );
}

// 这里截获崩溃吧
void actors_on_core(); // 定义在actor.h
void sig_process( int signo )
{
#ifndef WIN32
	// 先恢复信号为缺省，免得死循环进入了
    signal( SIGSEGV, SIG_DFL );
    signal( SIGBUS, SIG_DFL );
    signal( SIGFPE, SIG_DFL );
	signal( SIGABRT, SIG_DFL );
    printf("System core signal:%d\n",signo);
#endif
	actors_on_core();
}

int main( int argc, char *argv[] )
{
	char isoutput = 0;
	const char *rootpath;
#ifndef WIN32
	int childpid;//, fd, fdtablesize;
//	int fp;
	int waittimes;
	chdir( GetRootPath() );
#else
	SetCurrentDirectory( GetRootPath() );
#endif
	rootpath = GetRootPath();
	if( rootpath )
		strcpy( g_szWorkPath, rootpath );
	else
		g_szWorkPath[0] = 0;

	if( argc > 1 )
	{
		if( strncmp( argv[1], "-o", 2 ) == 0 )
			isoutput = 1;
#ifndef WIN32
		else if( strncmp( argv[1], "start", 5 ) == 0 )
		{
			// 开始启动
			serv_setstat( 3 );

			signal(SIGTTOU,SIG_IGN);
			signal(SIGTTIN,SIG_IGN);
			signal(SIGTSTP,SIG_IGN);
			signal(SIGHUP,SIG_IGN);

			if( fork() != 0 )
			{
				sleep(8);
				exit(1);
			}

			already_running();
		}
		else if( strncmp( argv[1], "stop", 4 ) == 0 )
		{
			// 开始关闭
			serv_setstat( 4 );

			childpid = getmypid();
			if( childpid > 0 )
			{
				waittimes = 10;
				kill( childpid, SIGHUP );
				while( waittimes > 0 )
				{
					sleep(1);
					childpid = getmypid();
					if( childpid <= 0 )
					{
						printf( "Server Stop.\n" );
						// 已经关闭
						serv_setstat( 2 );
						return 0;
					}
				}
				printf( "Server Cannot Stop.\n" );
				// 关闭失败
				serv_setstat( 6 );
			}
			else
			{
				// 已经关闭
				serv_setstat( 2 );
				printf( "no run pid:%d\n", -1 );
				return 0;
			}
			return -1;
		}
#endif

	}
#ifdef WIN32
	g_Config.max_connection = 512;
	// 开始启动
	serv_setstat( 3 );
#else
	g_Config.max_connection = 512;
	// 设置退出方式
	signal( SIGSEGV, sig_process );
	signal( SIGBUS, sig_process );
	signal( SIGFPE, sig_process );
	signal( SIGABRT, sig_process );
#endif

	g_Config.minver.value = 0;
	memset( g_Config.server_name, 0, sizeof(g_Config.server_name) );
	g_Config.server_exit_week = -1;
	g_Config.server_exit_hour = -1;
	g_Config.server_exit_min = -1;
	g_Config.server_plat = 0;
	g_Config.nopay = 0;
	g_Config.max_citycount = 20000;
	g_Config.max_clubcount = 256;
	u_getcfg( "server.cfg", configprocess );
	u_loginit( LOG_PATH );
	open_netlog();
	open_gamelog();
	
	// 随机种子
	srand( (unsigned int)time(NULL) );

	// 服务进程初始化
	if( process_init( g_Config.max_connection ) < 0 )
	{
		// 启动失败
		serv_setstat( 5 );
#ifdef WIN32
		Sleep( 5000 );
#else
		sleep( 5 );
#endif
		return -1;
	}

	//output_start( isoutput );

	if( server_init( g_Config.ipaddress, g_Config.port, g_Config.max_connection, g_Config.minver.value ) < 0 )
	{
#ifdef WIN32
		Sleep( 3000 );
#else
		sleep( 3 );
#endif
		// 启动失败
		serv_setstat( 5 );
		return -1;
	}
	init_timer();
	append_timer( (void *)target_incrementor, 0, 1193181/fps_limit );

	process_set_exit( g_Config.server_exit_week, g_Config.server_exit_hour, g_Config.server_exit_min, (g_Config.server_exit_week<0?0:1) );

	// 基本启动成功了
	serv_setstat( 1 );
	server_start( process_client, process_onclose, process_error, process_ondestory );

	//output_setmsg( "Now Exitting..." );
	exit_timer();
	printf( "Now Exitting...\n" );
	server_destroy();
	process_close();
	u_logcloseall();
	//output_setmsg( "System Exited." );
#ifdef WIN32
		Sleep( 1000 );
#else
		sleep( 1 );
#endif
	printf( "System Exited.\n" );
	// 已经关闭
	serv_setstat( 2 );
	return 0;
}
