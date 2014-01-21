#ifndef SRC_CORE_LUA_BRIDGES_LUADRAWBRIDGE_H
#define SRC_CORE_LUA_BRIDGES_LUADRAWBRIDGE_H

#include <lua.h>

namespace Core
{
    class LuaDrawBridge
    {
        public:
            LuaDrawBridge( lua_State * lua );
    };
}

#endif
