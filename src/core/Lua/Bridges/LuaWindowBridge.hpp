#ifndef SRC_CORE_LUA_BRIDGES_LUAWINDOWBRIDGE_H
#define SRC_CORE_LUA_BRIDGES_LUAWINDOWBRIDGE_H

#include <lua.h>

namespace Core
{
    class LuaWindowBridge
    {
    public:
        LuaWindowBridge( lua_State * L );
    };
}

#endif
