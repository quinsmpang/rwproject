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
#include "troop.h"
#include "global.h"

extern Global global;
extern MYSQL *myGame;
extern MYSQL *myData;

extern Actor *g_actors;
extern int g_maxactornum;

extern HeroInfo *g_HeroInfo;
extern short g_HeroCount;

extern TroopInfo **g_TroopInfo;
extern short g_TroopCount;

extern TroopCorpsInfo *g_TroopCorpsInfo;
extern short g_TroopCorpsCount;

extern Army *g_Army;
extern int g_maxArmyNum;

Fight g_Fight;

char g_restrain_order[11][11];
float g_restrain_value[11][11];

//-----------------------------------------------------------------------------
// fight_init
// 函数说明: 战斗系统初始化
//-----------------------------------------------------------------------------
int fight_init()
{
	MYSQL_RES		*res;
	MYSQL_ROW		row;
	char szSQL[2048] = { 0 };
	int attack_type;

	g_Fight.pMove = (FightMove *)malloc(2 * MAX_TROOPS_COUNT*MAX_TURN * sizeof(FightMove));
	g_Fight.pAction = (FightAction *)malloc(2 * MAX_TROOPS_COUNT*MAX_TROOPS_COUNT*MAX_TURN * sizeof(FightAction));

	sprintf(szSQL, "SELECT * FROM restrain;" );
	if (mysql_query(myData, szSQL))
	{
		printf("Query failed (%s) [%s](%d)\n", mysql_error(myData), __FUNCTION__, __LINE__);
		write_gamelog("%s", szSQL);
		if (mysql_ping(myData) != 0)
			db_reconnect_game();
		return -1;
	}

	res = mysql_store_result(myData);
	int index;
	memset(g_restrain_order, 0, sizeof(char)* 11 * 11);
	memset(g_restrain_value, 0, sizeof(float)* 11 * 11);
	while (row = mysql_fetch_row(res))
	{
		attack_type = atoi(row[0]);
		if (attack_type<1 || attack_type>10)
			continue;
		index = 1;
		for (int tmpi = 0; tmpi < 10; tmpi++)
			g_restrain_order[attack_type][tmpi + 1] = atoi(row[index++]);
		for (int tmpi = 0; tmpi < 10; tmpi++)
			g_restrain_value[attack_type][tmpi + 1] = (float)atof(row[index++]);
	}
	mysql_free_result(res);

	return fightrec_init();
}

