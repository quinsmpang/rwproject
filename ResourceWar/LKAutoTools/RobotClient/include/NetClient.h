#include "utils.h"

// ���������󳤶�
#define MAX_BUF_SIZE		2048*2
#define MAX_PACKAGE_SIZE	2048

// һ����Ϊ����һ��������󳤶�
#define SERV_PORT			8765
#define MAX_QUEUE_NUM		8192*8
#define MAX_WRITE_QUEUE_NUM		2048

// ����ṹ�����ȴ�����һ�������İ�(���������߼�������)
struct _sbuf
{
	int index;
	int exec_code;
	int buf_size;					// �ɲ��������ݴ�С
	char buf[MAX_BUF_SIZE];					// ��������
};
typedef struct _sbuf SBuf;

typedef int (CALLBACK *LPCALLBACKPROCESSPACK)( char *pack_data, int packlen, int sock_index );
typedef int (CALLBACK *LPCALLBACKDISCONNECTED)( int fd );

void SetCallbackProcessPack( LPCALLBACKPROCESSPACK pCallbackProcessPack );
void SetCallbackDisConnected( LPCALLBACKDISCONNECTED pCallbackDisConnected );

int sendtoqueue( const char *buf, int bufsize, int sock_index, int exec_code );

int client_init();
char client_isready();
int client_start( unsigned int nIP, unsigned short port, int socknum );
int client_disconnect();
int client_destroy();

int net_closeclient( int sock_index, char r );

void netcode_encode( int sock_index, char *dst, const char *src, int size );
