#ifndef SRC_CORE_LUA_BRIDGES_LUAANIMATIONBRIDGE_H
#define SRC_CORE_LUA_BRIDGES_LUAANIMATIONBRIDGE_H

#include <lua.h>

namespace Core
{
    class LuaAnimationBridge
    {
        public:
            LuaAnimationBridge( lua_State * L );  
    };
}

#endif
