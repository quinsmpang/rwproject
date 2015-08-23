#ifndef __CLIENTMESSAGE_H_
#define __CLIENTMESSAGE_H_

#include "ByteBuffer.h"
#include "thread.h"
#include "utils.h"
using namespace std;

#define MAXMSGSIZE 2048
#define MAXMSGCOUNT 2048

// 消息包结构
typedef struct _local_message
{
	int cmd;
	int buf_size;
	char buf[MAXMSGSIZE];
}LocalMessage;

// 客户端消息
class ClientMessage
{
public:
	static ClientMessage* GetInstance();
	virtual ~ClientMessage();
	bool Init();
	void Destroy();
	void Add( int cmd, char *msg, int size );
	void Dispatch();

private:
	ClientMessage();
	static ClientMessage *m_pInstance;

private:
	// 消息队列
	LocalMessage m_MessageQueue[MAXMSGCOUNT];
	short m_nMessageQueueHead;
	short m_nMessageQueueTail;
	ThrMux	*m_pMessageMutex;
};

#endif