// 完全排序
int fight_sort()
{
	int tmpi;
	int tmpj;
	short truespeed[MAX_TROOPS_COUNT * 2];
	char sort[MAX_TROOPS_COUNT * 2];
	int sortpos;

	short corps;
	short level;

	for (tmpi = 0; tmpi < MAX_TROOPS_COUNT * 2; tmpi++)
		sort[tmpi] = -1;

	// 计算可用于排序的速度值
	for (tmpi = 0; tmpi < MAX_TROOPS_COUNT; tmpi++)
	{
		if (tmpi < g_Fight.pAttackArmy->troops_count)
		{
			corps = g_Fight.pAttackArmy->troops[tmpi].corps;
			level = g_Fight.pAttackArmy->troops[tmpi].level;
			truespeed[tmpi] = g_TroopInfo[corps][level].mobility;
		}
		else
		{
			truespeed[tmpi] = 0;
		}

		if (tmpi < g_Fight.pDefenceArmy->troops_count)
		{
			corps = g_Fight.pDefenceArmy->troops[tmpi].corps;
			level = g_Fight.pDefenceArmy->troops[tmpi].level;
			truespeed[tmpi + MAX_TROOPS_COUNT] = g_TroopInfo[corps][level].mobility;
		}
		else
		{
			truespeed[tmpi + MAX_TROOPS_COUNT] = 0;
		}

	}
	// 开始排序
	for (tmpi = 0; tmpi < MAX_TROOPS_COUNT*2; tmpi++)
	{
		for (tmpj = 0; tmpj < tmpi; tmpj++)
		{
			if (sort[tmpj] < 0)
				break;
			sortpos = sort[tmpj];
			if (truespeed[tmpi] > truespeed[sortpos])
			{
				memmove(sort + tmpj + 1, sort + tmpj, 20 - tmpj - 1);
				break;
			}
		}
		sort[tmpj] = tmpi;
	}
	memcpy(g_Fight.move_sort, sort, MAX_TROOPS_COUNT * 2);

	// 战斗的排序
	for (tmpi = 0; tmpi < MAX_TROOPS_COUNT * 2; tmpi++)
		sort[tmpi] = -1;

	// 计算可用于排序的速度值
	for (tmpi = 0; tmpi < MAX_TROOPS_COUNT; tmpi++)
	{
		if (tmpi < g_Fight.pAttackArmy->troops_count)
		{
			corps = g_Fight.pAttackArmy->troops[tmpi].corps;
			level = g_Fight.pAttackArmy->troops[tmpi].level;
			truespeed[tmpi] = g_TroopInfo[corps][level].attack_order;
		}
		else
		{
			truespeed[tmpi] = 128;
		}

		if (tmpi < g_Fight.pDefenceArmy->troops_count)
		{
			corps = g_Fight.pDefenceArmy->troops[tmpi].corps;
			level = g_Fight.pDefenceArmy->troops[tmpi].level;
			truespeed[tmpi + MAX_TROOPS_COUNT] = g_TroopInfo[corps][level].attack_order;
		}
		else
		{
			truespeed[tmpi + MAX_TROOPS_COUNT] = 128;
		}

	}
	// 开始排序
	for (tmpi = 0; tmpi < MAX_TROOPS_COUNT * 2; tmpi++)
	{
		for (tmpj = 0; tmpj < tmpi; tmpj++)
		{
			if (sort[tmpj] < 0)
				break;
			sortpos = sort[tmpj];
			if (truespeed[tmpi] < truespeed[sortpos])
			{
				memmove(sort + tmpj + 1, sort + tmpj, 20 - tmpj - 1);
				break;
			}
		}
		sort[tmpj] = tmpi;
	}
	memcpy(g_Fight.attack_sort, sort, MAX_TROOPS_COUNT * 2);

	return 0;
}

int fight_reset()
{
	return 0;
}

