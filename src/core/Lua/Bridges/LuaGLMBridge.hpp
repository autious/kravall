#ifndef SRC_CORE_LUA_BRIDGES_LUAGLMBRIDGE_H
#define SRC_CORE_LUA_BRIDGES_LUAGLMBRIDGE_H

#include <lua.h>

#include <Lua/LuaMetatableTypes.hpp>


namespace Core
{
    class LuaGLMBridge
    {
        public:
            LuaGLMBridge( lua_State * L );
    };
}

#endif
