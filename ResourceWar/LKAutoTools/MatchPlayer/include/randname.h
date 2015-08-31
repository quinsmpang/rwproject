#ifndef _RANDNAME_H_
#define _RANDNAME_H_

typedef struct _name 
{
	char m_name[16];
}PlayerName;
int randname_init();
int randname_get( char *name );
#endif