// 创建一个战场
int fight_create(int attack_index, int defance_index, char isDef)
{
	int tmpi;

	Army *AttackArmy = &g_Army[attack_index];
	Army *DefenceArmy = &g_Army[defance_index];
	if (AttackArmy->actorid == 0 || AttackArmy->isnpc == 1)	// 不能是NPC也不能没有actorid
		return -1;
	if (DefenceArmy->actorid == 0 && DefenceArmy->isnpc == 0)
		return -1;

	// memset前先保存move和action的分配空间
	FightMove *pMove = g_Fight.pMove;
	FightAction *pAction = g_Fight.pAction;
	memset(&g_Fight, 0, sizeof(Fight));
	g_Fight.pMove = pMove;
	g_Fight.pAction = pAction;

	memset(g_Fight.pMove, 0, 2 * MAX_TROOPS_COUNT*MAX_TURN * sizeof(FightMove));
	memset(g_Fight.pAction, 0, 2 * MAX_TROOPS_COUNT*MAX_TROOPS_COUNT*MAX_TURN * sizeof(FightAction));
	g_Fight.movstep = 0;
	g_Fight.actstep = 0;

	if (AttackArmy->isnpc)
		return -1;
	if (DefenceArmy->isnpc)
	{
		g_Fight.type = FIGHT_TYPE_PVE;
	}
	else
	{
		g_Fight.type = FIGHT_TYPE_PVP;
		isDef = 0;
	}
	g_Fight.isdefance = isDef;
	g_Fight.pAttackArmy = AttackArmy;
	g_Fight.pDefenceArmy = DefenceArmy;

	// 战场位置
	// 0 1 2 3 4 - 5 6 7 8 9 10 11 12 13 14 15 16 - 17 18 19 20 21
	for (int tmpi = 0; tmpi < AttackArmy->troops_count; tmpi++)
	{
		AttackArmy->troops[tmpi].fight_position = troop_get_entry_position(AttackArmy->troops[tmpi].corps, AttackArmy->troops[tmpi].level,0);
	}

	for (int tmpi = 0; tmpi < DefenceArmy->troops_count; tmpi++)
	{
		DefenceArmy->troops[tmpi].fight_position = troop_get_entry_position(DefenceArmy->troops[tmpi].corps, DefenceArmy->troops[tmpi].level, 1);
	}

	// 按照速度排序
	fight_sort();
	fight_reset();

	// 双方战斗力计算
	float sol = 1.0f;
	float hp = 1.0f;
	float Atk = 1.0f;
	float Crit = 1.0f;
	float critHarm = 1.0f;
	float Hit = 1.0f;
	float Def = 1.0f;
	float Agl = 1.0f;
	float Dodge = 1.0f;
	float critReduce = 1.0f;

	int troop_hp;

	int attack_power = 0;
	int defence_power = 0;

	g_Fight.attack_num = AttackArmy->troops_count;
	g_Fight.defence_num = DefenceArmy->troops_count;
	for (int tmpi = 0; tmpi < AttackArmy->troops_count; tmpi++)
	{
		sol = g_TroopCorpsInfo[AttackArmy->troops[tmpi].corps].sol_modulus;
		hp = g_TroopCorpsInfo[AttackArmy->troops[tmpi].corps].hp_modulus;
		Atk = g_TroopCorpsInfo[AttackArmy->troops[tmpi].corps].atk_modulus;
		Crit = g_TroopCorpsInfo[AttackArmy->troops[tmpi].corps].crit_modulus;
		critHarm = g_TroopCorpsInfo[AttackArmy->troops[tmpi].corps].critharm_modulus;
		Hit = g_TroopCorpsInfo[AttackArmy->troops[tmpi].corps].hit_modulus;
		Def = g_TroopCorpsInfo[AttackArmy->troops[tmpi].corps].def_modulus;
		Agl = g_TroopCorpsInfo[AttackArmy->troops[tmpi].corps].agl_modulus;
		Dodge = g_TroopCorpsInfo[AttackArmy->troops[tmpi].corps].dodge_modulus;
		critReduce = g_TroopCorpsInfo[AttackArmy->troops[tmpi].corps].critreduce_modulus;

		troop_hp = troop_get_hp(AttackArmy->troops[tmpi].corps, AttackArmy->troops[tmpi].level);
		attack_power += 1 + (int)((AttackArmy->troops[tmpi].maxnumber * (sol / 10) * (troop_hp * (hp / 10) +
			AttackArmy->troops[tmpi].attack * Atk + AttackArmy->troops[tmpi].crit * Crit + AttackArmy->troops[tmpi].crit_harm * critHarm + AttackArmy->troops[tmpi].hit * Hit +
			AttackArmy->troops[tmpi].defance * Def + AttackArmy->troops[tmpi].agl * Agl + AttackArmy->troops[tmpi].dodge * Dodge + AttackArmy->troops[tmpi].crit_reduce * critReduce) / 10000));
	}

	for (int tmpi = 0; tmpi < DefenceArmy->troops_count; tmpi++)
	{
		sol = g_TroopCorpsInfo[DefenceArmy->troops[tmpi].corps].sol_modulus;
		hp = g_TroopCorpsInfo[DefenceArmy->troops[tmpi].corps].hp_modulus;
		Atk = g_TroopCorpsInfo[DefenceArmy->troops[tmpi].corps].atk_modulus;
		Crit = g_TroopCorpsInfo[DefenceArmy->troops[tmpi].corps].crit_modulus;
		critHarm = g_TroopCorpsInfo[DefenceArmy->troops[tmpi].corps].critharm_modulus;
		Hit = g_TroopCorpsInfo[DefenceArmy->troops[tmpi].corps].hit_modulus;
		Def = g_TroopCorpsInfo[DefenceArmy->troops[tmpi].corps].def_modulus;
		Agl = g_TroopCorpsInfo[DefenceArmy->troops[tmpi].corps].agl_modulus;
		Dodge = g_TroopCorpsInfo[DefenceArmy->troops[tmpi].corps].dodge_modulus;
		critReduce = g_TroopCorpsInfo[DefenceArmy->troops[tmpi].corps].critreduce_modulus;

		troop_hp = troop_get_hp(DefenceArmy->troops[tmpi].corps, DefenceArmy->troops[tmpi].level);
		defence_power += 1+(int)((DefenceArmy->troops[tmpi].maxnumber * (sol / 10) * (troop_hp * (hp / 10) +
			DefenceArmy->troops[tmpi].attack * Atk + DefenceArmy->troops[tmpi].crit * Crit + DefenceArmy->troops[tmpi].crit_harm * critHarm + DefenceArmy->troops[tmpi].hit * Hit +
			DefenceArmy->troops[tmpi].defance * Def + DefenceArmy->troops[tmpi].agl * Agl + DefenceArmy->troops[tmpi].dodge * Dodge + DefenceArmy->troops[tmpi].crit_reduce * critReduce) / 10000));
	}
	if (attack_power <= 0 || defence_power <= 0)
		return -1;
	int patt = (int)(attack_power*100 / defence_power);
	int pdef = (int)(defence_power*100 / attack_power);


	if (patt < 85)
		g_Fight.attack_factor = -15;
	else if (patt < 95)
		g_Fight.attack_factor = -6;
	else if (patt < 105)
		g_Fight.attack_factor = 0;
	else if (patt < 115)
		g_Fight.attack_factor = 6;
	else if (patt < 125)
		g_Fight.attack_factor = 15;
	else if (patt < 135)
		g_Fight.attack_factor = 25;
	else
		g_Fight.attack_factor = 30;

	if (pdef < 85)
		g_Fight.defence_factor = -15;
	else if (pdef < 95)
		g_Fight.defence_factor = -6;
	else if (pdef < 105)
		g_Fight.defence_factor = 0;
	else if (pdef < 115)
		g_Fight.defence_factor = 6;
	else if (pdef < 125)
		g_Fight.defence_factor = 15;
	else if (pdef < 135)
		g_Fight.defence_factor = 25;
	else
		g_Fight.defence_factor = 30;

	for (tmpi = 0; tmpi < MAX_TURN; tmpi++ )
	{
		g_Fight.winner = fight_oneturn();
		if (g_Fight.winner > 0)
			break;
	}
	if (g_Fight.winner == 0)
	{
		if (g_Fight.attack_type == 0 )
			g_Fight.winner = FIGHT_LOSE;
		else
			g_Fight.winner = FIGHT_WIN;
	}

	int size = fight_serialize_write();

	// 记录战斗记录到数据库
	return fight_save(size);
}

