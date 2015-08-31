#include <sys/stat.h>

#include "thread.h"
#include "utils.h"
#include "Command.h"
#include "NetProcess.h"
#include "Message.h"
#include "Game.h"

// 队列缓冲区
SLK_ClientMsg s_msg_queue[MAXMSGCOUNT];
int s_nMsgQueueHead = 0;
int s_nMsgQueueTail = 0;

ThrMux	*s_msg_mmux;

// 初始化消息管理器
int client_message_init()
{
	s_nMsgQueueHead = 0;
	s_nMsgQueueTail = 0;
	s_msg_mmux = mmux_open();
	LK_CommandInit();
	LK_NetInit();

	return 0;
}

void client_message_exit()
{
	mmux_close( s_msg_mmux );
}

// 插入一条消息到队列
int client_message_send( int cmd, char *msg, int size, int sock_index )
{
	int queue_tail;

	if( msg == NULL || size <= 0 )
		return -1;

	// 锁住队列缓冲
	mmux_lock( s_msg_mmux );
	
	queue_tail = s_nMsgQueueTail + 1;
	if( queue_tail >= MAXMSGCOUNT )
	{
		queue_tail = 0;
	}
	if( s_nMsgQueueHead == queue_tail )
	{
		// 队列已经满了
		mmux_unlock( s_msg_mmux );
		write_netlog("message queue is full!");
		return -1;
	}

	memcpy( s_msg_queue[s_nMsgQueueTail].buf, msg, size );
	s_msg_queue[s_nMsgQueueTail].buf_size = size;
	s_msg_queue[s_nMsgQueueTail].cmd = cmd;
	s_msg_queue[s_nMsgQueueTail].sock_index = sock_index;

	s_nMsgQueueTail = queue_tail;

	mmux_unlock( s_msg_mmux );
	return 0;
}

int client_message_poll()
{
	int tmpcmd;
	int tmpsize;
	int sock_index;
	char tmpbuf[MAXMSGSIZE];

	// 锁住队列缓冲
	mmux_lock( s_msg_mmux );
	while( s_nMsgQueueTail != s_nMsgQueueHead )
	{
		// 从队列中取出一项
		tmpcmd = s_msg_queue[s_nMsgQueueHead].cmd;
		tmpsize = s_msg_queue[s_nMsgQueueHead].buf_size;
		sock_index = s_msg_queue[s_nMsgQueueHead].sock_index;
		memcpy( tmpbuf, s_msg_queue[s_nMsgQueueHead].buf, s_msg_queue[s_nMsgQueueHead].buf_size );
		s_nMsgQueueHead++;
		if( s_nMsgQueueHead >= MAXMSGCOUNT )
		{
			s_nMsgQueueHead = 0;
		}

		if( LK_ProcessMsg( tmpcmd, tmpbuf, tmpsize, sock_index ) < 0 )
		{
			continue;
		}
	}
	mmux_unlock( s_msg_mmux );
	return 0;
}
