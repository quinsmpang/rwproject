#include "utils.h"
#include "TcpManager.h"
#include "ClientMessage.h"

TcpManager::TcpManager()
{

}

TcpManager::~TcpManager()
{

}

// ������
void TcpManager::onConnect()
{	
	int data = 0;
	int tmpsize = 0;
	char buf[2048] = {0};
	char *ptr = buf;
	LKSET_DWORD_SEND( ptr, &data, tmpsize );
	ClientMessage::GetInstance()->Add( -101, ptr, tmpsize );
}

// ����ʧ��
void TcpManager::onConnectFail()
{
	int data = 0;
	int tmpsize = 0;
	char buf[2048] = {0};
	char *ptr = buf;
	LKSET_DWORD_SEND( ptr, &data, tmpsize );
	ClientMessage::GetInstance()->Add( -102, ptr, tmpsize );
}
// ʧȥ����
void TcpManager::OnDisconnected()
{
	int data = 0;
	int tmpsize = 0;
	char buf[2048] = {0};
	char *ptr = buf;
	LKSET_DWORD_SEND( ptr, &data, tmpsize );
	ClientMessage::GetInstance()->Add( -103, ptr, tmpsize );
}

// �ͻ��������ر�����
void TcpManager::onClose()
{
	int data = 0;
	int tmpsize = 0;
	char buf[2048] = {0};
	char *ptr = buf;
	LKSET_DWORD_SEND( ptr, &data, tmpsize );
	ClientMessage::GetInstance()->Add( -103, ptr, tmpsize );
}

// �������
void TcpManager::onError( int error )
{

}

// ��������ָ��, �����õİ�������һ�������, ��Ҫ�ֲ�Ϊ������Ϣ��, �ӵ��ͻ�����Ϣ�������
void TcpManager::onMessage( const char *packdata, int packlen )
{
	short cmd;
	int allsize;
	int tmpsize;
	short datasize;
	char *ptr;
	if ( packdata == NULL || packlen < 0 || packlen >= 2048 )
		return;
	ptr = (char*)packdata;
	// || CMDS_COMMAND | datasize[short] | ... ||
	allsize = 0;
	while ( 1 )
	{
		tmpsize = 0;
		if ( allsize >= packlen )
			break;
		LKSET_WORD_L( &cmd, ptr, tmpsize );
		LKSET_WORD_L( &datasize, ptr, tmpsize );
		allsize += tmpsize;

		tmpsize = datasize;
		ClientMessage::GetInstance()->Add( cmd, ptr, tmpsize );
		ptr += tmpsize;
		allsize += tmpsize;
	}
}

// ���ӷ�����
void TcpManager::ConnectServer( const char* host, unsigned short port )
{
	m_TcpClient.SetDelegate( this );
	m_TcpClient.AsynConnect( host, port );
}

// ������Ϣ
void TcpManager::SendMessage( ByteBuffer *buffer )
{
	if ( !m_TcpClient.m_bStartReady )
		return;
	if ( !buffer )
		return;
	char sendbuf[MAX_PACKAGE_SIZE];
#ifdef NONECODE
		memcpy( sendbuf, buffer->m_WriteBuffer, buffer->m_WriteSize );
#else
		netcode_encode( sendbuf, buffer->m_WriteBuffer, buffer->m_WriteSize );
#endif
	m_TcpClient.SendToQueue( sendbuf, buffer->m_WriteSize );
}
