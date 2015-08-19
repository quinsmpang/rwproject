#ifndef __TCPMANAGER_H_
#define __TCPMANAGER_H_
#include "TcpClient.h"
#include "ByteBuffer.h"

// TCPÕ¯¬Áπ‹¿Ì
class TcpManager : public TcpClientDelegate
{
public:
	TcpManager();
	~TcpManager();
	virtual void onConnect();
	virtual void onConnectFail();
	virtual void OnDisconnected();
	virtual void onClose();
	virtual void onError( int error );
	virtual void onMessage( const char *packdata, int packlen );

public:
	void ConnectServer( const char* host, unsigned short port );
	void SendMessage( ByteBuffer *buffer );

private:
	TcpClient m_TcpClient;
};

#endif
