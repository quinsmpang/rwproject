#include "cocos2d.h"
#include "tolua_fix.h"
#include "LuaBasicConversions.h"
#include "CCLuaValue.h"
#include "cocos-ext.h"
#include "CCLuaEngine.h"
#include "EventAnimate.h"
#include "lua_oneself_manual.hpp"
#include "ScrollViewEx.h"

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


static int tolua_oneself_EventAnimate_registerScriptHandlerSelf( lua_State* tolua_S )
{
	if ( NULL == tolua_S )
		return 0;

	int argc = 0;
	EventAnimate* self = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if ( !tolua_isusertype( tolua_S, 1, "EventAnimate", 0, &tolua_err ) ) goto tolua_lerror;
#endif

	self = static_cast<EventAnimate*>(tolua_tousertype( tolua_S, 1, 0 ));

#if COCOS2D_DEBUG >= 1
	if ( nullptr == self ) {
		tolua_error( tolua_S, "invalid 'self' in function 'tolua_oneself_EventAnimate_registerScriptHandlerSelf'\n", NULL );
		return 0;
	}
#endif
	argc = lua_gettop( tolua_S ) - 1;
	if ( 2 == argc )
	{
#if COCOS2D_DEBUG >= 1
		if ( !toluafix_isfunction( tolua_S, 2, "LUA_FUNCTION", 0, &tolua_err ) ||
			!tolua_isnumber( tolua_S, 3, 0, &tolua_err ) )
		{
			goto tolua_lerror;
		}
#endif
		LUA_FUNCTION handler = (toluafix_ref_function( tolua_S, 2, 0 ));
		ScriptHandlerMgr::HandlerType handlerType = (ScriptHandlerMgr::HandlerType) ((int)tolua_tonumber( tolua_S, 3, 0 ));

		ScriptHandlerMgr::getInstance()->addObjectHandler( (void*)self, handler, handlerType );
		return 0;
	}

	luaL_error( tolua_S, "%s function of EventAnimate has wrong number of arguments: %d, was expecting %d\n", "my.EventAnimate:registerScriptHandlerSelf", argc, 2 );
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error( tolua_S, "#ferror in function 'tolua_oneself_EventAnimate_registerScriptHandlerSelf'.", &tolua_err );
	return 0;
#endif
}

static int tolua_oneself_EventAnimate_unregisterScriptHandlerSelf( lua_State* tolua_S )
{
	if ( NULL == tolua_S )
		return 0;

	int argc = 0;
	EventAnimate* self = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if ( !tolua_isusertype( tolua_S, 1, "EventAnimate", 0, &tolua_err ) ) goto tolua_lerror;
#endif

	self = static_cast<EventAnimate*>(tolua_tousertype( tolua_S, 1, 0 ));

#if COCOS2D_DEBUG >= 1
	if ( nullptr == self ) {
		tolua_error( tolua_S, "invalid 'self' in function 'tolua_oneself_EventAnimate_unregisterScriptHandlerSelf'\n", NULL );
		return 0;
	}
#endif

	argc = lua_gettop( tolua_S ) - 1;

	if ( 1 == argc )
	{
#if COCOS2D_DEBUG >= 1
		if ( !tolua_isnumber( tolua_S, 2, 0, &tolua_err ) )
			goto tolua_lerror;
#endif
		ScriptHandlerMgr::HandlerType handlerType = (ScriptHandlerMgr::HandlerType) ((int)tolua_tonumber( tolua_S, 2, 0 ));
		ScriptHandlerMgr::getInstance()->removeObjectHandler( (void*)self, handlerType );
		return 0;
	}

	luaL_error( tolua_S, "%s function of EventAnimate  has wrong number of arguments: %d, was expecting %d\n", "my.EventAnimate:unregisterScriptHandlerSelf", argc, 1 );
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error( tolua_S, "#ferror in function 'tolua_oneself_EventAnimate_unregisterScriptHandlerSelf'.", &tolua_err );
	return 0;
#endif
}

static void extendEventAnimate( lua_State* tolua_S )
{
	lua_pushstring( tolua_S, "EventAnimate" );
	lua_rawget( tolua_S, LUA_REGISTRYINDEX );
	if ( lua_istable( tolua_S, -1 ) )
	{
		lua_pushstring( tolua_S, "registerScriptHandlerSelf" );
		lua_pushcfunction( tolua_S, tolua_oneself_EventAnimate_registerScriptHandlerSelf );
		lua_rawset( tolua_S, -3 );
		lua_pushstring( tolua_S, "unregisterScriptHandlerSelf" );
		lua_pushcfunction( tolua_S, tolua_oneself_EventAnimate_unregisterScriptHandlerSelf );
		lua_rawset( tolua_S, -3 );
	}
	lua_pop( tolua_S, 1 );
}

