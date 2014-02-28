#ifndef CORE_LUA_BRIDGES_WEAPON_BRIDGE_HPP
#define CORE_LUA_BRIDGES_WEAPON_BRIDGE_HPP

#include <lua.h>

namespace Core
{
    class LuaWeaponBridge
    {
    public:
        LuaWeaponBridge(lua_State* L);
    };
}

#endif
