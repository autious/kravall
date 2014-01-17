#include "LuaMetatableTypes.hpp"

#include <lauxlib.h>
#include <lualib.h>

namespace Core
{
    LuaEntity* LuaUNewEntity( lua_State * L )
    {
        LuaEntity * ent = (LuaEntity*)lua_newuserdata( L, sizeof( LuaEntity ) ); 
        luaL_getmetatable( L, ENTITY_META_TYPE );
        lua_setmetatable( L, -2 );
        ent->light = false;

        return ent;
    }

    LuaEntity* LuaUNewLightEntity( lua_State * L )
    {
        LuaEntity* ent = LuaUNewEntity( L );
        ent->light = true;

        return ent;
    }

    uint64_t* LuaUNewBitmask( lua_State * L )
    {
        uint64_t * data = (uint64_t*)lua_newuserdata( L, sizeof( uint64_t ));
        luaL_getmetatable( L, BITMASK_META_TYPE );
        lua_setmetatable( L, -2 );

        return data;
    }

	///////////////////////////////////////////////

	glm::vec2* LuaUNewGLMVec2( lua_State * L )
    {
        glm::vec2* vec2 = static_cast<glm::vec2*>(lua_newuserdata( L, sizeof( glm::vec2 ) ) );
        *vec2 = glm::vec2();
        luaL_getmetatable( L, GLMVEC2_META_TYPE );
        lua_setmetatable( L, -2 );

        return vec2;
    }

	//////////////////////////////////////////

    glm::vec3* LuaUNewGLMVec3( lua_State * L )
    {
        glm::vec3* vec3 = static_cast<glm::vec3*>(lua_newuserdata( L, sizeof( glm::vec3 ) ) );
        *vec3 = glm::vec3();
        luaL_getmetatable( L, GLMVEC3_META_TYPE );
        lua_setmetatable( L, -2 );

        return vec3;
    }

    glm::vec4* LuaUNewGLMVec4( lua_State * L )
    {
        glm::vec4* vec4 = static_cast<glm::vec4*>(lua_newuserdata( L, sizeof( glm::vec4 ) ) );
        *vec4 = glm::vec4();
        luaL_getmetatable( L, GLMVEC4_META_TYPE );
        lua_setmetatable( L, -2 );

        return vec4;
    }

    glm::mat4* LuaUNewGLMMat4( lua_State *L )
    {
        glm::mat4* mat4 = static_cast<glm::mat4*>(lua_newuserdata( L, sizeof( glm::mat4 ) ) );
        *mat4 = glm::mat4();

        luaL_getmetatable( L, GLMMAT4_META_TYPE );
        lua_setmetatable( L, -2 );

        return mat4;
    }

    glm::quat* LuaUNewGLMQuat( lua_State * L )
    {
        glm::quat* quat = static_cast<glm::quat*>(lua_newuserdata( L, sizeof( glm::quat ) ) );
        *quat = glm::quat();

        luaL_getmetatable( L, GLMQUAT_META_TYPE );
        lua_setmetatable( L, -2 );

        return quat;
    }

    Camera** LuaUNewCamera( lua_State * L )
    {
        Camera** cam = static_cast<Camera**>(lua_newuserdata( L, sizeof( Camera* ) ) );
        *cam = nullptr;

        luaL_getmetatable( L, CAMERA_META_TYPE );
        lua_setmetatable( L, -2 ); 
       
        return cam;
    }
}
