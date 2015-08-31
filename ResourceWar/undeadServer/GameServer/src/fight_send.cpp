#ifdef WIN32
#include <winsock2.h>
#include <windows.h>
#endif
#include <stdio.h>
#include <string.h>
#include "define.h"
#include "actor.h"
#include "fight.h"
#include "troop.h"
#include "server_netsend_auto.h"
#include "actor_send.h"

extern Fight g_Fight;

void fight_send_fightin(int actor_index)
{
	SLK_NetS_FightIn Value = {};
	for (int tmpi = 0; tmpi < g_Fight.pAttackArmy->troops_count; tmpi++)
	{
		Value.m_corps = g_Fight.pAttackArmy->troops[tmpi].corps;
		Value.m_isenemy = 0;
		Value.m_curnumber = g_Fight.pAttackArmy->troops[tmpi].fightnumber;
		Value.m_maxnumber = g_Fight.pAttackArmy->troops[tmpi].maxnumber;
		Value.m_ratenumber = 100;
		strcpy(Value.m_name, g_Fight.pAttackArmy->troops[tmpi].hero_name);
		Value.m_namelen = (int)strlen(g_Fight.pAttackArmy->troops[tmpi].hero_name);
		Value.m_posion = troop_get_entry_position(g_Fight.pAttackArmy->troops[tmpi].corps, g_Fight.pAttackArmy->troops[tmpi].level, 0);
		Value.m_shape = g_Fight.pAttackArmy->troops[tmpi].hero_shape;
		Value.m_srcpos = tmpi;
		netsend_fightin_S(actor_index, SENDTYPE_ACTOR, &Value);
	}

	for (int tmpi = 0; tmpi < g_Fight.pDefenceArmy->troops_count; tmpi++)
	{
		Value.m_corps = g_Fight.pDefenceArmy->troops[tmpi].corps;
		Value.m_isenemy = 1;
		Value.m_curnumber = g_Fight.pDefenceArmy->troops[tmpi].fightnumber;
		Value.m_maxnumber = g_Fight.pAttackArmy->troops[tmpi].maxnumber;
		Value.m_ratenumber = 100;
		strcpy(Value.m_name, g_Fight.pDefenceArmy->troops[tmpi].hero_name);
		Value.m_namelen = (int)strlen(g_Fight.pDefenceArmy->troops[tmpi].hero_name);
		Value.m_posion = troop_get_entry_position(g_Fight.pDefenceArmy->troops[tmpi].corps, g_Fight.pDefenceArmy->troops[tmpi].level, 1);
		Value.m_shape = g_Fight.pDefenceArmy->troops[tmpi].hero_shape;
		Value.m_srcpos = tmpi + MAX_TROOPS_COUNT;
		netsend_fightin_S(actor_index, SENDTYPE_ACTOR, &Value);
	}
}

void fight_send_fightset(int actor_index, short turn, char flag)
{
	SLK_NetS_FightSet Value = {};
	Value.m_turn = turn;
	Value.m_fightflag = flag;
	netsend_fightset_S(actor_index, SENDTYPE_ACTOR, &Value);
}

void fight_send_moveact(int actor_index)
{
	SLK_NetS_FightMove movValue = {};
	SLK_NetS_FightAction actValue = {};

	int lastmoveturn = 0;
	int lastactturn = 0;

	for (int turn = 0; turn < g_Fight.cur_turns; turn++)
	{
		fight_send_fightset(actor_index, turn, 0);
		for (int tmpi = lastmoveturn; tmpi < g_Fight.movstep; tmpi++)
		{
			lastmoveturn = tmpi;
			if (g_Fight.pMove[tmpi].turns > turn)
				break;
			movValue.m_srcpos = g_Fight.pMove[tmpi].srcpos;
			movValue.m_movstep = g_Fight.pMove[tmpi].movstep;
			netsend_fightmove_S(actor_index, SENDTYPE_ACTOR, &movValue);
		}

		for (int tmpi = lastactturn; tmpi < g_Fight.actstep; tmpi++)
		{
			lastactturn = tmpi;
			if (g_Fight.pAction[tmpi].turns > turn)
				break;
			actValue.m_srcpos = g_Fight.pAction[tmpi].srcpos;
			actValue.m_tgtpos = g_Fight.pAction[tmpi].tgtpos;
			actValue.m_action = g_Fight.pAction[tmpi].action;
			actValue.m_damage = g_Fight.pAction[tmpi].damage;
			actValue.m_srcstat = g_Fight.pAction[tmpi].srcstat;
			actValue.m_tgtstat = g_Fight.pAction[tmpi].tgtstat;
			actValue.m_nexttgt = g_Fight.pAction[tmpi].nexttgt;
			netsend_fightaction_S(actor_index, SENDTYPE_ACTOR, &actValue);
			write_gamelog("[%d][%d]LOG src:%d tgt:%d damage:%d", tmpi, g_Fight.pAction[tmpi].turns, actValue.m_srcpos, actValue.m_tgtpos, actValue.m_damage);

		}
	}
}

int fight_send(int actor_index, int fightid)
{
	int size;
	size = fight_read(fightid);
	if (size <= 0)
		return -1;
	if (fight_serialize_read(size) < 0)
		return -1;
	fight_send_fightset(actor_index, -1, 0);
	fight_send_fightin(actor_index);
	fight_send_moveact(actor_index);
	fight_send_fightset(actor_index, g_Fight.cur_turns, -1);
	return 0;
}
