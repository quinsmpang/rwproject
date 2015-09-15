#ifndef _SHM_AO_H_
#define _SHM_AO_H_
#include "share_memory.h"

/* ���������ڴ��ָ�� */
enum SM_CMD
{
	SM_CMD_CLEARALL = 1,		//���ģʽ
	SM_CMD_LOADDUMP = 2,		//����dumpģʽ
};

/* �����ڴ�ͷ�ṹ */
struct SMHead
{
	SM_KEY m_Key;			// ��ֵ
	unsigned long m_Size;	// ��С
	unsigned int m_HeadVer; // �����̰汾
	SMHead()
	{
		m_Key = 0;
		m_Size = 0;
		m_HeadVer = 0;
	}
};

/*
 *		�����ڴ���ʶ���
 *		ShareMemory	Access	Object
 */
typedef struct shm_ao 
{
	int m_CmdArg;			// ����
	unsigned int m_Size;	// ShareMemory	�ڴ��С
	char* m_pDataPtr; 		// ShareMemory  ����ָ��
	char* m_pHeader; 		// ShareMemory	�ڴ�ͷָ��
	SM_HANDLE m_Handle;		// ShareMemory	���	
}ShmAO;


/* ����ShareMemory ���ʶ���(�´���) */
BOOL shm_ao_create( ShmAO *pShmAO, SM_KEY key, unsigned int size );

/* ���ٶ��� */
void shm_ao_destory( ShmAO *pShmAO );

/* ����ShareMem ���ʶ���(�����´���) */
BOOL shm_ao_attach( ShmAO *pShmAO, SM_KEY key, unsigned int size );

/* ��ô�СΪsize�ĵ�index��smu������ */
char* shm_ao_gettypeptr( ShmAO *pShmAO, unsigned int size, unsigned int index );

/* dump�����ڴ����ݵ��ļ� */
BOOL shm_ao_dumptofile( ShmAO *pShmAO, char *path );

/* ���ı����ݼ��ص��ڴ���� */
BOOL shm_ao_mergefromfile( ShmAO *pShmAO, char *path );

/* �����ڴ�汾 */
unsigned int shm_ao_get_headver();
void shm_ao_set_headver( unsigned int ver );
#endif
