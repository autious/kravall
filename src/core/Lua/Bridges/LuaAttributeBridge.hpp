#ifndef SRC_CORE_LUA_BRIDGES_LUAATTRIBUTEBRIDGE_HPP
#define SRC_CORE_LUA_BRIDGES_LUAATTRIBUTEBRIDGE_HPP

#include <lua.h>

namespace Core
{
    class LuaAttributeComponentBridge
    {
    public:
        LuaAttributeComponentBridge( lua_State * L );
    };
}

#endif
