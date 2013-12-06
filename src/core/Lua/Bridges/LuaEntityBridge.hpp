#ifndef SRC_CORE_LUA_LUAENTITYBRIDGE_H
#define SRC_CORE_LUA_LUAENTITYBRIDGE_H

#include <lua.h>

namespace Core
{
    class LuaEntityBridge
    {
        static void OpenLibs( lua_State *state );
    };
}


#endif
