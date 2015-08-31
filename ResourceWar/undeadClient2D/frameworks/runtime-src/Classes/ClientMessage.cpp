#include "ClientMessage.h"
#include "lua_oneself_manual.hpp"

ClientMessage *ClientMessage::m_pInstance = nullptr;
ClientMessage *ClientMessage::GetInstance()
{
	if ( !m_pInstance )
	{
		m_pInstance = new ClientMessage();
	}
	return m_pInstance;
}

ClientMessage::ClientMessage()
{
	Init();
}

ClientMessage::~ClientMessage()
{
	Destroy();
}

bool ClientMessage::Init()
{
	m_nMessageQueueHead = 0;
	m_nMessageQueueTail = 0;
	m_pMessageMutex = mmux_open();
	return true;
}

void ClientMessage::Destroy()
{
	m_nMessageQueueHead = 0;
	m_nMessageQueueTail = 0;
	mmux_close( m_pMessageMutex );
}

// ���
void ClientMessage::Add( int cmd, char *msg, int size )
{
	int queue_tail;
	if ( msg == nullptr || size < 0 || size > MAXMSGSIZE )
		return;

	// ��ס���л���
	mmux_lock( m_pMessageMutex );
	queue_tail = m_nMessageQueueTail + 1;
	if ( queue_tail >= MAXMSGCOUNT )
	{
		queue_tail = 0;
	}
	if ( m_nMessageQueueHead == queue_tail )
	{
		// �����Ѿ�����
		mmux_unlock( m_pMessageMutex );
		write_netlog( "message queue is full!" );
		return;
	}
	memcpy( m_MessageQueue[m_nMessageQueueTail].buf, msg, size );
	m_MessageQueue[m_nMessageQueueTail].buf_size = size;
	m_MessageQueue[m_nMessageQueueTail].cmd = cmd;
	m_nMessageQueueTail = queue_tail;
	// ����
	mmux_unlock( m_pMessageMutex );
}

// �ַ�
void ClientMessage::Dispatch()
{
	int tmpcmd;
	int tmpsize;
	char tmpbuf[MAXMSGSIZE];

	// ��ס���л���
	mmux_lock( m_pMessageMutex );
	while ( m_nMessageQueueTail != m_nMessageQueueHead )
	{
		// �Ӷ�����ȡ��һ��
		tmpcmd = m_MessageQueue[m_nMessageQueueHead].cmd;
		tmpsize = m_MessageQueue[m_nMessageQueueHead].buf_size;
		memcpy( tmpbuf, m_MessageQueue[m_nMessageQueueHead].buf, m_MessageQueue[m_nMessageQueueHead].buf_size );
		m_nMessageQueueHead++;
		if ( m_nMessageQueueHead >= MAXMSGCOUNT )
		{
			m_nMessageQueueHead = 0;
		}
		mmux_unlock( m_pMessageMutex );

		// Process Message
		call_luafun( "onMessageProc", "iui>", tmpcmd, tmpbuf, tmpsize );
		mmux_lock( m_pMessageMutex );
	}
	mmux_unlock( m_pMessageMutex );
}
