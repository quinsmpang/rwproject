#include <stdio.h>
#include <string.h>
#include <math.h>
#include "define.h"
#include "actor.h"
#include "hero.h"
#include "fight.h"
#include "troop.h"
#include "global.h"

extern Global global;

extern TroopCorpsInfo *g_TroopCorpsInfo;
extern short g_TroopCorpsCount;

extern Fight g_Fight;

// Ӱ�켼�ܹ���������
int fight_skill_result(char actor_pos, char target_pos, char target_num, char curr_target)
{
	return 0;
}

// Ӱ��������������
int fight_attack_result(char actor_pos, char target_pos, char target_num, char curr_target)
{
	Army *SourceArmy;
	Army *TargetArmy;
	int pos;
	int tpos;
	int factor;
	char target_side;
	if (actor_pos >= MAX_TROOPS_COUNT)
	{
		SourceArmy = g_Fight.pDefenceArmy;
		pos = actor_pos - MAX_TROOPS_COUNT;
		if (target_pos >= MAX_TROOPS_COUNT)
			return -1;
		factor = g_Fight.defence_factor;
	}
	else
	{
		SourceArmy = g_Fight.pAttackArmy;
		pos = actor_pos;
		if (target_pos < MAX_TROOPS_COUNT)
			return -1;
		factor = g_Fight.attack_factor;
	}

	if (target_pos >= MAX_TROOPS_COUNT)
	{
		TargetArmy = g_Fight.pDefenceArmy;
		tpos = target_pos - MAX_TROOPS_COUNT;
		target_side = 1;
	}
	else
	{
		TargetArmy = g_Fight.pAttackArmy;
		tpos = target_pos;
		target_side = 0;
	}
	
	float sol = g_TroopCorpsInfo[SourceArmy->troops[pos].corps].sol_modulus;
	float hp = g_TroopCorpsInfo[SourceArmy->troops[pos].corps].hp_modulus;
	float Atk = g_TroopCorpsInfo[SourceArmy->troops[pos].corps].atk_modulus;
	float Crit = g_TroopCorpsInfo[SourceArmy->troops[pos].corps].crit_modulus;
	float critHarm = g_TroopCorpsInfo[SourceArmy->troops[pos].corps].critharm_modulus;
	float Hit = g_TroopCorpsInfo[SourceArmy->troops[pos].corps].hit_modulus;
	float Def = g_TroopCorpsInfo[SourceArmy->troops[pos].corps].def_modulus;
	float Agl = g_TroopCorpsInfo[SourceArmy->troops[pos].corps].agl_modulus;
	float Dodge = g_TroopCorpsInfo[SourceArmy->troops[pos].corps].dodge_modulus;
	float critReduce = g_TroopCorpsInfo[SourceArmy->troops[pos].corps].critreduce_modulus;

	// ս���� =   ����ȡ��������ʿ����* (sol/10) * (���� * ��hp/10�� +    ����*Atk + ����*Crit + �����˺� * critHarm + ����*Hit + ����*Def +  ����*Agl  +  ��*Dodge  + ��������*critReduce) / 10000 ��
	// �˺����� = ����ȡ����ʣ��ʿ����* (sol/10000) * (���� * ��hp/10�� + ����*Atk + ����*Crit + �����˺� * critHarm + ����*Hit + ����*Def +  ����*Agl  +  ��*Dodge  + ��������*critReduce) / 10000 ��
	// �˺� = ��A����^para3+para1��/(B����^para3+para1)*(��A����+para2��/(B����+para2��)^para4*A�˺�����

	int source_hp = troop_get_hp(SourceArmy->troops[pos].corps, SourceArmy->troops[pos].level);
	int target_hp = troop_get_hp(TargetArmy->troops[tpos].corps, TargetArmy->troops[tpos].level);
	int damagevalue = 0;
	damagevalue = 1 + (int)((SourceArmy->troops[pos].fightnumber * (sol / 10000) * (source_hp * (hp / 10) +
		SourceArmy->troops[pos].attack * Atk + SourceArmy->troops[pos].crit * Crit + SourceArmy->troops[pos].crit_harm * critHarm + SourceArmy->troops[pos].hit * Hit + 
		SourceArmy->troops[pos].defance * Def + SourceArmy->troops[pos].agl * Agl + SourceArmy->troops[pos].dodge * Dodge + SourceArmy->troops[pos].crit_reduce * critReduce) / 10000));
	int damage = 0;
	float v1 = (pow((float)SourceArmy->troops[pos].attack, global.damage_para3) + global.damage_para1);
	float v2 = (pow((float)TargetArmy->troops[tpos].defance, global.damage_para3) + global.damage_para1);
	float v3 = v1 / v2;
	float v4 = pow((source_hp + global.damage_para2) / (target_hp + global.damage_para2), global.damage_para4);
	float v5 = v3*v4*damagevalue;
	damage = (int)((pow((float)SourceArmy->troops[pos].attack, global.damage_para3) + global.damage_para1) / (pow((float)TargetArmy->troops[tpos].defance, global.damage_para3) + global.damage_para1)*pow((source_hp + global.damage_para2) / (target_hp + global.damage_para2), global.damage_para4)*damagevalue);

	// �˺�����
	damage += (damage * factor) / 100;

	// �������ϵ��
	float restrain = 1.0f;
	damage = (int)(restrain * damage);

	float dodge_odds = pow((TargetArmy->troops[tpos].dodge + global.dodge_para1 + TargetArmy->troops[tpos].level * global.dodge_para2), global.dodge_para3) / pow((SourceArmy->troops[pos].hit + global.dodge_para1 + SourceArmy->troops[pos].level * global.dodge_para2), global.dodge_para3) - global.dodge_para4;
	if (dodge_odds < 0.15f)
		dodge_odds = 0.15f;
	if (dodge_odds > 1.0f)
		dodge_odds = 1.0f;

	float crit_odds = pow(SourceArmy->troops[pos].crit + global.crit_para1 + SourceArmy->troops[pos].level * global.crit_para2, global.crit_para3) / pow(TargetArmy->troops[tpos].agl + global.crit_para1 + TargetArmy->troops[tpos].level * global.crit_para2, global.crit_para3) - global.crit_para4;
	if (crit_odds > 1.0f)
		crit_odds = 1.0f;

	if (rand() % 100 < (int)(crit_odds * 100 ))
	{
		// ������Ч
		// �����˺�����= ��100 + A�����˺� �C B�������⣩/100
		float crit_harm = (100 + (float)SourceArmy->troops[pos].crit_harm - (float)TargetArmy->troops[tpos].crit_reduce) / 100;
		damage = (int)(damage * crit_harm);
	}

	if (rand() % 100 < (int)(dodge_odds * 100))
	{
		// ����Ч
		damage = damage / 2;
	}

	// �˺�������¸���5%
	damage += damage * (5-(rand() % 11)) / 100;

	int deadnumber = damage / troop_get_hp(TargetArmy->troops[tpos].corps, TargetArmy->troops[tpos].level );
	// ʵ���˺�����
	TargetArmy->troops[tpos].curnumber -= deadnumber;
	if (TargetArmy->troops[tpos].curnumber <= 0)
	{
		TargetArmy->troops[tpos].curnumber = 0;
		TargetArmy->troops[tpos].isdead = 1;
		if (target_side)
			g_Fight.defence_num--;
		else
			g_Fight.attack_num--;
	}
	TargetArmy->troops[tpos].stat = 0;

	g_Fight.pAction[g_Fight.actstep].turns = g_Fight.cur_turns;
	g_Fight.pAction[g_Fight.actstep].action = troop_get_attackaction(SourceArmy->troops[pos].corps, SourceArmy->troops[pos].level );
	g_Fight.pAction[g_Fight.actstep].damage = deadnumber;
	g_Fight.pAction[g_Fight.actstep].srcpos = actor_pos;
	g_Fight.pAction[g_Fight.actstep].tgtpos = target_pos;
	g_Fight.pAction[g_Fight.actstep].srcstat = 0;
	g_Fight.pAction[g_Fight.actstep].tgtstat = 0;
	if ( curr_target+1 < target_num )
		g_Fight.pAction[g_Fight.actstep].nexttgt = 1;
	else
		g_Fight.pAction[g_Fight.actstep].nexttgt = 0;

	write_gamelog("[%d][%d]Attack src:%d tgt:%d damage:%d/%d dead:%d", g_Fight.actstep,g_Fight.cur_turns, actor_pos, target_pos, deadnumber, TargetArmy->troops[tpos].curnumber, TargetArmy->troops[tpos].isdead);

	g_Fight.actstep++;
	return 0;
}

