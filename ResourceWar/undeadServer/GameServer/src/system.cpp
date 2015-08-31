#ifdef WIN32
#include <winsock2.h>
#include <windows.h>
#endif
#include <mysql.h>
#include <stdio.h>
#include <time.h>
#include "system.h"
#include "actor.h"
#include "gameproc.h"
#include "utils.h"
#include "db.h"
#include "gamelog.h"
#include "server_netsend_auto.h"
#include "actor_send.h"


extern MYSQL *myGame;
extern Actor *g_actors;
extern int g_maxactornum;
extern int g_max_connection;
extern SConfig g_Config;

static int s_begin_time = -1;
static int s_open_time = -1;

SLK_NetS_Talk g_TalkCacheQueue[2][TALK_CACHE_QUEUE_COUNT];

// 系统公告发送
int system_talk( const char *szMsg, int type )
{
	SLK_NetS_Talk Value = {};
	Value.m_actorid = 0;
	Value.m_name[0] = 0;
	Value.m_type = type;
	Value.m_msg_length = (int)strlen(szMsg);
	if ( Value.m_msg_length >= 256 )
		return -1;
	strcpy( Value.m_msg, szMsg );
	netsend_talk_S( 0, SENDTYPE_WORLD, &Value );
	return 0;
}

// 将指定对话发送给指定玩家
int system_talkto( int actor_index, char *szMsg )
{
	SLK_NetS_Talk Value = {};
	Value.m_actorid = 0;
	Value.m_name[0] = 0;
	Value.m_type = TALK_CHANNEL_SYSTEMTO;
	Value.m_msg_length = (int)strlen( szMsg );
	strcpy( Value.m_msg, szMsg );
	netsend_talk_S( actor_index, SENDTYPE_ACTOR, &Value );
	return 0;
}

int local_talk( int actor_index, const char *szMsg, int type )
{
	return 0;
}

// 获取开服流逝的时间 开服时间
int system_getruntime()
{
	if( s_begin_time < 0 )
	{
		int timestring = g_Config.server_start_date;
		struct tm BeginTm;
		if( timestring < 19000101 )
			timestring = 19000101;
		BeginTm.tm_year = timestring/10000 - 1900;
		timestring = timestring%10000;
		BeginTm.tm_mon  = timestring/100 - 1;
		timestring = timestring%100;
		BeginTm.tm_mday = timestring;
		BeginTm.tm_hour = 0;
		BeginTm.tm_min  = 0;
		BeginTm.tm_sec  = 0;

		s_begin_time = (int)mktime( &BeginTm );
	}
	return (int)time(NULL)-s_begin_time;
}

int system_getopentime()
{
	if ( s_open_time < 0 )
	{
		int timestring = g_Config.server_start_date;
		struct tm BeginTm;
		if ( timestring < 19000101 )
			timestring = 19000101;
		BeginTm.tm_year = timestring / 10000 - 1900;
		timestring = timestring % 10000;
		BeginTm.tm_mon = timestring / 100 - 1;
		timestring = timestring % 100;
		BeginTm.tm_mday = timestring;
		BeginTm.tm_hour = 0;
		BeginTm.tm_min = 0;
		BeginTm.tm_sec = 0;

		s_open_time = (int)mktime( &BeginTm );
	}
	return s_open_time;
}

int system_gettoday()
{
	time_t t;
	time(&t);
	struct tm *newtime = localtime( &t );

	return ((1900+newtime->tm_year)*10000 + (newtime->tm_mon+1)*100 + newtime->tm_mday);
}
/* 获取月份 */
int system_getmonth()
{
	time_t t;
	time( &t );
	struct tm *newtime = localtime( &t );

	return (newtime->tm_mon + 1);
}
/* 获取星期 */
int system_getweek()
{
	time_t t;
	time( &t );
	struct tm *newtime = localtime( &t );

	return newtime->tm_wday;
}

/* 获取最大天数 */
int system_getmaxday( int year, int month )
{
	int maxday = 0;
	if ( month == 1 || month == 3 || month == 5 || month == 7 || month == 8 || month == 10 || month == 12 )
		maxday = 31;
	else if ( month == 4 || month == 6 || month == 9 || month == 11 )
		maxday = 30;
	else if ( month == 2 )
	{
		if ( IS_LEAP_YEAR( year ) )
			maxday = 29;
		else
			maxday = 28;
	}
	return maxday;
}

/* 获取当月最大天数 */
int system_getcurmonth_maxday()
{
	time_t t;
	time( &t );
	struct tm *newtime = localtime( &t );

	return system_getmaxday( (1900 + newtime->tm_year), (newtime->tm_mon + 1) );
}

/* 随机 */
int random( int min, int max )
{
	int value = (max - min + 1);
	if ( value <= 0 )
		value = 1;
	return min + rand() % value;
}

/* 洗牌 */
void ruffle( int *list, int count )
{
	for ( int tmpi = 0; tmpi < count; tmpi++ )
	{
		int index = rand() % count;
		int swap = list[index];
		swap = list[tmpi];
		list[tmpi] = list[index];
		list[index] = swap;
	}
}

/* 添加聊天缓存队列 */
int talk_cache_queue_add( char type, SLK_NetS_Talk *info )
{
	int tmpi = 0;
	for ( tmpi = 0; tmpi < TALK_CACHE_QUEUE_COUNT; tmpi++ )
	{
		if ( g_TalkCacheQueue[type][tmpi].m_msg_length <= 0 )
		{
			memcpy( &g_TalkCacheQueue[type][tmpi], info, sizeof(SLK_NetS_Talk) );
			break;
		}
	}
	if ( tmpi >= TALK_CACHE_QUEUE_COUNT )
	{
		memmove( &g_TalkCacheQueue[type][0], &g_TalkCacheQueue[type][1], sizeof(SLK_NetS_Talk)*(TALK_CACHE_QUEUE_COUNT - 1) );
		memcpy( &g_TalkCacheQueue[type][TALK_CACHE_QUEUE_COUNT-1], info, sizeof(SLK_NetS_Talk) );
	}
	return 0;
}
/*获取缓存队列里面的 当前的最后内容位置
//@return 未找到返回-1*/
int talk_cache_queue_end(char type)
{
	for ( int tmpi = TALK_CACHE_QUEUE_COUNT - 1 ; tmpi >= 0; tmpi-- )
	{
		if ( g_TalkCacheQueue[type][tmpi].m_msg_length > 0 )
		{
			return tmpi;
		}
	}
	return -1;
}
/*根据发送的时间戳 获取在缓存队列里面的索引位置
//@return  未找到返回 0*/
int talk_cache_queue_sendtime_index(char type, int sendtime)
{
	for ( int tmpi = TALK_CACHE_QUEUE_COUNT - 1 ; tmpi >= 0; tmpi-- )
	{
		if ( g_TalkCacheQueue[type][tmpi].m_msg_length > 0  && g_TalkCacheQueue[type][tmpi].m_sendtime ==sendtime )
		{
			return tmpi;
		}
	}
	return 0;
}
/*根据类别和索引位置 获取聊天内容
//@return 没有找到返回空*/
SLK_NetS_Talk *talk_cache_queue_get( char type,int index )
{
	if(TALK_CACHE_QUEUE_COUNT - 1 >=index && index>=0)
	{
		return &g_TalkCacheQueue[type][index];
	}
	else
	{
		return NULL;
	}
}
