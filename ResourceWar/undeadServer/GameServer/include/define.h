#ifndef __DEFINE_H
#define __DEFINE_H
#include "utils.h"
#include "gamelog.h"
#include "client.h"
#include "auto_actor_hero.h"
#include "auto_actor_item.h"
#include "server_struct_auto.h"
#include "plat.h"

#define LOG_PATH	"./log/"

#ifndef NAME_SIZE
#define NAME_SIZE					22		// 玩家名字和物品名字长度
#endif

#define MAX_TEAMMEMBER	HERO_MAXCOUNT

// 最小的角色ID值
#define MINACTORID	1300

#define MOVECLICK_DELAY 12

#define CHAT_MASK_LIST_MAXCOUNT 64	// 聊天屏蔽列表大小

#define ACTOR_CHECK_INDEX(actor_index)		do{ if( actor_index < 0 || actor_index >= g_maxactornum ) return -1;}while(0)
#define HERO_CHECK_INDEX(hero_index)		do{ if( hero_index < 0 || hero_index >= HERO_MAXCOUNT ) return -1;}while(0)
#define CITY_CHECK_INDEX(city_index)		do{ if( city_index < 0 || city_index >= g_city_maxcount ) return -1;}while(0)
// 闰年
#define IS_LEAP_YEAR(y)  (((y) % 4 == 0 && (y) % 100 != 0) || (y) % 400 == 0)
// 本地化
#define ZH( language, textid ) localize_gettext( language, textid )

/* 配置文件 */
typedef struct _sconfig {
	int area_code;
	int server_code;
	unsigned long ipaddress;
	unsigned short port;
	unsigned long user_ipaddress;
	unsigned short user_port;
	char user_host[32];

	unsigned long ds_ipaddress;
	unsigned short ds_port;

	char datadb_host[64];
	char datadb_user[32];
	char datadb_pass[32];
	char datadb_database[32];

	char gamedb_host[64];
	char gamedb_user[32];
	char gamedb_pass[32];
	char gamedb_database[32];

	char logdb_host[64];
	char logdb_user[32];
	char logdb_pass[32];
	char logdb_database[32];

	char server_name[32];
	int server_start_date;
	int max_connection;
	int max_citycount;
	int max_clubcount;
	int server_exit_week;
	int server_exit_hour;
	int server_exit_min;
	char server_plat;
	char nopay;

	unsigned int client_mask_functions;	// 客户端屏蔽的功能点
	union
	{
		int value; 
		struct
		{
			unsigned char m1;
			unsigned char m2;
			unsigned short m3;
		}ss;
	}minver;
} SConfig;

void sendtoclient( int actor_index, char *tmpbuf, int size );
void sendtoclient_team( int actor_index, char *tmpbuf, int size );
#endif
