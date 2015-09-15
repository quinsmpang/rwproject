#ifndef _SHM_H_
#define _SHM_H_
#include "shm_ao.h"
//#define SHM_USE
#define SHM_POOL_MAXCOUNT	8 // �����ڴ�ش�С

#define SHM_KEY_BASE	(g_Config.server_code*10000) // �����ڴ���ʼkey����Ϊһ̨���������ж��������ʵ��
#define SHM_KEY_CITY	1	// ���ǹ����ڴ�key
#define SHM_KEY_ARMY	2	// ���ӹ����ڴ�key

/* ��ʼ�������ڴ�ķ���ֵ */
enum SHM_INIT_RET
{
	SHM_ERROR = 0,	// ��ʼ��ʧ��
	SHM_ATTACH,		// ���ŵķ�ʽ
	SHM_CREATE,		// �����ķ�ʽ
};

/* �����ڴ�� */
typedef struct _shm_pool
{
	ShmAO *m_pShmAo[SHM_POOL_MAXCOUNT];	// �����ڴ���ʶ���
}SHMPool;

int shm_pool_add( SM_KEY key, unsigned int size, void **ptr );
void shm_pool_del( SM_KEY key );
void shm_pool_delall();
#endif