int fight_moveone(int actor_pos)
{
	Army *SourceArmy;
	Army *TargetArmy;
	int pos;
	if (actor_pos >= MAX_TROOPS_COUNT)
	{
		SourceArmy = g_Fight.pDefenceArmy;
		TargetArmy = g_Fight.pAttackArmy;
		pos = actor_pos - MAX_TROOPS_COUNT;
	}
	else
	{
		SourceArmy = g_Fight.pAttackArmy;
		TargetArmy = g_Fight.pDefenceArmy;
		pos = actor_pos;
	}
	if (g_Fight.cur_turns <= 0)
	{
		// 进场直接走5格
		g_Fight.pMove[g_Fight.movstep].turns = g_Fight.cur_turns;
		g_Fight.pMove[g_Fight.movstep].srcpos = actor_pos;
		g_Fight.pMove[g_Fight.movstep].movstep = 5;
		if (actor_pos >= MAX_TROOPS_COUNT)
			g_Fight.pMove[g_Fight.movstep].movstep *= -1;

		SourceArmy->troops[pos].fight_position += g_Fight.pMove[g_Fight.movstep].movstep;

		g_Fight.movstep++;

		return 0;
	}

	int nearposition;
	int range;
	// 索敌，如果发现敌人，不移动，发现最近敌人距离
	if (actor_pos >= MAX_TROOPS_COUNT)
	{
		nearposition = -1;
		for (int tmpi = 0; tmpi < TargetArmy->troops_count; tmpi++)
		{
			if (TargetArmy->troops[tmpi].isdead == 0 && TargetArmy->troops[tmpi].fight_position > nearposition)
				nearposition = TargetArmy->troops[tmpi].fight_position;
		}
		if (nearposition < 0)
			return -1;
		range = SourceArmy->troops[pos].fight_position - nearposition;
	}
	else
	{
		nearposition = 100;
		for (int tmpi = 0; tmpi < TargetArmy->troops_count; tmpi++)
		{
			if (TargetArmy->troops[tmpi].isdead == 0 && TargetArmy->troops[tmpi].fight_position < nearposition)
				nearposition = TargetArmy->troops[tmpi].fight_position;
		}
		if (nearposition >= 100)
			return -1;
		range = nearposition - SourceArmy->troops[pos].fight_position;
	}

	if (range <= 0)
		return -1;
	char attack_range = troop_get_attack_range(SourceArmy->troops[pos].corps, SourceArmy->troops[pos].level);
	if (range <= attack_range)
		return 0;

	char move_range = troop_get_move_range(SourceArmy->troops[pos].corps, SourceArmy->troops[pos].level);
	if (move_range > (range - attack_range))
		move_range = (range - attack_range);

	g_Fight.pMove[g_Fight.movstep].turns = g_Fight.cur_turns;
	g_Fight.pMove[g_Fight.movstep].srcpos = actor_pos;
	g_Fight.pMove[g_Fight.movstep].movstep = move_range;
	if (actor_pos >= MAX_TROOPS_COUNT)
		g_Fight.pMove[g_Fight.movstep].movstep *= -1;
	// 实际移动操作
	SourceArmy->troops[pos].fight_position += g_Fight.pMove[g_Fight.movstep].movstep;
	g_Fight.movstep++;

	return 0;
}

