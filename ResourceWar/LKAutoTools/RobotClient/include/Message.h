#ifndef MESSAGE_H_
#define MESSAGE_H_

#include "utils.h"
#define MAXMSGSIZE 4096
#define MAXMSGCOUNT 4096

#define	RECMODE_RECORD	0
#define	RECMODE_LOAD	1
#define	RECMODE_NORMAL	2

struct _client_input
{
	UINT uMsg;
	WPARAM wParam;
	LPARAM lParam;
	BOOL bSafe;
};
typedef struct _client_input SLK_ClientInput;

struct _client_message
{
	int cmd;
	int sock_index;
	int buf_size;
	char buf[MAXMSGSIZE];
};
typedef struct _client_message SLK_ClientMsg;

int client_message_init();
void client_message_exit();
int client_message_send( int cmd, char *msg, int size, int sock_index  );
int client_message_poll();

void input_mssage_append( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL bSafe = FALSE );
void set_defaultrecfile( char *recfile );

#endif
