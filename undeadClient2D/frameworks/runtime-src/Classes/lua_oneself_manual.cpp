#include "cocos2d.h"
#include "tolua_fix.h"
#include "LuaBasicConversions.h"
#include "CCLuaValue.h"
#include "cocos-ext.h"
#include "CCLuaEngine.h"
#include "lua_oneself_manual.hpp"

USING_NS_CC;
USING_NS_CC_EXT;

//call_luafun("f", "dd>d", x, y, &z);
int call_luafun( const char *func, const char *sig, ... )
{
	lua_State* L = LuaEngine::getInstance()->getLuaStack()->getLuaState();

	va_list vl;
	int narg, nres; /* number of arguments and results */

	va_start( vl, sig );
	lua_getglobal( L, func ); /* push function */

	for ( narg = 0; *sig; narg++ )
	{ /* repeat for each argument */
		/* check stack space */
		luaL_checkstack( L, 1, "call_luafun too many arguments\n" );
		switch ( *sig++ )
		{
		case 'd': /* double argument */
			lua_pushnumber( L, va_arg( vl, double ) );
			break;
		case 'i': /* int argument */
			lua_pushinteger( L, va_arg( vl, int ) );
			break;
		case 's': /* string argument */
			lua_pushstring( L, va_arg( vl, char * ) );
			break;
		case 'u': /* userdata argument */
			lua_pushlightuserdata( L, va_arg( vl, void * ) );
			break;
		case '>': /* end of arguments */
			goto endargs;
		default:
			//handle_error( uiL, "call_luafun invalid option (%c)\n", *(sig - 1) );
			return -1;
		}
	}
endargs:

	nres = (int)strlen( sig ); /* number of expected results */
	/* do the call */
	if ( lua_pcall( L, narg, nres, 0 ) != 0 )
	{/* do the call */
		//handle_error( uiL, "call_luafun error calling '%s': \n", func );
		return -1;
	}

	nres = -nres; /* stack index of first result */
	while ( *sig )
	{ /* repeat for each result */
		switch ( *sig++ )
		{
		case 'd': /* double result */
			if ( !lua_isnumber( L, nres ) ) {
				//handle_error( uiL, "call_luafun wrong result type\n" );
				return -1;
			}
			*va_arg( vl, double * ) = lua_tonumber( L, nres );
			break;
		case 'i': /* int result */
			if ( !lua_isnumber( L, nres ) ) {
				//handle_error( uiL, "call_luafun wrong result type\n" );
				return -1;
			}
			*va_arg( vl, int * ) = (int)lua_tointeger( L, nres );
			break;
		case 's': /* string result */
			if ( !lua_isstring( L, nres ) ) {
				//handle_error( uiL, "call_luafun wrong result type\n" );
				return -1;
			}
			strcpy( va_arg( vl, char * ), lua_tostring( L, nres ) );
			break;
		default:
			//handle_error( uiL, "call_luafun invalid option (%c)\n", *(sig - 1) );
			return -1;
		}
		nres++;
	}

	va_end( vl );

	return 0;
}


int register_all_oneself_manual( lua_State* tolua_S )
{
	lua_getglobal( tolua_S, "_G" );
	if ( lua_istable( tolua_S, -1 ) )//stack:...,_G,
	{
		
	}
	lua_pop( tolua_S, 1 );
	return 1;
}