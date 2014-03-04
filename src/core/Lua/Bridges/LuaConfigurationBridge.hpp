#ifndef SRC_CORE_LUA_BRIDGES_LUACONFIGURATIONBRIDGE_H
#define SRC_CORE_LUA_BRIDGES_LUACONFIGURATIONBRIDGE_H

#include <lua.h>

namespace Core
{
    class LuaConfigurationBridge
    {
        public:
            LuaConfigurationBridge( lua_State * L );
    };
}

#endif
