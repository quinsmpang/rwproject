#include "utils.h"
#include "TcpManager.h"
#include "ClientMessage.h"

TcpManager::TcpManager()
{

}

TcpManager::~TcpManager()
{

}

// 已连接
void TcpManager::onConnect()
{	
	int data = 0;
	int tmpsize = 0;
	char buf[2048] = {0};
	char *ptr = buf;
	LKSET_DWORD_SEND( ptr, &data, tmpsize );
	ClientMessage::GetInstance()->Add( -101, ptr, tmpsize );
}

// 连接失败
void TcpManager::onConnectFail()
{
	int data = 0;
	int tmpsize = 0;
	char buf[2048] = {0};
	char *ptr = buf;
	LKSET_DWORD_SEND( ptr, &data, tmpsize );
	ClientMessage::GetInstance()->Add( -102, ptr, tmpsize );
}
// 失去连接
void TcpManager::OnDisconnected()
{
	int data = 0;
	int tmpsize = 0;
	char buf[2048] = {0};
	char *ptr = buf;
	LKSET_DWORD_SEND( ptr, &data, tmpsize );
	ClientMessage::GetInstance()->Add( -103, ptr, tmpsize );
}

// 客户端主动关闭连接
void TcpManager::onClose()
{
	int data = 0;
	int tmpsize = 0;
	char buf[2048] = {0};
	char *ptr = buf;
	LKSET_DWORD_SEND( ptr, &data, tmpsize );
	ClientMessage::GetInstance()->Add( -103, ptr, tmpsize );
}

// 网络错误
void TcpManager::onError( int error )
{

}

// 网络来的指令, 这里获得的包数据是一个网络包, 需要分拆为各个消息包, 扔到客户端消息处理队列
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

// 连接服务器
void TcpManager::ConnectServer( const char* host, unsigned short port )
{
	m_TcpClient.SetDelegate( this );
	m_TcpClient.AsynConnect( host, port );
}

// 发送消息
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
