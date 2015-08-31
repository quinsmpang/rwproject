#include <string.h>
#include "server_structrecv_auto.h"
#include "actor_send.h"

int struct_NetC_Login_recv( char **pptr, int *psize, SLK_NetC_Login *pValue )
{
	int tmpi = 0;

	LKSET_WORD_RECV( &pValue->m_username_length, (*pptr), (*psize) );
	if( pValue->m_username_length < 0 || pValue->m_username_length > (*psize)*(int)sizeof(char) )
		return -1;
	LKSET_MEM_RECV( pValue->m_username, (*pptr), pValue->m_username_length*sizeof(char), (*psize) );
	LKSET_WORD_RECV( &pValue->m_password_length, (*pptr), (*psize) );
	if( pValue->m_password_length < 0 || pValue->m_password_length > (*psize)*(int)sizeof(char) )
		return -1;
	LKSET_MEM_RECV( pValue->m_password, (*pptr), pValue->m_password_length*sizeof(char), (*psize) );
	LKSET_WORD_RECV( &pValue->m_deviceid_length, (*pptr), (*psize) );
	if( pValue->m_deviceid_length < 0 || pValue->m_deviceid_length > (*psize)*(int)sizeof(char) )
		return -1;
	LKSET_MEM_RECV( pValue->m_deviceid, (*pptr), pValue->m_deviceid_length*sizeof(char), (*psize) );
	return 0;
}

int struct_NetU_Logined_recv( char **pptr, int *psize, SLK_NetU_Logined *pValue )
{
	int tmpi = 0;

	LKSET_DWORD_RECV( &pValue->m_result, (*pptr), (*psize) );
	LKSET_DWORD_RECV( &pValue->m_client_index, (*pptr), (*psize) );
	LKSET_DWORD_RECV( &pValue->m_authid, (*pptr), (*psize) );
	LKSET_MEM_RECV( pValue->m_userid, (*pptr), 21*sizeof(char), (*psize) );
	LKSET_MEM_RECV( pValue->m_username, (*pptr), 51*sizeof(char), (*psize) );
	LKSET_WORD_RECV( &pValue->m_usertype, (*pptr), (*psize) );
	LKSET_BYTE_RECV( &pValue->m_checkflag, (*pptr), (*psize) );
	LKSET_BYTE_RECV( &pValue->m_isdeep, (*pptr), (*psize) );
	LKSET_MEM_RECV( pValue->m_access_token, (*pptr), 65*sizeof(char), (*psize) );
	LKSET_DWORD_RECV( &pValue->m_expires_in, (*pptr), (*psize) );
	LKSET_MEM_RECV( pValue->m_refresh_token, (*pptr), 65*sizeof(char), (*psize) );
	LKSET_MEM_RECV( pValue->m_deviceid, (*pptr), 65*sizeof(char), (*psize) );
	return 0;
}

int struct_NetC_Create_recv( char **pptr, int *psize, SLK_NetC_Create *pValue )
{
	int tmpi = 0;

	LKSET_BYTE_RECV( &pValue->m_aclass, (*pptr), (*psize) );
	LKSET_WORD_RECV( &pValue->m_name_length, (*pptr), (*psize) );
	if( pValue->m_name_length < 0 || pValue->m_name_length > (*psize)*(int)sizeof(char) )
		return -1;
	LKSET_MEM_RECV( pValue->m_name, (*pptr), pValue->m_name_length*sizeof(char), (*psize) );
	return 0;
}

int struct_NetC_Heart_recv( char **pptr, int *psize, SLK_NetC_Heart *pValue )
{
	int tmpi = 0;

	LKSET_MEM_RECV( pValue->m_value, (*pptr), 2*sizeof(int), (*psize) );
	return 0;
}

