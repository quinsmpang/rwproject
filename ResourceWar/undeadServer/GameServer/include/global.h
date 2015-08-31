#ifndef _GLOBAL_H_
#include "share_memory.h"

// 全局变量
typedef struct _global
{
	float damage_para1;
	float damage_para2;
	float damage_para3;
	float damage_para4;
	float dodge_para1;
	float dodge_para2;
	float dodge_para3;
	float dodge_para4;
	float crit_para1;
	float crit_para2;
	float crit_para3;
	float crit_para4;
	int robot_city_count;
}Global;

int global_init();


/* 放入共享内存的全局变量 */
/* 类型 */
enum SM_TYPE
{
	SMT_INVAILD = -1,
};

/* 共享内存数据 */
typedef struct _share_memory_data
{
	SM_KEY		m_Key;
	SM_TYPE		m_Type;

	_share_memory_data()
	{
		m_Key = 0;
		m_Type = SMT_INVAILD;
	}
}SM_DATA;


#endif
