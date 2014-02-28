#ifndef CORE_LUA_BRIDGES_UNIT_ORDER_BRIDGE_HPP
#define CORE_LUA_BRIDGES_UNIT_ORDER_BRIDGE_HPP

#include <lua.h>

namespace Core
{
    class LuaUnitOrderBridge
    {
    public:
        LuaUnitOrderBridge(lua_State* L);
    };
}

#endif
