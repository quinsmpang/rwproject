#ifndef _SHM_H_
#define _SHM_H_
#include "shm_ao.h"
//#define SHM_USE
#define SHM_POOL_MAXCOUNT	8 // 共享内存池大小

#define SHM_KEY_BASE	(g_Config.server_code*10000) // 共享内存起始key，因为一台主机会运行多个服务器实例
#define SHM_KEY_CITY	1	// 主城共享内存key
#define SHM_KEY_ARMY	2	// 部队共享内存key

/* 初始化共享内存的返回值 */
enum SHM_INIT_RET
{
	SHM_ERROR = 0,	// 初始化失败
	SHM_ATTACH,		// 附着的方式
	SHM_CREATE,		// 创建的方式
};

/* 共享内存池 */
typedef struct _shm_pool
{
	ShmAO *m_pShmAo[SHM_POOL_MAXCOUNT];	// 共享内存访问对象
}SHMPool;

int shm_pool_add( SM_KEY key, unsigned int size, void **ptr );
void shm_pool_del( SM_KEY key );
void shm_pool_delall();
#endif
