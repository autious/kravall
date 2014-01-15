#include "LuaConsoleBridge.hpp"

#include <cassert>

#include <lauxlib.h>
#include <lualib.h>

#include <Console/Console.hpp>
#include <utility/Colors.hpp>
#include <Lua/LuaUtility.hpp>

extern "C"
{
    static int LuaIsVisible( lua_State * L )
    {
        lua_pushboolean( L, Core::Console().IsVisible() );
        return 1;
    }

    static int LuaSetVisible( lua_State * L )
    {
        if( !lua_isboolean( L, 1 )  )
            return luaL_error( L, "Value must be boolean" );
        Core::Console().SetVisible( lua_toboolean( L, 1 ) );
        return 0;
    }

    static int LuaPrint( lua_State * L )
    {
        Core::Console().PrintLine( std::string( luaL_checkstring( L, 1 ) ), Colors::White );
        return 0;
    }

    static int LuaGetInputLine( lua_State * L)
    {
        lua_pushstring( L, Core::Console().GetInputLine().c_str() );

        return 1;
    }

    static int LuaSetInputLine( lua_State * L )
    {
        Core::Console().SetInputLine( std::string( luaL_checkstring( L, 1 ) ) ); 
        return 0;
    }
}

namespace Core
{
    LuaConsoleBridge::LuaConsoleBridge( lua_State * L )
    {
        int sanity = lua_gettop( L );  

        lua_getglobal( L, "core" );  
            lua_newtable( L );
                luau_setfunction( L, "isVisible", LuaIsVisible );
                luau_setfunction( L, "setVisible", LuaSetVisible );
                luau_setfunction( L, "print", LuaPrint );
                luau_setfunction( L, "getInputLine", LuaGetInputLine );
                luau_setfunction( L, "setInputLine", LuaSetInputLine );
            lua_setfield( L, -2, "console" );
        lua_pop( L, 1 );

        assert( sanity == lua_gettop( L ) );
    }
}
