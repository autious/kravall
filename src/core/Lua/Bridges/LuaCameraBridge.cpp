#include "LuaCameraBridge.hpp"

#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

#include <Camera/Camera.hpp>
#include <Lua/LuaMetatableTypes.hpp>

extern "C" 
{
    static int LuaSetProjection( lua_State * state )
    {
        glm::mat4 * mat4 = luau_checkglmmat4( state, 1 );

         
    }

    static int LuaSetView( lua_State * state )
    {

    }
    static int LuaGetProjection( lua_State *state )
    {

    }

    static int LuaGetView( lua_State * state )
    {

    }
}

namespace Core
{
    LuaCameraBridge::LuaCameraBridge( lua_State * L )
    {
        lua_getglobal( L, "core" );
            lua_newtable(L);
                  
            lua_setfield( L, -2, "camera" );            
        lua_pop( L, 1 );
    }
}
