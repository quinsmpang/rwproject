#ifndef _SHM_AO_H_
#define _SHM_AO_H_
#include "share_memory.h"

/* 操作共享内存的指令 */
enum SM_CMD
{
	SM_CMD_CLEARALL = 1,		//清除模式
	SM_CMD_LOADDUMP = 2,		//载入dump模式
};

/* 共享内存头结构 */
struct SMHead
{
	SM_KEY m_Key;			// 键值
	unsigned long m_Size;	// 大小
	unsigned int m_HeadVer; // 最后存盘版本
	SMHead()
	{
		m_Key = 0;
		m_Size = 0;
		m_HeadVer = 0;
	}
};

/*
 *		共享内存访问对象
 *		ShareMemory	Access	Object
 */
typedef struct shm_ao 
{
	int m_CmdArg;			// 命令
	unsigned int m_Size;	// ShareMemory	内存大小
	char* m_pDataPtr; 		// ShareMemory  数据指针
	char* m_pHeader; 		// ShareMemory	内存头指针
	SM_HANDLE m_Handle;		// ShareMemory	句柄	
}ShmAO;


/* 创建ShareMemory 访问对象(新创建) */
BOOL shm_ao_create( ShmAO *pShmAO, SM_KEY key, unsigned int size );

/* 销毁对象 */
void shm_ao_destory( ShmAO *pShmAO );

/* 附着ShareMem 访问对象(不是新创建) */
BOOL shm_ao_attach( ShmAO *pShmAO, SM_KEY key, unsigned int size );

/* 获得大小为size的第index个smu的数据 */
char* shm_ao_gettypeptr( ShmAO *pShmAO, unsigned int size, unsigned int index );

/* dump共享内存数据到文件 */
BOOL shm_ao_dumptofile( ShmAO *pShmAO, char *path );

/* 将文本数据加载到内存操作 */
BOOL shm_ao_mergefromfile( ShmAO *pShmAO, char *path );

/* 共享内存版本 */
unsigned int shm_ao_get_headver();
void shm_ao_set_headver( unsigned int ver );
#endif
