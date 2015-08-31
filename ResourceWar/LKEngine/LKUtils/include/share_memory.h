#ifndef __SHARE_MEMORY_H
#define __SHARE_MEMORY_H

#ifndef WIN32
#include <sys/ipc.h> 
#include <sys/shm.h> 
#endif
#include "utils.h"

/* �����ڴ��� */
#ifdef WIN32
typedef		VOID*	SM_HANDLE;
#else
typedef		int		SM_HANDLE;
#endif

/* ��ֵ */
typedef	ULONG SM_KEY;

/* ���������ڴ��� */
SM_HANDLE share_memory_create( SM_KEY key, unsigned int size );

/* �򿪹����ڴ��� */
SM_HANDLE share_memory_open( SM_KEY key, unsigned int size );

/* ӳ�乲���ڴ���*/
char* share_memory_mapping( SM_HANDLE smh );

/* �ر�ӳ�乲���ڴ��� */
void share_memory_unmapping( char* memptr );

/*	�رչ����ڴ��� */
void share_memory_close( SM_HANDLE smh );
#endif
