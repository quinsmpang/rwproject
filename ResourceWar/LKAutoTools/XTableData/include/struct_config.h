#ifndef __STRUCT_CONFIG_H
#define __STRUCT_CONFIG_H

struct _fieldinfo
{
	char m_hasdef;
	char m_field[32];
	char m_type[32];
	char m_null[8];
	char m_key[8];
	char m_default[24];
	char m_extra[24];
};
typedef struct _fieldinfo FInfo;

#define MAX_TABCOUNT	100
#define MAX_FIELDCOUNT	240

#define SCUI_CREATE_PATH	"./res/srvdata"

int tabcreate();
#endif
