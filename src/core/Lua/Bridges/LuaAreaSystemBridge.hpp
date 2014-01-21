#ifndef SRC_CORE_LUA_BRIDGES_LUAAREASYSTEMBRIDGE_H
#define SRC_CORE_LUA_BRIDGES_LUAAREASYSTEMBRIDGE_H

#include <lua.h>

namespace Core
{
    class LuaAreaSystemBridge
    {
        public:
            LuaAreaSystemBridge( lua_State * L );
    };
}

#endif
