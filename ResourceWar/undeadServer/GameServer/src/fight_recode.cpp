#ifdef WIN32
#include <winsock2.h>
#include <windows.h>
#endif
#include <mysql.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "db.h"
#include "define.h"
#include "actor.h"
#include "hero.h"
#include "fight.h"
#include "fight_recode.h"
#include "troop.h"

extern SConfig g_Config;
extern MYSQL *myGame;
extern MYSQL *myData;

extern Fight g_Fight;

int g_fightbufsize;
char *g_pFightBuf;

char *g_pRecordSQL;
char *g_pTextRecordBuf;

int fightrec_init()
{
	g_fightbufsize = sizeof(Fight)+sizeof(Army)* 2 + 2 * MAX_TROOPS_COUNT*MAX_TURN * sizeof(FightMove)+2 * MAX_TROOPS_COUNT*MAX_TROOPS_COUNT*MAX_TURN * sizeof(FightAction);
	g_pFightBuf = (char *)malloc(g_fightbufsize);

	g_pRecordSQL = (char *)malloc(MAX_RECORDBUFF_SIZE * 2 + 8192);
	g_pTextRecordBuf = (char *)malloc(MAX_RECORDBUFF_SIZE * 2);
	return 0;
}

int fight_read(int fightid)
{
	MYSQL_RES		*res;
	MYSQL_ROW		row;
	char szSQL[2048] = { 0 };
	unsigned long *lens;
	int size = 0;

	sprintf(szSQL, "SELECT fighttype, fighttime, not_overdue, fightbuf FROM fight_record WHERE id='%d';", fightid);
	if (mysql_query(myGame, szSQL))
	{
		printf("Query failed (%s) [%s](%d)\n", mysql_error(myGame), __FUNCTION__, __LINE__);
		write_gamelog("%s", szSQL);
		if (mysql_ping(myGame) != 0)
			db_reconnect_game();
		return -1;
	}

	res = mysql_store_result(myGame);
	if (row = mysql_fetch_row(res))
	{
		lens = mysql_fetch_lengths(res);
		if (lens)
		{
			size = lens[3];
			memcpy(g_pFightBuf, row[3], sizeof(char)* size);
		}
	}
	else
	{
		return -1;
	}
	mysql_free_result(res);
	return size;
}

