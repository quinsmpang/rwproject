#ifndef __NETSEND_AUTO_H0
#define __NETSEND_AUTO_H0

#include "server_structsend_auto.h"
int netsend_login_S( int actor_index, char send_type, SLK_NetS_Login *pValue );
int netsend_list_S( int actor_index, char send_type, SLK_NetS_List *pValue );
int netsend_create_S( int actor_index, char send_type, SLK_NetS_Create *pValue );
int netsend_enterinfo_S( int actor_index, char send_type, SLK_NetS_EnterInfo *pValue );
int netsend_delete_S( int actor_index, char send_type, SLK_NetS_Delete *pValue );
int netsend_heart_S( int actor_index, char send_type, SLK_NetS_Heart *pValue );
int netsend_notify_S( int actor_index, char send_type, SLK_NetS_Notify *pValue );
int netsend_talk_S( int actor_index, char send_type, SLK_NetS_Talk *pValue );
int netsend_fightin_S( int actor_index, char send_type, SLK_NetS_FightIn *pValue );
int netsend_fightset_S( int actor_index, char send_type, SLK_NetS_FightSet *pValue );
int netsend_fightmove_S( int actor_index, char send_type, SLK_NetS_FightMove *pValue );
int netsend_fightaction_S( int actor_index, char send_type, SLK_NetS_FightAction *pValue );
int netsend_actorinfo_S( int actor_index, char send_type, SLK_NetS_ActorInfo *pValue );
int netsend_cityresinfo_S( int actor_index, char send_type, SLK_NetS_CityResInfo *pValue );
int netsend_citybuildinglist_S( int actor_index, char send_type, SLK_NetS_CityBuildingList *pValue );
int netsend_addmapunit_S( char *databuf, int size, SLK_NetS_AddMapUnit *pValue );
int netsend_delmapunit_S( char *databuf, int size, SLK_NetS_DelMapUnit *pValue );
int netsend_itemuse_S( int actor_index, char send_type, SLK_NetS_ItemUse *pValue );
int netsend_itemput_S( int actor_index, char send_type, SLK_NetS_ItemPut *pValue );
int netsend_itemsettle_S( int actor_index, char send_type, SLK_NetS_ItemSettle *pValue );
int netsend_lostitem_S( int actor_index, char send_type, SLK_NetS_LostItem *pValue );
int netsend_getitem_S( int actor_index, char send_type, SLK_NetS_GetItem *pValue );
int netsend_itemlist_S( int actor_index, char send_type, SLK_NetS_ItemList *pValue );
int netsend_iteminfo_S( int actor_index, char send_type, SLK_NetS_ItemInfo *pValue );
int netsend_buildtimeinfo_S( int actor_index, char send_type, SLK_NetS_BuildTimeInfo *pValue );
int netsend_buildtimeinfolist_S( int actor_index, char send_type, SLK_NetS_BuildTimeInfoList *pValue );
int netsend_cityreschangeinfo_S( int actor_index, char send_type, SLK_NetS_CityResChangeInfo *pValue );
int netsend_buildingbaseinfo_S( int actor_index, char send_type, SLK_NetS_BuildingBaseInfo *pValue );
int netsend_buildingabilityinfo_S( int actor_index, char send_type, SLK_NetS_BuildingAbilityInfo *pValue );
int netsend_worldmapinfo_S( int actor_index, char send_type, SLK_NetS_WorldMapInfo *pValue );
int netsend_mapunitcorrdinate_S( char *databuf, int size, SLK_NetS_MapUnitCorrdinate *pValue );
int netsend_marchroute_S( int actor_index, char send_type, SLK_NetS_MarchRoute *pValue );
int netsend_outsidemapinfo_S( int actor_index, char send_type, SLK_NetS_OutsideMapInfo *pValue );
int netsend_mailsimpleinfo_S( int actor_index, char send_type, SLK_NetS_MailSimpleInfo *pValue );
int netsend_maillist_S( int actor_index, char send_type, SLK_NetS_MailList *pValue );
int netsend_mailfullinfo_S( int actor_index, char send_type, SLK_NetS_MailFullInfo *pValue );
int netsend_fightmsgsimpleinfo_S( int actor_index, char send_type, SLK_NetS_FightMsgSimpleInfo *pValue );
int netsend_fightmsglist_S( int actor_index, char send_type, SLK_NetS_FightMsgList *pValue );
int netsend_fightmsginfo_S( int actor_index, char send_type, SLK_NetS_FightMsgInfo *pValue );
int netsend_buildingabilitylist_S( int actor_index, char send_type, SLK_NetS_BuildingAbilityList *pValue );
int netsend_chatmasklist_S( int actor_index, char send_type, SLK_NetS_ChatMaskList *pValue );
int netsend_troopinfo_S( int actor_index, char send_type, SLK_NetS_TroopInfo *pValue );
int netsend_troopinfolist_S( int actor_index, char send_type, SLK_NetS_TroopInfoList *pValue );
int netsend_heroinfolist_S( int actor_index, char send_type, SLK_NetS_HeroList *pValue );
int netsend_heroinfo_S( int actor_index, char send_type, SLK_NetS_HeroInfo *pValue );
int netsend_citybattleinfo_S( int actor_index, char send_type, SLK_NetS_CityBattleInfo *pValue );

int netsend_logined_S( int actor_index, char send_type, SLK_NetU_Logined *pValue );
int netsend_timer_S( int actor_index, char send_type, char *pValue );

#endif
