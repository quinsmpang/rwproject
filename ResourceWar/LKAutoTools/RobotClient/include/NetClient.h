#include "utils.h"

// 网络包的最大长度
#define MAX_BUF_SIZE		2048*2
#define MAX_PACKAGE_SIZE	2048

// 一般认为这是一个包的最大长度
#define SERV_PORT			8765
#define MAX_QUEUE_NUM		8192*8
#define MAX_WRITE_QUEUE_NUM		2048

// 这个结构用来等待接收一个完整的包(仅仅是主逻辑访问他)
struct _sbuf
{
	int index;
	int exec_code;
	int buf_size;					// 可操作的数据大小
	char buf[MAX_BUF_SIZE];					// 读缓冲区
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