int fight_sortconf(Army *pArmy, int cur, int next, char type, int value )
{
	if (pArmy == NULL)
		return -1;
	int cur_value;
	int next_value;
	switch (type)
	{
	case 1:	// 护甲类型
		cur_value = troop_get_defancetype(pArmy->troops[cur].corps, pArmy->troops[cur].level);
		next_value = troop_get_defancetype(pArmy->troops[next].corps, pArmy->troops[next].level);
		if (value <= 0 || value > 10 || cur_value <= 0 || cur_value > 10 || next_value <= 0 || next_value > 10)
			return 0;
		if (g_restrain_order[value][cur_value] < g_restrain_order[value][next_value])
			return 1;
		//		if (pArmy->troops[cur]->attack_inc.turns > 0 && pArmy->troops[next]->attack_inc.turns <= 0)
//			return 1;
		break;
	case 6:	// 防御
		break;
	}
	return 0;
}

int fight_sortorder(Army *pArmy, int *porder, char type, int value )
{
	int tmpi, tmpj;
	int tmpindex;
	if (porder == NULL)
		return -1;
	for (tmpi = 0; tmpi < MAX_TROOPS_COUNT; tmpi++)
	{
		porder[tmpi] = tmpi;
	}
	for (tmpi = 0; tmpi < MAX_TROOPS_COUNT; tmpi++)
	{
		for (tmpj = tmpi + 1; tmpj < MAX_TROOPS_COUNT; tmpj++)
		{
			if (fight_sortconf(pArmy, porder[tmpi], porder[tmpj], type, value) == 1)
			{
				tmpindex = porder[tmpi];
				porder[tmpi] = porder[tmpj];
				porder[tmpj] = tmpindex;
			}
		}
	}
	return 0;
}