static int tolua_oneself_ScrollViewEx_registerScriptHandlerSelf( lua_State* tolua_S )
{
	if ( NULL == tolua_S )
		return 0;

	int argc = 0;
	ScrollViewEx* self = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if ( !tolua_isusertype( tolua_S, 1, "ScrollViewEx", 0, &tolua_err ) ) goto tolua_lerror;
#endif

	self = static_cast<ScrollViewEx*>(tolua_tousertype( tolua_S, 1, 0 ));

#if COCOS2D_DEBUG >= 1
	if ( nullptr == self ) {
		tolua_error( tolua_S, "invalid 'self' in function 'tolua_oneself_ScrollViewEx_registerScriptHandlerSelf'\n", NULL );
		return 0;
	}
#endif
	argc = lua_gettop( tolua_S ) - 1;
	if ( 2 == argc )
	{
#if COCOS2D_DEBUG >= 1
		if ( !toluafix_isfunction( tolua_S, 2, "LUA_FUNCTION", 0, &tolua_err ) ||
			!tolua_isnumber( tolua_S, 3, 0, &tolua_err ) )
		{
			goto tolua_lerror;
		}
#endif
		LUA_FUNCTION handler = (toluafix_ref_function( tolua_S, 2, 0 ));
		ScriptHandlerMgr::HandlerType handlerType = (ScriptHandlerMgr::HandlerType) ((int)tolua_tonumber( tolua_S, 3, 0 ));

		ScriptHandlerMgr::getInstance()->addObjectHandler( (void*)self, handler, handlerType );
		return 0;
	}

	luaL_error( tolua_S, "%s function of ScrollViewEx has wrong number of arguments: %d, was expecting %d\n", "my.ScrollViewEx:registerScriptHandlerSelf", argc, 2 );
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error( tolua_S, "#ferror in function 'tolua_oneself_ScrollViewEx_registerScriptHandlerSelf'.", &tolua_err );
	return 0;
#endif
}

static int tolua_oneself_ScrollViewEx_unregisterScriptHandlerSelf( lua_State* tolua_S )
{
	if ( NULL == tolua_S )
		return 0;

	int argc = 0;
	ScrollViewEx* self = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if ( !tolua_isusertype( tolua_S, 1, "ScrollViewEx", 0, &tolua_err ) ) goto tolua_lerror;
#endif

	self = static_cast<ScrollViewEx*>(tolua_tousertype( tolua_S, 1, 0 ));

#if COCOS2D_DEBUG >= 1
	if ( nullptr == self ) {
		tolua_error( tolua_S, "invalid 'self' in function 'tolua_oneself_ScrollViewEx_unregisterScriptHandlerSelf'\n", NULL );
		return 0;
	}
#endif

	argc = lua_gettop( tolua_S ) - 1;

	if ( 1 == argc )
	{
#if COCOS2D_DEBUG >= 1
		if ( !tolua_isnumber( tolua_S, 2, 0, &tolua_err ) )
			goto tolua_lerror;
#endif
		ScriptHandlerMgr::HandlerType handlerType = (ScriptHandlerMgr::HandlerType) ((int)tolua_tonumber( tolua_S, 2, 0 ));
		ScriptHandlerMgr::getInstance()->removeObjectHandler( (void*)self, handlerType );
		return 0;
	}

	luaL_error( tolua_S, "%s function of ScrollViewEx  has wrong number of arguments: %d, was expecting %d\n", "my.ScrollViewEx:unregisterScriptHandlerSelf", argc, 1 );
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error( tolua_S, "#ferror in function 'tolua_oneself_ScrollViewEx_unregisterScriptHandlerSelf'.", &tolua_err );
	return 0;
#endif
}

static void extendScrollViewEx( lua_State* tolua_S )
{
	lua_pushstring( tolua_S, "ScrollViewEx" );
	lua_rawget( tolua_S, LUA_REGISTRYINDEX );
	if ( lua_istable( tolua_S, -1 ) )
	{
		lua_pushstring( tolua_S, "registerScriptHandlerSelf" );
		lua_pushcfunction( tolua_S, tolua_oneself_ScrollViewEx_registerScriptHandlerSelf );
		lua_rawset( tolua_S, -3 );
		lua_pushstring( tolua_S, "unregisterScriptHandlerSelf" );
		lua_pushcfunction( tolua_S, tolua_oneself_ScrollViewEx_unregisterScriptHandlerSelf );
		lua_rawset( tolua_S, -3 );
	}
	lua_pop( tolua_S, 1 );
}

int register_all_oneself_manual( lua_State* tolua_S )
{
	lua_getglobal( tolua_S, "_G" );
	if ( lua_istable( tolua_S, -1 ) )//stack:...,_G,
	{
		extendEventAnimate( tolua_S );
		extendScrollViewEx( tolua_S );
	}
	lua_pop( tolua_S, 1 );
	return 1;
}