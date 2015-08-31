#include <assert.h>

#include "Game.h"
#include "Message.h"
#include "Command.h"
#include "NetClient.h"
#include "NetProcess.h"

int g_enter_num = 0;

// 从服务器回来的消息
int CALLBACK Net_ProcessRecv( char *pack_data, int packlen, int sock_index  );
int CALLBACK Net_ProcessExit( int fd );
void LK_NetInit()
{
	SetCallbackProcessPack( Net_ProcessRecv );
	SetCallbackDisConnected( Net_ProcessExit );
}

// 网络来的指令, 这里获得的包数据是一个网络包, 需要分拆为各个消息包.
int CALLBACK Net_ProcessRecv( char *pack_data, int packlen, int sock_index  )
{
	short cmd;
	int allsize;
	int tmpsize;
	short datasize;
	char *ptr;
	if( pack_data == NULL || packlen < 0 || packlen >= 2048 )
		return -1;
	ptr = pack_data;
	// || CMDS_COMMAND | datasize[short] | ... ||
	allsize = 0;
	while(1)
	{
		tmpsize = 0;
		if( allsize >= packlen )
			break;
		cmd = *((short *)ptr); ptr+=sizeof(short);tmpsize+=sizeof(short);
		datasize = *((short *)ptr); ptr+=sizeof(short);tmpsize+=sizeof(short);
		allsize += tmpsize;
		if( datasize < 0 )
			return -1;
		assert(allsize>=0);
		assert(datasize>=0);
		switch(cmd)
		{
		case CMDS_LOGIN:		// || ... | result[byte] ||
			tmpsize = datasize;
			client_message_send( CMD_LOGIN, ptr, tmpsize, sock_index );
			ptr += tmpsize;
			allsize += tmpsize;
			break;
		case CMDS_ENTERINFO:		// || ... | result[byte] ||
			tmpsize = datasize;
			client_message_send( CMD_ENTERINFO, ptr, tmpsize, sock_index );
			ptr += tmpsize;
			allsize += tmpsize;
			break;
		case CMDS_LIST:		// || ... | result[byte] ||
			tmpsize = datasize;
			client_message_send( CMD_LIST, ptr, tmpsize, sock_index );
			ptr += tmpsize;
			allsize += tmpsize;
			break;
		case CMDS_CREATE:		// || ... | result[byte] ||
			tmpsize = datasize;
			client_message_send( CMD_CREATE, ptr, tmpsize, sock_index );
			ptr += tmpsize;
			allsize += tmpsize;
			break;
		case CMDS_ENTERGAME:	// || ... | result[byte] | ActorID[int] | characterset[short] | posx[short] | posy[short] ||
			tmpsize = datasize;
			client_message_send( CMD_ENTERGAME, ptr, tmpsize, sock_index );
			ptr += tmpsize;
			allsize += tmpsize;
			g_enter_num++;
			break;
		case CMDS_MOVE:			// || ... | ActorID[int] | posx[short] | posy[short] ||
			tmpsize = datasize;
			client_message_send( CMD_MOVE, ptr, tmpsize, sock_index );
			ptr += tmpsize;
			allsize += tmpsize;
			break;
		case CMDS_TALK:			// || ... | type[byte] | ActorID[int] | msgsize[short] | pText[msgsize] ||
			tmpsize = datasize;
			client_message_send( CMD_TALK, ptr, tmpsize, sock_index );
			ptr += tmpsize;
			allsize += tmpsize;
			break;
		case CMDS_FIGHT:
			tmpsize = datasize;
			client_message_send( CMD_FIGHT, ptr, tmpsize, sock_index );
			ptr += tmpsize;
			allsize += tmpsize;
			break;
		case CMDS_FIGHT_PROCESS:
			tmpsize = datasize;
			client_message_send( CMD_FIGHT_PROCESS, ptr, tmpsize, sock_index );
			ptr += tmpsize;
			allsize += tmpsize;
			break;
		case CMDS_COORDINATE:
			tmpsize = datasize;
			client_message_send( CMD_COORDINATE, ptr, tmpsize, sock_index );
			ptr += tmpsize;
			allsize += tmpsize;
			break;
		case CMDS_STSTCHANGE:
			tmpsize = datasize;
			client_message_send( CMD_STSTCHANGE, ptr, tmpsize, sock_index );
			ptr += tmpsize;
			allsize += tmpsize;
			break;
		default:
			tmpsize = datasize;
			ptr += tmpsize;
			allsize += tmpsize;
			break;
		}
	}
	return 0;
}

int CALLBACK Net_ProcessExit( int fd )
{
	return 0;
}
