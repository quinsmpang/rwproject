#ifndef __WQUEUE_H
#define __WQUEUE_H


#define MAX_WQUEUE_NUM			4096
#define MAX_CMDPACKAGE_SIZE		2048

#define WCMD_ENTERGAME			1
#define WCMD_LEAVE				2
#define WCMD_LIST				3
#define WCMD_CREATE				4
#define WCMD_SAVE				5

struct _swqueue
{
	short cmd;						// ָ��
	int client_index;				// �ͻ�������
	int buf_size;					// ���������
	char buf[MAX_CMDPACKAGE_SIZE];	// ���������
	int exec_code;
};
typedef struct _swqueue SWQueue;

void init_wqueue();
int add_wqueue( int client_index, short cmd, const char *pBuf, int wsize, int exec_code, int line );
int fetch_wqueue( int *pclient_index, short *pcmd, char *pBuf, int *pwsize, int *pexec_code, int line );

#endif