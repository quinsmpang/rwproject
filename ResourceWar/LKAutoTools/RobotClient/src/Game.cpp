#include <time.h>
#include "utils.h"
#include "Message.h"

#include "Game.h"
#include "Command.h"
#include "NetClient.h"
#include "NetProcess.h"

////////////////////////////////////////
// 是否退出
extern char g_nIsExited;
extern char g_xor[1000];
extern int g_sockset[1000];
// 玩家状态(暂时录像使用)
char g_nGameStat = 0;
// 场景改变记录
char g_bIsChanged = 0;

extern SLK_Player g_Player[1000];

int g_nClientNum = 0;
int g_beginno = 0;
extern int g_argc;
extern char **g_argv;

int g_mode = 0;

// cmd 主机 端口 数量 [起点号码]
void System_Init()
{
	if( g_argc < 4 )
		return;
	if( g_argc >= 5 )
		g_beginno = atoi(g_argv[4]);
	else
		g_beginno = 1;
	if( g_argc >= 6 )
		g_mode = atoi(g_argv[5]);
	srand( (unsigned)time( NULL ) );

	g_nClientNum = atoi(g_argv[3]);

	if( client_message_init() < 0 )
	{
		return;
	}
	client_init();
	for( int tmpi = 0; tmpi < g_nClientNum; tmpi++ )
	{
		g_Player[tmpi].stat = STAT_NONE;
		g_Player[tmpi].isconnected = 0;
		g_Player[tmpi].actorid = -1;
	}
	if( client_start( inet_addr( g_argv[1] ), atoi(g_argv[2]), g_nClientNum ) < 0 )
	{
		return;
	}
//	Net_Login( "UserName", "PassWord" );
}
/*
void randCommand( int tmpi )
{
	int mapid;
	int posx,posy;
	int value = rand()%15;
	if( value > 7 )
	{
		Net_GMCommand( 0, 0, 0, 0, 0, NULL, tmpi );
		return;
	}
	switch( value )
	{
	case 0:
		posx = 40; posy = 50; mapid = 3;
		break;
	case 1:
		posx = 144; posy = 48; mapid = 3;
		break;
	case 2:
		posx = 271; posy = 20; mapid = 3;
		break;
	case 3:
		posx = 53; posy = 118; mapid = 3;
		break;
	case 4:
		posx = 142; posy = 109; mapid = 3;
		break;
	case 5:
		posx = 249; posy = 111; mapid = 3;
		break;
	case 6:
		posx = 56; posy = 60; mapid = 4;
		break;
	case 7:
		posx = 30; posy = 20; mapid = 5;
		break;
	}
	Net_GMCommand( 3, posx, posy, mapid, 0, NULL, tmpi );
}
*/

void randCommand( int tmpi )
{
	int mapid;
	int posx,posy;
	int value = rand()%10;
	switch( value )
	{
	case 0:
		posx = 126; posy = 18; mapid = 10;
		break;
	case 1:
		posx = 110; posy = 25; mapid = 10;
		break;
	case 2:
		posx = 106; posy = 35; mapid = 10;
		break;
	case 3:
		posx = 94; posy = 29; mapid = 10;
		break;
	case 4:
		posx = 108; posy = 29; mapid = 10;
		break;
	case 5:
		posx = 120; posy = 28; mapid = 10;
		break;
	case 6:
		posx = 129; posy = 29; mapid = 10;
		break;
	case 7:
		posx = 134; posy = 34; mapid = 10;
		break;
	case 8:
		posx = 137; posy = 13; mapid = 10;
		break;
	case 9:
		posx = 141; posy = 15; mapid = 10;
		break;
	default:
		posx = 141; posy = 15; mapid = 10;
		break;
	}
	Net_GMCommand( 3, posx, posy, mapid, 0, NULL, tmpi );
}

