#ifndef SRC_CORE_LUA_BRIDGES_LUALINEARHEAPBRIDGE_HPP
#define SRC_CORE_LUA_BRIDGES_LUALINEARHEAPBRIDGE_HPP

#include <lua.h>

namespace Core
{
    class LuaLevelHeapBridge
    {
    public:
        LuaLevelHeapBridge( lua_State * L );
    };
}

#endif