int fight_serialize_read(int size)
{
	char *data = g_pFightBuf;

	LKSET_BYTE_RECV(&g_Fight.isdefance, data, size);
	LKSET_BYTE_RECV(&g_Fight.level, data, size);
	LKSET_WORD_RECV(&g_Fight.cur_turns, data, size);
	LKSET_BYTE_RECV(&g_Fight.type, data, size);
	LKSET_BYTE_RECV(&g_Fight.attack_type, data, size);
	LKSET_DWORD_RECV(&g_Fight.attack_num, data, size);
	LKSET_DWORD_RECV(&g_Fight.defence_num, data, size);
	LKSET_WORD_RECV(&g_Fight.attack_factor, data, size);
	LKSET_WORD_RECV(&g_Fight.defence_factor, data, size);
	LKSET_MEM_RECV(&g_Fight.move_sort, data, MAX_TROOPS_COUNT * 2, size);
	LKSET_MEM_RECV(&g_Fight.attack_sort, data, MAX_TROOPS_COUNT * 2, size);
	LKSET_DWORD_RECV(&g_Fight.lastacttype, data, size);
	LKSET_DWORD_RECV(&g_Fight.lastactpos, data, size);

	LKSET_DWORD_RECV(&g_Fight.pAttackArmy->actorid, data, size);
	LKSET_BYTE_RECV(&g_Fight.pAttackArmy->isnpc, data, size);
	LKSET_WORD_RECV(&g_Fight.pAttackArmy->shape, data, size);
	LKSET_WORD_RECV(&g_Fight.pAttackArmy->target_posx, data, size);
	LKSET_WORD_RECV(&g_Fight.pAttackArmy->target_posy, data, size);
	LKSET_DWORD_RECV(&g_Fight.pAttackArmy->troops_count, data, size);
	for (int tmpi = 0; tmpi < g_Fight.pAttackArmy->troops_count; tmpi++)
	{
		LKSET_MEM_RECV(&g_Fight.pAttackArmy->troops[tmpi], data, sizeof(TroopUnit), size);
	}
	LKSET_DWORD_RECV(&g_Fight.pDefenceArmy->actorid, data, size);
	LKSET_BYTE_RECV(&g_Fight.pDefenceArmy->isnpc, data, size);
	LKSET_WORD_RECV(&g_Fight.pDefenceArmy->shape, data, size);
	LKSET_WORD_RECV(&g_Fight.pDefenceArmy->target_posx, data, size);
	LKSET_WORD_RECV(&g_Fight.pDefenceArmy->target_posy, data, size);
	LKSET_DWORD_RECV(&g_Fight.pDefenceArmy->troops_count, data, size);
	for (int tmpi = 0; tmpi < g_Fight.pDefenceArmy->troops_count; tmpi++)
	{
		LKSET_MEM_RECV(&g_Fight.pDefenceArmy->troops[tmpi], data, sizeof(TroopUnit), size);
	}
	LKSET_DWORD_RECV(&g_Fight.movstep, data, size);
	LKSET_DWORD_RECV(&g_Fight.actstep, data, size);
	for (int tmpi = 0; tmpi < g_Fight.movstep; tmpi++)
	{
		LKSET_WORD_RECV(&g_Fight.pMove[tmpi].turns, data, size);
		LKSET_BYTE_RECV(&g_Fight.pMove[tmpi].srcpos, data, size);
		LKSET_BYTE_RECV(&g_Fight.pMove[tmpi].movstep, data, size);
	}
	for (int tmpi = 0; tmpi < g_Fight.actstep; tmpi++)
	{
		LKSET_WORD_RECV(&g_Fight.pAction[tmpi].turns, data, size);
		LKSET_BYTE_RECV(&g_Fight.pAction[tmpi].srcpos, data, size);
		LKSET_BYTE_RECV(&g_Fight.pAction[tmpi].tgtpos, data, size);
		LKSET_BYTE_RECV(&g_Fight.pAction[tmpi].action, data, size);
		LKSET_DWORD_RECV(&g_Fight.pAction[tmpi].damage, data, size);
		LKSET_DWORD_RECV(&g_Fight.pAction[tmpi].srcstat, data, size);
		LKSET_DWORD_RECV(&g_Fight.pAction[tmpi].tgtstat, data, size);
		LKSET_BYTE_RECV(&g_Fight.pAction[tmpi].nexttgt, data, size);
	}

	return size;
}


