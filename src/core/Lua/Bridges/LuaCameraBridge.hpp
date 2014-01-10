#ifndef SRC_CORE_LUA_BRIDGES_LUACAMERABRIDGE_H
#define SRC_CORE_LUA_BRIDGES_LUACAMERABRIDGE_H

#include <lua.h>

namespace Core
{
    class LuaCameraBridge
    {
        public:
            LuaCameraBridge( lua_State * L );
    };
}

#endif
