#ifndef __oneself_manual_h__
#define __oneself_manual_h__

#ifdef __cplusplus
extern "C" {
#endif
#include "tolua++.h"
#ifdef __cplusplus
}
#endif

int register_all_oneself_manual( lua_State* tolua_S );
int call_luafun( const char *func, const char *sig, ... );

#endif // __oneself_manual_h__