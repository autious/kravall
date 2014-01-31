#ifndef SRC_CORE_LUA_BRIDGES_LUAGLMVEC3BRIDGE_H
#define SRC_CORE_LUA_BRIDGES_LUAGLMVEC3BRIDGE_H

#include "LuaGLMBridge.hpp"

#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>


#include <Lua/LuaUtility.hpp>
#include <Lua/LuaMetatableTypes.hpp>

extern "C"
{

	static int LuaVec2Newindex( lua_State * L )
    {
        return luaL_error( L, "Read only" );
    }

	static int LuaVec2New( lua_State * L )
    {
        int params = lua_gettop( L );
        glm::vec2 * vec2 = Core::LuaUNewGLMVec2( L );

        if( params >= 2 )
        {
            (*vec2)[0] = luau_checkfloat( L, 1 );
            (*vec2)[1] = luau_checkfloat( L, 2 );

            return 1;
        }
        else if( params == 1 )
        {
            glm::vec2 * vec2_first = luau_checkglmvec2( L, 1 );
            
            *vec2 = *vec2_first;

            return 1;
        }
        else if( params == 0 )
        {
            return 1;
        }
        else
        {
            return luaL_error( L, "Invalid number of parameters" );
        }
    }

	static int LuaVec2Dot( lua_State * L )
    {
        glm::vec2 * vec2_first = luau_checkglmvec2( L, 1 );
        glm::vec2 * vec2_second = luau_checkglmvec2( L, 2 );

        lua_pushnumber( L, glm::dot( *vec2_first, *vec2_second ) );

        return 1;
    }

    static int LuaVec2Subtract( lua_State * L )
    {
        glm::vec2 * vec2_first = luau_checkglmvec2( L, 1 );
        glm::vec2 * vec2_second = luau_checkglmvec2( L, 2 );
        glm::vec2 * vec2 = Core::LuaUNewGLMVec2( L );

        *vec2 = *vec2_first - *vec2_second; 

        return 1;
    }

    static int LuaVec2Add( lua_State *L )
    {
        glm::vec2 * vec2_first = luau_checkglmvec2( L, 1 );
        glm::vec2 * vec2_second = luau_checkglmvec2( L, 2 );
        glm::vec2 * vec2 = Core::LuaUNewGLMVec2( L );

        *vec2 = *vec2_first + *vec2_second; 

        return 1;
    }

	static int LuaVec2Eq( lua_State * L )
    {
        glm::vec2 * vec2_first = luau_checkglmvec2( L, 1 );
        glm::vec2 * vec2_second = luau_checkglmvec2( L, 2 );

        lua_pushboolean( L, *vec2_first == *vec2_second );

        return 1;
    }

    static int LuaVec2Normalized( lua_State *L )
    {
        glm::vec2 * vec2_first = luau_checkglmvec2( L, 1 );
        glm::vec2 * vec2 = Core::LuaUNewGLMVec2( L );

        *vec2 = glm::normalize( *vec2_first );

        return 1;
    }

    static int LuaVec2Reflect( lua_State *L )
    {
        glm::vec3 * vec2_first = luau_checkglmvec3( L, 1 );
        glm::vec3 * vec2_second = luau_checkglmvec3( L, 2 );
        glm::vec3 * vec2 = Core::LuaUNewGLMVec3( L );

        *vec2 = glm::reflect( *vec2_first, *vec2_second );

        return 1;
    } 

    static int LuaVec2Get( lua_State * L )
    {	
        glm::vec2 * vec2_first = luau_checkglmvec2( L, 1 );

        if( lua_gettop( L ) == 2 )
        {
            int index = luaL_checkinteger( L, 2 );
            if( index > 0 && index <= 2)
            {
                lua_pushnumber( L, (*vec2_first)[index-1] );
                return 1;
            }
            else
            {
                return luaL_error( L, "Invalid index" );
            }
        }
        
        lua_pushnumber( L, (*vec2_first)[0]);
        lua_pushnumber( L, (*vec2_first)[1]);

        return 2;
    }

    static int LuaVec2Multiply( lua_State *L )
    {
        glm::vec2 * vec2_first = luau_checkglmvec2( L, 1 );
        float second = luau_checkfloat( L, 2 );

        glm::vec2 * vec2_ret = Core::LuaUNewGLMVec2( L );

        *vec2_ret = *vec2_first * second;

        return 1;
    }

	static int LuaVec2Length( lua_State * L )
    {
        glm::vec2 * vec2_first = luau_checkglmvec2( L, 1 );
        lua_pushnumber( L, glm::length( *vec2_first ) );
        return 1;
    }


	static int LuaVec3Newindex( lua_State * L )
    {
        return luaL_error( L, "Read only" );
    }

    static int LuaVec3New( lua_State * L )
    {
        int params = lua_gettop( L );
        glm::vec3 * vec3 = Core::LuaUNewGLMVec3( L );

        if( params >= 3 )
        {
            (*vec3)[0] = luau_checkfloat( L, 1 );
            (*vec3)[1] = luau_checkfloat( L, 2 );
            (*vec3)[2] = luau_checkfloat( L, 3 );

            return 1;
        }
        else if( params == 1 )
        {
            glm::vec3 * vec3_first = luau_checkglmvec3( L, 1 );
            
            *vec3 = *vec3_first;

            return 1;
        }
        else if( params == 0 )
        {
            return 1;
        }
        else
        {
            return luaL_error( L, "Invalid number of parameters" );
        }
    }

    static int LuaVec3Length( lua_State * L )
    {
        glm::vec3 * vec3_first = luau_checkglmvec3( L, 1 );
        lua_pushnumber( L, glm::length( *vec3_first ) );
        return 1;
    }

    static int LuaVec3Cross( lua_State * L )
    {
        glm::vec3 * vec3_first = luau_checkglmvec3( L, 1 );
        glm::vec3 * vec3_second = luau_checkglmvec3( L, 2 );
        glm::vec3 * vec3 = Core::LuaUNewGLMVec3( L );

        *vec3 = glm::cross( *vec3_first, *vec3_second );

        return 1;
    }

    static int LuaVec3Dot( lua_State * L )
    {
        glm::vec3 * vec3_first = luau_checkglmvec3( L, 1 );
        glm::vec3 * vec3_second = luau_checkglmvec3( L, 2 );

        lua_pushnumber( L, glm::dot( *vec3_first, *vec3_second ) );

        return 1;
    }

    static int LuaVec3Subtract( lua_State * L )
    {
        glm::vec3 * vec3_first = luau_checkglmvec3( L, 1 );
        glm::vec3 * vec3_second = luau_checkglmvec3( L, 2 );
        glm::vec3 * vec3 = Core::LuaUNewGLMVec3( L );

        *vec3 = *vec3_first - *vec3_second; 

        return 1;
    }

    static int LuaVec3Add( lua_State *L )
    {
        glm::vec3 * vec3_first = luau_checkglmvec3( L, 1 );
        glm::vec3 * vec3_second = luau_checkglmvec3( L, 2 );
        glm::vec3 * vec3 = Core::LuaUNewGLMVec3( L );

        *vec3 = *vec3_first + *vec3_second; 

        return 1;
    }

    static int LuaVec3Eq( lua_State * L )
    {
        glm::vec3 * vec3_first = luau_checkglmvec3( L, 1 );
        glm::vec3 * vec3_second = luau_checkglmvec3( L, 2 );

        lua_pushboolean( L, *vec3_first == *vec3_second );

        return 1;
    }

    static int LuaVec3Normalized( lua_State *L )
    {
        glm::vec3 * vec3_first = luau_checkglmvec3( L, 1 );
        glm::vec3 * vec3 = Core::LuaUNewGLMVec3( L );

        *vec3 = glm::normalize( *vec3_first );

        return 1;
    }

    static int LuaVec3Reflect( lua_State *L )
    {
        glm::vec3 * vec3_first = luau_checkglmvec3( L, 1 );
        glm::vec3 * vec3_second = luau_checkglmvec3( L, 2 );
        glm::vec3 * vec3 = Core::LuaUNewGLMVec3( L );

        *vec3 = glm::reflect( *vec3_first, *vec3_second );

        return 1;
    } 

    static int LuaVec3Get( lua_State * L )
    {
        glm::vec3 * vec3_first = luau_checkglmvec3( L, 1 );

        if( lua_gettop( L ) == 2 )
        {
            int index = luaL_checkinteger( L, 2 );
            if( index > 0 && index <= 3)
            {
                lua_pushnumber( L, (*vec3_first)[index-1] );
                return 1;
            }
            else
            {
                return luaL_error( L, "Invalid index" );
            }
        }
        
        lua_pushnumber( L, (*vec3_first)[0]);
        lua_pushnumber( L, (*vec3_first)[1]);
        lua_pushnumber( L, (*vec3_first)[2]);

        return 3;
    }

    static int LuaVec3Multiply( lua_State *L )
    {
        glm::vec3 * vec3_first = luau_checkglmvec3( L, 1 );
        float second = luau_checkfloat( L, 2 );

        glm::vec3 * vec3_ret = Core::LuaUNewGLMVec3( L );

        *vec3_ret = *vec3_first * second;

        return 1;
    }

    static int LuaVec4Newindex( lua_State * L )
    {
        return luaL_error( L, "Read only" );
    }

    static int LuaVec4New( lua_State * L )
    {
        int params = lua_gettop( L );
        glm::vec4 * vec4_ret = Core::LuaUNewGLMVec4( L );

        if( params >= 4 )
        {
            (*vec4_ret)[0] = luau_checkfloat( L, 1 ); 
            (*vec4_ret)[1] = luau_checkfloat( L, 2 ); 
            (*vec4_ret)[2] = luau_checkfloat( L, 3 ); 
            (*vec4_ret)[3] = luau_checkfloat( L, 4 ); 

            return 1;
        }
        else if( params == 1 )
        {
            glm::vec4 * vec4_first = luau_checkglmvec4( L, 1 );

            *vec4_ret = *vec4_first;

            return 1;
        }
        else if( params == 0 )
        {
            return 1;
        }

        return luaL_error( L, "Invalid argument count" );
    }

    static int LuaVec4Add( lua_State * L )
    {
        glm::vec4 * vec4_first = luau_checkglmvec4( L, 1 );
        glm::vec4 * vec4_second = luau_checkglmvec4( L, 2 );
        glm::vec4 * vec4_ret = Core::LuaUNewGLMVec4( L );
    
        *vec4_ret = *vec4_first + *vec4_second; 

        return 1;
    }

    static int LuaVec4Subtract( lua_State * L )
    {
        glm::vec4 * vec4_first = luau_checkglmvec4( L, 1 );
        glm::vec4 * vec4_second = luau_checkglmvec4( L, 2 );
        glm::vec4 * vec4_ret = Core::LuaUNewGLMVec4( L );
    
        *vec4_ret = *vec4_first - *vec4_second; 

        return 1;
    }

    static int LuaVec4Get( lua_State * L )
    {
        glm::vec4 * vec4_first = luau_checkglmvec4( L, 1 );

        if( lua_gettop( L ) == 2 )
        {
            int index = luaL_checkinteger( L, 2 );
            if( index > 0 && index <= 4)
            {
                lua_pushnumber( L, (*vec4_first)[index-1] );
                return 1;
            }
            else
            {
                return luaL_error( L, "Invalid index" );
            }
        }
        
        lua_pushnumber( L, (*vec4_first)[0]);
        lua_pushnumber( L, (*vec4_first)[1]);
        lua_pushnumber( L, (*vec4_first)[2]);
        lua_pushnumber( L, (*vec4_first)[3]);

        return 4;
    }
   
    static int LuaVec4Multiply( lua_State *L )
    {
        glm::vec4 * vec4_first = luau_checkglmvec4( L, 1 );
        float second = luau_checkfloat( L, 2 );

        glm::vec4 * vec4_ret = Core::LuaUNewGLMVec4( L );

        *vec4_ret = *vec4_first * second;

        return 1;
    }

    static int LuaMat4Newindex( lua_State * L )
    {
        return luaL_error( L, "Read only" );
    }

    static int LuaMat4New( lua_State * L )
    {
        int params = lua_gettop( L );
        glm::mat4 * mat4 = Core::LuaUNewGLMMat4( L );

        if( params == 16 )
        {
            for( int i = 0; i < 16; i++ )
            {
                (&(*mat4)[0][0])[i] = luau_checkfloat( L, i+1 );
            }             

            return 1;
        }
        else if( params == 1 )
        {
            if( lua_isuserdata(L ,1 ) )
            {
                glm::mat4 * mat4_first = luau_checkglmmat4( L, 1 );
                *mat4 = *mat4_first;

                return 1;
            }
            else if( lua_isnumber( L, 1 ) )
            {
                *mat4 = glm::mat4( luau_tofloat( L, 1 ) ); 

                return 1;
            }
           
            return luaL_error( L, "Invalid param" );
        }
        else if( params == 0 )
        {
            return 1;
        }
        else
        {
            return luaL_error( L, "Invalid param count" );
        }
    }

    static int LuaMat4Multiply( lua_State *L ) 
    {
        if( lua_gettop( L ) == 2 )
        {
            lua_getmetatable( L, 2 ); 
            int second_meta = lua_gettop( L );

            luaL_newmetatable( L, GLMMAT4_META_TYPE ); 
            int mat4_meta = lua_gettop( L );
            luaL_newmetatable( L, GLMVEC4_META_TYPE ); 
            int vec4_meta = lua_gettop( L );


            if( lua_equal( L, second_meta, mat4_meta ) )
            {
                glm::mat4 * mat4_first = luau_checkglmmat4( L, 1 );
                glm::mat4 * mat4_second = luau_checkglmmat4( L, 2 );

                glm::mat4 * mat4_ret  = Core::LuaUNewGLMMat4( L );

                *mat4_ret = (*mat4_first) * (*mat4_second);
    
                return 1;
            }
            else if( lua_equal( L, second_meta, vec4_meta ) )
            {
                glm::mat4 * mat4_first = luau_checkglmmat4( L, 1 );
                glm::vec4 * vec4_second = luau_checkglmvec4( L, 2 );

                glm::vec4 * vec4_ret  = Core::LuaUNewGLMVec4( L );

                *vec4_ret = (*mat4_first) * (*vec4_second);

                return 1;
            }
        }
        else
        {
            return luaL_error( L, "Invalid parameter count" );
        }

        return luaL_error( L, "Invalid parameter" );
    }

    static int LuaMat4Transpose( lua_State *L )
    {
        glm::mat4 * mat4_first = luau_checkglmmat4( L, 1 );
        glm::mat4 * mat4 = Core::LuaUNewGLMMat4( L );

        *mat4 = glm::transpose( *mat4_first );
          
        return 1;
    }

    static int LuaMat4Inverse( lua_State *L )
    {
        glm::mat4 * mat4_first = luau_checkglmmat4( L, 1 );
        glm::mat4 * mat4 = Core::LuaUNewGLMMat4( L );

        *mat4 = glm::inverse( *mat4_first );

        return 1;
    }

    static int LuaMat4Add( lua_State * L )
    {
        glm::mat4 * mat4 = Core::LuaUNewGLMMat4( L );
        glm::mat4 * mat4_first = luau_checkglmmat4( L, 1 );
        glm::mat4 * mat4_second = luau_checkglmmat4( L, 2 );

        *mat4 = *mat4_first + *mat4_second; 

        return 1;
    }

    static int LuaMat4Subtract( lua_State * L )
    {
        glm::mat4 * mat4_first = luau_checkglmmat4( L, 1 );
        glm::mat4 * mat4_second = luau_checkglmmat4( L, 2 );
        glm::mat4 * mat4 = Core::LuaUNewGLMMat4( L );

        *mat4 = *mat4_first - *mat4_second; 

        return 1;
    }

    static int LuaMat4Eq( lua_State * L )
    {
        glm::mat4 * mat4_first = luau_checkglmmat4( L, 1 );
        glm::mat4 * mat4_second = luau_checkglmmat4( L, 2 );

        lua_pushboolean( L, *mat4_first == *mat4_second );

        return 1;
    }

    static int LuaMat4Get( lua_State * L )
    {
        glm::mat4 * mat4_first = luau_checkglmmat4( L, 1 );

        if( lua_gettop( L ) == 2 )
        {
            int index = luaL_checkinteger( L, 2 );
            if( index > 0 && index <= 16)
            {
                lua_pushnumber( L, (&((*mat4_first)[0][0]))[index-1] );
                return 1;
            }
            else
            {
                return luaL_error( L, "Invalid index" );
            }
        }

        for( int i = 0; i < 16; i++ )
        {
            lua_pushnumber( L, (&(*mat4_first)[0][0])[i] );
        }

        return 16;
    }

    static int LuaMat4Perspective( lua_State *L )
    {
        float fov = luau_checkfloat( L, 1);
        float aspect = luau_checkfloat( L, 2);
        float zNear = luau_checkfloat( L, 3);
        float zFar = luau_checkfloat( L, 4); 
        glm::mat4 * mat4_ret = Core::LuaUNewGLMMat4( L );

        *mat4_ret = glm::perspective( fov, aspect, zNear, zFar );

        return 1;
    }


    static int LuaMat4PerspectiveFov( lua_State * L )
    {
         
        float fov = luau_checkfloat( L, 1);
        float width = luau_checkfloat( L, 2 );
        float height = luau_checkfloat( L, 3);
        float zNear = luau_checkfloat( L, 4);
        float zFar = luau_checkfloat( L, 5); 

        glm::mat4 * mat4_ret = Core::LuaUNewGLMMat4( L );

        *mat4_ret = glm::perspectiveFov( fov, width, height, zNear, zFar );

        return 1;
    }

    static int LuaMat4Project( lua_State * L )
    {
        glm::vec3 * vec3_first = luau_checkglmvec3( L, 1 );
        glm::mat4 * mat4_second = luau_checkglmmat4( L, 2 );
        glm::mat4 * mat4_third = luau_checkglmmat4( L, 3 );
        glm::vec4 * vec4_fourth = luau_checkglmvec4( L, 4 );

        glm::vec3 * vec3_ret = Core::LuaUNewGLMVec3( L );

        *vec3_ret = glm::project( *vec3_first, *mat4_second, *mat4_third, *vec4_fourth ); 

        return 1;
    }

    static int LuaMat4Ortho( lua_State * L )
    {
        int params = lua_gettop( L );
        float left = luau_checkfloat( L, 1 );
        float right = luau_checkfloat( L, 2 );
        float bottom = luau_checkfloat( L, 3 );
        float top = luau_checkfloat( L, 4 );

        if( params > 4 )
        {
            float zNear = luau_checkfloat( L, 5 );
            float zFar = luau_checkfloat( L, 6 );
            glm::mat4 * mat4_ret = Core::LuaUNewGLMMat4( L );
            *mat4_ret = glm::ortho( left, right, bottom, top, zNear, zFar );
        }
        else
        {
            glm::mat4 * mat4_ret = Core::LuaUNewGLMMat4( L );
            *mat4_ret = glm::ortho( left, right, bottom, top );
        }
        return 1;
    }

    static int LuaMat4Rotate( lua_State * L )
    {
        glm::mat4 * mat4_first = luau_checkglmmat4( L, 1 );
        float angle = luau_checkfloat( L, 2 );
        glm::vec3 * vec3_third = luau_checkglmvec3( L, 3 );

        glm::mat4 * mat4_ret = Core::LuaUNewGLMMat4( L );
    
        *mat4_ret = glm::rotate( *mat4_first, angle, *vec3_third );

        return 1;
    }

    static int LuaMat4Scale( lua_State * L )
    {
        glm::mat4 * mat4_first = luau_checkglmmat4( L, 1 );
        glm::vec3 * vec3_second = luau_checkglmvec3( L, 2 );

        glm::mat4 * mat4_ret = Core::LuaUNewGLMMat4( L );

        *mat4_ret = glm::scale( *mat4_first, *vec3_second );
        
        return 1;  
    }

    static int LuaMat4Translate( lua_State * L )
    {
        glm::mat4 * mat4_first = luau_checkglmmat4( L, 1 );
        glm::vec3 * vec3_second = luau_checkglmvec3( L, 2 );

        glm::mat4 * mat4_ret = Core::LuaUNewGLMMat4( L );

        *mat4_ret = glm::translate( *mat4_first, *vec3_second );
        
        return 1;  
    }

    static int LuaMat4UnProject( lua_State * L )
    {
        glm::vec3 * vec3_first = luau_checkglmvec3( L, 1 );
        glm::mat4 * mat4_second = luau_checkglmmat4( L, 2 );
        glm::mat4 * mat4_third = luau_checkglmmat4( L, 3 );
        glm::vec4 * vec4_fourth = luau_checkglmvec4( L, 4 );

        glm::vec3 * vec3_ret = Core::LuaUNewGLMVec3( L );

        *vec3_ret = glm::unProject( *vec3_first, *mat4_second, *mat4_third, *vec4_fourth ); 

        return 1;
    }

    static int LuaMat4Interpolate( lua_State * L )
    {
        glm::mat4 * mat4_first = luau_checkglmmat4( L, 1 );
        glm::mat4 * mat4_second = luau_checkglmmat4( L, 2 );
        float delta = luau_checkfloat( L, 3 );

        glm::mat4 * mat4_ret = Core::LuaUNewGLMMat4( L );

        *mat4_ret = glm::interpolate( *mat4_first, *mat4_second, delta );

        return 1;
    }

    static int LuaQuatNewindex( lua_State * L )
    {
        return luaL_error( L, "Read only" );
    }

    static int LuaQuatNew( lua_State * L )
    {
        int params = lua_gettop( L );
        glm::quat * quat_ret = Core::LuaUNewGLMQuat( L );

        if( params >= 4 )
        {
            (*quat_ret)[0] = luau_checkfloat( L, 1 );
            (*quat_ret)[1] = luau_checkfloat( L, 2 );
            (*quat_ret)[2] = luau_checkfloat( L, 3 );
            (*quat_ret)[3] = luau_checkfloat( L, 4 );

            return 1;
        } 
        else if( params == 1 )
        {
            glm::quat *quat_first = luau_checkglmquat( L, 1 ); 

            *quat_ret = *quat_first;
            return 1;
        }
        else
        {
            return 1;     
        }
    }

    static int LuaQuatAngle( lua_State *L )
    {
        glm::quat *quat_first = luau_checkglmquat( L, 1 ); 
          
        lua_pushnumber( L, glm::angle( *quat_first ) );

        return 1;
    }

    static int LuaQuatAngleAxis( lua_State * L )
    {

        float angle = luau_checkfloat( L, 1 );
        glm::vec3 *vec3_second = luau_checkglmvec3( L, 2 ); 
        glm::quat * quat_ret = Core::LuaUNewGLMQuat( L );

        *quat_ret = glm::angleAxis( angle, *vec3_second );

        return 1;
    }

    static int LuaQuatAxis( lua_State * L )
    {
        glm::quat *quat_first = luau_checkglmquat( L, 1 ); 
        glm::vec3 *vec3_ret = Core::LuaUNewGLMVec3( L );

        *vec3_ret = glm::axis( *quat_first );

        return 1;
    }

    static int LuaQuatConjugate( lua_State * L )
    {
        glm::quat *quat_first = luau_checkglmquat( L, 1 );
        glm::quat *quat_ret = Core::LuaUNewGLMQuat( L );

        *quat_ret = glm::conjugate( *quat_first );

        return 1;
    }

    static int LuaQuatDot( lua_State * L )
    {
        glm::quat *quat_first = luau_checkglmquat( L, 1 );
        glm::quat *quat_second = luau_checkglmquat( L, 2 );

        lua_pushnumber( L, glm::dot( *quat_first, *quat_second ) );

        return 1;
    }

    static int LuaQuatEq( lua_State * L )
    {
        glm::quat *quat_first = luau_checkglmquat( L, 1 );
        glm::quat *quat_second = luau_checkglmquat( L, 2 );

        lua_pushboolean( L, *quat_first == *quat_second );

        return 1;
    }

    static int LuaQuatInverse( lua_State *L )
    {
        glm::quat *quat_first = luau_checkglmquat( L, 1 );
        glm::quat *quat_ret = Core::LuaUNewGLMQuat( L );

        *quat_ret = glm::inverse( *quat_first );

        return 1;
    }  

    static int LuaQuatLength( lua_State *L )
    {
        glm::quat *quat_first = luau_checkglmquat( L, 1 );
        
        lua_pushnumber( L, glm::length( *quat_first ) );

        return 1;
    } 

    static int LuaQuatLerp( lua_State *L )
    {
        glm::quat *quat_first = luau_checkglmquat( L, 1 );
        glm::quat *quat_second = luau_checkglmquat( L, 2 );
        float lerpFactor = luau_checkfloat( L, 3 );

        glm::quat *quat_ret = Core::LuaUNewGLMQuat( L );
        
        *quat_ret = glm::lerp( *quat_first, *quat_second, lerpFactor );

        return 1;
    } 

    static int LuaQuatMat4Cast( lua_State * L )
    {
        glm::quat *quat_first = luau_checkglmquat( L, 1 );
        glm::mat4 *mat4_ret = Core::LuaUNewGLMMat4( L );

        *mat4_ret = glm::mat4_cast( *quat_first ); 

        return 1;
    }

    static int LuaQuatNormalize( lua_State * L) 
    {
        glm::quat *quat_first = luau_checkglmquat( L, 1 );
        glm::quat *quat_ret = Core::LuaUNewGLMQuat( L );

        *quat_ret = glm::normalize( *quat_first );

        return 1;
    }

    static int LuaQuatRotate( lua_State *L )
    {
        glm::quat *quat_first =  luau_checkglmquat( L, 1 );
        float angle = luau_checkfloat( L, 2 );
        glm::vec3 *vec_third = luau_checkglmvec3( L, 3 );

        glm::quat *quat_ret = Core::LuaUNewGLMQuat( L );

        *quat_ret = glm::rotate( *quat_first, angle, *vec_third );
        return 1;
    }

    static int LuaQuatMix( lua_State *L )
    {
        glm::quat *quat_first = luau_checkglmquat( L, 1 );
        glm::quat *quat_second = luau_checkglmquat( L, 2 );
        float lerpFactor = luau_checkfloat( L, 3 );

        glm::quat *quat_ret = Core::LuaUNewGLMQuat( L );
        
        *quat_ret = glm::mix( *quat_first, *quat_second, lerpFactor );

        return 1;
    }
}

