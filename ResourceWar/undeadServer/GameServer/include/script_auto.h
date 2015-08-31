#ifndef __SCRIPT_auto_H
#define __SCRIPT_auto_H

void lua_func_register();
void lua_func_function();

int sc_Script_Init();
int sc_Script_Timer();
int sc_Script_Command( int nValue1, int nValue2, int nValue3, int nValue4, char *pMsg, int actor_index );

#endif
