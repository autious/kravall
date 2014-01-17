#ifndef SRC_CORE_LUA_BRIDGES_LUANAMESYSTEMBRIDGE_H
#define SRC_CORE_LUA_BRIDGES_LUANAMESYSTEMBRIDGE_H

#include <lua.h>

namespace Core
{
    class LuaNameSystemBridge
    {
        public:
            LuaNameSystemBridge( lua_State * L );  
    };
}

#endif
