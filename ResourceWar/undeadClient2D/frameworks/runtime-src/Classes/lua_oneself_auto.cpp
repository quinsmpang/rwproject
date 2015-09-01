#include "lua_oneself_auto.hpp"
#include "cocos-ext.h"
#include "ByteBuffer.h"
#include "TcpManager.h"
#include "GlobalSchedule.h"
#include "IniUtil.h"
#include "TableUtil.h"
#include "ScrollViewEx.h"
#include "PlatformUtils.h"
#include "EventAnimate.h"
#include "tolua_fix.h"
#include "LuaBasicConversions.h"



int lua_oneself_ByteBuffer_WriteLong(lua_State* tolua_S)
{
    int argc = 0;
    ByteBuffer* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"ByteBuffer",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (ByteBuffer*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_oneself_ByteBuffer_WriteLong'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        long arg0;

        ok &= luaval_to_long(tolua_S, 2, &arg0, "ByteBuffer:WriteLong");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_oneself_ByteBuffer_WriteLong'", nullptr);
            return 0;
        }
        cobj->WriteLong(arg0);
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "ByteBuffer:WriteLong",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_oneself_ByteBuffer_WriteLong'.",&tolua_err);
#endif

    return 0;
}
int lua_oneself_ByteBuffer_ReadLong(lua_State* tolua_S)
{
    int argc = 0;
    ByteBuffer* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"ByteBuffer",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (ByteBuffer*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_oneself_ByteBuffer_ReadLong'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_oneself_ByteBuffer_ReadLong'", nullptr);
            return 0;
        }
        long long ret = cobj->ReadLong();
        tolua_pushnumber(tolua_S,(lua_Number)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "ByteBuffer:ReadLong",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_oneself_ByteBuffer_ReadLong'.",&tolua_err);
#endif

    return 0;
}
int lua_oneself_ByteBuffer_WriteInt(lua_State* tolua_S)
{
    int argc = 0;
    ByteBuffer* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"ByteBuffer",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (ByteBuffer*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_oneself_ByteBuffer_WriteInt'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        int arg0;

        ok &= luaval_to_int32(tolua_S, 2,(int *)&arg0, "ByteBuffer:WriteInt");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_oneself_ByteBuffer_WriteInt'", nullptr);
            return 0;
        }
        cobj->WriteInt(arg0);
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "ByteBuffer:WriteInt",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_oneself_ByteBuffer_WriteInt'.",&tolua_err);
#endif

    return 0;
}
int lua_oneself_ByteBuffer_WriteUInt(lua_State* tolua_S)
{
    int argc = 0;
    ByteBuffer* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"ByteBuffer",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (ByteBuffer*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_oneself_ByteBuffer_WriteUInt'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        unsigned int arg0;

        ok &= luaval_to_uint32(tolua_S, 2,&arg0, "ByteBuffer:WriteUInt");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_oneself_ByteBuffer_WriteUInt'", nullptr);
            return 0;
        }
        cobj->WriteUInt(arg0);
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "ByteBuffer:WriteUInt",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_oneself_ByteBuffer_WriteUInt'.",&tolua_err);
#endif

    return 0;
}
int lua_oneself_ByteBuffer_WriteString(lua_State* tolua_S)
{
    int argc = 0;
    ByteBuffer* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"ByteBuffer",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (ByteBuffer*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_oneself_ByteBuffer_WriteString'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        std::string arg0;

        ok &= luaval_to_std_string(tolua_S, 2,&arg0, "ByteBuffer:WriteString");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_oneself_ByteBuffer_WriteString'", nullptr);
            return 0;
        }
        cobj->WriteString(arg0);
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "ByteBuffer:WriteString",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_oneself_ByteBuffer_WriteString'.",&tolua_err);
#endif

    return 0;
}
int lua_oneself_ByteBuffer_WriteShort(lua_State* tolua_S)
{
    int argc = 0;
    ByteBuffer* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"ByteBuffer",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (ByteBuffer*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_oneself_ByteBuffer_WriteShort'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        int32_t arg0;

        ok &= luaval_to_int32(tolua_S, 2,&arg0, "ByteBuffer:WriteShort");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_oneself_ByteBuffer_WriteShort'", nullptr);
            return 0;
        }
        cobj->WriteShort(arg0);
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "ByteBuffer:WriteShort",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_oneself_ByteBuffer_WriteShort'.",&tolua_err);
#endif

    return 0;
}
int lua_oneself_ByteBuffer_WriteSByte(lua_State* tolua_S)
{
    int argc = 0;
    ByteBuffer* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"ByteBuffer",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (ByteBuffer*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_oneself_ByteBuffer_WriteSByte'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        uint16_t arg0;

        ok &= luaval_to_uint16(tolua_S, 2,&arg0, "ByteBuffer:WriteSByte");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_oneself_ByteBuffer_WriteSByte'", nullptr);
            return 0;
        }
        cobj->WriteSByte(arg0);
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "ByteBuffer:WriteSByte",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_oneself_ByteBuffer_WriteSByte'.",&tolua_err);
#endif

    return 0;
}
int lua_oneself_ByteBuffer_ReadStringWithLen(lua_State* tolua_S)
{
    int argc = 0;
    ByteBuffer* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"ByteBuffer",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (ByteBuffer*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_oneself_ByteBuffer_ReadStringWithLen'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        unsigned short arg0;

        ok &= luaval_to_ushort(tolua_S, 2, &arg0, "ByteBuffer:ReadStringWithLen");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_oneself_ByteBuffer_ReadStringWithLen'", nullptr);
            return 0;
        }
        std::string ret = cobj->ReadStringWithLen(arg0);
        tolua_pushcppstring(tolua_S,ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "ByteBuffer:ReadStringWithLen",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_oneself_ByteBuffer_ReadStringWithLen'.",&tolua_err);
#endif

    return 0;
}
int lua_oneself_ByteBuffer_ReadByte(lua_State* tolua_S)
{
    int argc = 0;
    ByteBuffer* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"ByteBuffer",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (ByteBuffer*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_oneself_ByteBuffer_ReadByte'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_oneself_ByteBuffer_ReadByte'", nullptr);
            return 0;
        }
        int ret = cobj->ReadByte();
        tolua_pushnumber(tolua_S,(lua_Number)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "ByteBuffer:ReadByte",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_oneself_ByteBuffer_ReadByte'.",&tolua_err);
#endif

    return 0;
}
int lua_oneself_ByteBuffer_WriteStringWithLength(lua_State* tolua_S)
{
    int argc = 0;
    ByteBuffer* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"ByteBuffer",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (ByteBuffer*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_oneself_ByteBuffer_WriteStringWithLength'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        std::string arg0;

        ok &= luaval_to_std_string(tolua_S, 2,&arg0, "ByteBuffer:WriteStringWithLength");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_oneself_ByteBuffer_WriteStringWithLength'", nullptr);
            return 0;
        }
        cobj->WriteStringWithLength(arg0);
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "ByteBuffer:WriteStringWithLength",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_oneself_ByteBuffer_WriteStringWithLength'.",&tolua_err);
#endif

    return 0;
}
int lua_oneself_ByteBuffer_ReadSByte(lua_State* tolua_S)
{
    int argc = 0;
    ByteBuffer* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"ByteBuffer",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (ByteBuffer*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_oneself_ByteBuffer_ReadSByte'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_oneself_ByteBuffer_ReadSByte'", nullptr);
            return 0;
        }
        int ret = cobj->ReadSByte();
        tolua_pushnumber(tolua_S,(lua_Number)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "ByteBuffer:ReadSByte",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_oneself_ByteBuffer_ReadSByte'.",&tolua_err);
#endif

    return 0;
}
int lua_oneself_ByteBuffer_WriteUShort(lua_State* tolua_S)
{
    int argc = 0;
    ByteBuffer* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"ByteBuffer",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (ByteBuffer*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_oneself_ByteBuffer_WriteUShort'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        unsigned short arg0;

        ok &= luaval_to_ushort(tolua_S, 2, &arg0, "ByteBuffer:WriteUShort");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_oneself_ByteBuffer_WriteUShort'", nullptr);
            return 0;
        }
        cobj->WriteUShort(arg0);
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "ByteBuffer:WriteUShort",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_oneself_ByteBuffer_WriteUShort'.",&tolua_err);
#endif

    return 0;
}
int lua_oneself_ByteBuffer_ReadFloat(lua_State* tolua_S)
{
    int argc = 0;
    ByteBuffer* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"ByteBuffer",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (ByteBuffer*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_oneself_ByteBuffer_ReadFloat'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_oneself_ByteBuffer_ReadFloat'", nullptr);
            return 0;
        }
        double ret = cobj->ReadFloat();
        tolua_pushnumber(tolua_S,(lua_Number)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "ByteBuffer:ReadFloat",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_oneself_ByteBuffer_ReadFloat'.",&tolua_err);
#endif

    return 0;
}
int lua_oneself_ByteBuffer_WriteDouble(lua_State* tolua_S)
{
    int argc = 0;
    ByteBuffer* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"ByteBuffer",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (ByteBuffer*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_oneself_ByteBuffer_WriteDouble'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        double arg0;

        ok &= luaval_to_number(tolua_S, 2,&arg0, "ByteBuffer:WriteDouble");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_oneself_ByteBuffer_WriteDouble'", nullptr);
            return 0;
        }
        cobj->WriteDouble(arg0);
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "ByteBuffer:WriteDouble",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_oneself_ByteBuffer_WriteDouble'.",&tolua_err);
#endif

    return 0;
}
int lua_oneself_ByteBuffer_WriteFloat(lua_State* tolua_S)
{
    int argc = 0;
    ByteBuffer* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"ByteBuffer",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (ByteBuffer*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_oneself_ByteBuffer_WriteFloat'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        double arg0;

        ok &= luaval_to_number(tolua_S, 2,&arg0, "ByteBuffer:WriteFloat");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_oneself_ByteBuffer_WriteFloat'", nullptr);
            return 0;
        }
        cobj->WriteFloat(arg0);
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "ByteBuffer:WriteFloat",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_oneself_ByteBuffer_WriteFloat'.",&tolua_err);
#endif

    return 0;
}
int lua_oneself_ByteBuffer_ReadShort(lua_State* tolua_S)
{
    int argc = 0;
    ByteBuffer* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"ByteBuffer",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (ByteBuffer*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_oneself_ByteBuffer_ReadShort'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_oneself_ByteBuffer_ReadShort'", nullptr);
            return 0;
        }
        int32_t ret = cobj->ReadShort();
        tolua_pushnumber(tolua_S,(lua_Number)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "ByteBuffer:ReadShort",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_oneself_ByteBuffer_ReadShort'.",&tolua_err);
#endif

    return 0;
}
int lua_oneself_ByteBuffer_ReadInt(lua_State* tolua_S)
{
    int argc = 0;
    ByteBuffer* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"ByteBuffer",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (ByteBuffer*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_oneself_ByteBuffer_ReadInt'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_oneself_ByteBuffer_ReadInt'", nullptr);
            return 0;
        }
        int ret = cobj->ReadInt();
        tolua_pushnumber(tolua_S,(lua_Number)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "ByteBuffer:ReadInt",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_oneself_ByteBuffer_ReadInt'.",&tolua_err);
#endif

    return 0;
}
int lua_oneself_ByteBuffer_WriteByte(lua_State* tolua_S)
{
    int argc = 0;
    ByteBuffer* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"ByteBuffer",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (ByteBuffer*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_oneself_ByteBuffer_WriteByte'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        int32_t arg0;

        ok &= luaval_to_int32(tolua_S, 2,&arg0, "ByteBuffer:WriteByte");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_oneself_ByteBuffer_WriteByte'", nullptr);
            return 0;
        }
        cobj->WriteByte(arg0);
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "ByteBuffer:WriteByte",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_oneself_ByteBuffer_WriteByte'.",&tolua_err);
#endif

    return 0;
}
int lua_oneself_ByteBuffer_ReadDouble(lua_State* tolua_S)
{
    int argc = 0;
    ByteBuffer* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"ByteBuffer",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (ByteBuffer*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_oneself_ByteBuffer_ReadDouble'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_oneself_ByteBuffer_ReadDouble'", nullptr);
            return 0;
        }
        double ret = cobj->ReadDouble();
        tolua_pushnumber(tolua_S,(lua_Number)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "ByteBuffer:ReadDouble",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_oneself_ByteBuffer_ReadDouble'.",&tolua_err);
#endif

    return 0;
}
int lua_oneself_ByteBuffer_ReadUShort(lua_State* tolua_S)
{
    int argc = 0;
    ByteBuffer* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"ByteBuffer",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (ByteBuffer*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_oneself_ByteBuffer_ReadUShort'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_oneself_ByteBuffer_ReadUShort'", nullptr);
            return 0;
        }
        unsigned short ret = cobj->ReadUShort();
        tolua_pushnumber(tolua_S,(lua_Number)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "ByteBuffer:ReadUShort",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_oneself_ByteBuffer_ReadUShort'.",&tolua_err);
#endif

    return 0;
}
int lua_oneself_ByteBuffer_ReadUInt(lua_State* tolua_S)
{
    int argc = 0;
    ByteBuffer* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"ByteBuffer",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (ByteBuffer*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_oneself_ByteBuffer_ReadUInt'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_oneself_ByteBuffer_ReadUInt'", nullptr);
            return 0;
        }
        unsigned int ret = cobj->ReadUInt();
        tolua_pushnumber(tolua_S,(lua_Number)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "ByteBuffer:ReadUInt",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_oneself_ByteBuffer_ReadUInt'.",&tolua_err);
#endif

    return 0;
}
int lua_oneself_ByteBuffer_ReadString(lua_State* tolua_S)
{
    int argc = 0;
    ByteBuffer* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"ByteBuffer",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (ByteBuffer*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_oneself_ByteBuffer_ReadString'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_oneself_ByteBuffer_ReadString'", nullptr);
            return 0;
        }
        std::string ret = cobj->ReadString();
        tolua_pushcppstring(tolua_S,ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "ByteBuffer:ReadString",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_oneself_ByteBuffer_ReadString'.",&tolua_err);
#endif

    return 0;
}
int lua_oneself_ByteBuffer_constructor(lua_State* tolua_S)
{
    int argc = 0;
    ByteBuffer* cobj = nullptr;
    bool ok  = true;
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

    argc = lua_gettop(tolua_S)-1;
    do{
        if (argc == 2) {
            void* arg0;
            ok &= luaval_to_void(tolua_S, 2, &arg0, "ByteBuffer:ByteBuffer");

            if (!ok) { break; }
            int arg1;
            ok &= luaval_to_int32(tolua_S, 3,(int *)&arg1, "ByteBuffer:ByteBuffer");

            if (!ok) { break; }
            cobj = new ByteBuffer(arg0, arg1);
            tolua_pushusertype(tolua_S,(void*)cobj,"ByteBuffer");
            tolua_register_gc(tolua_S,lua_gettop(tolua_S));
            return 1;
        }
    }while(0);
    ok  = true;
    do{
        if (argc == 0) {
            cobj = new ByteBuffer();
            tolua_pushusertype(tolua_S,(void*)cobj,"ByteBuffer");
            tolua_register_gc(tolua_S,lua_gettop(tolua_S));
            return 1;
        }
    }while(0);
    ok  = true;
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n",  "ByteBuffer:ByteBuffer",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_error(tolua_S,"#ferror in function 'lua_oneself_ByteBuffer_constructor'.",&tolua_err);
#endif

    return 0;
}

static int lua_oneself_ByteBuffer_finalize(lua_State* tolua_S)
{
    printf("luabindings: finalizing LUA object (ByteBuffer)");
    return 0;
}

int lua_register_oneself_ByteBuffer(lua_State* tolua_S)
{
    tolua_usertype(tolua_S,"ByteBuffer");
    tolua_cclass(tolua_S,"ByteBuffer","ByteBuffer","",nullptr);

    tolua_beginmodule(tolua_S,"ByteBuffer");
        tolua_function(tolua_S,"new",lua_oneself_ByteBuffer_constructor);
        tolua_function(tolua_S,"WriteLong",lua_oneself_ByteBuffer_WriteLong);
        tolua_function(tolua_S,"ReadLong",lua_oneself_ByteBuffer_ReadLong);
        tolua_function(tolua_S,"WriteInt",lua_oneself_ByteBuffer_WriteInt);
        tolua_function(tolua_S,"WriteUInt",lua_oneself_ByteBuffer_WriteUInt);
        tolua_function(tolua_S,"WriteString",lua_oneself_ByteBuffer_WriteString);
        tolua_function(tolua_S,"WriteShort",lua_oneself_ByteBuffer_WriteShort);
        tolua_function(tolua_S,"WriteSByte",lua_oneself_ByteBuffer_WriteSByte);
        tolua_function(tolua_S,"ReadStringWithLen",lua_oneself_ByteBuffer_ReadStringWithLen);
        tolua_function(tolua_S,"ReadByte",lua_oneself_ByteBuffer_ReadByte);
        tolua_function(tolua_S,"WriteStringWithLength",lua_oneself_ByteBuffer_WriteStringWithLength);
        tolua_function(tolua_S,"ReadSByte",lua_oneself_ByteBuffer_ReadSByte);
        tolua_function(tolua_S,"WriteUShort",lua_oneself_ByteBuffer_WriteUShort);
        tolua_function(tolua_S,"ReadFloat",lua_oneself_ByteBuffer_ReadFloat);
        tolua_function(tolua_S,"WriteDouble",lua_oneself_ByteBuffer_WriteDouble);
        tolua_function(tolua_S,"WriteFloat",lua_oneself_ByteBuffer_WriteFloat);
        tolua_function(tolua_S,"ReadShort",lua_oneself_ByteBuffer_ReadShort);
        tolua_function(tolua_S,"ReadInt",lua_oneself_ByteBuffer_ReadInt);
        tolua_function(tolua_S,"WriteByte",lua_oneself_ByteBuffer_WriteByte);
        tolua_function(tolua_S,"ReadDouble",lua_oneself_ByteBuffer_ReadDouble);
        tolua_function(tolua_S,"ReadUShort",lua_oneself_ByteBuffer_ReadUShort);
        tolua_function(tolua_S,"ReadUInt",lua_oneself_ByteBuffer_ReadUInt);
        tolua_function(tolua_S,"ReadString",lua_oneself_ByteBuffer_ReadString);
    tolua_endmodule(tolua_S);
    std::string typeName = typeid(ByteBuffer).name();
    g_luaType[typeName] = "ByteBuffer";
    g_typeCast["ByteBuffer"] = "ByteBuffer";
    return 1;
}

int lua_oneself_TcpManager_ConnectServer(lua_State* tolua_S)
{
    int argc = 0;
    TcpManager* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"TcpManager",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (TcpManager*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_oneself_TcpManager_ConnectServer'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 2) 
    {
        const char* arg0;
        unsigned short arg1;

        std::string arg0_tmp; ok &= luaval_to_std_string(tolua_S, 2, &arg0_tmp, "TcpManager:ConnectServer"); arg0 = arg0_tmp.c_str();

        ok &= luaval_to_ushort(tolua_S, 3, &arg1, "TcpManager:ConnectServer");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_oneself_TcpManager_ConnectServer'", nullptr);
            return 0;
        }
        cobj->ConnectServer(arg0, arg1);
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "TcpManager:ConnectServer",argc, 2);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_oneself_TcpManager_ConnectServer'.",&tolua_err);
#endif

    return 0;
}
int lua_oneself_TcpManager_onConnect(lua_State* tolua_S)
{
    int argc = 0;
    TcpManager* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"TcpManager",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (TcpManager*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_oneself_TcpManager_onConnect'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_oneself_TcpManager_onConnect'", nullptr);
            return 0;
        }
        cobj->onConnect();
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "TcpManager:onConnect",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_oneself_TcpManager_onConnect'.",&tolua_err);
#endif

    return 0;
}
int lua_oneself_TcpManager_onError(lua_State* tolua_S)
{
    int argc = 0;
    TcpManager* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"TcpManager",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (TcpManager*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_oneself_TcpManager_onError'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        int arg0;

        ok &= luaval_to_int32(tolua_S, 2,(int *)&arg0, "TcpManager:onError");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_oneself_TcpManager_onError'", nullptr);
            return 0;
        }
        cobj->onError(arg0);
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "TcpManager:onError",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_oneself_TcpManager_onError'.",&tolua_err);
#endif

    return 0;
}
int lua_oneself_TcpManager_onClose(lua_State* tolua_S)
{
    int argc = 0;
    TcpManager* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"TcpManager",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (TcpManager*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_oneself_TcpManager_onClose'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_oneself_TcpManager_onClose'", nullptr);
            return 0;
        }
        cobj->onClose();
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "TcpManager:onClose",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_oneself_TcpManager_onClose'.",&tolua_err);
#endif

    return 0;
}
int lua_oneself_TcpManager_SendMessage(lua_State* tolua_S)
{
    int argc = 0;
    TcpManager* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"TcpManager",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (TcpManager*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_oneself_TcpManager_SendMessage'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        ByteBuffer* arg0;

        ok &= luaval_to_object<ByteBuffer>(tolua_S, 2, "ByteBuffer",&arg0);
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_oneself_TcpManager_SendMessage'", nullptr);
            return 0;
        }
        cobj->SendMessage(arg0);
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "TcpManager:SendMessage",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_oneself_TcpManager_SendMessage'.",&tolua_err);
#endif

    return 0;
}
int lua_oneself_TcpManager_OnDisconnected(lua_State* tolua_S)
{
    int argc = 0;
    TcpManager* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"TcpManager",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (TcpManager*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_oneself_TcpManager_OnDisconnected'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_oneself_TcpManager_OnDisconnected'", nullptr);
            return 0;
        }
        cobj->OnDisconnected();
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "TcpManager:OnDisconnected",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_oneself_TcpManager_OnDisconnected'.",&tolua_err);
#endif

    return 0;
}
int lua_oneself_TcpManager_onMessage(lua_State* tolua_S)
{
    int argc = 0;
    TcpManager* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"TcpManager",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (TcpManager*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_oneself_TcpManager_onMessage'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 2) 
    {
        const char* arg0;
        int arg1;

        std::string arg0_tmp; ok &= luaval_to_std_string(tolua_S, 2, &arg0_tmp, "TcpManager:onMessage"); arg0 = arg0_tmp.c_str();

        ok &= luaval_to_int32(tolua_S, 3,(int *)&arg1, "TcpManager:onMessage");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_oneself_TcpManager_onMessage'", nullptr);
            return 0;
        }
        cobj->onMessage(arg0, arg1);
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "TcpManager:onMessage",argc, 2);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_oneself_TcpManager_onMessage'.",&tolua_err);
#endif

    return 0;
}
int lua_oneself_TcpManager_onConnectFail(lua_State* tolua_S)
{
    int argc = 0;
    TcpManager* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"TcpManager",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (TcpManager*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_oneself_TcpManager_onConnectFail'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_oneself_TcpManager_onConnectFail'", nullptr);
            return 0;
        }
        cobj->onConnectFail();
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "TcpManager:onConnectFail",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_oneself_TcpManager_onConnectFail'.",&tolua_err);
#endif

    return 0;
}
int lua_oneself_TcpManager_constructor(lua_State* tolua_S)
{
    int argc = 0;
    TcpManager* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif



    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_oneself_TcpManager_constructor'", nullptr);
            return 0;
        }
        cobj = new TcpManager();
        tolua_pushusertype(tolua_S,(void*)cobj,"TcpManager");
        tolua_register_gc(tolua_S,lua_gettop(tolua_S));
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "TcpManager:TcpManager",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_error(tolua_S,"#ferror in function 'lua_oneself_TcpManager_constructor'.",&tolua_err);
#endif

    return 0;
}

static int lua_oneself_TcpManager_finalize(lua_State* tolua_S)
{
    printf("luabindings: finalizing LUA object (TcpManager)");
    return 0;
}

int lua_register_oneself_TcpManager(lua_State* tolua_S)
{
    tolua_usertype(tolua_S,"TcpManager");
    tolua_cclass(tolua_S,"TcpManager","TcpManager","TcpClientDelegate",nullptr);

    tolua_beginmodule(tolua_S,"TcpManager");
        tolua_function(tolua_S,"new",lua_oneself_TcpManager_constructor);
        tolua_function(tolua_S,"ConnectServer",lua_oneself_TcpManager_ConnectServer);
        tolua_function(tolua_S,"onConnect",lua_oneself_TcpManager_onConnect);
        tolua_function(tolua_S,"onError",lua_oneself_TcpManager_onError);
        tolua_function(tolua_S,"onClose",lua_oneself_TcpManager_onClose);
        tolua_function(tolua_S,"SendMessage",lua_oneself_TcpManager_SendMessage);
        tolua_function(tolua_S,"OnDisconnected",lua_oneself_TcpManager_OnDisconnected);
        tolua_function(tolua_S,"onMessage",lua_oneself_TcpManager_onMessage);
        tolua_function(tolua_S,"onConnectFail",lua_oneself_TcpManager_onConnectFail);
    tolua_endmodule(tolua_S);
    std::string typeName = typeid(TcpManager).name();
    g_luaType[typeName] = "TcpManager";
    g_typeCast["TcpManager"] = "TcpManager";
    return 1;
}

int lua_oneself_GlobalSchedule_start(lua_State* tolua_S)
{
    int argc = 0;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertable(tolua_S,1,"GlobalSchedule",0,&tolua_err)) goto tolua_lerror;
#endif

    argc = lua_gettop(tolua_S) - 1;

    if (argc == 0)
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_oneself_GlobalSchedule_start'", nullptr);
            return 0;
        }
        GlobalSchedule::start();
        lua_settop(tolua_S, 1);
        return 1;
    }
    if (argc == 1)
    {
        double arg0;
        ok &= luaval_to_number(tolua_S, 2,&arg0, "GlobalSchedule:start");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_oneself_GlobalSchedule_start'", nullptr);
            return 0;
        }
        GlobalSchedule::start(arg0);
        lua_settop(tolua_S, 1);
        return 1;
    }
    if (argc == 2)
    {
        double arg0;
        double arg1;
        ok &= luaval_to_number(tolua_S, 2,&arg0, "GlobalSchedule:start");
        ok &= luaval_to_number(tolua_S, 3,&arg1, "GlobalSchedule:start");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_oneself_GlobalSchedule_start'", nullptr);
            return 0;
        }
        GlobalSchedule::start(arg0, arg1);
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n ", "GlobalSchedule:start",argc, 0);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_oneself_GlobalSchedule_start'.",&tolua_err);
#endif
    return 0;
}
int lua_oneself_GlobalSchedule_pause(lua_State* tolua_S)
{
    int argc = 0;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertable(tolua_S,1,"GlobalSchedule",0,&tolua_err)) goto tolua_lerror;
#endif

    argc = lua_gettop(tolua_S) - 1;

    if (argc == 0)
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_oneself_GlobalSchedule_pause'", nullptr);
            return 0;
        }
        GlobalSchedule::pause();
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n ", "GlobalSchedule:pause",argc, 0);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_oneself_GlobalSchedule_pause'.",&tolua_err);
#endif
    return 0;
}
int lua_oneself_GlobalSchedule_stop(lua_State* tolua_S)
{
    int argc = 0;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertable(tolua_S,1,"GlobalSchedule",0,&tolua_err)) goto tolua_lerror;
#endif

    argc = lua_gettop(tolua_S) - 1;

    if (argc == 0)
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_oneself_GlobalSchedule_stop'", nullptr);
            return 0;
        }
        GlobalSchedule::stop();
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n ", "GlobalSchedule:stop",argc, 0);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_oneself_GlobalSchedule_stop'.",&tolua_err);
#endif
    return 0;
}
int lua_oneself_GlobalSchedule_resume(lua_State* tolua_S)
{
    int argc = 0;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertable(tolua_S,1,"GlobalSchedule",0,&tolua_err)) goto tolua_lerror;