int struct_NetC_Gmcmd_recv( char **pptr, int *psize, SLK_NetC_Gmcmd *pValue )
{
	int tmpi = 0;

	LKSET_WORD_RECV( &pValue->m_cmd, (*pptr), (*psize) );
	LKSET_MEM_RECV( pValue->m_value, (*pptr), 4*sizeof(int), (*psize) );
	LKSET_WORD_RECV( &pValue->m_msg_length, (*pptr), (*psize) );
	if( pValue->m_msg_length < 0 || pValue->m_msg_length > (*psize)*(int)sizeof(char) )
		return -1;
	LKSET_MEM_RECV( pValue->m_msg, (*pptr), pValue->m_msg_length*sizeof(char), (*psize) );
	return 0;
}

int struct_NetC_Talk_recv( char **pptr, int *psize, SLK_NetC_Talk *pValue )
{
	int tmpi = 0;

	LKSET_BYTE_RECV( &pValue->m_type, (*pptr), (*psize) );
	LKSET_WORD_RECV( &pValue->m_msg_length, (*pptr), (*psize) );
	if( pValue->m_msg_length < 0 || pValue->m_msg_length > (*psize)*(int)sizeof(char) )
		return -1;
	LKSET_MEM_RECV( pValue->m_msg, (*pptr), pValue->m_msg_length*sizeof(char), (*psize) );
	return 0;
}

int struct_NetC_AskInfo_recv( char **pptr, int *psize, SLK_NetC_AskInfo *pValue )
{
	int tmpi = 0;

	LKSET_WORD_RECV( &pValue->m_msgid, (*pptr), (*psize) );
	LKSET_WORD_RECV( &pValue->m_valuenum, (*pptr), (*psize) );
	if( pValue->m_valuenum < 0 || pValue->m_valuenum > (*psize)*(int)sizeof(int) )
		return -1;
	LKSET_MEM_RECV( pValue->m_value, (*pptr), pValue->m_valuenum*sizeof(int), (*psize) );
	LKSET_WORD_RECV( &pValue->m_msg_length, (*pptr), (*psize) );
	if( pValue->m_msg_length < 0 || pValue->m_msg_length > (*psize)*(int)sizeof(char) )
		return -1;
	LKSET_MEM_RECV( pValue->m_msg, (*pptr), pValue->m_msg_length*sizeof(char), (*psize) );
	return 0;
}

int struct_NetC_MailOp_recv( char **pptr, int *psize, SLK_NetC_MailOp *pValue )
{
	int tmpi = 0;

	LKSET_LONG_RECV( &pValue->m_mailid, (*pptr), (*psize) );
	LKSET_BYTE_RECV( &pValue->m_op, (*pptr), (*psize) );
	return 0;
}

int struct_NetC_CityBattleTroopList_recv( char **pptr, int *psize, SLK_NetC_CityBattleTroopList *pValue )
{
	int tmpi = 0;

	LKSET_WORD_RECV( &pValue->m_corps, (*pptr), (*psize) );
	LKSET_BYTE_RECV( &pValue->m_level, (*pptr), (*psize) );
	LKSET_DWORD_RECV( &pValue->m_count, (*pptr), (*psize) );
	return 0;
}

int struct_NetC_CityBattleInfo_recv( char **pptr, int *psize, SLK_NetC_CityBattleInfo *pValue )
{
	int tmpi = 0;

	LKSET_DWORD_RECV( &pValue->m_to_unit_index, (*pptr), (*psize) );
	LKSET_WORD_RECV( &pValue->m_to_posx, (*pptr), (*psize) );
	LKSET_WORD_RECV( &pValue->m_to_posy, (*pptr), (*psize) );
	LKSET_WORD_RECV( &pValue->m_troop_count, (*pptr), (*psize) );
	for( tmpi = 0; tmpi < pValue->m_troop_count; tmpi++ )
	{
		struct_NetC_CityBattleTroopList_recv( pptr, psize, &pValue->m_troop_list[tmpi] );
	}
	LKSET_WORD_RECV( &pValue->m_hero_count, (*pptr), (*psize) );
	if( pValue->m_hero_count < 0 || pValue->m_hero_count > (*psize)*(int)sizeof(short) )
		return -1;
	LKSET_MEM_RECV( pValue->m_hero_list, (*pptr), pValue->m_hero_count*sizeof(short), (*psize) );
	return 0;
}

