#ifndef SRC_CORE_LUA_LUAMETATABLETYPES_H
#define SRC_CORE_LUA_LUAMETATABLETYPES_H

#define COMPONENT_META_TYPE "metatype_core_component"
#define ENTITY_META_TYPE "metatype_core_entity"
#define BITMASK_META_TYPE "metatype_bitmask"
#define GLMVEC3_META_TYPE "metatype_glm_vec3"
#define GLMVEC4_META_TYPE "metatype_glm_vec4"
#define GLMMAT4_META_TYPE "metatype_glm_mat4"
#define GLMQUAT_META_TYPE "metatype_glm_quat"

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <cstdint>

#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

#include <ComponentFramework/SystemHandlerTemplate.hpp>

inline glm::vec3* luau_checkglmvec3( lua_State* state, int pos ) { return static_cast<glm::vec3*>(luaL_checkudata( state, pos, GLMVEC3_META_TYPE )); }
inline glm::vec4* luau_checkglmvec4( lua_State* state, int pos ) { return static_cast<glm::vec4*>(luaL_checkudata( state, pos, GLMVEC4_META_TYPE )); }
inline glm::mat4* luau_checkglmmat4( lua_State* state, int pos ) { return static_cast<glm::mat4*>(luaL_checkudata( state, pos, GLMMAT4_META_TYPE )); }
inline glm::quat* luau_checkglmquat( lua_State* state, int pos ) { return static_cast<glm::quat*>(luaL_checkudata( state, pos, GLMQUAT_META_TYPE )); }

namespace Core
{
    Entity* LuaUNewEntity( lua_State * L );

    uint64_t* LuaUNewBitmask( lua_State * L );

    glm::vec3* LuaUNewGLMVec3( lua_State * L );
    glm::vec4* LuaUNewGLMVec4( lua_State * L );
    glm::mat4* LuaUNewGLMMat4( lua_State * L );
    glm::quat* LuaUNewGLMQuat( lua_State * L );
}

#endif