#endif

    argc = lua_gettop(tolua_S) - 1;

    if (argc == 0)
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_oneself_GlobalSchedule_resume'", nullptr);
            return 0;
        }
        GlobalSchedule::resume();
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n ", "GlobalSchedule:resume",argc, 0);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_oneself_GlobalSchedule_resume'.",&tolua_err);
#endif
    return 0;
}
static int lua_oneself_GlobalSchedule_finalize(lua_State* tolua_S)
{
    printf("luabindings: finalizing LUA object (GlobalSchedule)");
    return 0;
}

int lua_register_oneself_GlobalSchedule(lua_State* tolua_S)
{
    tolua_usertype(tolua_S,"GlobalSchedule");
    tolua_cclass(tolua_S,"GlobalSchedule","GlobalSchedule","cc.Ref",nullptr);

    tolua_beginmodule(tolua_S,"GlobalSchedule");
        tolua_function(tolua_S,"start", lua_oneself_GlobalSchedule_start);
        tolua_function(tolua_S,"pause", lua_oneself_GlobalSchedule_pause);
        tolua_function(tolua_S,"stop", lua_oneself_GlobalSchedule_stop);
        tolua_function(tolua_S,"resume", lua_oneself_GlobalSchedule_resume);
    tolua_endmodule(tolua_S);
    std::string typeName = typeid(GlobalSchedule).name();
    g_luaType[typeName] = "GlobalSchedule";
    g_typeCast["GlobalSchedule"] = "GlobalSchedule";
    return 1;
}

