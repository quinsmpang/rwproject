#include <string.h>
#include "server_structsend_auto.h"
#include "actor_send.h"

int struct_NetU_Logined_send( char **pptr, int *psize, SLK_NetU_Logined *pValue )
{
	int tmpi = 0;

	LKSET_DWORD_SEND( (*pptr), &pValue->m_result, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_client_index, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_authid, (*psize) );
	LKSET_MEM_SEND( (*pptr), pValue->m_userid, 21*sizeof(char), (*psize) );
	LKSET_MEM_SEND( (*pptr), pValue->m_username, 51*sizeof(char), (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_usertype, (*psize) );
	LKSET_BYTE_SEND( (*pptr), &pValue->m_checkflag, (*psize) );
	LKSET_BYTE_SEND( (*pptr), &pValue->m_isdeep, (*psize) );
	LKSET_MEM_SEND( (*pptr), pValue->m_access_token, 65*sizeof(char), (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_expires_in, (*psize) );
	LKSET_MEM_SEND( (*pptr), pValue->m_refresh_token, 65*sizeof(char), (*psize) );
	LKSET_MEM_SEND( (*pptr), pValue->m_deviceid, 65*sizeof(char), (*psize) );
	return 0;
}

int struct_NetS_Login_send( char **pptr, int *psize, SLK_NetS_Login *pValue )
{
	int tmpi = 0;

	LKSET_DWORD_SEND( (*pptr), &pValue->m_result, (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_username_length, (*psize) );
	if( pValue->m_username_length > 0 && pValue->m_username_length <= 51 )
		LKSET_MEM_SEND( (*pptr), pValue->m_username, pValue->m_username_length*sizeof(char), (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_token_length, (*psize) );
	if( pValue->m_token_length > 0 && pValue->m_token_length <= 256 )
		LKSET_MEM_SEND( (*pptr), pValue->m_token, pValue->m_token_length*sizeof(char), (*psize) );
	return 0;
}

int struct_ListInfo_send( char **pptr, int *psize, SLK_ListInfo *pValue )
{
	int tmpi = 0;

	LKSET_DWORD_SEND( (*pptr), &pValue->m_actorid, (*psize) );
	LKSET_MEM_SEND( (*pptr), pValue->m_name, 22*sizeof(char), (*psize) );
	LKSET_BYTE_SEND( (*pptr), &pValue->m_aclass, (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_level, (*psize) );
	LKSET_BYTE_SEND( (*pptr), &pValue->m_offset, (*psize) );
	LKSET_BYTE_SEND( (*pptr), &pValue->m_lockstat, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_delete_stoptime, (*psize) );
	return 0;
}

int struct_NetS_List_send( char **pptr, int *psize, SLK_NetS_List *pValue )
{
	int tmpi = 0;

	LKSET_BYTE_SEND( (*pptr), &pValue->m_actor_num, (*psize) );
	for( tmpi = 0; tmpi < pValue->m_actor_num; tmpi++ )
	{
		struct_ListInfo_send( pptr, psize, &pValue->m_listinfo[tmpi] );
	}
	return 0;
}

int struct_NetS_Create_send( char **pptr, int *psize, SLK_NetS_Create *pValue )
{
	int tmpi = 0;

	LKSET_BYTE_SEND( (*pptr), &pValue->m_result, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_actorid, (*psize) );
	return 0;
}

int struct_NetS_EnterInfo_send( char **pptr, int *psize, SLK_NetS_EnterInfo *pValue )
{
	int tmpi = 0;

	LKSET_DWORD_SEND( (*pptr), &pValue->m_actorid, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_data_int, (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_data_short, (*psize) );
	LKSET_BYTE_SEND( (*pptr), &pValue->m_data_char, (*psize) );
	return 0;
}

int struct_NetS_Delete_send( char **pptr, int *psize, SLK_NetS_Delete *pValue )
{
	int tmpi = 0;

	LKSET_DWORD_SEND( (*pptr), &pValue->m_result, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_actorid, (*psize) );
	return 0;
}

int struct_NetS_Heart_send( char **pptr, int *psize, SLK_NetS_Heart *pValue )
{
	int tmpi = 0;

	LKSET_MEM_SEND( (*pptr), pValue->m_value, 2*sizeof(int), (*psize) );
	return 0;
}

int struct_NetS_Notify_send( char **pptr, int *psize, SLK_NetS_Notify *pValue )
{
	int tmpi = 0;

	LKSET_WORD_SEND( (*pptr), &pValue->m_msgid, (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_valuenum, (*psize) );
	if( pValue->m_valuenum > 0 && pValue->m_valuenum <= 10 )
		LKSET_MEM_SEND( (*pptr), pValue->m_value, pValue->m_valuenum*sizeof(int), (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_msg_length, (*psize) );
	if( pValue->m_msg_length > 0 && pValue->m_msg_length <= 256 )
		LKSET_MEM_SEND( (*pptr), pValue->m_msg, pValue->m_msg_length*sizeof(char), (*psize) );
	return 0;
}

int struct_NetS_Talk_send( char **pptr, int *psize, SLK_NetS_Talk *pValue )
{
	int tmpi = 0;

	LKSET_BYTE_SEND( (*pptr), &pValue->m_type, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_actorid, (*psize) );
	LKSET_MEM_SEND( (*pptr), pValue->m_name, 22*sizeof(char), (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_msg_length, (*psize) );
	if( pValue->m_msg_length > 0 && pValue->m_msg_length <= 256 )
		LKSET_MEM_SEND( (*pptr), pValue->m_msg, pValue->m_msg_length*sizeof(char), (*psize) );
	LKSET_BYTE_SEND( (*pptr), &pValue->m_shape, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_sendtime, (*psize) );
	return 0;
}

int struct_NetS_FightIn_send( char **pptr, int *psize, SLK_NetS_FightIn *pValue )
{
	int tmpi = 0;

	LKSET_BYTE_SEND( (*pptr), &pValue->m_srcpos, (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_shape, (*psize) );
	LKSET_BYTE_SEND( (*pptr), &pValue->m_namelen, (*psize) );
	if( pValue->m_namelen > 0 && pValue->m_namelen <= 22 )
		LKSET_MEM_SEND( (*pptr), pValue->m_name, pValue->m_namelen*sizeof(char), (*psize) );
	LKSET_BYTE_SEND( (*pptr), &pValue->m_isenemy, (*psize) );
	LKSET_BYTE_SEND( (*pptr), &pValue->m_posion, (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_corps, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_curnumber, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_maxnumber, (*psize) );
	LKSET_BYTE_SEND( (*pptr), &pValue->m_ratenumber, (*psize) );
	return 0;
}

int struct_NetS_FightSet_send( char **pptr, int *psize, SLK_NetS_FightSet *pValue )
{
	int tmpi = 0;

	LKSET_WORD_SEND( (*pptr), &pValue->m_turn, (*psize) );
	LKSET_BYTE_SEND( (*pptr), &pValue->m_fightflag, (*psize) );
	return 0;
}

int struct_NetS_FightMove_send( char **pptr, int *psize, SLK_NetS_FightMove *pValue )
{
	int tmpi = 0;

	LKSET_BYTE_SEND( (*pptr), &pValue->m_srcpos, (*psize) );
	LKSET_BYTE_SEND( (*pptr), &pValue->m_movstep, (*psize) );
	return 0;
}

int struct_NetS_FightAction_send( char **pptr, int *psize, SLK_NetS_FightAction *pValue )
{
	int tmpi = 0;

	LKSET_BYTE_SEND( (*pptr), &pValue->m_srcpos, (*psize) );
	LKSET_BYTE_SEND( (*pptr), &pValue->m_tgtpos, (*psize) );
	LKSET_BYTE_SEND( (*pptr), &pValue->m_action, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_damage, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_srcstat, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_tgtstat, (*psize) );
	LKSET_BYTE_SEND( (*pptr), &pValue->m_nexttgt, (*psize) );
	return 0;
}

int struct_ArmyCorrdinate_send( char **pptr, int *psize, SLK_ArmyCorrdinate *pValue )
{
	int tmpi = 0;

	LKSET_DWORD_SEND( (*pptr), &pValue->m_unit_index, (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_posx, (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_posy, (*psize) );
	return 0;
}

int struct_NetS_CityResInfo_send( char **pptr, int *psize, SLK_NetS_CityResInfo *pValue )
{
	int tmpi = 0;

	LKSET_DWORD_SEND( (*pptr), &pValue->m_wood, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_food, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_iron, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_mithril, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_gold, (*psize) );
	LKSET_BYTE_SEND( (*pptr), &pValue->m_towncenter_level, (*psize) );
	LKSET_BYTE_SEND( (*pptr), &pValue->m_enter_worldmap, (*psize) );
	return 0;
}

int struct_NetS_BuildingBaseInfo_send( char **pptr, int *psize, SLK_NetS_BuildingBaseInfo *pValue )
{
	int tmpi = 0;

	LKSET_BYTE_SEND( (*pptr), &pValue->m_kind, (*psize) );
	LKSET_BYTE_SEND( (*pptr), &pValue->m_level, (*psize) );
	LKSET_BYTE_SEND( (*pptr), &pValue->m_offset, (*psize) );
	return 0;
}

int struct_NetS_CityBuildingList_send( char **pptr, int *psize, SLK_NetS_CityBuildingList *pValue )
{
	int tmpi = 0;

	LKSET_WORD_SEND( (*pptr), &pValue->m_count, (*psize) );
	for( tmpi = 0; tmpi < pValue->m_count; tmpi++ )
	{
		struct_NetS_BuildingBaseInfo_send( pptr, psize, &pValue->m_list[tmpi] );
	}
	return 0;
}

int struct_NetS_ActorInfo_send( char **pptr, int *psize, SLK_NetS_ActorInfo *pValue )
{
	int tmpi = 0;

	LKSET_DWORD_SEND( (*pptr), &pValue->m_actorid, (*psize) );
	LKSET_MEM_SEND( (*pptr), pValue->m_name, 22*sizeof(char), (*psize) );
	LKSET_BYTE_SEND( (*pptr), &pValue->m_aclass, (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_shape, (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_level, (*psize) );
	LKSET_LONG_SEND( (*pptr), &pValue->m_experience, (*psize) );
	LKSET_LONG_SEND( (*pptr), &pValue->m_experience_max, (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_leadership, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_token, (*psize) );
	LKSET_BYTE_SEND( (*pptr), &pValue->m_viplevel, (*psize) );
	return 0;
}

int struct_NetS_ItemUse_send( char **pptr, int *psize, SLK_NetS_ItemUse *pValue )
{
	int tmpi = 0;

	LKSET_WORD_SEND( (*pptr), &pValue->m_itemoffset, (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_usenum, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_effres, (*psize) );
	return 0;
}

int struct_NetS_ItemPut_send( char **pptr, int *psize, SLK_NetS_ItemPut *pValue )
{
	int tmpi = 0;

	LKSET_WORD_SEND( (*pptr), &pValue->m_res_offset, (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_res_num, (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_target_offset, (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_target_num, (*psize) );
	return 0;
}

int struct_NetS_ItemSettle_send( char **pptr, int *psize, SLK_NetS_ItemSettle *pValue )
{
	int tmpi = 0;

	LKSET_WORD_SEND( (*pptr), &pValue->m_itemnum, (*psize) );
	if( pValue->m_itemnum > 0 && pValue->m_itemnum <= 100 )
		LKSET_MEM_SEND( (*pptr), pValue->m_itemoffset, pValue->m_itemnum*sizeof(short), (*psize) );
	return 0;
}

int struct_NetS_LostItem_send( char **pptr, int *psize, SLK_NetS_LostItem *pValue )
{
	int tmpi = 0;

	LKSET_WORD_SEND( (*pptr), &pValue->m_itemoffset, (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_itemnum, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_targetid, (*psize) );
	LKSET_BYTE_SEND( (*pptr), &pValue->m_path, (*psize) );
	return 0;
}

int struct_NetS_GetItem_send( char **pptr, int *psize, SLK_NetS_GetItem *pValue )
{
	int tmpi = 0;

	LKSET_WORD_SEND( (*pptr), &pValue->m_itemoffset, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_kind, (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_type, (*psize) );
	LKSET_BYTE_SEND( (*pptr), &pValue->m_color, (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_num, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_targetid, (*psize) );
	LKSET_BYTE_SEND( (*pptr), &pValue->m_path, (*psize) );
	return 0;
}

int struct_ItemSmpInfo_send( char **pptr, int *psize, SLK_ItemSmpInfo *pValue )
{
	int tmpi = 0;

	LKSET_WORD_SEND( (*pptr), &pValue->m_offset, (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_kind, (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_type, (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_num, (*psize) );
	LKSET_BYTE_SEND( (*pptr), &pValue->m_colorlevel, (*psize) );
	return 0;
}

int struct_NetS_ItemList_send( char **pptr, int *psize, SLK_NetS_ItemList *pValue )
{
	int tmpi = 0;

	LKSET_BYTE_SEND( (*pptr), &pValue->m_itemext, (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_kindnum, (*psize) );
	for( tmpi = 0; tmpi < pValue->m_kindnum; tmpi++ )
	{
		struct_ItemSmpInfo_send( pptr, psize, &pValue->m_item[tmpi] );
	}
	return 0;
}

int struct_ItemAttr_send( char **pptr, int *psize, SLK_ItemAttr *pValue )
{
	int tmpi = 0;

	LKSET_WORD_SEND( (*pptr), &pValue->m_type, (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_ability, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_value, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_addvalue, (*psize) );
	return 0;
}

int struct_NetS_ItemInfo_send( char **pptr, int *psize, SLK_NetS_ItemInfo *pValue )
{
	int tmpi = 0;

	LKSET_WORD_SEND( (*pptr), &pValue->m_itemoffset, (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_itemkind, (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_type, (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_level, (*psize) );
	LKSET_BYTE_SEND( (*pptr), &pValue->m_color_level, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_price, (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_attr_num, (*psize) );
	for( tmpi = 0; tmpi < pValue->m_attr_num; tmpi++ )
	{
		struct_ItemAttr_send( pptr, psize, &pValue->m_attr[tmpi] );
	}
	return 0;
}

int struct_NetS_BuildTimeInfo_send( char **pptr, int *psize, SLK_NetS_BuildTimeInfo *pValue )
{
	int tmpi = 0;

	LKSET_BYTE_SEND( (*pptr), &pValue->m_offset, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_time, (*psize) );
	LKSET_BYTE_SEND( (*pptr), &pValue->m_state, (*psize) );
	return 0;
}

int struct_NetS_BuildTimeInfoList_send( char **pptr, int *psize, SLK_NetS_BuildTimeInfoList *pValue )
{
	int tmpi = 0;

	LKSET_BYTE_SEND( (*pptr), &pValue->m_count, (*psize) );
	for( tmpi = 0; tmpi < pValue->m_count; tmpi++ )
	{
		struct_NetS_BuildTimeInfo_send( pptr, psize, &pValue->m_timeinfo[tmpi] );
	}
	LKSET_DWORD_SEND( (*pptr), &pValue->m_servertime, (*psize) );
	return 0;
}

int struct_NetS_CityResChangeInfo_send( char **pptr, int *psize, SLK_NetS_CityResChangeInfo *pValue )
{
	int tmpi = 0;

	LKSET_BYTE_SEND( (*pptr), &pValue->m_restype, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_resvalue, (*psize) );
	LKSET_BYTE_SEND( (*pptr), &pValue->m_path, (*psize) );
	return 0;
}

int struct_NetS_BuildingAbilityInfo_send( char **pptr, int *psize, SLK_NetS_BuildingAbilityInfo *pValue )
{
	int tmpi = 0;

	LKSET_BYTE_SEND( (*pptr), &pValue->m_offset, (*psize) );
	LKSET_MEM_SEND( (*pptr), pValue->m_value, 4*sizeof(int), (*psize) );
	return 0;
}

int struct_NetS_AddMapUnit_send( char **pptr, int *psize, SLK_NetS_AddMapUnit *pValue )
{
	int tmpi = 0;

	LKSET_DWORD_SEND( (*pptr), &pValue->m_unit_index, (*psize) );
	LKSET_BYTE_SEND( (*pptr), &pValue->m_type, (*psize) );
	LKSET_BYTE_SEND( (*pptr), &pValue->m_shape, (*psize) );
	LKSET_BYTE_SEND( (*pptr), &pValue->m_stat, (*psize) );
	LKSET_BYTE_SEND( (*pptr), &pValue->m_lastway, (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_posx, (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_posy, (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_tposx, (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_tposy, (*psize) );
	LKSET_BYTE_SEND( (*pptr), &pValue->m_namelen, (*psize) );
	if( pValue->m_namelen > 0 && pValue->m_namelen <= 22 )
		LKSET_MEM_SEND( (*pptr), pValue->m_name, pValue->m_namelen*sizeof(char), (*psize) );
	LKSET_BYTE_SEND( (*pptr), &pValue->m_level, (*psize) );
	LKSET_BYTE_SEND( (*pptr), &pValue->m_underfire_count, (*psize) );
	for( tmpi = 0; tmpi < pValue->m_underfire_count; tmpi++ )
	{
		struct_ArmyCorrdinate_send( pptr, psize, &pValue->m_underfire_frompos[tmpi] );
	}
	return 0;
}

int struct_NetS_DelMapUnit_send( char **pptr, int *psize, SLK_NetS_DelMapUnit *pValue )
{
	int tmpi = 0;

	LKSET_DWORD_SEND( (*pptr), &pValue->m_unit_index, (*psize) );
	return 0;
}

int struct_NetS_WorldMapInfo_send( char **pptr, int *psize, SLK_NetS_WorldMapInfo *pValue )
{
	int tmpi = 0;

	LKSET_WORD_SEND( (*pptr), &pValue->m_map_width, (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_map_height, (*psize) );
	LKSET_BYTE_SEND( (*pptr), &pValue->m_area_width, (*psize) );
	LKSET_BYTE_SEND( (*pptr), &pValue->m_area_height, (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_map_area_xnum, (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_map_area_ynum, (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_my_city_posx, (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_my_city_posy, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_my_city_unit_index, (*psize) );
	return 0;
}

int struct_NetS_MapUnitCorrdinate_send( char **pptr, int *psize, SLK_NetS_MapUnitCorrdinate *pValue )
{
	int tmpi = 0;

	LKSET_DWORD_SEND( (*pptr), &pValue->m_unit_index, (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_posx, (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_posy, (*psize) );
	return 0;
}

int struct_NetS_MarchRoute_send( char **pptr, int *psize, SLK_NetS_MarchRoute *pValue )
{
	int tmpi = 0;

	LKSET_DWORD_SEND( (*pptr), &pValue->m_from_unit_index, (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_from_posx, (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_from_posy, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_to_unit_index, (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_to_posx, (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_to_posy, (*psize) );
	return 0;
}

int struct_NetS_OutsideMapInfo_send( char **pptr, int *psize, SLK_NetS_OutsideMapInfo *pValue )
{
	int tmpi = 0;

	LKSET_WORD_SEND( (*pptr), &pValue->m_map_width, (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_map_height, (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_my_city_posx, (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_my_city_posy, (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_match_city_posx, (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_match_city_posy, (*psize) );
	return 0;
}

int struct_NetS_MailSimpleInfo_send( char **pptr, int *psize, SLK_NetS_MailSimpleInfo *pValue )
{
	int tmpi = 0;

	LKSET_LONG_SEND( (*pptr), &pValue->m_mailid, (*psize) );
	LKSET_BYTE_SEND( (*pptr), &pValue->m_type, (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_from_name_length, (*psize) );
	if( pValue->m_from_name_length > 0 && pValue->m_from_name_length <= 22 )
		LKSET_MEM_SEND( (*pptr), pValue->m_from_name, pValue->m_from_name_length*sizeof(char), (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_title_length, (*psize) );
	if( pValue->m_title_length > 0 && pValue->m_title_length <= 32 )
		LKSET_MEM_SEND( (*pptr), pValue->m_title, pValue->m_title_length*sizeof(char), (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_simple_content_length, (*psize) );
	if( pValue->m_simple_content_length > 0 && pValue->m_simple_content_length <= 32 )
		LKSET_MEM_SEND( (*pptr), pValue->m_simple_content, pValue->m_simple_content_length*sizeof(char), (*psize) );
	LKSET_BYTE_SEND( (*pptr), &pValue->m_has_attach, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_recvtime, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_deltime, (*psize) );
	LKSET_BYTE_SEND( (*pptr), &pValue->m_read, (*psize) );
	LKSET_BYTE_SEND( (*pptr), &pValue->m_attach_isget, (*psize) );
	return 0;
}

int struct_NetS_MailList_send( char **pptr, int *psize, SLK_NetS_MailList *pValue )
{
	int tmpi = 0;

	LKSET_WORD_SEND( (*pptr), &pValue->m_count, (*psize) );
	for( tmpi = 0; tmpi < pValue->m_count; tmpi++ )
	{
		struct_NetS_MailSimpleInfo_send( pptr, psize, &pValue->m_list[tmpi] );
	}
	return 0;
}

int struct_NetS_MailFullInfo_send( char **pptr, int *psize, SLK_NetS_MailFullInfo *pValue )
{
	int tmpi = 0;

	LKSET_LONG_SEND( (*pptr), &pValue->m_mailid, (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_content_length, (*psize) );
	if( pValue->m_content_length > 0 && pValue->m_content_length <= 1024 )
		LKSET_MEM_SEND( (*pptr), pValue->m_content, pValue->m_content_length*sizeof(char), (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_attach_length, (*psize) );
	if( pValue->m_attach_length > 0 && pValue->m_attach_length <= 128 )
		LKSET_MEM_SEND( (*pptr), pValue->m_attach, pValue->m_attach_length*sizeof(char), (*psize) );
	LKSET_BYTE_SEND( (*pptr), &pValue->m_read, (*psize) );
	return 0;
}

int struct_NetS_FightMsgSimpleInfo_send( char **pptr, int *psize, SLK_NetS_FightMsgSimpleInfo *pValue )
{
	int tmpi = 0;

	LKSET_LONG_SEND( (*pptr), &pValue->m_msgid, (*psize) );
	LKSET_BYTE_SEND( (*pptr), &pValue->m_read, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_recvtime, (*psize) );
	LKSET_BYTE_SEND( (*pptr), &pValue->m_status, (*psize) );
	LKSET_BYTE_SEND( (*pptr), &pValue->m_target_type, (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_title_length, (*psize) );
	if( pValue->m_title_length > 0 && pValue->m_title_length <= 32 )
		LKSET_MEM_SEND( (*pptr), pValue->m_title, pValue->m_title_length*sizeof(char), (*psize) );
	return 0;
}

int struct_NetS_FightMsgList_send( char **pptr, int *psize, SLK_NetS_FightMsgList *pValue )
{
	int tmpi = 0;

	LKSET_WORD_SEND( (*pptr), &pValue->m_count, (*psize) );
	for( tmpi = 0; tmpi < pValue->m_count; tmpi++ )
	{
		struct_NetS_FightMsgSimpleInfo_send( pptr, psize, &pValue->m_list[tmpi] );
	}
	return 0;
}

int struct_NetS_FightMsgInfo_send( char **pptr, int *psize, SLK_NetS_FightMsgInfo *pValue )
{
	int tmpi = 0;

	LKSET_LONG_SEND( (*pptr), &pValue->m_msgid, (*psize) );
	LKSET_BYTE_SEND( (*pptr), &pValue->m_read, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_recvtime, (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_posx, (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_posy, (*psize) );
	LKSET_BYTE_SEND( (*pptr), &pValue->m_status, (*psize) );
	LKSET_BYTE_SEND( (*pptr), &pValue->m_target_type, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_target_id, (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_target_posx, (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_target_posy, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_wood, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_iron, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_food, (*psize) );
	LKSET_LONG_SEND( (*pptr), &pValue->m_recordid, (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_title_length, (*psize) );
	if( pValue->m_title_length > 0 && pValue->m_title_length <= 32 )
		LKSET_MEM_SEND( (*pptr), pValue->m_title, pValue->m_title_length*sizeof(char), (*psize) );
	return 0;
}

int struct_NetS_BuildingAbilityList_send( char **pptr, int *psize, SLK_NetS_BuildingAbilityList *pValue )
{
	int tmpi = 0;

	LKSET_WORD_SEND( (*pptr), &pValue->m_count, (*psize) );
	for( tmpi = 0; tmpi < pValue->m_count; tmpi++ )
	{
		struct_NetS_BuildingAbilityInfo_send( pptr, psize, &pValue->m_list[tmpi] );
	}
	return 0;
}

int struct_NetS_ChatMaskList_send( char **pptr, int *psize, SLK_NetS_ChatMaskList *pValue )
{
	int tmpi = 0;

	LKSET_WORD_SEND( (*pptr), &pValue->m_count, (*psize) );
	if( pValue->m_count > 0 && pValue->m_count <= 64 )
		LKSET_MEM_SEND( (*pptr), pValue->m_chat_mask_list, pValue->m_count*sizeof(int), (*psize) );
	return 0;
}

int struct_NetS_TroopInfo_send( char **pptr, int *psize, SLK_NetS_TroopInfo *pValue )
{
	int tmpi = 0;

	LKSET_WORD_SEND( (*pptr), &pValue->m_corpstype, (*psize) );
	LKSET_BYTE_SEND( (*pptr), &pValue->m_level, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_have, (*psize) );
	LKSET_DWORD_SEND( (*pptr), &pValue->m_wound, (*psize) );
	return 0;
}

int struct_NetS_TroopInfoList_send( char **pptr, int *psize, SLK_NetS_TroopInfoList *pValue )
{
	int tmpi = 0;

	LKSET_WORD_SEND( (*pptr), &pValue->m_count, (*psize) );
	for( tmpi = 0; tmpi < pValue->m_count; tmpi++ )
	{
		struct_NetS_TroopInfo_send( pptr, psize, &pValue->m_troop_list[tmpi] );
	}
	return 0;
}

int struct_NetS_HeroSmpInfo_send( char **pptr, int *psize, SLK_NetS_HeroSmpInfo *pValue )
{
	int tmpi = 0;

	LKSET_WORD_SEND( (*pptr), &pValue->m_offset, (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_herokind, (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_level, (*psize) );
	LKSET_BYTE_SEND( (*pptr), &pValue->m_official, (*psize) );
	return 0;
}

int struct_NetS_HeroList_send( char **pptr, int *psize, SLK_NetS_HeroList *pValue )
{
	int tmpi = 0;

	LKSET_WORD_SEND( (*pptr), &pValue->m_count, (*psize) );
	for( tmpi = 0; tmpi < pValue->m_count; tmpi++ )
	{
		struct_NetS_HeroSmpInfo_send( pptr, psize, &pValue->m_list[tmpi] );
	}
	return 0;
}

int struct_NetS_HeroInfo_send( char **pptr, int *psize, SLK_NetS_HeroInfo *pValue )
{
	int tmpi = 0;

	LKSET_WORD_SEND( (*pptr), &pValue->m_offset, (*psize) );
	if( pValue->m_offset > 0 && pValue->m_offset <= 22 )
		LKSET_MEM_SEND( (*pptr), pValue->m_name, pValue->m_offset*sizeof(char), (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_attack, (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_defance, (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_agility, (*psize) );
	LKSET_WORD_SEND( (*pptr), &pValue->m_hp, (*psize) );
	LKSET_LONG_SEND( (*pptr), &pValue->m_experience, (*psize) );
	LKSET_MEM_SEND( (*pptr), pValue->m_skill, 4*sizeof(short), (*psize) );
	LKSET_MEM_SEND( (*pptr), pValue->m_equip, 6*sizeof(short), (*psize) );
	return 0;
}