// 检测普通战斗距离是否足够
int fight_check_attackrange(int actor_pos, short corps, char level, char source_position, char target_position)
{
	int range;
	if (actor_pos >= MAX_TROOPS_COUNT)
	{
		range = source_position - target_position;
	}
	else
	{
		range = target_position - source_position;
	}

	if (range <= 0)
		return -1;
	char attack_range = troop_get_attack_range(corps, level);
	if (range <= attack_range)
		return 0;
	return -1;
}

// 检测目标合法性
int fight_check_target(int actor_pos, char target_pos, int effobject, TroopUnit *pSourceTroop, TroopUnit *pTargetTroop, short act_type)
{
	if (pTargetTroop == NULL)
		return -1;
	if (act_type == 0)
	{
		// 普通攻击计算距离
		if (fight_check_attackrange(actor_pos, pSourceTroop->corps, pSourceTroop->level, pSourceTroop->fight_position, pTargetTroop->fight_position) < 0)
			return -1;
	}
	if (effobject == EFF_MYSELF)
	{
		if (target_pos != actor_pos )
			return -1;
	}
	else
	{
		// 仅仅对生者用的，却是死的
		if (pTargetTroop->isdead)
			return -1;
	}
	return 0;
}

// 根据基本的目标数量 计算其他目标, 并且重新计算目标的数量(战场上可能没那么多人给你打)
// 1要取得哪一方的目标 2战场 3攻击类型 4修改角色中的目标 5修改目标的位置 6修改目标的数量
int fight_getanyattr(int actor_pos, int *ptarget_pos, int *ptargetnum, short act_type)
{
	int tmpi;
	int count;

	int nLeftTargetNum;
	int idx[MAX_TROOPS_COUNT];

	int order_by_random[MAX_TROOPS_COUNT]; // 随机
	int order_by_restrain[MAX_TROOPS_COUNT]; // 按攻击
	int order_by_statdef[MAX_TROOPS_COUNT]; // 按防御

	int * p_order;

	int effobject;
	char target_side;

	if (ptarget_pos == NULL || ptargetnum == NULL)
		return -1;

	Army *SourceArmy;
	Army *TargetArmy;
	TroopUnit *pSourceTroop;
	int pos;
	if (actor_pos >= MAX_TROOPS_COUNT)
	{
		SourceArmy = g_Fight.pDefenceArmy;
		pos = actor_pos - MAX_TROOPS_COUNT;
		target_side = 0;
	}
	else
	{
		SourceArmy = g_Fight.pAttackArmy;
		pos = actor_pos;
		target_side = 1;
	}
	pSourceTroop = &SourceArmy->troops[pos];

	effobject = 0;	// func(act_type);
//	target_side = 1;	// func(effobject,actor_pos);
	*ptargetnum = 1;	// func(act_type);

	if (target_side == 0)
	{
		TargetArmy = g_Fight.pAttackArmy;
		nLeftTargetNum = g_Fight.attack_num;
	}
	else
	{
		TargetArmy = g_Fight.pDefenceArmy;
		nLeftTargetNum = g_Fight.defence_num;
	}

	for (tmpi = 0; tmpi < MAX_TROOPS_COUNT; tmpi++)
	{
		idx[tmpi] = tmpi;

		order_by_random[tmpi] = tmpi;
		order_by_restrain[tmpi] = tmpi;
		order_by_statdef[tmpi] = tmpi;
	}

	for (tmpi = 0; tmpi < MAX_TROOPS_COUNT; tmpi++)
	{
		// 随机排序的那种
		int tmp_index = rand() % MAX_TROOPS_COUNT;
		int tmp_value = order_by_random[tmpi];
		order_by_random[tmpi] = order_by_random[tmp_index];
		order_by_random[tmp_index] = tmp_value;
	}
	
	fight_sortorder(TargetArmy, order_by_restrain, 1, troop_get_attacktype(pSourceTroop->corps, pSourceTroop->level));
//	fight_sortorder(TargetArmy, order_by_statdef, 6);

	if (*ptargetnum >= nLeftTargetNum)
	{
		// 如果全部选中就不需要洗牌
		*ptargetnum = nLeftTargetNum;
	}

//	p_order = order_by_random;
	p_order = order_by_restrain;
//	p_order = order_by_statdef;

	memcpy(idx, p_order, sizeof(int)* MAX_TROOPS_COUNT);

	count = 0;
	for (tmpi = 0; tmpi < MAX_TROOPS_COUNT; tmpi++)
	{
		if (idx[tmpi] < 0)
			continue;
		if (fight_check_target(actor_pos, idx[tmpi] + MAX_TROOPS_COUNT * target_side, effobject, pSourceTroop, &TargetArmy->troops[idx[tmpi]], act_type) == 0)
		{
			ptarget_pos[count] = idx[tmpi] + MAX_TROOPS_COUNT * target_side;
			if (ptarget_pos[count] == actor_pos && effobject != EFF_MYSELF)
				continue;
			count++;
			if (count >= *ptargetnum)
				break;
		}
	}
	if (count == 0)
		return -1;
	else if (count < *ptargetnum)
		*ptargetnum = count;
	return 0;
}

