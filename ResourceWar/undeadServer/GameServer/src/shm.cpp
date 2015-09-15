#ifdef WIN32
#include <winsock2.h>
#include <windows.h>
#endif
#include <mysql.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "db.h"
#include "define.h"
#include "shm.h"
extern SConfig g_Config;

// 共享内存池
SHMPool g_shmpool;

int shm_pool_add( SM_KEY key, unsigned int size, void **ptr )
{
	int ret = SHM_ERROR;
	if ( key <= 0 || key >= SHM_POOL_MAXCOUNT )
		return ret;

	g_shmpool.m_pShmAo[key] = (ShmAO*)malloc( sizeof(ShmAO) );
	if ( shm_ao_attach( g_shmpool.m_pShmAo[key], SHM_KEY_BASE+key, size + sizeof(SMHead) ) )
	{ // 附着方式
		ret = SHM_ATTACH;
	}
	else if ( shm_ao_create( g_shmpool.m_pShmAo[key], SHM_KEY_BASE+key, size + sizeof(SMHead) ) )
	{ // 新创建方式
		ret = SHM_CREATE;
	}
	
	if ( ret != SHM_ERROR )
	{
		*ptr = g_shmpool.m_pShmAo[key]->m_pDataPtr;
	}
	return ret;
}

void shm_pool_del( SM_KEY key )
{
	if ( key <= 0 || key >= SHM_POOL_MAXCOUNT )
		return;
	shm_ao_destory( g_shmpool.m_pShmAo[key] );
}

void shm_pool_delall()
{
	for ( int key = 1; key < SHM_POOL_MAXCOUNT; key++ )
	{
		shm_pool_del( key );
	}
}