int lua_oneself_IniUtil_getDouble(lua_State* tolua_S)
{
    int argc = 0;
    IniUtil* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"IniUtil",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (IniUtil*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_oneself_IniUtil_getDouble'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 3) 
    {
        std::string arg0;
        std::string arg1;
        double arg2;

        ok &= luaval_to_std_string(tolua_S, 2,&arg0, "IniUtil:getDouble");

        ok &= luaval_to_std_string(tolua_S, 3,&arg1, "IniUtil:getDouble");

        ok &= luaval_to_number(tolua_S, 4,&arg2, "IniUtil:getDouble");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_oneself_IniUtil_getDouble'", nullptr);
            return 0;
        }
        double ret = cobj->getDouble(arg0, arg1, arg2);
        tolua_pushnumber(tolua_S,(lua_Number)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "IniUtil:getDouble",argc, 3);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_oneself_IniUtil_getDouble'.",&tolua_err);
#endif

    return 0;
}
int lua_oneself_IniUtil_setDouble(lua_State* tolua_S)
{
    int argc = 0;
    IniUtil* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"IniUtil",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (IniUtil*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_oneself_IniUtil_setDouble'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 4) 
    {
        std::string arg0;
        std::string arg1;
        double arg2;
        int arg3;

        ok &= luaval_to_std_string(tolua_S, 2,&arg0, "IniUtil:setDouble");

        ok &= luaval_to_std_string(tolua_S, 3,&arg1, "IniUtil:setDouble");

        ok &= luaval_to_number(tolua_S, 4,&arg2, "IniUtil:setDouble");

        ok &= luaval_to_int32(tolua_S, 5,(int *)&arg3, "IniUtil:setDouble");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_oneself_IniUtil_setDouble'", nullptr);
            return 0;
        }
        cobj->setDouble(arg0, arg1, arg2, arg3);
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "IniUtil:setDouble",argc, 4);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_oneself_IniUtil_setDouble'.",&tolua_err);
#endif

    return 0;
}
int lua_oneself_IniUtil_getString(lua_State* tolua_S)
{
    int argc = 0;
    IniUtil* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"IniUtil",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (IniUtil*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_oneself_IniUtil_getString'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 3) 
    {
        std::string arg0;
        std::string arg1;
        std::string arg2;

        ok &= luaval_to_std_string(tolua_S, 2,&arg0, "IniUtil:getString");

        ok &= luaval_to_std_string(tolua_S, 3,&arg1, "IniUtil:getString");

        ok &= luaval_to_std_string(tolua_S, 4,&arg2, "IniUtil:getString");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_oneself_IniUtil_getString'", nullptr);
            return 0;
        }
        std::string ret = cobj->getString(arg0, arg1, arg2);
        tolua_pushcppstring(tolua_S,ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "IniUtil:getString",argc, 3);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_oneself_IniUtil_getString'.",&tolua_err);
#endif

    return 0;
}
int lua_oneself_IniUtil_deleteSection(lua_State* tolua_S)
{
    int argc = 0;
    IniUtil* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"IniUtil",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (IniUtil*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_oneself_IniUtil_deleteSection'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        std::string arg0;

        ok &= luaval_to_std_string(tolua_S, 2,&arg0, "IniUtil:deleteSection");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_oneself_IniUtil_deleteSection'", nullptr);
            return 0;
        }
        bool ret = cobj->deleteSection(arg0);
        tolua_pushboolean(tolua_S,(bool)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "IniUtil:deleteSection",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_oneself_IniUtil_deleteSection'.",&tolua_err);
#endif

    return 0;
}
int lua_oneself_IniUtil_getInteger(lua_State* tolua_S)
{
    int argc = 0;
    IniUtil* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"IniUtil",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (IniUtil*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_oneself_IniUtil_getInteger'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 3) 
    {
        std::string arg0;
        std::string arg1;
        int arg2;

        ok &= luaval_to_std_string(tolua_S, 2,&arg0, "IniUtil:getInteger");

        ok &= luaval_to_std_string(tolua_S, 3,&arg1, "IniUtil:getInteger");

        ok &= luaval_to_int32(tolua_S, 4,(int *)&arg2, "IniUtil:getInteger");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_oneself_IniUtil_getInteger'", nullptr);
            return 0;
        }
        int ret = cobj->getInteger(arg0, arg1, arg2);
        tolua_pushnumber(tolua_S,(lua_Number)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "IniUtil:getInteger",argc, 3);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_oneself_IniUtil_getInteger'.",&tolua_err);
#endif

    return 0;
}
int lua_oneself_IniUtil_loadFromFile(lua_State* tolua_S)
{
    int argc = 0;
    IniUtil* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"IniUtil",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (IniUtil*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_oneself_IniUtil_loadFromFile'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        std::string arg0;

        ok &= luaval_to_std_string(tolua_S, 2,&arg0, "IniUtil:loadFromFile");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_oneself_IniUtil_loadFromFile'", nullptr);
            return 0;
        }
        bool ret = cobj->loadFromFile(arg0);
        tolua_pushboolean(tolua_S,(bool)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "IniUtil:loadFromFile",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_oneself_IniUtil_loadFromFile'.",&tolua_err);
#endif

    return 0;
}
int lua_oneself_IniUtil_isSectionExist(lua_State* tolua_S)
{
    int argc = 0;
    IniUtil* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"IniUtil",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (IniUtil*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_oneself_IniUtil_isSectionExist'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        std::string arg0;

        ok &= luaval_to_std_string(tolua_S, 2,&arg0, "IniUtil:isSectionExist");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_oneself_IniUtil_isSectionExist'", nullptr);
            return 0;
        }
        bool ret = cobj->isSectionExist(arg0);
        tolua_pushboolean(tolua_S,(bool)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "IniUtil:isSectionExist",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_oneself_IniUtil_isSectionExist'.",&tolua_err);
#endif

    return 0;
}
int lua_oneself_IniUtil_getBoolean(lua_State* tolua_S)
{
    int argc = 0;
    IniUtil* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"IniUtil",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (IniUtil*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_oneself_IniUtil_getBoolean'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 3) 
    {
        std::string arg0;
        std::string arg1;
        bool arg2;

        ok &= luaval_to_std_string(tolua_S, 2,&arg0, "IniUtil:getBoolean");

        ok &= luaval_to_std_string(tolua_S, 3,&arg1, "IniUtil:getBoolean");

        ok &= luaval_to_boolean(tolua_S, 4,&arg2, "IniUtil:getBoolean");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_oneself_IniUtil_getBoolean'", nullptr);
            return 0;
        }
        bool ret = cobj->getBoolean(arg0, arg1, arg2);
        tolua_pushboolean(tolua_S,(bool)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "IniUtil:getBoolean",argc, 3);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_oneself_IniUtil_getBoolean'.",&tolua_err);
#endif

    return 0;
}
int lua_oneself_IniUtil_setInteger(lua_State* tolua_S)
{
    int argc = 0;
    IniUtil* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"IniUtil",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (IniUtil*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_oneself_IniUtil_setInteger'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 3) 
    {
        std::string arg0;
        std::string arg1;
        int arg2;

        ok &= luaval_to_std_string(tolua_S, 2,&arg0, "IniUtil:setInteger");

        ok &= luaval_to_std_string(tolua_S, 3,&arg1, "IniUtil:setInteger");

        ok &= luaval_to_int32(tolua_S, 4,(int *)&arg2, "IniUtil:setInteger");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_oneself_IniUtil_setInteger'", nullptr);
            return 0;
        }
        cobj->setInteger(arg0, arg1, arg2);
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "IniUtil:setInteger",argc, 3);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_oneself_IniUtil_setInteger'.",&tolua_err);
#endif

    return 0;
}
int lua_oneself_IniUtil_flush(lua_State* tolua_S)
{
    int argc = 0;
    IniUtil* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"IniUtil",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (IniUtil*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_oneself_IniUtil_flush'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        std::string arg0;

        ok &= luaval_to_std_string(tolua_S, 2,&arg0, "IniUtil:flush");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_oneself_IniUtil_flush'", nullptr);
            return 0;
        }
        cobj->flush(arg0);
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "IniUtil:flush",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_oneself_IniUtil_flush'.",&tolua_err);
#endif

    return 0;
}
int lua_oneself_IniUtil_getFilePath(lua_State* tolua_S)
{
    int argc = 0;
    IniUtil* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"IniUtil",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (IniUtil*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_oneself_IniUtil_getFilePath'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_oneself_IniUtil_getFilePath'", nullptr);
            return 0;
        }
        std::string ret = cobj->getFilePath();
        tolua_pushcppstring(tolua_S,ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "IniUtil:getFilePath",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_oneself_IniUtil_getFilePath'.",&tolua_err);
#endif

    return 0;
}
int lua_oneself_IniUtil_setString(lua_State* tolua_S)
{
    int argc = 0;
    IniUtil* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"IniUtil",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (IniUtil*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_oneself_IniUtil_setString'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 3) 
    {
        std::string arg0;
        std::string arg1;
        std::string arg2;

        ok &= luaval_to_std_string(tolua_S, 2,&arg0, "IniUtil:setString");

        ok &= luaval_to_std_string(tolua_S, 3,&arg1, "IniUtil:setString");

        ok &= luaval_to_std_string(tolua_S, 4,&arg2, "IniUtil:setString");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_oneself_IniUtil_setString'", nullptr);
            return 0;
        }
        cobj->setString(arg0, arg1, arg2);
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "IniUtil:setString",argc, 3);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_oneself_IniUtil_setString'.",&tolua_err);
#endif

    return 0;
}
int lua_oneself_IniUtil_setBoolean(lua_State* tolua_S)
{
    int argc = 0;
    IniUtil* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"IniUtil",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (IniUtil*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_oneself_IniUtil_setBoolean'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 3) 
    {
        std::string arg0;
        std::string arg1;
        bool arg2;

        ok &= luaval_to_std_string(tolua_S, 2,&arg0, "IniUtil:setBoolean");

        ok &= luaval_to_std_string(tolua_S, 3,&arg1, "IniUtil:setBoolean");

        ok &= luaval_to_boolean(tolua_S, 4,&arg2, "IniUtil:setBoolean");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_oneself_IniUtil_setBoolean'", nullptr);
            return 0;
        }
        cobj->setBoolean(arg0, arg1, arg2);
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "IniUtil:setBoolean",argc, 3);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_oneself_IniUtil_setBoolean'.",&tolua_err);
#endif

    return 0;
}
int lua_oneself_IniUtil_createFromFile(lua_State* tolua_S)
{
    int argc = 0;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertable(tolua_S,1,"IniUtil",0,&tolua_err)) goto tolua_lerror;
#endif

    argc = lua_gettop(tolua_S) - 1;

    if (argc == 1)
    {
        std::string arg0;
        ok &= luaval_to_std_string(tolua_S, 2,&arg0, "IniUtil:createFromFile");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_oneself_IniUtil_createFromFile'", nullptr);
            return 0;
        }
        IniUtil* ret = IniUtil::createFromFile(arg0);
        object_to_luaval<IniUtil>(tolua_S, "IniUtil",(IniUtil*)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n ", "IniUtil:createFromFile",argc, 1);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_oneself_IniUtil_createFromFile'.",&tolua_err);
#endif
    return 0;
}
int lua_oneself_IniUtil_constructor(lua_State* tolua_S)
{
    int argc = 0;
    IniUtil* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif



    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_oneself_IniUtil_constructor'", nullptr);
            return 0;
        }
        cobj = new IniUtil();
        tolua_pushusertype(tolua_S,(void*)cobj,"IniUtil");
        tolua_register_gc(tolua_S,lua_gettop(tolua_S));
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "IniUtil:IniUtil",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_error(tolua_S,"#ferror in function 'lua_oneself_IniUtil_constructor'.",&tolua_err);
#endif

    return 0;
}

static int lua_oneself_IniUtil_finalize(lua_State* tolua_S)
{
    printf("luabindings: finalizing LUA object (IniUtil)");
    return 0;
}

int lua_register_oneself_IniUtil(lua_State* tolua_S)
{
    tolua_usertype(tolua_S,"IniUtil");
    tolua_cclass(tolua_S,"IniUtil","IniUtil","",nullptr);

    tolua_beginmodule(tolua_S,"IniUtil");
        tolua_function(tolua_S,"new",lua_oneself_IniUtil_constructor);
        tolua_function(tolua_S,"getDouble",lua_oneself_IniUtil_getDouble);
        tolua_function(tolua_S,"setDouble",lua_oneself_IniUtil_setDouble);
        tolua_function(tolua_S,"getString",lua_oneself_IniUtil_getString);
        tolua_function(tolua_S,"deleteSection",lua_oneself_IniUtil_deleteSection);
        tolua_function(tolua_S,"getInteger",lua_oneself_IniUtil_getInteger);
        tolua_function(tolua_S,"loadFromFile",lua_oneself_IniUtil_loadFromFile);
        tolua_function(tolua_S,"isSectionExist",lua_oneself_IniUtil_isSectionExist);
        tolua_function(tolua_S,"getBoolean",lua_oneself_IniUtil_getBoolean);
        tolua_function(tolua_S,"setInteger",lua_oneself_IniUtil_setInteger);
        tolua_function(tolua_S,"flush",lua_oneself_IniUtil_flush);
        tolua_function(tolua_S,"getFilePath",lua_oneself_IniUtil_getFilePath);
        tolua_function(tolua_S,"setString",lua_oneself_IniUtil_setString);
        tolua_function(tolua_S,"setBoolean",lua_oneself_IniUtil_setBoolean);
        tolua_function(tolua_S,"createFromFile", lua_oneself_IniUtil_createFromFile);
    tolua_endmodule(tolua_S);
    std::string typeName = typeid(IniUtil).name();
    g_luaType[typeName] = "IniUtil";
    g_typeCast["IniUtil"] = "IniUtil";
    return 1;
}

int lua_oneself_TableUtil_GetValue(lua_State* tolua_S)
{
    int argc = 0;
    TableUtil* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"TableUtil",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (TableUtil*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_oneself_TableUtil_GetValue'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 2) 
    {
        int arg0;
        int arg1;

        ok &= luaval_to_int32(tolua_S, 2,(int *)&arg0, "TableUtil:GetValue");

        ok &= luaval_to_int32(tolua_S, 3,(int *)&arg1, "TableUtil:GetValue");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_oneself_TableUtil_GetValue'", nullptr);
            return 0;
        }
        std::string ret = cobj->GetValue(arg0, arg1);
        tolua_pushcppstring(tolua_S,ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "TableUtil:GetValue",argc, 2);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_oneself_TableUtil_GetValue'.",&tolua_err);
#endif

    return 0;
}
int lua_oneself_TableUtil_GetFieldsNum(lua_State* tolua_S)
{
    int argc = 0;
    TableUtil* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"TableUtil",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (TableUtil*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_oneself_TableUtil_GetFieldsNum'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_oneself_TableUtil_GetFieldsNum'", nullptr);
            return 0;
        }
        int ret = cobj->GetFieldsNum();
        tolua_pushnumber(tolua_S,(lua_Number)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "TableUtil:GetFieldsNum",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_oneself_TableUtil_GetFieldsNum'.",&tolua_err);
#endif

    return 0;
}
int lua_oneself_TableUtil_GetRecordsNum(lua_State* tolua_S)
{
    int argc = 0;
    TableUtil* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"TableUtil",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (TableUtil*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_oneself_TableUtil_GetRecordsNum'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_oneself_TableUtil_GetRecordsNum'", nullptr);
            return 0;
        }
        int ret = cobj->GetRecordsNum();
        tolua_pushnumber(tolua_S,(lua_Number)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "TableUtil:GetRecordsNum",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_oneself_TableUtil_GetRecordsNum'.",&tolua_err);
#endif

    return 0;
}
int lua_oneself_TableUtil_createFromFile(lua_State* tolua_S)
{
    int argc = 0;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertable(tolua_S,1,"TableUtil",0,&tolua_err)) goto tolua_lerror;
#endif

    argc = lua_gettop(tolua_S) - 1;

    if (argc == 1)
    {
        std::string arg0;
        ok &= luaval_to_std_string(tolua_S, 2,&arg0, "TableUtil:createFromFile");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_oneself_TableUtil_createFromFile'", nullptr);
            return 0;
        }
        TableUtil* ret = TableUtil::createFromFile(arg0);
        object_to_luaval<TableUtil>(tolua_S, "TableUtil",(TableUtil*)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n ", "TableUtil:createFromFile",argc, 1);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_oneself_TableUtil_createFromFile'.",&tolua_err);
#endif
    return 0;
}
int lua_oneself_TableUtil_constructor(lua_State* tolua_S)
{
    int argc = 0;
    TableUtil* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif



    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_oneself_TableUtil_constructor'", nullptr);
            return 0;
        }
        cobj = new TableUtil();
        cobj->autorelease();
        int ID =  (int)cobj->_ID ;
        int* luaID =  &cobj->_luaID ;
        toluafix_pushusertype_ccobject(tolua_S, ID, luaID, (void*)cobj,"TableUtil");
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "TableUtil:TableUtil",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_error(tolua_S,"#ferror in function 'lua_oneself_TableUtil_constructor'.",&tolua_err);
#endif

    return 0;
}

static int lua_oneself_TableUtil_finalize(lua_State* tolua_S)
{
    printf("luabindings: finalizing LUA object (TableUtil)");
    return 0;
}

int lua_register_oneself_TableUtil(lua_State* tolua_S)
{
    tolua_usertype(tolua_S,"TableUtil");
    tolua_cclass(tolua_S,"TableUtil","TableUtil","cc.Ref",nullptr);

    tolua_beginmodule(tolua_S,"TableUtil");
        tolua_function(tolua_S,"new",lua_oneself_TableUtil_constructor);
        tolua_function(tolua_S,"GetValue",lua_oneself_TableUtil_GetValue);
        tolua_function(tolua_S,"GetFieldsNum",lua_oneself_TableUtil_GetFieldsNum);
        tolua_function(tolua_S,"GetRecordsNum",lua_oneself_TableUtil_GetRecordsNum);
        tolua_function(tolua_S,"createFromFile", lua_oneself_TableUtil_createFromFile);
    tolua_endmodule(tolua_S);
    std::string typeName = typeid(TableUtil).name();
    g_luaType[typeName] = "TableUtil";
    g_typeCast["TableUtil"] = "TableUtil";
    return 1;
}

int lua_oneself_ScrollViewEx_onTouchMoved(lua_State* tolua_S)
{
    int argc = 0;
    ScrollViewEx* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"ScrollViewEx",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (ScrollViewEx*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_oneself_ScrollViewEx_onTouchMoved'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 2) 
    {
        cocos2d::Touch* arg0;
        cocos2d::Event* arg1;

        ok &= luaval_to_object<cocos2d::Touch>(tolua_S, 2, "cc.Touch",&arg0);

        ok &= luaval_to_object<cocos2d::Event>(tolua_S, 3, "cc.Event",&arg1);
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_oneself_ScrollViewEx_onTouchMoved'", nullptr);
            return 0;
        }
        cobj->onTouchMoved(arg0, arg1);
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "ScrollViewEx:onTouchMoved",argc, 2);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_oneself_ScrollViewEx_onTouchMoved'.",&tolua_err);
#endif

    return 0;
}
int lua_oneself_ScrollViewEx_onTouchEnded(lua_State* tolua_S)
{
    int argc = 0;
    ScrollViewEx* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"ScrollViewEx",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (ScrollViewEx*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_oneself_ScrollViewEx_onTouchEnded'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 2) 
    {
        cocos2d::Touch* arg0;
        cocos2d::Event* arg1;

        ok &= luaval_to_object<cocos2d::Touch>(tolua_S, 2, "cc.Touch",&arg0);

        ok &= luaval_to_object<cocos2d::Event>(tolua_S, 3, "cc.Event",&arg1);
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_oneself_ScrollViewEx_onTouchEnded'", nullptr);
            return 0;
        }
        cobj->onTouchEnded(arg0, arg1);
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "ScrollViewEx:onTouchEnded",argc, 2);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_oneself_ScrollViewEx_onTouchEnded'.",&tolua_err);
#endif

    return 0;
}
int lua_oneself_ScrollViewEx_TouchEndCallFun(lua_State* tolua_S)
{
    int argc = 0;
    ScrollViewEx* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"ScrollViewEx",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (ScrollViewEx*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_oneself_ScrollViewEx_TouchEndCallFun'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_oneself_ScrollViewEx_TouchEndCallFun'", nullptr);
            return 0;
        }
        cobj->TouchEndCallFun();
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "ScrollViewEx:TouchEndCallFun",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_oneself_ScrollViewEx_TouchEndCallFun'.",&tolua_err);
#endif

    return 0;
}
int lua_oneself_ScrollViewEx_onTouchCancelled(lua_State* tolua_S)
{
    int argc = 0;
    ScrollViewEx* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"ScrollViewEx",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (ScrollViewEx*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_oneself_ScrollViewEx_onTouchCancelled'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 2) 
    {
        cocos2d::Touch* arg0;
        cocos2d::Event* arg1;

        ok &= luaval_to_object<cocos2d::Touch>(tolua_S, 2, "cc.Touch",&arg0);

        ok &= luaval_to_object<cocos2d::Event>(tolua_S, 3, "cc.Event",&arg1);
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_oneself_ScrollViewEx_onTouchCancelled'", nullptr);
            return 0;
        }
        cobj->onTouchCancelled(arg0, arg1);
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "ScrollViewEx:onTouchCancelled",argc, 2);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_oneself_ScrollViewEx_onTouchCancelled'.",&tolua_err);
#endif

    return 0;
}
int lua_oneself_ScrollViewEx_onTouchBegan(lua_State* tolua_S)
{
    int argc = 0;
    ScrollViewEx* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"ScrollViewEx",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (ScrollViewEx*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_oneself_ScrollViewEx_onTouchBegan'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 2) 
    {
        cocos2d::Touch* arg0;
        cocos2d::Event* arg1;

        ok &= luaval_to_object<cocos2d::Touch>(tolua_S, 2, "cc.Touch",&arg0);

        ok &= luaval_to_object<cocos2d::Event>(tolua_S, 3, "cc.Event",&arg1);
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_oneself_ScrollViewEx_onTouchBegan'", nullptr);
            return 0;
        }
        bool ret = cobj->onTouchBegan(arg0, arg1);
        tolua_pushboolean(tolua_S,(bool)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "ScrollViewEx:onTouchBegan",argc, 2);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_oneself_ScrollViewEx_onTouchBegan'.",&tolua_err);
#endif

    return 0;
}
int lua_oneself_ScrollViewEx_isTouchEnabled(lua_State* tolua_S)
{
    int argc = 0;
    ScrollViewEx* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"ScrollViewEx",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (ScrollViewEx*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_oneself_ScrollViewEx_isTouchEnabled'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_oneself_ScrollViewEx_isTouchEnabled'", nullptr);
            return 0;
        }
        bool ret = cobj->isTouchEnabled();
        tolua_pushboolean(tolua_S,(bool)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "ScrollViewEx:isTouchEnabled",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_oneself_ScrollViewEx_isTouchEnabled'.",&tolua_err);
#endif

    return 0;
}
int lua_oneself_ScrollViewEx_setTouchEnabled(lua_State* tolua_S)
{
    int argc = 0;
    ScrollViewEx* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"ScrollViewEx",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (ScrollViewEx*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_oneself_ScrollViewEx_setTouchEnabled'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        bool arg0;

        ok &= luaval_to_boolean(tolua_S, 2,&arg0, "ScrollViewEx:setTouchEnabled");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_oneself_ScrollViewEx_setTouchEnabled'", nullptr);
            return 0;
        }
        cobj->setTouchEnabled(arg0);
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "ScrollViewEx:setTouchEnabled",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_oneself_ScrollViewEx_setTouchEnabled'.",&tolua_err);
#endif

    return 0;
}
int lua_oneself_ScrollViewEx_create(lua_State* tolua_S)
{
    int argc = 0;
    bool ok  = true;
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertable(tolua_S,1,"ScrollViewEx",0,&tolua_err)) goto tolua_lerror;
#endif

    argc = lua_gettop(tolua_S)-1;

    do 
    {
        if (argc == 0)
        {
            ScrollViewEx* ret = ScrollViewEx::create();
            object_to_luaval<ScrollViewEx>(tolua_S, "ScrollViewEx",(ScrollViewEx*)ret);
            return 1;
        }
    } while (0);
    ok  = true;
    do 
    {
        if (argc == 1)
        {
            cocos2d::Size arg0;
            ok &= luaval_to_size(tolua_S, 2, &arg0, "ScrollViewEx:create");
            if (!ok) { break; }
            ScrollViewEx* ret = ScrollViewEx::create(arg0);
            object_to_luaval<ScrollViewEx>(tolua_S, "ScrollViewEx",(ScrollViewEx*)ret);
            return 1;
        }
    } while (0);
    ok  = true;
    do 
    {
        if (argc == 2)
        {
            cocos2d::Size arg0;
            ok &= luaval_to_size(tolua_S, 2, &arg0, "ScrollViewEx:create");
            if (!ok) { break; }
            cocos2d::Node* arg1;
            ok &= luaval_to_object<cocos2d::Node>(tolua_S, 3, "cc.Node",&arg1);
            if (!ok) { break; }
            ScrollViewEx* ret = ScrollViewEx::create(arg0, arg1);
            object_to_luaval<ScrollViewEx>(tolua_S, "ScrollViewEx",(ScrollViewEx*)ret);
            return 1;
        }
    } while (0);
    ok  = true;
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d", "ScrollViewEx:create",argc, 1);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_oneself_ScrollViewEx_create'.",&tolua_err);
#endif
    return 0;
}
int lua_oneself_ScrollViewEx_constructor(lua_State* tolua_S)
{
    int argc = 0;
    ScrollViewEx* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif



    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_oneself_ScrollViewEx_constructor'", nullptr);
            return 0;
        }
        cobj = new ScrollViewEx();
        cobj->autorelease();
        int ID =  (int)cobj->_ID ;
        int* luaID =  &cobj->_luaID ;
        toluafix_pushusertype_ccobject(tolua_S, ID, luaID, (void*)cobj,"ScrollViewEx");
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "ScrollViewEx:ScrollViewEx",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_error(tolua_S,"#ferror in function 'lua_oneself_ScrollViewEx_constructor'.",&tolua_err);
#endif

    return 0;
}

static int lua_oneself_ScrollViewEx_finalize(lua_State* tolua_S)
{
    printf("luabindings: finalizing LUA object (ScrollViewEx)");
    return 0;
}

int lua_register_oneself_ScrollViewEx(lua_State* tolua_S)
{
    tolua_usertype(tolua_S,"ScrollViewEx");
    tolua_cclass(tolua_S,"ScrollViewEx","ScrollViewEx","cc.ScrollView",nullptr);

    tolua_beginmodule(tolua_S,"ScrollViewEx");
        tolua_function(tolua_S,"new",lua_oneself_ScrollViewEx_constructor);
        tolua_function(tolua_S,"onTouchMoved",lua_oneself_ScrollViewEx_onTouchMoved);
        tolua_function(tolua_S,"onTouchEnded",lua_oneself_ScrollViewEx_onTouchEnded);
        tolua_function(tolua_S,"TouchEndCallFun",lua_oneself_ScrollViewEx_TouchEndCallFun);
        tolua_function(tolua_S,"onTouchCancelled",lua_oneself_ScrollViewEx_onTouchCancelled);
        tolua_function(tolua_S,"onTouchBegan",lua_oneself_ScrollViewEx_onTouchBegan);
        tolua_function(tolua_S,"isTouchEnabled",lua_oneself_ScrollViewEx_isTouchEnabled);
        tolua_function(tolua_S,"setTouchEnabled",lua_oneself_ScrollViewEx_setTouchEnabled);
        tolua_function(tolua_S,"create", lua_oneself_ScrollViewEx_create);
    tolua_endmodule(tolua_S);
    std::string typeName = typeid(ScrollViewEx).name();
    g_luaType[typeName] = "ScrollViewEx";
    g_typeCast["ScrollViewEx"] = "ScrollViewEx";
    return 1;
}

int lua_oneself_PlatformUtils_GetDeviceID(lua_State* tolua_S)
{
    int argc = 0;
    PlatformUtils* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"PlatformUtils",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (PlatformUtils*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_oneself_PlatformUtils_GetDeviceID'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_oneself_PlatformUtils_GetDeviceID'", nullptr);
            return 0;
        }
        std::string ret = cobj->GetDeviceID();
        tolua_pushcppstring(tolua_S,ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "PlatformUtils:GetDeviceID",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_oneself_PlatformUtils_GetDeviceID'.",&tolua_err);
#endif

    return 0;
}
int lua_oneself_PlatformUtils_GetInstance(lua_State* tolua_S)
{
    int argc = 0;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertable(tolua_S,1,"PlatformUtils",0,&tolua_err)) goto tolua_lerror;
#endif

    argc = lua_gettop(tolua_S) - 1;

    if (argc == 0)
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_oneself_PlatformUtils_GetInstance'", nullptr);
            return 0;
        }
        PlatformUtils* ret = PlatformUtils::GetInstance();
        object_to_luaval<PlatformUtils>(tolua_S, "PlatformUtils",(PlatformUtils*)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n ", "PlatformUtils:GetInstance",argc, 0);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_oneself_PlatformUtils_GetInstance'.",&tolua_err);
#endif
    return 0;
}
static int lua_oneself_PlatformUtils_finalize(lua_State* tolua_S)
{
    printf("luabindings: finalizing LUA object (PlatformUtils)");
    return 0;
}

int lua_register_oneself_PlatformUtils(lua_State* tolua_S)
{
    tolua_usertype(tolua_S,"PlatformUtils");
    tolua_cclass(tolua_S,"PlatformUtils","PlatformUtils","",nullptr);

    tolua_beginmodule(tolua_S,"PlatformUtils");
        tolua_function(tolua_S,"GetDeviceID",lua_oneself_PlatformUtils_GetDeviceID);
        tolua_function(tolua_S,"GetInstance", lua_oneself_PlatformUtils_GetInstance);
    tolua_endmodule(tolua_S);
    std::string typeName = typeid(PlatformUtils).name();
    g_luaType[typeName] = "PlatformUtils";
    g_typeCast["PlatformUtils"] = "PlatformUtils";
    return 1;
}

int lua_oneself_EventAnimate_create(lua_State* tolua_S)
{
    int argc = 0;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertable(tolua_S,1,"EventAnimate",0,&tolua_err)) goto tolua_lerror;
#endif

    argc = lua_gettop(tolua_S) - 1;

    if (argc == 1)
    {
        cocos2d::Animation* arg0;
        ok &= luaval_to_object<cocos2d::Animation>(tolua_S, 2, "cc.Animation",&arg0);
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_oneself_EventAnimate_create'", nullptr);
            return 0;
        }
        EventAnimate* ret = EventAnimate::create(arg0);
        object_to_luaval<EventAnimate>(tolua_S, "EventAnimate",(EventAnimate*)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n ", "EventAnimate:create",argc, 1);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_oneself_EventAnimate_create'.",&tolua_err);
#endif
    return 0;
}
static int lua_oneself_EventAnimate_finalize(lua_State* tolua_S)
{
    printf("luabindings: finalizing LUA object (EventAnimate)");
    return 0;
}

int lua_register_oneself_EventAnimate(lua_State* tolua_S)
{
    tolua_usertype(tolua_S,"EventAnimate");
    tolua_cclass(tolua_S,"EventAnimate","EventAnimate","cc.Animate",nullptr);

    tolua_beginmodule(tolua_S,"EventAnimate");
        tolua_function(tolua_S,"create", lua_oneself_EventAnimate_create);
    tolua_endmodule(tolua_S);
    std::string typeName = typeid(EventAnimate).name();
    g_luaType[typeName] = "EventAnimate";
    g_typeCast["EventAnimate"] = "EventAnimate";
    return 1;
}
TOLUA_API int register_all_oneself(lua_State* tolua_S)
{
	tolua_open(tolua_S);
	
	tolua_module(tolua_S,"my",0);
	tolua_beginmodule(tolua_S,"my");

	lua_register_oneself_TcpManager(tolua_S);
	lua_register_oneself_TableUtil(tolua_S);
	lua_register_oneself_IniUtil(tolua_S);
	lua_register_oneself_GlobalSchedule(tolua_S);
	lua_register_oneself_PlatformUtils(tolua_S);
	lua_register_oneself_ByteBuffer(tolua_S);
	lua_register_oneself_EventAnimate(tolua_S);
	lua_register_oneself_ScrollViewEx(tolua_S);

	tolua_endmodule(tolua_S);
	return 1;
}

