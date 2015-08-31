#ifndef __GAMELOG_H
#define __GAMELOG_H

#include "utils.h"

/* ���� */
#define LOGOP_HERO				1	// Ӣ��
#define LOGOP_ITEMGET			2	// ��õ���
#define LOGOP_ITEMLOST			3	// ��ʧ����
#define LOGOP_CITYRES			4	// �ǳ���Դ


/* ;�� */
#define PATH_SYSTEM					1	// ϵͳ
#define PATH_GM						2	// GM
#define PATH_ITEMUSE				3	// ����ʹ�÷�ʽ
#define PATH_SELL					4	// ��������
#define PATH_BUILDING_GATHER		5	// �ɼ�������Դ

int log_init();
int wlog( char type, int op, char path, int object, int objectnum, i64 objectid, int source, int target );
int wcount();

#endif

