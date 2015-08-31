#ifndef __DB_H
#define __DB_H

int db_init( char *host, char *user, char *pass, char *database );
int db_closedata();
char *db_escape( const char *pStr, char *pBuf, int size );
#endif
