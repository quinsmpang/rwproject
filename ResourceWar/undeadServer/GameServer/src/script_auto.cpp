extern "C" {
	#include "lua.h"
	#include "lualib.h"
	#include "lauxlib.h"
}

#include "script.h"
#include "define.h"
#include "thread.h"
#include "utils.h"

extern lua_State* servL;
//--Automatically define function

static int _Script_Init_ref;
static int _Script_Timer_ref;
static int _Script_Command_ref;

void lua_func_function()
{
	lua_getglobal(servL, "IN_Script_Init");
	_Script_Init_ref = luaL_ref( servL, LUA_REGISTRYINDEX );
	lua_getglobal(servL, "IN_Script_Timer");
	_Script_Timer_ref = luaL_ref( servL, LUA_REGISTRYINDEX );
	lua_getglobal(servL, "IN_Script_Command");
	_Script_Command_ref = luaL_ref( servL, LUA_REGISTRYINDEX );
}

int sc_Script_Init()
{
	int nResult = 0;
	lua_rawgeti( servL, LUA_REGISTRYINDEX, _Script_Init_ref );
	lua_call( servL, 0, 1 );
	nResult = (int)lua_tonumber( servL, -1 );
	lua_pop( servL, 1 );
	return nResult;
}

int sc_Script_Timer()
{
	int nResult = 0;
	lua_rawgeti( servL, LUA_REGISTRYINDEX, _Script_Timer_ref );
	lua_call( servL, 0, 1 );
	nResult = (int)lua_tonumber( servL, -1 );
	lua_pop( servL, 1 );
	return nResult;
}

int sc_Script_Command( int nValue1, int nValue2, int nValue3, int nValue4, char *pMsg, int actor_index )
{
	int nResult = 0;
	lua_rawgeti( servL, LUA_REGISTRYINDEX, _Script_Command_ref );
	lua_pushinteger( servL, nValue1 );
	lua_pushinteger( servL, nValue2 );
	lua_pushinteger( servL, nValue3 );
	lua_pushinteger( servL, nValue4 );
	lua_pushstring( servL, pMsg );
	lua_pushinteger( servL, actor_index );
	lua_call( servL, 6, 1 );
	nResult = (int)lua_tonumber( servL, -1 );
	lua_pop( servL, 1 );
	return nResult;
}

