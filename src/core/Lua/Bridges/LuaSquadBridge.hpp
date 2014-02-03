#ifndef CORE_LUA_BRIDGES_SQUAD_BRIDGE_HPP
#define CORE_LUA_BRIDGES_SQUAD_BRIDGE_HPP

#include <lua.h>

namespace Core
{
    class LuaSquadBridge
    {
    public:
        LuaSquadBridge(lua_State* L);
    };
}

#endif
