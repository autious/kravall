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
        Core::Camera ** cam = luau_checkcamera( L, 1 );
        glm::mat4 * mat4 = luau_checkglmmat4( L, 2);
		float nearZ = luaL_checknumber(L, 3);
		float farZ = luaL_checknumber(L, 4);
        (*cam)->SetProjectionMatrix( *mat4, nearZ, farZ ); 
        return 0;
    }

    static int LuaSetView( lua_State * L )
    {
        Core::Camera ** cam = luau_checkcamera( L, 1 );
        glm::mat4 * mat4 = luau_checkglmmat4( L, 2 );
        (*cam)->SetViewMatrix( *mat4 ); 
        return 0;
    }
    static int LuaGetProjection( lua_State *L )
    {
        Core::Camera ** cam = luau_checkcamera( L, 1 );
        glm::mat4 * mat4 = Core::LuaUNewGLMMat4( L );
        *mat4 = (*cam)->GetProjectionMatrix(); 
        return 1;
    }

    static int LuaGetView( lua_State * L )
    {
        Core::Camera ** cam = luau_checkcamera( L, 1 );
        glm::mat4 * mat4 = Core::LuaUNewGLMMat4( L );
        *mat4 = (*cam)->GetViewMatrix(  ); 
        return 1;
    }

    static int LuaGetForward( lua_State * L )
    {
        Core::Camera ** cam = luau_checkcamera( L, 1 );
        glm::vec3 * vec3 = Core::LuaUNewGLMVec3(L);
        *vec3 = (*cam)->GetForward();
        return 1;
    }
    
    static int LuaGetUp( lua_State * L )
    {
        Core::Camera ** cam = luau_checkcamera( L, 1 );
        glm::vec3 * vec3 = Core::LuaUNewGLMVec3(L);
        *vec3 = (*cam)->GetUp();
        return 1;
    }

    static int LuaGetRight( lua_State * L )
    {
        Core::Camera ** cam = luau_checkcamera( L, 1 );
        glm::vec3 * vec3 = Core::LuaUNewGLMVec3(L);
        *vec3 = (*cam)->GetRight();
        return 1;
    }

    static int LuaGetPosition( lua_State *L )
    {
        Core::Camera ** cam = luau_checkcamera( L, 1 );
        glm::vec3 * vec3 = Core::LuaUNewGLMVec3(L);
        *vec3 = (*cam)->GetPosition();
        return 1;
    }
}

namespace Core
{
    LuaCameraBridge::LuaCameraBridge( lua_State * L )
    {
        lua_getglobal( L, "core" );
            lua_newtable(L);
            int camera_table = lua_gettop( L );
                luau_setfunction( L, "getView", LuaGetView );
                luau_setfunction( L, "getProjection", LuaGetProjection );
                luau_setfunction( L, "setView", LuaSetView );
                luau_setfunction( L, "setProjection", LuaSetProjection );
                luau_setfunction( L, "getForward", LuaGetForward );
                luau_setfunction( L, "getUp", LuaGetUp);
                luau_setfunction( L, "getRight", LuaGetRight );
                luau_setfunction( L, "getPosition", LuaGetPosition );

                luaL_newmetatable( L, CAMERA_META_TYPE );
                    lua_pushvalue( L, camera_table );
                    lua_setfield( L, -2, "__index" );
                lua_pop( L, 1 );

                Core::Camera ** gamecam = LuaUNewCamera( L ); 
                *gamecam = &Core::gameCamera;
                lua_setfield(L, -2, "gameCamera" );

                Core::Camera ** overlaycam = LuaUNewCamera( L ); 
                *overlaycam = &Core::overlayCamera;
                lua_setfield(L, -2, "overlayCamera" );

            lua_setfield( L, -2, "camera" );            
        lua_pop( L, 1 );
    }
}
