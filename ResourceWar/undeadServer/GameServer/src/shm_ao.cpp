#include <assert.h>
#include "shm_ao.h"

/* 创建ShareMemory访问对象(新创建)
SM_KEY	key			访问键值
unsigned int size	访问数据区字节个数
*/
BOOL shm_ao_create( ShmAO *pShmAO, SM_KEY key, unsigned int size )
{
	if ( !pShmAO )
		return FALSE;
	if ( pShmAO->m_CmdArg == SM_CMD_CLEARALL )
		return FALSE;

	// 创建指定key和size的共享内存区
	pShmAO->m_Handle = share_memory_create( key, size );
	if ( pShmAO->m_Handle == INVALID_SM_HANDLE )
	{
		write_netlog( "create share_memory Error SM_KEY = %d", key );
		return FALSE;
	}

	// 获得共享内存区的映射指针，起始点
	pShmAO->m_pHeader = share_memory_mapping( pShmAO->m_Handle );
	if ( pShmAO->m_pHeader )
	{
		// 设置数据开始指针地址，起始点 
		pShmAO->m_pDataPtr = pShmAO->m_pHeader + sizeof(SMHead);
		memset( pShmAO->m_pDataPtr, 0, size );
		// 设置Head信息，key 和 size信息
		((SMHead*)pShmAO->m_pHeader)->m_Key = key;
		((SMHead*)pShmAO->m_pHeader)->m_Size = size;
		// 数据块大小
		pShmAO->m_Size = size;
		write_netlog( "create share_memory ok SM_KEY = %d", key );
		return TRUE;
	}
	else
	{
		write_netlog( "create share_memory Error SM_KEY = %d", key );
		return FALSE;
	}
	return FALSE;
}

/* 销毁对象 */
void shm_ao_destory( ShmAO *pShmAO )
{
	if ( !pShmAO )
		return;
	if ( pShmAO->m_pHeader )
	{ // 关闭映射
		share_memory_unmapping( pShmAO->m_pHeader );
		pShmAO->m_pHeader = 0;
	}
	if ( pShmAO->m_Handle )
	{ // 关闭句柄
		share_memory_close( pShmAO->m_Handle );
		pShmAO->m_Handle = 0;
	}
	pShmAO->m_Size = 0;
}


/* 附着ShareMemory访问对象(不是新创建)
SM_KEY	key			访问键值
unsigned int size	访问数据区字节个数
*/
BOOL shm_ao_attach( ShmAO *pShmAO, SM_KEY key, unsigned int size )
{
	if ( !pShmAO )
		return FALSE;

	// 根据key和size打开共享内存区
	pShmAO->m_Handle = share_memory_open( key, size );

	if ( pShmAO->m_CmdArg == SM_CMD_CLEARALL )
	{
		shm_ao_destory( pShmAO );
		printf( "Close ShareMemory key = %d \r\n", key );
		return FALSE;
	}

	if ( pShmAO->m_Handle == INVALID_SM_HANDLE )
	{
		write_netlog( "attach share_memory error SM_KEY = %d", key );
		return FALSE;
	}

	// 映射共享内存区，获得内存块首地址
	pShmAO->m_pHeader = share_memory_mapping( pShmAO->m_Handle );
	if ( pShmAO->m_pHeader )
	{
		// 设置数据内容开始内存块首地址
		pShmAO->m_pDataPtr = pShmAO->m_pHeader + sizeof(SMHead);
		pShmAO->m_Size = size;
		write_netlog( "attach share_memory ok SM_KEY = %d", key );
		return TRUE;
	}
	else
	{
		write_netlog( "attach share_memory error SM_KEY = %d", key );
		return FALSE;
	}
	return FALSE;
}

/* 获得大小为size的第index个smu的数据 */
char* shm_ao_gettypeptr( ShmAO *pShmAO, unsigned int size, unsigned int index )
{
	if ( !pShmAO )
		return FALSE;
	assert( size > 0 );
	assert( size*index < pShmAO->m_Size );
	if ( size <= 0 || index >= pShmAO->m_Size )
		return NULL;
	return pShmAO->m_pDataPtr + size*index;
}

/* dump共享内存数据到文件 */
BOOL shm_ao_dumptofile( ShmAO *pShmAO, char *path )
{
	if ( !pShmAO )
		return FALSE;

	FILE* f = fopen( path, "wb" );
	if ( !f )
		return FALSE;
	fwrite( pShmAO->m_pHeader, 1, pShmAO->m_Size, f );
	fclose( f );
	return TRUE;
}

/* 将文本数据加载到内存操作 */
BOOL shm_ao_mergefromfile( ShmAO *pShmAO, char *path )
{
	if ( !pShmAO )
		return FALSE;

	FILE* f = fopen( path, "rb" );
	if ( !f )
		return FALSE;
	fseek( f, 0L, SEEK_END );
	int filelength = ftell( f );
	fseek( f, 0L, SEEK_SET );
	fread( pShmAO->m_pHeader, filelength, 1, f );
	fclose( f );
	return TRUE;
}

/* 共享内存版本 */
unsigned int shm_ao_get_headver( ShmAO *pShmAO )
{
	if ( !pShmAO )
		return 0;
	return ((SMHead*)pShmAO->m_pHeader)->m_HeadVer;
}
void shm_ao_set_headver( ShmAO *pShmAO, unsigned int ver )
{
	if ( !pShmAO )
		return;
	((SMHead*)pShmAO->m_pHeader)->m_HeadVer = ver;
}

