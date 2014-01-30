#ifndef CORE_LUA_BRIDGES_LUA_PICKING_BRIDGE_HPP
#define CORE_LUA_BRIDGES_LUA_PICKING_BRIDGE_HPP

#include <lua.h>

namespace Core
{
    class LuaPickingBridge
    {
    public:
        LuaPickingBridge( lua_State* L );
    };
}


#endif
