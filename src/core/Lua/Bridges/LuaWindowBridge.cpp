#include "LuaWindowBridge.hpp"

#include <WindowHandling/GLFWInclude.hpp>
#include <Lua/LuaUtility.hpp>

extern "C" 
{
    static int LuaGetWindowSize( lua_State * L )
    {
        int width, height; 
        glfwGetWindowSize( mainWindow, &width, &height );

        lua_pushinteger( L, width );
        lua_pushinteger( L, height );

        return 2;
    }
}

namespace Core
{
    LuaWindowBridge::LuaWindowBridge( lua_State *L )
    {
        lua_getglobal( L, "core" );
            lua_newtable( L );
                luau_setfunction( L, "getSize", LuaGetWindowSize );
            lua_setfield( L, -2, "window" );
        lua_pop( L, 1 );
    }
}