int fight_check_right(int actor_pos, int *ptarget_pos, int *ptargetnum, short act_type )
{
	for (int tmpi = 0; tmpi < MAX_TROOPS_COUNT; tmpi++)
	{
		ptarget_pos[tmpi] = -1;
	}
	if (act_type < 0)
	{
		return -1;
	}
	int rtn = fight_getanyattr(actor_pos, ptarget_pos, ptargetnum, act_type);
	if (ptargetnum <= 0)
		return -1;
	return rtn;
	// 返回值<0表示目标非法，返回值==0表示完全通过，返回值==1表示需要智能查询
}

int fight_skillone(int actor_pos)
{
	int nResult;
	int tmpi;

	int target_pos[MAX_TROOPS_COUNT] = { 0 };

	char act_side;
	int target_num;

	int checkok;

	Army *SourceArmy;
	Army *TargetArmy;
	TroopUnit *pSourceTroop;
	int pos;
	if (actor_pos >= MAX_TROOPS_COUNT)
	{
		SourceArmy = g_Fight.pDefenceArmy;
		TargetArmy = g_Fight.pAttackArmy;
		pos = actor_pos - MAX_TROOPS_COUNT;
		act_side = 1;
	}
	else
	{
		SourceArmy = g_Fight.pAttackArmy;
		TargetArmy = g_Fight.pDefenceArmy;
		pos = actor_pos;
		act_side = 0;
	}
	pSourceTroop = &SourceArmy->troops[pos];

	if (pSourceTroop->isdead)
		return -1;
//	write_gamelog("F----actor_pos:%d, actor_type:%d, curgrade:%d, gradeleft:%d,fight_index:%d,turn:%d,attr:%d,type:%d,attack_num:%d,defense_num:%d", actor_pos, pAttr_Source->act_type[curgrade], curgrade, pAttr_Source->gradeleft, fight_index, g_fight[fight_index].cur_turns, g_fight[fight_index].attr, g_fight[fight_index].type, g_fight[fight_index].attack_num, g_fight[fight_index].defense_num);
	// 检查攻击的动作是否合法,并且获得影响的目标个数（初步的个数）(这个函数计算出了攻击的个数)
	checkok = fight_check_right(actor_pos, target_pos, &target_num, -1);
	if (checkok < 0)
	{
		return -1;
	}
	for (tmpi = 0; tmpi < target_num; tmpi++)
	{
		// 攻击方( 攻击的类型 )
		nResult = fight_skill_result(actor_pos, target_pos[tmpi], target_num, tmpi);

		// 判断战斗是否结束
		if (g_Fight.attack_num <= 0)
		{
			// 防守方胜利
			return FIGHT_LOSE;
		}
		else if (g_Fight.defence_num <= 0)
		{
			// 攻击方胜利
			return FIGHT_WIN;
		}
	}
	return 0;
}

