#ifndef SRC_CORE_LUA_BRIDGES_LUACONSOLEBRIDGE_H
#define SRC_CORE_LUA_BRIDGES_LUACONSOLEBRIDGE_H

#include <lua.h>

namespace Core
{
    class LuaConsoleBridge
    {
        public:
            LuaConsoleBridge( lua_State * L );
    };

}

#endif
