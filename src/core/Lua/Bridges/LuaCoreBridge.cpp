#include "LuaCoreBridge.hpp"

#include "main.hpp"
#include <lua.h>
#include <Lua/LuaUtility.hpp>

extern "C"
{
    static int LuaFlagExit( lua_State * L )
    {
        KillProgram();
        return 0;
    }
}

namespace Core
{
    LuaCoreBridge::LuaCoreBridge( lua_State * L )
    {
        lua_getglobal( L, "core" );
            luau_setfunction( L, "flagExit", LuaFlagExit );  
        lua_pop( L, 1 ); 
    }
}
