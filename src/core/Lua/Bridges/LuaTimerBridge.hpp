#ifndef SRC_CORE_LUA_BRIDGES_LUATIMERBRIDGE_H
#define SRC_CORE_LUA_BRIDGES_LUATIMERBRIDGE_H

#include <lua.h>

namespace Core
{
    class LuaTimerBridge
    {
    public:
        LuaTimerBridge( lua_State * L );
    };

}

#endif
