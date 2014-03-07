#ifndef SRC_CORE_LUA_BRIDGES_LUACOREBRIDGE_H
#define SRC_CORE_LUA_BRIDGES_LUACOREBRIDGE_H

#include <lua.h>


namespace Core
{
    class LuaCoreBridge
    {
        public:
            LuaCoreBridge( lua_State * L );
    };
}

#endif
