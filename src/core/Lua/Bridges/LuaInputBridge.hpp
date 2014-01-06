#ifndef CORE_LUA_BRIDGES_LUA_INPUT_BRIDGE_HPP
#define CORE_LUA_BRIDGES_LUA_INPUT_BRIDGE_HPP

#include <lua.h>

namespace Core
{
    class LuaInputBridge
    {
    public:
        static void OpenLibs(lua_State* state);

    };
}


#endif