int fight_serialize_write()
{
	//g_pFightBuf = (char *)malloc(g_fightbufsize);
	char *data;
	int size;
	memset(g_pFightBuf, 0, g_fightbufsize);
	size = 0;
	data = g_pFightBuf;
	LKSET_BYTE_SEND(data, &g_Fight.isdefance, size);
	LKSET_BYTE_SEND(data, &g_Fight.level, size);
	LKSET_WORD_SEND(data, &g_Fight.cur_turns, size);
	LKSET_BYTE_SEND(data, &g_Fight.type, size);
	LKSET_BYTE_SEND(data, &g_Fight.attack_type, size);
	LKSET_DWORD_SEND(data, &g_Fight.attack_num, size);
	LKSET_DWORD_SEND(data, &g_Fight.defence_num, size);
	LKSET_WORD_SEND(data, &g_Fight.attack_factor, size);
	LKSET_WORD_SEND(data, &g_Fight.defence_factor, size);
	LKSET_MEM_SEND(data, &g_Fight.move_sort, MAX_TROOPS_COUNT * 2, size);
	LKSET_MEM_SEND(data, &g_Fight.attack_sort, MAX_TROOPS_COUNT * 2, size);
	LKSET_DWORD_SEND(data, &g_Fight.lastacttype, size);
	LKSET_DWORD_SEND(data, &g_Fight.lastactpos, size);

	LKSET_DWORD_SEND(data, &g_Fight.pAttackArmy->actorid, size);
	LKSET_BYTE_SEND(data, &g_Fight.pAttackArmy->isnpc, size);
	LKSET_WORD_SEND(data, &g_Fight.pAttackArmy->shape, size);
	LKSET_WORD_SEND(data, &g_Fight.pAttackArmy->target_posx, size);
	LKSET_WORD_SEND(data, &g_Fight.pAttackArmy->target_posy, size);
	LKSET_DWORD_SEND(data, &g_Fight.pAttackArmy->troops_count, size);
	for (int tmpi = 0; tmpi < g_Fight.pAttackArmy->troops_count; tmpi++)
	{
		LKSET_MEM_SEND(data, &g_Fight.pAttackArmy->troops[tmpi], sizeof(TroopUnit), size);
	}
	LKSET_DWORD_SEND(data, &g_Fight.pDefenceArmy->actorid, size);
	LKSET_BYTE_SEND(data, &g_Fight.pDefenceArmy->isnpc, size);
	LKSET_WORD_SEND(data, &g_Fight.pDefenceArmy->shape, size);
	LKSET_WORD_SEND(data, &g_Fight.pDefenceArmy->target_posx, size);
	LKSET_WORD_SEND(data, &g_Fight.pDefenceArmy->target_posy, size);
	LKSET_DWORD_SEND(data, &g_Fight.pDefenceArmy->troops_count, size);
	for (int tmpi = 0; tmpi < g_Fight.pDefenceArmy->troops_count; tmpi++)
	{
		LKSET_MEM_SEND(data, &g_Fight.pDefenceArmy->troops[tmpi], sizeof(TroopUnit), size);
	}
	LKSET_DWORD_SEND(data, &g_Fight.movstep, size);
	LKSET_DWORD_SEND(data, &g_Fight.actstep, size);
	for (int tmpi = 0; tmpi < g_Fight.movstep; tmpi++)
	{
		LKSET_WORD_SEND(data, &g_Fight.pMove[tmpi].turns, size);
		LKSET_BYTE_SEND(data, &g_Fight.pMove[tmpi].srcpos, size);
		LKSET_BYTE_SEND(data, &g_Fight.pMove[tmpi].movstep, size);
	}
	for (int tmpi = 0; tmpi < g_Fight.actstep; tmpi++)
	{
		LKSET_WORD_SEND(data, &g_Fight.pAction[tmpi].turns, size);
		LKSET_BYTE_SEND(data, &g_Fight.pAction[tmpi].srcpos, size);
		LKSET_BYTE_SEND(data, &g_Fight.pAction[tmpi].tgtpos, size);
		LKSET_BYTE_SEND(data, &g_Fight.pAction[tmpi].action, size);
		LKSET_DWORD_SEND(data, &g_Fight.pAction[tmpi].damage, size);
		LKSET_DWORD_SEND(data, &g_Fight.pAction[tmpi].srcstat, size);
		LKSET_DWORD_SEND(data, &g_Fight.pAction[tmpi].tgtstat, size);
		LKSET_BYTE_SEND(data, &g_Fight.pAction[tmpi].nexttgt, size);
	}

	return size;
}

// 保存战斗结果数据，返回战斗id
int fight_save(int size)
{
	g_pRecordSQL = (char *)malloc(MAX_RECORDBUFF_SIZE * 2 + 8192);
	g_pTextRecordBuf = (char *)malloc(MAX_RECORDBUFF_SIZE * 2);
	sprintf(g_pRecordSQL, "INSERT INTO fight_record ( fighttype, fighttime, not_overdue, fightbuf ) VALUES( '%d','%d','%d','%s' )",
		0,
		(int)time(NULL),
		0,
		db_escape((const char *)g_pFightBuf, g_pTextRecordBuf, sizeof(char)*size));
	if (mysql_query(myGame, g_pRecordSQL))
	{
		printf("Query failed (%s) [%s](%d)\n", mysql_error(myGame), __FUNCTION__, __LINE__);
		write_gamelog("%s", g_pRecordSQL);
		if (mysql_ping(myGame) != 0)
			db_reconnect_game();
		return -1;
	}
	// 返回的刚刚插入的id
	return (unsigned int)mysql_insert_id(myGame);
}
