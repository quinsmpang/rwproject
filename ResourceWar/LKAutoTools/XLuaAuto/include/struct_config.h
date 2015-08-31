#ifndef __STRUCT_CONFIG_H
#define __STRUCT_CONFIG_H

struct _struct_config {
	int m_structid;
	char m_name[64];
	char m_context[128];
	char m_type[16];
	char m_arg[16][32];
	char m_desc[16][32];
	int m_arg_count;
	int m_rtn_count;
	int m_limit_count;
};
typedef struct _struct_config SStructConfig;

#define DATATYPE_CHAR	-1
#define DATATYPE_SHORT	-2
#define DATATYPE_INT	-3
#define DATATYPE_INT64	-4
#define DATATYPE_FLOAT	-5
#define DATATYPE_DOUBLE	-6
#define DATATYPE_BIN	-7

#define DATATYPE_CHARPTR	-9
#define DATATYPE_VOIDPTR	-10

#define DATATYPE_UNCHAR	-11
#define DATATYPE_UNSHORT	-12
#define DATATYPE_UNINT	-13

#define SCUI_C_CREATE_PATH	"../undeadServer/GameServer/AutoS"
#define SCUI_IN_CREATE_PATH	"../undeadServer/GameServer"

char *struct_getdatatype( short datatype, char *ptr, char *lkset );

int struct_init( char *ptab );
int struct_reset( char *ptab );

int struct_create();
#endif