int fight_actone(int actor_pos)
{
	int tmpi;

	int target_pos[MAX_TROOPS_COUNT] = { 0 };

	char act_side;
	int target_num;

	int checkok;

	Army *SourceArmy;
	Army *TargetArmy;
	TroopUnit *pSourceTroop;
	int pos;
	if (actor_pos >= MAX_TROOPS_COUNT)
	{
		SourceArmy = g_Fight.pDefenceArmy;
		TargetArmy = g_Fight.pAttackArmy;
		pos = actor_pos - MAX_TROOPS_COUNT;
		act_side = 1;
	}
	else
	{
		SourceArmy = g_Fight.pAttackArmy;
		TargetArmy = g_Fight.pDefenceArmy;
		pos = actor_pos;
		act_side = 0;
	}
	pSourceTroop = &SourceArmy->troops[pos];

	if (pSourceTroop->isdead)
		return -1;
	//	write_gamelog("F----actor_pos:%d, actor_type:%d, curgrade:%d, gradeleft:%d,fight_index:%d,turn:%d,attr:%d,type:%d,attack_num:%d,defense_num:%d", actor_pos, pAttr_Source->act_type[curgrade], curgrade, pAttr_Source->gradeleft, fight_index, g_fight[fight_index].cur_turns, g_fight[fight_index].attr, g_fight[fight_index].type, g_fight[fight_index].attack_num, g_fight[fight_index].defense_num);
	// 检查攻击的动作是否合法,并且获得影响的目标个数（初步的个数）(这个函数计算出了攻击的个数)
	checkok = fight_check_right(actor_pos, target_pos, &target_num, 0);
	if (checkok < 0)
	{
		return -1;
	}
	for (tmpi = 0; tmpi < target_num; tmpi++)
	{
		// 攻击方( 攻击的类型 )
		fight_attack_result(actor_pos, target_pos[tmpi], target_num, tmpi);

		// 判断战斗是否结束
		if (g_Fight.attack_num <= 0)
		{
			// 防守方胜利
			return FIGHT_LOSE;
		}
		else if (g_Fight.defence_num <= 0)
		{
			// 攻击方胜利
			return FIGHT_WIN;
		}
	}
	return 0;
}

int fight_oneturn()
{
	int tmpi;
	int winner;
	int curpos;

	// 计算移动的次序
	for (tmpi = 0; tmpi < MAX_TROOPS_COUNT*2; tmpi++)
	{
		// 计算出当前要处理的角色位置
		curpos = g_Fight.move_sort[tmpi];
		if (curpos < 0)
			continue;
		fight_moveone(curpos);
	}

	// 计算技能的次序
	winner = 0;
	for (tmpi = 0; tmpi < MAX_TROOPS_COUNT * 2; tmpi++)
	{
		// 计算出当前要处理的角色位置
		curpos = g_Fight.attack_sort[tmpi];
		if (curpos < 0)
			continue;
		winner = fight_skillone(curpos);
		if (winner > 0)
			break;
	}

	if (winner <= 0)
	{
		// 计算攻击的次序
		for (tmpi = 0; tmpi < MAX_TROOPS_COUNT * 2; tmpi++)
		{
			// 计算出当前要处理的角色位置
			curpos = g_Fight.attack_sort[tmpi];
			if (curpos < 0)
				continue;

			winner = fight_actone(curpos);

			if (winner > 0)
				break;
		}
	}

//	winner = fight_godact(fight_index, winner);
	// 重置战斗中所有角色的动作以及行动次数
//	fight_reset(fight_index);

	g_Fight.cur_turns++;

	if (winner > 0)
	{
//		fight_destroy(winner);
	}

	return winner;
}
