
#ifndef __GMCMD_H
#define __GMCMD_H

#define GMC_BASE			0
#define GMC_TEST			(GMC_BASE+1)	// �ű�ʹ��
#define GMC_SC				(GMC_BASE+2)	// �ű�ʹ��
#define GMC_SYSTALK			(GMC_BASE+3)	// ϵͳ����
#define GMC_EXP				(GMC_BASE+4)	// ����
#define GMC_RES				(GMC_BASE+5)	// ���
#define GMC_FIGHT			(GMC_BASE+9)	// ս��
#define GMC_CITY			(GMC_BASE+10)	// ��ǳ���Ϣ

// GMָ��ʹ����Ϣ
enum GMERROR
{
	GMERROR_NORMAL = 0,
	GMERROR_ACCESS_DENIED = -100, // ��Ȩ��
};
#endif
