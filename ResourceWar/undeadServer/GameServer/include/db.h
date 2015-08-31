#ifndef __DB_H
#define __DB_H

int db_init();
int db_opendata();
int db_closedata();
int db_closegame();
char *db_escape( const char *pStr, char *pBuf, int size );
int db_reconnect_game();
int db_reconnect_log();

#endif



