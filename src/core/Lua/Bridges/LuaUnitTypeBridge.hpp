#ifndef SRC_CORE_LUA_BRIDGES_LUALUAUNITTYPEBRIDGE_HPP
#define SRC_CORE_LUA_BRIDGES_LUALUAUNITTYPEBRIDGE_HPP

#include <lua.h>
#define UNIT_TYPE_OBJECT_TYPE_META "unit_type_object_type_meta"

namespace Core
{
    class LuaUnitTypeComponentBridge
    {
    public:
         LuaUnitTypeComponentBridge( lua_State * L );
    };
}

#endif
