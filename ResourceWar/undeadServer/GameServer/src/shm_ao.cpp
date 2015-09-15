#include <assert.h>
#include "shm_ao.h"

/* ����ShareMemory���ʶ���(�´���)
SM_KEY	key			���ʼ�ֵ
unsigned int size	�����������ֽڸ���
*/
BOOL shm_ao_create( ShmAO *pShmAO, SM_KEY key, unsigned int size )
{
	if ( !pShmAO )
		return FALSE;
	if ( pShmAO->m_CmdArg == SM_CMD_CLEARALL )
		return FALSE;

	// ����ָ��key��size�Ĺ����ڴ���
	pShmAO->m_Handle = share_memory_create( key, size );
	if ( pShmAO->m_Handle == INVALID_SM_HANDLE )
	{
		write_netlog( "create share_memory Error SM_KEY = %d", key );
		return FALSE;
	}

	// ��ù����ڴ�����ӳ��ָ�룬��ʼ��
	pShmAO->m_pHeader = share_memory_mapping( pShmAO->m_Handle );
	if ( pShmAO->m_pHeader )
	{
		// �������ݿ�ʼָ���ַ����ʼ�� 
		pShmAO->m_pDataPtr = pShmAO->m_pHeader + sizeof(SMHead);
		memset( pShmAO->m_pDataPtr, 0, size );
		// ����Head��Ϣ��key �� size��Ϣ
		((SMHead*)pShmAO->m_pHeader)->m_Key = key;
		((SMHead*)pShmAO->m_pHeader)->m_Size = size;
		// ���ݿ��С
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

/* ���ٶ��� */
void shm_ao_destory( ShmAO *pShmAO )
{
	if ( !pShmAO )
		return;
	if ( pShmAO->m_pHeader )
	{ // �ر�ӳ��
		share_memory_unmapping( pShmAO->m_pHeader );
		pShmAO->m_pHeader = 0;
	}
	if ( pShmAO->m_Handle )
	{ // �رվ��
		share_memory_close( pShmAO->m_Handle );
		pShmAO->m_Handle = 0;
	}
	pShmAO->m_Size = 0;
}


/* ����ShareMemory���ʶ���(�����´���)
SM_KEY	key			���ʼ�ֵ
unsigned int size	�����������ֽڸ���
*/
BOOL shm_ao_attach( ShmAO *pShmAO, SM_KEY key, unsigned int size )
{
	if ( !pShmAO )
		return FALSE;

	// ����key��size�򿪹����ڴ���
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

	// ӳ�乲���ڴ���������ڴ���׵�ַ
	pShmAO->m_pHeader = share_memory_mapping( pShmAO->m_Handle );
	if ( pShmAO->m_pHeader )
	{
		// �����������ݿ�ʼ�ڴ���׵�ַ
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

/* ��ô�СΪsize�ĵ�index��smu������ */
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

/* dump�����ڴ����ݵ��ļ� */
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

/* ���ı����ݼ��ص��ڴ���� */
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

/* �����ڴ�汾 */
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