namespace Core
{
    LuaGLMBridge::LuaGLMBridge( lua_State * L )
    {
        lua_getglobal( L, "core" );         
            lua_newtable( L ); //glm

				lua_newtable( L ); //vec2
                int vec2_table = lua_gettop( L );
                    luau_setfunction( L, "new", LuaVec2New );
                    luau_setfunction( L, "dot", LuaVec2Dot );
                    luau_setfunction( L, "subtract", LuaVec2Subtract );
                    luau_setfunction( L, "add", LuaVec2Add );
                    luau_setfunction( L, "eq", LuaVec2Eq );
                    luau_setfunction( L, "normalize", LuaVec2Normalized );
                    luau_setfunction( L, "reflect", LuaVec2Reflect );
                    luau_setfunction( L, "length", LuaVec2Length );
                    luau_setfunction( L, "get", LuaVec2Get );
                    luau_setfunction( L, "multiply", LuaVec2Multiply );

                    luaL_newmetatable( L, GLMVEC2_META_TYPE );
                        lua_pushvalue( L, vec2_table );
                        lua_setfield( L, -2, "__index" );

                        luau_setfunction( L, "__newindex", LuaVec2Newindex );
                        luau_setfunction( L, "__add", LuaVec2Add );
                        luau_setfunction( L, "__sub", LuaVec2Subtract );
                        luau_setfunction( L, "__eq", LuaVec2Eq );
                        luau_setfunction( L, "__mul", LuaVec2Multiply );
                    lua_pop( L, 1 );
                lua_setfield( L, -2, "vec2" );


                lua_newtable( L ); //vec3
                int vec3_table = lua_gettop( L );
                    luau_setfunction( L, "new", LuaVec3New );
                    luau_setfunction( L, "cross", LuaVec3Cross );
                    luau_setfunction( L, "dot", LuaVec3Dot );
                    luau_setfunction( L, "subtract", LuaVec3Subtract );
                    luau_setfunction( L, "add", LuaVec3Add );
                    luau_setfunction( L, "eq", LuaVec3Eq );
                    luau_setfunction( L, "normalize", LuaVec3Normalized );
                    luau_setfunction( L, "reflect", LuaVec3Reflect );
                    luau_setfunction( L, "length", LuaVec3Length );
                    luau_setfunction( L, "get", LuaVec3Get );
                    luau_setfunction( L, "multiply", LuaVec3Multiply );

                    luaL_newmetatable( L, GLMVEC3_META_TYPE );
                        lua_pushvalue( L, vec3_table );
                        lua_setfield( L, -2, "__index" );

                        luau_setfunction( L, "__newindex", LuaVec3Newindex ); 
                        luau_setfunction( L, "__add", LuaVec3Add );
                        luau_setfunction( L, "__sub", LuaVec3Subtract );
                        luau_setfunction( L, "__eq", LuaVec3Eq );
                        luau_setfunction( L, "__mul", LuaVec3Multiply );
                    lua_pop( L, 1 );
                lua_setfield( L, -2, "vec3" );

                lua_newtable( L );
                int vec4_table = lua_gettop( L );
                    luau_setfunction( L, "new", LuaVec4New );
                    luau_setfunction( L, "subtract", LuaVec4Subtract );
                    luau_setfunction( L, "add", LuaVec4Add );
                    luau_setfunction( L, "get", LuaVec4Get );
                    luau_setfunction( L, "multiply", LuaVec4Multiply );

                    luaL_newmetatable( L, GLMVEC4_META_TYPE );
                        lua_pushvalue( L, vec4_table );
                        lua_setfield( L, -2, "__index" );
                       
                        luau_setfunction( L, "__newindex", LuaVec4Newindex ); 
                        luau_setfunction( L, "__sub", LuaVec4Subtract );
                        luau_setfunction( L, "__add", LuaVec4Add );
                        luau_setfunction( L, "__mul", LuaVec4Multiply );
                    lua_pop( L, 1 );
                lua_setfield( L, -2, "vec4" );

                lua_newtable( L ); //mat4
                int mat4_table = lua_gettop( L );
                    luau_setfunction( L, "new", LuaMat4New );
                    luau_setfunction( L, "multiply", LuaMat4Multiply );
                    luau_setfunction( L, "transpose", LuaMat4Transpose );
                    luau_setfunction( L, "inverse", LuaMat4Inverse );
                    luau_setfunction( L, "add", LuaMat4Add );
                    luau_setfunction( L, "subtract", LuaMat4Subtract );
                    luau_setfunction( L, "eq", LuaMat4Eq );
                    luau_setfunction( L, "get", LuaMat4Get );

                    luau_setfunction( L, "perspective", LuaMat4Perspective ); 
                    luau_setfunction( L, "perspectiveFov", LuaMat4PerspectiveFov );
                    luau_setfunction( L, "project", LuaMat4Project );
                    luau_setfunction( L, "ortho", LuaMat4Ortho );
                    luau_setfunction( L, "rotate", LuaMat4Rotate );
                    luau_setfunction( L, "scale", LuaMat4Scale );
                    luau_setfunction( L, "translate", LuaMat4Translate );
                    luau_setfunction( L, "unproject", LuaMat4UnProject );
                    luau_setfunction( L, "interpolate", LuaMat4Interpolate );

                    luaL_newmetatable( L, GLMMAT4_META_TYPE );
                        lua_pushvalue( L, mat4_table );
                        lua_setfield( L, -2, "__index" );

                        luau_setfunction( L, "__newindex", LuaMat4Newindex );
                        luau_setfunction( L, "__mul", LuaMat4Multiply );
                        luau_setfunction( L, "__add", LuaMat4Add );
                        luau_setfunction( L, "__sub", LuaMat4Subtract );
                        luau_setfunction( L, "__eq", LuaMat4Eq );
                    lua_pop( L, 1 ); 
                lua_setfield( L, -2, "mat4" );
                
                lua_newtable( L );
                int quat_table = lua_gettop( L );
                    
                    luau_setfunction( L, "new", LuaQuatNew );
                    luau_setfunction( L, "normalize", LuaQuatNormalize );
                    luau_setfunction( L, "angle", LuaQuatAngle );
                    luau_setfunction( L, "angleAxis", LuaQuatAngleAxis );
                    luau_setfunction( L, "axis", LuaQuatAxis );
                    luau_setfunction( L, "conjugate", LuaQuatConjugate );
                    luau_setfunction( L, "dot", LuaQuatDot );
                    luau_setfunction( L, "eq", LuaQuatEq );
                    luau_setfunction( L, "inverse", LuaQuatInverse );
                    luau_setfunction( L, "length", LuaQuatLength );
                    luau_setfunction( L, "lerp", LuaQuatLerp );
                    luau_setfunction( L, "mat4Cast", LuaQuatMat4Cast );
                    luau_setfunction( L, "rotate", LuaQuatRotate );
                    luau_setfunction( L, "slerp", LuaQuatMix );
                    luau_setfunction( L, "mix", LuaQuatMix );

                    luaL_newmetatable( L, GLMQUAT_META_TYPE );
                        lua_pushvalue( L, quat_table );
                        lua_setfield( L, -2, "__index" );

                        luau_setfunction( L, "__newindex", LuaQuatNewindex );
                        luau_setfunction( L, "__eq", LuaQuatEq );
                    lua_pop( L, 1 ); 

                lua_setfield( L, -2, "quat" );
            lua_setfield( L, -2, "glm" );
        lua_pop( L, 1 );
    }
}

#endif
