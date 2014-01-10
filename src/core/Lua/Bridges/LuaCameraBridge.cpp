#include "LuaCameraBridge.hpp"

#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

#include <Camera/Camera.hpp>
#include <Lua/LuaMetatableTypes.hpp>
#include <Lua/LuaUtility.hpp>

extern "C" 
{
    static int LuaSetProjection( lua_State * L )
    {
        glm::mat4 * mat4 = luau_checkglmmat4( L, 1 );
        Core::gameCamera.SetProjectionMatrix( *mat4 ); 
        return 0;
    }

    static int LuaSetView( lua_State * L )
    {
        glm::mat4 * mat4 = luau_checkglmmat4( L, 1 );
        Core::gameCamera.SetViewMatrix( *mat4 ); 
        return 0;
    }
    static int LuaGetProjection( lua_State *L )
    {
        glm::mat4 * mat4 = Core::LuaUNewGLMMat4( L );
        *mat4 = Core::gameCamera.GetProjectionMatrix(); 
        return 1;
    }

    static int LuaGetView( lua_State * L )
    {
        glm::mat4 * mat4 = Core::LuaUNewGLMMat4( L );
        *mat4 = Core::gameCamera.GetViewMatrix(  ); 
        return 1;
    }

    static int LuaGetForward( lua_State * L )
    {
        glm::vec3 * vec3 = Core::LuaUNewGLMVec3(L);
        *vec3 = Core::gameCamera.GetForward();
        return 1;
    }
    
    static int LuaGetUp( lua_State * L )
    {
        glm::vec3 * vec3 = Core::LuaUNewGLMVec3(L);
        *vec3 = Core::gameCamera.GetUp();
        return 1;
    }

    static int LuaGetRight( lua_State * L )
    {
        glm::vec3 * vec3 = Core::LuaUNewGLMVec3(L);
        *vec3 = Core::gameCamera.GetRight();
        return 1;
    }

    static int LuaGetPosition( lua_State *L )
    {
        glm::vec3 * vec3 = Core::LuaUNewGLMVec3(L);
        *vec3 = Core::gameCamera.GetPosition();
        return 1;
    }
}

namespace Core
{
    LuaCameraBridge::LuaCameraBridge( lua_State * L )
    {
        lua_getglobal( L, "core" );
            lua_newtable(L);
                luau_setfunction( L, "getView", LuaGetView );
                luau_setfunction( L, "getProjection", LuaGetProjection );
                luau_setfunction( L, "setView", LuaSetView );
                luau_setfunction( L, "setProjection", LuaSetProjection );
                luau_setfunction( L, "getForward", LuaGetForward );
                luau_setfunction( L, "getUp", LuaGetUp);
                luau_setfunction( L, "getRight", LuaGetRight );
                luau_setfunction( L, "getPosition", LuaGetPosition );
            lua_setfield( L, -2, "camera" );            
        lua_pop( L, 1 );
    }
}
