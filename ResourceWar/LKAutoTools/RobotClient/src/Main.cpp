#include "utils.h"
#include "mytimer.h"
#include "Game.h"
#include "Message.h"
#include "Command.h"
#include "NetClient.h"

extern int g_enter_num;

extern SLK_Player g_Player[1000];
extern int g_nClientNum;

// ͬ�����Ʊ���
volatile int target_cycle;
volatile int actual_cycle;

#ifdef WIN32
HANDLE g_timer_event = NULL;
#else
sem_t g_timer_event;
#endif

int fps = 0;
int fps_count=0;
int count = 0;
int fps_limit = 1;
int wait_count = 0;
int fps_wait = 0;

int g_argc;
char **g_argv;
char g_szPathRoot[MAX_PATH];

void target_incrementor()
{
	wait_count--;
	if( wait_count > 0 )
		return;
	else
		wait_count = fps_wait;
	// ���Ǽ�ʱ���е��õ�,����һ���߳�,ע�����е����ݱ���,�����ͻ
	target_cycle++;
	// �����¼�( ���ź�״̬ )
	SetEvent( g_timer_event );
	count++;
	if( count > fps_limit )
	{
		fps = fps_count;
		fps_count = 0;
		count = 0;
	}
}

//-----------------------------------------------------------------------------
// InitApp
// ����˵��: ��ʼ��Ӧ�ó���
// ����    :  - 
//-----------------------------------------------------------------------------
void InitApp()
{
	init_timer();
	append_timer( (void *)target_incrementor, 0, 1193181/fps_limit );
	actual_cycle = target_cycle = 0;        /* ��ʼ�� */
	System_Init();
	g_timer_event = CreateEvent(NULL, FALSE, FALSE, NULL);
}

void CheckSystemPath()
{
	int tmpi;
	int nLen;
	// ���ϵͳ��ִ��·��
	GetModuleFileName( NULL, g_szPathRoot, MAX_PATH );
	nLen = (int)strlen(g_szPathRoot);

	for( tmpi = nLen-1; tmpi > 0; tmpi-- )
	{
		if( g_szPathRoot[tmpi] == '\\' || g_szPathRoot[tmpi] == '/' || g_szPathRoot[tmpi] == ':' )
		{
			g_szPathRoot[tmpi+1] = 0;
			break;
		}
	}
	SetCurrentDirectory( g_szPathRoot );
}

extern int max_read_list_count;
extern int max_write_list_count;
void clrscr()
{
	HANDLE hConsole;
	COORD coordScreen = { 0, 0 }; /* here's where we'll home the cursor */
	BOOL bSuccess;
	DWORD cCharsWritten;
	CONSOLE_SCREEN_BUFFER_INFO csbi; /* to get buffer info */
	DWORD dwConSize; /* number of character cells in the current buffer */

	hConsole = GetStdHandle( STD_OUTPUT_HANDLE );

	/* get the number of character cells in the current buffer */
	bSuccess = GetConsoleScreenBufferInfo( hConsole, &csbi );
	dwConSize = csbi.dwSize.X * csbi.dwSize.Y;

	/* fill the entire screen with blanks */
	bSuccess = FillConsoleOutputCharacter( hConsole, (TCHAR) ' ', dwConSize, coordScreen, &cCharsWritten );

	/* get the current text attribute */
	bSuccess = GetConsoleScreenBufferInfo( hConsole, &csbi );

	/* now set the buffer's attributes accordingly */
	bSuccess = FillConsoleOutputAttribute( hConsole, csbi.wAttributes, dwConSize, coordScreen, &cCharsWritten );

	/* put the cursor at (0, 0) */
	bSuccess = SetConsoleCursorPosition( hConsole, coordScreen );
	return;
}
int g_send_count = 0;
int g_send_num = 0;
int main( int argc, char *argv[] )
{
	int tmpcount;
	g_argc = argc;
	g_argv = argv;
	
	g_argc = 5;
	g_argv[0] = 0;
	g_argv[1] = "121.199.44.76";
	g_argv[2] = "22221";
	//g_argv[1] = "127.0.0.1";
	//g_argv[2] = "11521";
	g_argv[3] = "200";
	g_argv[4] = "1000";

	CheckSystemPath();
	
	InitApp();

	fps_count = 0;
	while(1)
	{
		//		if( fps_count < 10 )
		//			Sleep(1000);
		WaitForSingleObject( g_timer_event, INFINITE );
		ResetEvent( g_timer_event);
		System_Logic();
		fps_count++;
		clrscr();
		printf("read list:%d write_list:%d -- %d                                      \r",max_read_list_count, max_write_list_count, g_enter_num );
		tmpcount = 0;
		for( int tmpi = 0; tmpi < g_nClientNum; tmpi++ )
		{
			if( tmpi % 25 == 0 )
				printf( "\n" );
			if( g_Player[tmpi].stat < 0 )
				printf( "#%d", g_Player[tmpi].wait_stat );
			else
				printf( "%2d", g_Player[tmpi].stat );
			if( g_Player[tmpi].isconnected )
				tmpcount++;
		}
		printf( "\nConnected:%d\tSendcount:%d(%d)", tmpcount,g_send_count,g_send_num );

		printf( "\n\n" );
		printf( " # �ȴ�ĳ״̬\t" );
		printf( " 0 ��״̬\t" );
		printf( " 1 ս���ȴ���\n" );
		printf( " 2 ��ʼ(δ��¼)\t" );
		printf( " 3 �Ѿ���¼\t" );
		printf( " 4 �Ѿ��н�ɫ\n" );
		printf( " 5 ��Ϸ��ͨ״̬\t" );
		printf( " 6 ��Ϸ����ս��\t" );
		printf( " 7 �ȴ�ս��ָ��\n" );
		printf( " 8 �ѽ�����Ϸ\t" );
		printf( " 9 ������ɫ\n" );
		printf( " 10 �ǳ�\n" );
	}
	return 0;
}

