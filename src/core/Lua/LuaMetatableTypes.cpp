#include "LuaMetatableTypes.hpp"

#include <lauxlib.h>
#include <lualib.h>

namespace Core
{
    Entity* LuaUNewEntity( lua_State * L )
    {
        Entity * ent = (Entity*)lua_newuserdata( L, sizeof( Entity ) ); 
        luaL_getmetatable( L, ENTITY_META_TYPE );
        lua_setmetatable( L, -2 );

        return ent;
    }

    uint64_t* LuaUNewBitmask( lua_State * L )
    {
        uint64_t * data = (uint64_t*)lua_newuserdata( L, sizeof( uint64_t ));
        luaL_getmetatable( L, BITMASK_META_TYPE );
        lua_setmetatable( L, -2 );

        return data;
    }

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
}
