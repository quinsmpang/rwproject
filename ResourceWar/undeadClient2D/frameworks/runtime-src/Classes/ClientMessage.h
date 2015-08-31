#ifndef __CLIENTMESSAGE_H_
#define __CLIENTMESSAGE_H_

#include "ByteBuffer.h"
#include "thread.h"
#include "utils.h"
using namespace std;

#define MAXMSGSIZE 2048
#define MAXMSGCOUNT 2048

// ��Ϣ���ṹ
typedef struct _local_message
{
	int cmd;
	int buf_size;
	char buf[MAXMSGSIZE];
}LocalMessage;

// �ͻ�����Ϣ
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
	// ��Ϣ����
	LocalMessage m_MessageQueue[MAXMSGCOUNT];
	short m_nMessageQueueHead;
	short m_nMessageQueueTail;
	ThrMux	*m_pMessageMutex;
};

#endif
