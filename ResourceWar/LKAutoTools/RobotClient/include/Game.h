#ifndef GAME_H_
#define GAME_H_

#define	NAME_SIZE	16
#define MAX_USERNAME_LENGTH 50
#define MAX_PASSWORD_LENGTH 20

#define EXIT_ERROR_NONE		0
#define EXIT_ERROR_NET		1

void System_Init();
void System_Logic();
void System_Destroy();
void System_ExitGame( char type = EXIT_ERROR_NONE );
void System_SetExited( char exit );

void System_SetStat( int stat );
void System_SetStat_Direct( int stat );
int System_GetStat();

#endif