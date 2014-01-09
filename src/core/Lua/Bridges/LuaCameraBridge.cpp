#include "LuaCameraBridge.hpp"

#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

#include <Camera/Camera.hpp>

extern "C" 
{
    static int LuaLookAt()
    {

    } 

    static int LuaSetPosition()
    {

    }

    static int LuaGetPosition()
    {

    }

    static int LuaSetFOV()
    {

    }

    static int LuaSetNearFar( lua_State * L )
    {

    }
}

namespace Core
{
    LuaCameraBridge::LuaCameraBridge( lua_State * L )
    {
                
    }
}
