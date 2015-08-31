
#ifndef __STRUCT_CONFIG_H
#define __STRUCT_CONFIG_H

struct _struct_config {
	int m_dataid;
	char m_name[32];
	short data_type;
	short data_count;
	short data_size;
	char m_context[128];
	char m_notsave;	// 不保存，用于属性类数据
	char m_kind;	// 分类
	char m_fix;	// 是否有前缀m_
};
typedef struct _struct_config SStructConfig;

struct _struct_config_group {
	int m_structid;
	char m_name[32];
	char m_context[128];
	char m_struct_type;
	int m_count;
	SStructConfig *m_sconf;
};
typedef struct _struct_config_group SStructConfigGroup;

#define DATATYPE_CHAR	-1
#define DATATYPE_SHORT	-2
#define DATATYPE_INT	-3
#define DATATYPE_INT64	-4
#define DATATYPE_FLOAT	-5
#define DATATYPE_DOUBLE	-6
#define DATATYPE_BIN	-7

#define DATATYPE_UNCHAR	-11
#define DATATYPE_UNSHORT	-12
#define DATATYPE_UNINT	-13

#define SERVER_CREATE_PATH	"../../XmServer/LKGameServer/AutoS"
#define CLIENT_CREATE_PATH	"../LKGame/AutoC"

char *struct_getdatatype( short datatype, char *ptr, char *lkset, char type );

int struct_init();
int struct_reset();
int struct_add( char *struct_name, char *struct_context );
int struct_del( char *struct_name );
int struct_adddata( char *name, short type, short count, short size, char *context, char *struct_name );
int struct_deldata( char *name, char *struct_name );

int struct_create();
#endif
