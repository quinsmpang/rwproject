
#ifndef __SYSTEM_H
#define __SYSTEM_H
#include "server_netsend_auto.h"

enum TALK_CHANNEL
{
	TALK_CHANNEL_ACTOR_WORLD = 0,	// 世界喊话
	TALK_CHANNEL_ACTOR_CLUB	= 1,	// 联盟
	TALK_CHANNEL_SYSTEM = 100,		// 系统全服
	TALK_CHANNEL_SYSTEMTO = 101,	// 系统针对某一玩家
};

int local_talk( int actor_index, const char *szMsg, int type );
int system_talk( const char *szMsg, int type );
int system_talkto( int actor_index, char *szMsg );

int system_getruntime();
int system_getopentime();

int system_gettoday();
int system_getmonth();
int system_getweek();
int system_getmaxday( int year, int month );
int system_getcurmonth_maxday();

int random( int min, int max );
void ruffle( int *list, int count );


// 聊天缓存队列
#define TALK_CACHE_QUEUE_COUNT	128
int talk_cache_queue_add( char type, SLK_NetS_Talk *info );
SLK_NetS_Talk *talk_cache_queue_get( char type, int index );
int talk_cache_queue_end(char type);
int talk_cache_queue_sendtime_index(char type, int sendtime);
#endif 
