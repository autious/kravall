#ifndef SRC_CORE_LUA_BRIDGES_LUALUAUNITTYPEBRIDGE_HPP
#define SRC_CORE_LUA_BRIDGES_LUALUAUNITTYPEBRIDGE_HPP

#include <lua.h>

namespace Core
{
    class LuaUnitTypeComponentBridge
    {
    public:
         LuaUnitTypeComponentBridge( lua_State * L );
    };
}

#endif
