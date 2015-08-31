#include "server_process_auto.h"
#include "actor_send.h"
#include "login.h"
#include "gameproc.h"
#include "system_askinfo.h"
#include "actor.h"
#include "mail_msg.h"
#include "fight_msg.h"
#include "city.h"

void proc_list_S( int client_index, char *pValue )
{
	// process.
}

void proc_heart_S( int client_index, SLK_NetC_Heart *pValue )
{
	// process.
}

void proc_delete_S( int client_index, int *pValue )
{
	// process.
}

void proc_entergame_S( int client_index, int *pValue )
{
	// process.
	client_setwait( client_index, 0 );
	actor_entergame( client_index, *pValue );
}

void proc_login_S( int client_index, SLK_NetC_Login *pValue )
{
	// process.
	user_login( client_index, pValue->m_username, pValue->m_password, pValue->m_deviceid );
}

void proc_timer_S( int client_index, char *pValue )
{
	// process.
	if ( client_index < 0 )
		process_logic();
}

void proc_create_S( int client_index, SLK_NetC_Create *pValue )
{
	// process.
	client_setwait( client_index, 0 );
	actor_create( client_index, pValue->m_aclass, pValue->m_name );
}

void proc_logined_S( int client_index, SLK_NetU_Logined *pValue )
{
	// process.
	SClientInfo ClientInfo = {0};

	ClientInfo.userid = atoll( pValue->m_userid );
	ClientInfo.usertype = pValue->m_usertype;
	ClientInfo.result = pValue->m_result;
	strncpy( ClientInfo.username, pValue->m_username, MAX_USERNAME_LENGTH + 1 );
	strncpy( ClientInfo.deviceid, pValue->m_deviceid, 64 );

	if ( pValue->m_expires_in > 0 )
	{
		strncpy( ClientInfo.access_token, pValue->m_access_token, 65 );
		ClientInfo.expires_in = (int)time( NULL ) + pValue->m_expires_in - 60;
		strncpy( ClientInfo.refresh_token, pValue->m_refresh_token, 65 );
	}
	user_logined( pValue->m_client_index, pValue->m_authid, &ClientInfo );
}

void proc_gmcmd_S( int client_index, SLK_NetC_Gmcmd *pValue )
{
	// process.
	actor_command( client_index, pValue->m_cmd, pValue->m_value, pValue->m_msg );
}

void proc_talk_S( int client_index, SLK_NetC_Talk *pValue )
{
	// process.
	actor_talk( client_index, pValue->m_type, pValue->m_msg, pValue->m_msg_length );
}

void proc_askinfo_S( int client_index, SLK_NetC_AskInfo *pValue )
{
	// process.
	system_askinfo( client_index, pValue->m_msgid, pValue->m_msg, pValue->m_value );
}

void proc_mailop_S( int client_index, SLK_NetC_MailOp *pValue )
{
	// process.
	if ( pValue->m_op == 1 )
	{
		mail_read_content( client_index, pValue->m_mailid );
	}
	else if( pValue->m_op == 2 )
	{
		mail_read_attach( client_index, pValue->m_mailid );
	}
	else if ( pValue->m_op == 3 )
	{
		mail_delete( client_index, pValue->m_mailid );
	}
	else if ( pValue->m_op == 10 )
	{	//战报内容
		fight_msg_loadlist( client_index, pValue->m_mailid );
	}
	else if(pValue->m_op==11)
	{
		//读取邮件列表	

		mail_loadlist(client_index,pValue->m_mailid);	
	}
	else if(pValue->m_op==12)
	{
		//战斗信息明细
		fight_msg_read( client_index, pValue->m_mailid );
	}
	else if(pValue->m_op==13)
	{
		 //邮件明细
		mail_read_content( client_index,pValue->m_mailid );
	}


}

void proc_citybattle_S( int client_index, SLK_NetC_CityBattleInfo *pValue )
{
	// process.

	city_battle( client_index, city_getptr(client_index), pValue );
}

