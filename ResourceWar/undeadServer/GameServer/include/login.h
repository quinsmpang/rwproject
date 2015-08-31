#ifndef __LOGIN_H
#define __LOGIN_H
#include "define.h"
#include "server_structsend_auto.h"

#define MAX_LOGINQUEUENUM			32 // ��½���г���

#define COMMAND_PACK_LEN			512
#define RETURN_PACK_LEN				256

#define USERCMDS_LOGIN				1
#define USERCMDS_LOCKUSER			2

#define USERCMDC_LOGIN				1
#define USERCMDC_LOCKUSER			2

// ��½����
struct _sloginqueue
{
	int client_index;
	int authid;
	short command;
	int platcode;
	char data[COMMAND_PACK_LEN];
};
typedef struct _sloginqueue SLoginQueue;

int login_init();
int login_send_proc();
int login_recv_proc();

// �û����͵�½���󵽵�½����
int user_login( int client_index, const char *szUserName, const char *szPassWord, char *szDeviceID );

// �û����������ص�½���
int user_logined( int client_index, int authid, SClientInfo *pClientInfo );

// �û������˺�
int user_lock( int client_index, int authid, i64 player_userid, int lockmin );

// �Ŷӻ���
int user_queue_logic();
void login_queue_clear();
#endif



