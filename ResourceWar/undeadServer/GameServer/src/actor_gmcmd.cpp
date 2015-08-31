#include "actor.h"
#include "gmcmd.h"
#include "system.h"
#include "city.h"
#include "army.h"
#include "fight.h"
#include "fight_msg.h"
#include "building.h"
#include "hero.h"
#include "troop.h"

extern Actor *g_actors;
extern int g_actornum;
extern int g_maxactornum;

extern TroopInfo **g_TroopInfo;
extern short g_TroopCount;

extern HeroInfo *g_HeroInfo;
extern short g_HeroCount;

extern Army *g_Army;
extern int g_maxArmyNum;

extern Fight g_Fight;

int actor_command( int actor_index, short cmd, int *pValue, char *pMsg )
{
	int target_index;
	if ( pValue[3] > 0 &&
		cmd != GMC_SC )
	{
		target_index = actor_getindex_withid( pValue[3] );
	}
	else
	{
		target_index = actor_index;
	}

	if ( actor_index >= 0 )
	{
#ifndef	WIN32
		if ( g_actors[actor_index].admin < 100 )
		{
			system_talkto( actor_index, "Access denied!" );
			return GMERROR_ACCESS_DENIED;
		}
#endif // DEBUG
		if ( pMsg )
			write_gamelog( "<GmProcess>cmd:%d value:%d,%d,%d,%d,%s [%s(%d)]", cmd, pValue[0], pValue[1], pValue[2], pValue[3], pMsg, g_actors[actor_index].name, g_actors[actor_index].actorid );
		else
			write_gamelog( "<GmProcess>cmd:%d value:%d,%d,%d,%d,NULL [%s(%d)]", cmd, pValue[0], pValue[1], pValue[2], pValue[3], g_actors[actor_index].name, g_actors[actor_index].actorid );
	}
	else
	{
		if ( pMsg )
			write_gamelog( "<GmTools>cmd:%d value:%d,%d,%d,%d,%s", cmd, pValue[0], pValue[1], pValue[2], pValue[3], pMsg );
		else
			write_gamelog( "<GmTools>cmd:%d value:%d,%d,%d,%d,NULL", cmd, pValue[0], pValue[1], pValue[2], pValue[3] );
	}

	switch ( cmd )
	{
	case GMC_TEST:
		break;
	case GMC_SC:
		break;
	case GMC_SYSTALK:
		break;
	case GMC_EXP:
		break;
	case GMC_RES:
		city_change_res( city_getptr( actor_index ), (CityRes)pValue[0], pValue[1], PATH_GM );
		break;
	case GMC_FIGHT:
		if (pValue[0] == 0)
		{

			int atk_army_index = army_create(g_actors[actor_index].actorid, 0, 1, "Player");
			short corps_array[5] = {1,2,3,4,5};
			TroopUnit troopUnit;
			int herokind;

			for (int tmpi = 0; tmpi < 5; tmpi++)
			{
				memset(&troopUnit, 0, sizeof(TroopUnit));
				herokind = 1;
				troopUnit.hero_kind = herokind;
				sprintf(troopUnit.hero_name, "Hero%d", tmpi);
				troopUnit.hero_shape = g_HeroInfo[herokind].shape;
				troopUnit.hero_level = 1;
				memcpy(troopUnit.hero_skill, g_HeroInfo[herokind].skill, sizeof(short)* 4);

				short corps = corps_array[tmpi];
				char level = 1;
				troopUnit.corps = corps;
				troopUnit.level = level;
				troopUnit.attack = g_TroopInfo[corps][level].attack;
				troopUnit.defance = g_TroopInfo[corps][level].defance;
				troopUnit.hit = g_TroopInfo[corps][level].hit;
				troopUnit.dodge = g_TroopInfo[corps][level].dodge;
				troopUnit.crit = g_TroopInfo[corps][level].crit;
				troopUnit.agl = g_TroopInfo[corps][level].agl;
				troopUnit.crit_harm = g_TroopInfo[corps][level].crit_harm;
				troopUnit.crit_reduce = g_TroopInfo[corps][level].crit_reduce;

				troopUnit.curnumber = troopUnit.maxnumber = 1500;
				troopUnit.fightnumber = troopUnit.curnumber;

				army_addtroop(atk_army_index, &troopUnit);
			}


			int def_army_index = army_create(0, 1, 0, "Computer");
			if (pValue[1] <= 0)
				pValue[1] = 1;
			army_addenemy(def_army_index, pValue[1]);
			int fight_recodeid = fight_create(atk_army_index, def_army_index, 0);
			SLK_NetS_FightMsgInfo Value = {};
			Value.m_recordid = fight_recodeid;
			Value.m_posx = rand() % 100 + 10;
			Value.m_posy = rand() % 100 + 10;
			Value.m_status = g_Fight.winner;
			Value.m_target_type = MAX_FIGHTMSG_TYPE_FIGHT_CITY;
			Value.m_target_id = g_Army[atk_army_index].cityid;
			Value.m_target_posx = g_Army[atk_army_index].from_posx;
			Value.m_target_posy = g_Army[atk_army_index].from_posy;
			sprintf( Value.m_title, "you attack %s", g_Army[atk_army_index].armyname );
			Value.m_title_length = strlen( Value.m_title );
			fight_msg_send(g_actors[actor_index].actorid, &Value);
			fight_send(actor_index, fight_recodeid);
			char szMsg[MAX_PATH];
			sprintf(szMsg, "Fight start (RecID:%d) commmmmmmmmmmmmmmmm!", fight_recodeid);
			system_talkto(actor_index, szMsg );
		}
		else
		{
			int fight_recodeid = pValue[0];
			fight_send(actor_index, fight_recodeid);
			char szMsg[MAX_PATH];
			sprintf(szMsg, "Fight start (RecID:%d) commmmmmmmmmmmmmmmm!", fight_recodeid);
			system_talkto(actor_index, szMsg);
		}
		break;
	case GMC_CITY:
		city_save( NULL );
		break;
	default:
		break;
	}
	return 0;
}