void System_Logic()
{
	char szName[256];
	char szPass[256];
	SLK_Act thisAct[7];
	client_message_poll();

	for( int tmpi = 0; tmpi < g_nClientNum; tmpi++ )
	{
		if( g_Player[tmpi].isconnected==0 || g_sockset[tmpi] == 0 )
		{
			continue;
		}
		switch( g_Player[tmpi].stat )
		{
		case STAT_NONE:
//			if( rand()%20 == 0 )
				g_Player[tmpi].stat = STAT_BEGIN;
			g_Player[tmpi].wait_times = 0;
			break;
		case STAT_WAIT:
//			Net_HeartBeat( tmpi );
			g_Player[tmpi].wait_times--;
			if( g_Player[tmpi].wait_times < 0 )
			{
				if ( g_Player[tmpi].wait_stat == STAT_LOGIN )
				{
					g_Player[tmpi].stat = STAT_BEGIN;
					break;
				}
				g_Player[tmpi].wait_times = 10;
				net_closeclient( tmpi, 0 );
				g_Player[tmpi].stat = STAT_NONE;
			}
			break;
		case STAT_FWAIT:
			g_Player[tmpi].wait_times--;
			if( g_Player[tmpi].wait_times < 0 )
			{
				g_Player[tmpi].stat = STAT_FIGHTWAIT;
				g_Player[tmpi].wait_times = 0;
			}
			break;
		case STAT_BEGIN:
			g_Player[tmpi].stat = STAT_WAIT;
			g_Player[tmpi].wait_times = 10000;
			sprintf( szName, "robot%d", tmpi+g_beginno );
			strcpy( szPass, "0xffffffff" );
			Net_Login( szName, szPass, tmpi );
			g_Player[tmpi].wait_stat = STAT_LOGIN;
			break;
		case STAT_LOGIN:
			g_Player[tmpi].stat = STAT_WAIT;
			g_Player[tmpi].wait_times = 10000;
			Net_List( tmpi );
			g_Player[tmpi].wait_stat = STAT_LIST;
			break;
		case STAT_LIST:
			g_Player[tmpi].stat = STAT_WAIT;
			g_Player[tmpi].wait_times = 10000;
			if( g_Player[tmpi].actorid < 0 )
			{
				sprintf( szName, "机器人%d", tmpi+g_beginno );
				Net_Create( rand()%2, szName, tmpi );
				g_Player[tmpi].wait_stat = STAT_CREATE;
			}
			else
			{
				Net_EnterGame( g_Player[tmpi].actorid, tmpi );
				g_Player[tmpi].wait_stat = STAT_ENTER;
			}
			break;
		case STAT_ENTER:
			if( g_mode )
			{
				g_Player[tmpi].stat = STAT_STEP1;
				Net_GMCommand( 3, 110, 19, 59, 0, NULL, tmpi );	// 移动到起点
				Net_GMCommand( 12, 1, 0, 0, 0, NULL, tmpi );	// 等级到1级
				Net_GMCommand( 10, 0, 1, 314, 0, NULL, tmpi );	// 任务清零
				g_Player[tmpi].waitframe = rand()%2+1;
			}
			else
			{
				//g_Player[tmpi].stat = STAT_WAIT;
				//g_Player[tmpi].wait_times = 300;
				//g_Player[tmpi].wait_stat = STAT_LOGOUT;
				//g_Player[tmpi].wait_stat = STAT_LOGOUT;
				//g_Player[tmpi].waitframe = 5;
				//randCommand( tmpi );
			}
			break;

		case STAT_NORMAN:
			// 根据当前的坐标移动一个距离, 
			g_Player[tmpi].waitframe--;
			if( g_Player[tmpi].waitframe < 0 )
			{
				short posx,posy;
				g_Player[tmpi].waitframe = rand()%5+3;
//				g_Player[tmpi].waitframe = 2;
				if( g_Player[tmpi].wayx == 0 )
				{
					posx = g_Player[tmpi].posx + rand()%40;
					if( g_Player[tmpi].posx > 100 )
						g_Player[tmpi].wayx = 1;
				}
				else
				{
					posx = g_Player[tmpi].posx + rand()%40;
					if( g_Player[tmpi].posx < 20 )
						g_Player[tmpi].wayx = 0;
				}
				if( g_Player[tmpi].wayy == 0 )
				{
					posy = g_Player[tmpi].posy + rand()%40 ;
					if( g_Player[tmpi].posy > 50 )
						g_Player[tmpi].wayy = 1;
				}
				else
				{
					posy = g_Player[tmpi].posy + rand()%40;
					if( g_Player[tmpi].posy < 20 )
						g_Player[tmpi].wayy = 0;
				}
				Net_Move( posx, posy, tmpi );
				/*if( g_Player[tmpi].waitframe < 5 && rand()%2 == 0 )
					Net_Talk( "我是机器人，应该正好有30个字符", 0, tmpi );*/
			}
			break;
		case STAT_FIGHT:
			if( g_mode )
				Net_GMCommand( 12, -1, 0, 0, 0, NULL, tmpi );
			else
				Net_GMCommand( 12, 30, 0, 0, 0, NULL, tmpi );
			g_Player[tmpi].stat = STAT_FWAIT;
			g_Player[tmpi].wait_times = 4;
			break;
		case STAT_FIGHTWAIT:
			g_Player[tmpi].waitframe--;
			if( g_Player[tmpi].waitframe < 0 )
			{
				memset( thisAct, 0, sizeof(SLK_Act)*7 );
				for( int tmpz = 0; tmpz < 7; tmpz++ )
				{
					thisAct[tmpz].m_nPos = -1;
					thisAct[tmpz].m_nType = -1;
					thisAct[tmpz].m_nValue = 0;
				}
				if( g_Player[tmpi].stat2 == 2 )
				{
					thisAct[0].m_nType = 2;
				}
				g_Player[tmpi].stat = STAT_FWAIT;
				g_Player[tmpi].wait_times = 8;
				Net_Fight( thisAct, tmpi );
			}
			break;
		case STAT_LOGOUT:
			g_Player[tmpi].waitframe--;
			if ( g_Player[tmpi].waitframe < 0 )
			{
				net_closeclient( tmpi, 0 );
				g_Player[tmpi].stat = STAT_NONE;
			}
			break;
		case STAT_STEP1:
			g_Player[tmpi].waitframe--;
			if( g_Player[tmpi].waitframe < 0 )
			{
				Net_Move( 125, 36, tmpi );
				g_Player[tmpi].stat = STAT_STEP2;
				g_Player[tmpi].waitframe = rand()%6+8;
			}
			break;
		case STAT_STEP2:
			g_Player[tmpi].waitframe--;
			if( g_Player[tmpi].waitframe < 0 )
			{
				Net_TouchNpc( 0, 0, -529, tmpi );
				g_Player[tmpi].stat = STAT_STEP3;
				g_Player[tmpi].waitframe = 2;
			}
			break;
		case STAT_STEP3:
			g_Player[tmpi].waitframe--;
			if( g_Player[tmpi].waitframe < 0 )
			{
				Net_TouchNpc( 0, 1, 0, tmpi );
				g_Player[tmpi].stat = STAT_STEP4;
				g_Player[tmpi].waitframe = 2;
			}
			break;
		case STAT_STEP4:
			g_Player[tmpi].waitframe--;
			if( g_Player[tmpi].waitframe < 0 )
			{
				Net_Move( 146, 58, tmpi );
				g_Player[tmpi].stat = STAT_STEP5;
				g_Player[tmpi].waitframe = rand()%6+8;
			}
			break;
		case STAT_STEP5:
			g_Player[tmpi].waitframe--;
			if( g_Player[tmpi].waitframe < 0 )
			{
				Net_Move( 163, 70, tmpi );
				g_Player[tmpi].stat = STAT_STEP6;
				g_Player[tmpi].waitframe = rand()%6+8;
			}
			break;
		case STAT_STEP6:
			g_Player[tmpi].waitframe--;
			if( g_Player[tmpi].waitframe < 0 )
			{
				Net_Move( 157, 91, tmpi );
				g_Player[tmpi].stat = STAT_STEP7;
				g_Player[tmpi].waitframe = rand()%6+8;
			}
			break;
		case STAT_STEP7:
			g_Player[tmpi].waitframe--;
			if( g_Player[tmpi].waitframe < 0 )
			{
				Net_TouchNpc( 0, 0, -530, tmpi );
				g_Player[tmpi].stat = STAT_STEP8;
				g_Player[tmpi].waitframe = 2;
			}
			break;
		case STAT_STEP8:
			g_Player[tmpi].waitframe--;
			if( g_Player[tmpi].waitframe < 0 )
			{
				Net_TouchNpc( 0, 1, 0, tmpi );
				g_Player[tmpi].stat = STAT_STEP9;
				g_Player[tmpi].waitframe = 2;
			}
			break;
		case STAT_STEP9:
			g_Player[tmpi].waitframe--;
			if( g_Player[tmpi].waitframe < 0 )
			{
				Net_Move( 135, 104, tmpi );
				g_Player[tmpi].stat = STAT_STEP10;
				g_Player[tmpi].waitframe = rand()%6+8;
			}
			break;
		case STAT_STEP10:
			g_Player[tmpi].waitframe--;
			if( g_Player[tmpi].waitframe < 0 )
			{
				Net_Move( 109, 103, tmpi );
				g_Player[tmpi].stat = STAT_STEP11;
				g_Player[tmpi].waitframe = rand()%6+8;
			}
			break;
		case STAT_STEP11:
			g_Player[tmpi].waitframe--;
			if( g_Player[tmpi].waitframe < 0 )
			{
				Net_Move( 84, 100, tmpi );
				g_Player[tmpi].stat = STAT_STEP12;
				g_Player[tmpi].waitframe = rand()%6+8;
			}
			break;
		case STAT_STEP12:
			g_Player[tmpi].waitframe--;
			if( g_Player[tmpi].waitframe < 0 )
			{
				Net_TouchNpc( 0, 0, -538, tmpi );
				g_Player[tmpi].stat = STAT_STEP13;
				g_Player[tmpi].waitframe = 2;
			}
			break;
		case STAT_STEP13:
			g_Player[tmpi].waitframe--;
			if( g_Player[tmpi].waitframe < 0 )
			{
				Net_TouchNpc( 0, 1, 0, tmpi );
				g_Player[tmpi].stat = STAT_STEP14;
				g_Player[tmpi].waitframe = 2;
			}
			break;
		case STAT_STEP14:
			g_Player[tmpi].waitframe--;
			if( g_Player[tmpi].waitframe < 0 )
			{
				Net_Move( 63, 98, tmpi );
				g_Player[tmpi].stat = STAT_STEP15;
				g_Player[tmpi].waitframe = rand()%6+8;
			}
			break;
		case STAT_STEP15:
			g_Player[tmpi].waitframe--;
			if( g_Player[tmpi].waitframe < 0 )
			{
				Net_Move( 45, 95, tmpi );
				g_Player[tmpi].stat = STAT_STEP16;
				g_Player[tmpi].waitframe = rand()%6+8;
			}
			break;
		case STAT_STEP16:
			g_Player[tmpi].waitframe--;
			if( g_Player[tmpi].waitframe < 0 )
			{
				Net_TouchNpc( 0, 0, -544, tmpi );
				g_Player[tmpi].stat = STAT_STEP17;
				g_Player[tmpi].waitframe = 2;
			}
			break;
		case STAT_STEP17:
			g_Player[tmpi].waitframe--;
			if( g_Player[tmpi].waitframe < 0 )
			{
				Net_TouchNpc( 0, 1, 0, tmpi );
				g_Player[tmpi].stat2 = 1;
				g_Player[tmpi].waitframe = 2;
			}
			break;
		case STAT_STEP18:
			g_Player[tmpi].waitframe--;
			if( g_Player[tmpi].waitframe < 0 )
			{
				Net_Move( 68, 95, tmpi );
				g_Player[tmpi].stat = STAT_STEP19;
				g_Player[tmpi].waitframe = rand()%6+8;
			}
			break;
		case STAT_STEP19:
			g_Player[tmpi].waitframe--;
			if( g_Player[tmpi].waitframe < 0 )
			{
				Net_Move( 87, 94, tmpi );
				g_Player[tmpi].stat = STAT_STEP20;
				g_Player[tmpi].waitframe = rand()%6+8;
			}
			break;
		case STAT_STEP20:
			g_Player[tmpi].waitframe--;
			if( g_Player[tmpi].waitframe < 0 )
			{
				Net_TouchNpc( 0, 0, -531, tmpi );
				g_Player[tmpi].stat = STAT_STEP21;
				g_Player[tmpi].waitframe = 2;
			}
			break;
		case STAT_STEP21:
			g_Player[tmpi].waitframe--;
			if( g_Player[tmpi].waitframe < 0 )
			{
				Net_TouchNpc( 0, 1, 0, tmpi );
				g_Player[tmpi].stat = STAT_STEP22;
				g_Player[tmpi].waitframe = 2;
			}
			break;
		case STAT_STEP22:
			g_Player[tmpi].waitframe--;
			if( g_Player[tmpi].waitframe < 0 )
			{
				Net_Move( 109, 103, tmpi );
				g_Player[tmpi].stat = STAT_STEP23;
				g_Player[tmpi].waitframe = rand()%6+8;
			}
			break;
		case STAT_STEP23:
			g_Player[tmpi].waitframe--;
			if( g_Player[tmpi].waitframe < 0 )
			{
				Net_Move( 134, 104, tmpi );
				g_Player[tmpi].stat = STAT_STEP24;
				g_Player[tmpi].waitframe = rand()%6+8;
			}
			break;
		case STAT_STEP24:
			g_Player[tmpi].waitframe--;
			if( g_Player[tmpi].waitframe < 0 )
			{
				Net_Move( 163, 97, tmpi );
				g_Player[tmpi].stat = STAT_STEP25;
				g_Player[tmpi].waitframe = rand()%4+10;
			}
			break;
		case STAT_STEP25:
			g_Player[tmpi].waitframe--;
			if( g_Player[tmpi].waitframe < 0 )
			{
				Net_Move( 181, 86, tmpi );
				g_Player[tmpi].stat = STAT_STEP26;
				g_Player[tmpi].waitframe = rand()%4+10;
			}
			break;
		case STAT_STEP26:
			g_Player[tmpi].waitframe--;
			if( g_Player[tmpi].waitframe < 0 )
			{
				Net_TouchNpc( 0, 0, -533, tmpi );
				g_Player[tmpi].stat = STAT_STEP27;
				g_Player[tmpi].waitframe = 2;
			}
			break;
		case STAT_STEP27:
			g_Player[tmpi].waitframe--;
			if( g_Player[tmpi].waitframe < 0 )
			{
				Net_TouchNpc( 0, 1, 0, tmpi );
				g_Player[tmpi].stat = STAT_STEP28;
				g_Player[tmpi].waitframe = 2;
			}
			break;
		case STAT_STEP28:
			g_Player[tmpi].waitframe--;
			if( g_Player[tmpi].waitframe < 0 )
			{
				for( int tmpz = 1; tmpz < 30; tmpz++ )
				{
					Net_ItemUse( tmpz, -1, -1, tmpi );
				}
				g_Player[tmpi].stat = STAT_STEP29;
				g_Player[tmpi].waitframe = 2;
			}
			break;
		case STAT_STEP29:
			g_Player[tmpi].waitframe--;
			if( g_Player[tmpi].waitframe < 0 )
			{
				Net_TouchNpc( 0, 0, -533, tmpi );
				g_Player[tmpi].stat = STAT_STEP30;
				g_Player[tmpi].waitframe = 2;
			}
			break;
		case STAT_STEP30:
			g_Player[tmpi].waitframe--;
			if( g_Player[tmpi].waitframe < 0 )
			{
				Net_TouchNpc( 0, 1, 0, tmpi );
				g_Player[tmpi].stat = STAT_STEP31;
				g_Player[tmpi].waitframe = 2;
			}
			break;
		case STAT_STEP31:
			g_Player[tmpi].waitframe--;
			if( g_Player[tmpi].waitframe < 0 )
			{
				Net_TouchNpc( 0, 0, -535, tmpi );
				g_Player[tmpi].stat = STAT_STEP32;
				g_Player[tmpi].waitframe = 2;
			}
			break;
		case STAT_STEP32:
			g_Player[tmpi].waitframe--;
			if( g_Player[tmpi].waitframe < 0 )
			{
				Net_TouchNpc( 0, 1, 0, tmpi );
				g_Player[tmpi].stat = STAT_STEP33;
				g_Player[tmpi].waitframe = 2;
			}
			break;
		case STAT_STEP33:
			g_Player[tmpi].waitframe--;
			if( g_Player[tmpi].waitframe < 0 )
			{
				for( int tmpz = 1; tmpz < 30; tmpz++ )
				{
					Net_ItemUse( tmpz, 1, -1, tmpi );
				}
				g_Player[tmpi].stat = STAT_STEP34;
				g_Player[tmpi].waitframe = 2;
			}
			break;
		case STAT_STEP34:
			g_Player[tmpi].waitframe--;
			if( g_Player[tmpi].waitframe < 0 )
			{
				Net_TouchNpc( 0, 0, -535, tmpi );
				g_Player[tmpi].stat = STAT_STEP35;
				g_Player[tmpi].waitframe = 2;
			}
			break;
		case STAT_STEP35:
			g_Player[tmpi].waitframe--;
			if( g_Player[tmpi].waitframe < 0 )
			{
				Net_TouchNpc( 0, 1, 0, tmpi );
				g_Player[tmpi].stat = STAT_STEP36;
				g_Player[tmpi].waitframe = 2;
			}
			break;
		case STAT_STEP36:
			g_Player[tmpi].waitframe--;
			if( g_Player[tmpi].waitframe < 0 )
			{
				Net_Move( 152, 100, tmpi );
				g_Player[tmpi].stat = STAT_STEP37;
				g_Player[tmpi].waitframe = rand()%6+12;
			}
			break;
		case STAT_STEP37:
			g_Player[tmpi].waitframe--;
			if( g_Player[tmpi].waitframe < 0 )
			{
				Net_Move( 124, 104, tmpi );
				g_Player[tmpi].stat = STAT_STEP38;
				g_Player[tmpi].waitframe = rand()%6+12;
			}
			break;
		case STAT_STEP38:
			g_Player[tmpi].waitframe--;
			if( g_Player[tmpi].waitframe < 0 )
			{
				Net_Move( 109, 103, tmpi );
				g_Player[tmpi].stat = STAT_STEP39;
				g_Player[tmpi].waitframe = rand()%6+12;
			}
			break;
		case STAT_STEP39:
			g_Player[tmpi].waitframe--;
			if( g_Player[tmpi].waitframe < 0 )
			{
				Net_Move( 91, 102, tmpi );
				g_Player[tmpi].stat = STAT_STEP40;
				g_Player[tmpi].waitframe = rand()%6+12;
			}
			break;
		case STAT_STEP40:
			g_Player[tmpi].waitframe--;
			if( g_Player[tmpi].waitframe < 0 )
			{
				Net_Move( 64, 99, tmpi );
				g_Player[tmpi].stat = STAT_STEP40x;
				g_Player[tmpi].waitframe = rand()%6+12;
			}
			break;
		case STAT_STEP40x:
			g_Player[tmpi].waitframe--;
			if( g_Player[tmpi].waitframe < 0 )
			{
				Net_Move( 46, 99, tmpi );
				g_Player[tmpi].stat = STAT_STEP41;
				g_Player[tmpi].waitframe = rand()%6+12;
			}
			break;
		case STAT_STEP41:
			g_Player[tmpi].waitframe--;
			if( g_Player[tmpi].waitframe < 0 )
			{
				Net_Move( 32, 100, tmpi );
				g_Player[tmpi].stat = STAT_STEP42;
				g_Player[tmpi].waitframe = rand()%6+8;
			}
			break;
		case STAT_STEP42:
			g_Player[tmpi].waitframe--;
			if( g_Player[tmpi].waitframe < 0 )
			{
				Net_TouchNpc( 0, 0, -551, tmpi );
				g_Player[tmpi].stat = STAT_STEP43;
				g_Player[tmpi].waitframe = 2;
			}
			break;
		case STAT_STEP43:
			g_Player[tmpi].waitframe--;
			if( g_Player[tmpi].waitframe < 0 )
			{
				Net_TouchNpc( 0, 1, 0, tmpi );
				g_Player[tmpi].stat = STAT_STEP44;
				g_Player[tmpi].waitframe = 2;
			}
			break;
		case STAT_STEP44:
			g_Player[tmpi].waitframe--;
			if( g_Player[tmpi].waitframe < 0 )
			{
				Net_Move( 56, 96, tmpi );
				g_Player[tmpi].stat = STAT_STEP45;
				g_Player[tmpi].waitframe = rand()%6+8;
			}
			break;
		case STAT_STEP45:
			g_Player[tmpi].waitframe--;
			if( g_Player[tmpi].waitframe < 0 )
			{
				Net_Move( 73, 98, tmpi );
				g_Player[tmpi].stat = STAT_STEP46;
				g_Player[tmpi].waitframe = rand()%6+8;
			}
			break;
		case STAT_STEP46:
			g_Player[tmpi].waitframe--;
			if( g_Player[tmpi].waitframe < 0 )
			{
				Net_TouchNpc( 0, 0, -538, tmpi );
				g_Player[tmpi].stat = STAT_STEP47;
				g_Player[tmpi].waitframe = 2;
			}
			break;
		case STAT_STEP47:
			g_Player[tmpi].waitframe--;
			if( g_Player[tmpi].waitframe < 0 )
			{
				Net_TouchNpc( 0, 1, 0, tmpi );
				g_Player[tmpi].stat = STAT_STEP48;
				g_Player[tmpi].waitframe = 2;
			}
			break;
		case STAT_STEP48:
			g_Player[tmpi].waitframe--;
			if( g_Player[tmpi].waitframe < 0 )
			{
				Net_Move( 47, 90, tmpi );
				g_Player[tmpi].stat = STAT_STEP49;
				g_Player[tmpi].waitframe = rand()%6+8;
			}
			break;
		case STAT_STEP49:
			g_Player[tmpi].waitframe--;
			if( g_Player[tmpi].waitframe < 0 )
			{
				Net_TouchNpc( 0, 0, -539, tmpi );
				g_Player[tmpi].stat = STAT_STEP50;
				g_Player[tmpi].waitframe = 2;
			}
			break;
		case STAT_STEP50:
			g_Player[tmpi].waitframe--;
			if( g_Player[tmpi].waitframe < 0 )
			{
				Net_TouchNpc( 0, 1, 0, tmpi );
				g_Player[tmpi].stat2 = 2;
				g_Player[tmpi].waitframe = 2;
			}
			break;
		case STAT_STEP51:
			g_Player[tmpi].waitframe--;
			if( g_Player[tmpi].waitframe < 0 )
			{
				Net_TouchNpc( 0, 0, -544, tmpi );
				g_Player[tmpi].stat2 = 3;
				g_Player[tmpi].waitframe = 2;
			}
			break;
		case STAT_STEP52:
			g_Player[tmpi].waitframe--;
			if( g_Player[tmpi].waitframe < 0 )
			{
				Net_Move( 69, 99, tmpi );
				g_Player[tmpi].stat = STAT_STEP53;
				g_Player[tmpi].waitframe = rand()%6+8;
			}
			break;
		case STAT_STEP53:
			g_Player[tmpi].waitframe--;
			if( g_Player[tmpi].waitframe < 0 )
			{
				Net_TouchNpc( 0, 0, -538, tmpi );
				g_Player[tmpi].stat = STAT_STEP54;
				g_Player[tmpi].waitframe = 2;
			}
			break;
		case STAT_STEP54:
			g_Player[tmpi].waitframe--;
			if( g_Player[tmpi].waitframe < 0 )
			{
				Net_TouchNpc( 0, 1, 0, tmpi );
				g_Player[tmpi].stat = STAT_STEP55;
				g_Player[tmpi].waitframe = 2;
			}
			break;
		case STAT_STEP55:
			g_Player[tmpi].waitframe--;
			if( g_Player[tmpi].waitframe < 0 )
			{
				Net_Move( 40, 99, tmpi );
				g_Player[tmpi].stat = STAT_STEP56;
				g_Player[tmpi].waitframe = rand()%6+8;
			}
			break;
		case STAT_STEP56:
			g_Player[tmpi].waitframe--;
			if( g_Player[tmpi].waitframe < 0 )
			{
				Net_Move( 23, 97, tmpi );
				g_Player[tmpi].stat = STAT_STEP57;
				g_Player[tmpi].waitframe = rand()%6+8;
			}
			break;
		case STAT_STEP57:
			g_Player[tmpi].waitframe--;
			if( g_Player[tmpi].waitframe < 0 )
			{
				Net_TouchNpc( 0, 0, -532, tmpi );
				g_Player[tmpi].stat = STAT_STEP58;
				g_Player[tmpi].waitframe = 2;
			}
			break;
		case STAT_STEP58:
			g_Player[tmpi].waitframe--;
			if( g_Player[tmpi].waitframe < 0 )
			{
				Net_TouchNpc( 0, 1, 0, tmpi );
				g_Player[tmpi].stat = STAT_STEP59;
				g_Player[tmpi].waitframe = 2;
			}
			break;


		default:
			break;
		}
	}
}

void System_Destroy()
{
	client_message_exit();
	client_destroy();
}

