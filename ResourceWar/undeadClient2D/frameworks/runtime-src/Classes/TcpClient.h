#ifndef __TCPSOCKET_H_
#define __TCPSOCKET_H_
#define NONECODE

#include "thread.h"
#include "utils.h"

// 网络包的最大长度
#define MAX_BUF_SIZE		2048*2
#define MAX_PACKAGE_SIZE	2048

// 读写队列长度
#define MAX_QUEUE_NUM		2048

// 读写缓冲
typedef struct _sbuf
{
	int buf_size;
	char buf[MAX_BUF_SIZE];
}SBuf;

// TCP 客户端的代理
class TcpClientDelegate
{
public:
	virtual ~TcpClientDelegate() {}
	virtual void onConnect() = 0;
	virtual void onConnectFail() = 0;
	virtual void OnDisconnected() = 0;
	virtual void onClose() = 0;
	virtual void onError( int error ) = 0;
	virtual void onMessage( const char *packdata, int packlen ) = 0;
};

// TCP客户端
class TcpClient
{
public:
	TcpClient();
	~TcpClient();

	void SetDelegate( TcpClientDelegate *delegate ){ m_Delegate = delegate; }
	void AsynConnect( const char *host, unsigned short port );
	void Start();
	void CloseSocket();
	void Destroy();

	// 线程需要静态函数
	static int ThreadReadQueueProc( void *arg );
	static int ThreadRecvProc( void *arg );
	static int ThreadSendProc( void *arg );

	// 真正的处理函数
	int ReadQueueProc();
	int RecvProc();
	int SendProc();

	int RecvPackage( char *buf, int size );
	int ReadToQueue( char *pBuf, int readsize );
	int SendToQueue( const char *buf, int bufsize );

public:
	bool m_bStartReady;
	bool m_bRecvExit;
	bool m_bExit;

private:
	int m_socket;
	TcpClientDelegate *m_Delegate;

	// 读
	SBuf m_ReadQueue[MAX_QUEUE_NUM];
	int m_nReadQueueHead;
	int m_nReadQueueTail;
	ThrMux *m_read_mmux;
	ThrMux	*m_recv_synch_mutex;
	Mthr *m_pthr_recv;
	Mthr *m_pthr_readqueue;

	// 写
	SBuf m_WriteQueue[MAX_QUEUE_NUM];
	int m_nWriteQueueHead;
	int m_nWriteQueueTail;
	ThrMux *m_write_mmux;
	Mthr *m_pthr_send;
};


#endif
