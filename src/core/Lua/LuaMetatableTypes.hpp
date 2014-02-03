#ifndef SRC_CORE_LUA_LUAMETATABLETYPES_H
#define SRC_CORE_LUA_LUAMETATABLETYPES_H


#define COMPONENT_META_TYPE "metatype_core_component"
#define ENTITY_META_TYPE "metatype_core_entity"
#define BITMASK_META_TYPE "metatype_bitmask"
#define ASPECT_META_TYPE "metatype_aspect"
#define GLMVEC2_META_TYPE "metatype_glm_vec2"
#define GLMVEC3_META_TYPE "metatype_glm_vec3"
#define GLMVEC4_META_TYPE "metatype_glm_vec4"
#define GLMMAT4_META_TYPE "metatype_glm_mat4"
#define GLMQUAT_META_TYPE "metatype_glm_quat"
#define CAMERA_META_TYPE "metatype_camera"
#define UNIT_TYPE_OBJECT_TYPE_META "unit_type_object_type_meta"
#define SQUAD_FORMATION_META_TYPE "metatype_squad_formation"

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <cstdint>

#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

#include <ComponentFramework/SystemHandlerTemplate.hpp>
#include <ComponentFramework/SystemTypes.hpp>
#include <Components/UnitTypeComponent.hpp>
#include <Components/SquadComponent.hpp>
#include <Camera/Camera.hpp>

struct LuaEntity
{
    Core::Entity entity;
    bool light;
};

struct LuaAspect
{
    Core::Aspect aspect;
};

inline glm::vec2* luau_checkglmvec2( lua_State* state, int pos ) { return static_cast<glm::vec2*>(luaL_checkudata( state, pos, GLMVEC2_META_TYPE )); }
inline glm::vec3* luau_checkglmvec3( lua_State* state, int pos ) { return static_cast<glm::vec3*>(luaL_checkudata( state, pos, GLMVEC3_META_TYPE )); }
inline glm::vec4* luau_checkglmvec4( lua_State* state, int pos ) { return static_cast<glm::vec4*>(luaL_checkudata( state, pos, GLMVEC4_META_TYPE )); }
inline glm::mat4* luau_checkglmmat4( lua_State* state, int pos ) { return static_cast<glm::mat4*>(luaL_checkudata( state, pos, GLMMAT4_META_TYPE )); }
inline glm::quat* luau_checkglmquat( lua_State* state, int pos ) { return static_cast<glm::quat*>(luaL_checkudata( state, pos, GLMQUAT_META_TYPE )); }
inline Core::Camera** luau_checkcamera( lua_State* state, int pos ) { return static_cast<Core::Camera**>(luaL_checkudata( state, pos, CAMERA_META_TYPE )); }
inline LuaEntity* luau_checkentity( lua_State* state, int pos ) { return static_cast<LuaEntity*>(luaL_checkudata( state, pos, ENTITY_META_TYPE ) ); }
inline LuaAspect* luau_checkaspect( lua_State* state, int pos ) { return static_cast<LuaAspect*>(luaL_checkudata( state, pos, ASPECT_META_TYPE) ) ; }
inline Core::ComponentType luau_checkcomponent( lua_State * state, int pos ) { return *static_cast<Core::ComponentType*>(luaL_checkudata( state, pos, COMPONENT_META_TYPE) ); }
inline Core::UnitType* luau_checkunittype( lua_State * state, int pos) { return static_cast<Core::UnitType*>(luaL_checkudata(state, pos, UNIT_TYPE_OBJECT_TYPE_META) ); }
inline Core::SquadFormation* luau_checksquadformation( lua_State * state, int pos) { return static_cast<Core::SquadFormation*>(luaL_checkudata(state, pos, SQUAD_FORMATION_META_TYPE) ); }

namespace Core
{
    LuaEntity* LuaUNewEntity( lua_State * L );
    LuaEntity* LuaUNewLightEntity( lua_State * L );

    LuaAspect* LuaUNewAspect( lua_State * L );

    Core::SquadFormation* LuaUNewSquadFormation(lua_State * L);

    uint64_t* LuaUNewBitmask( lua_State * L );

	glm::vec2* LuaUNewGLMVec2( lua_State * L );
    glm::vec3* LuaUNewGLMVec3( lua_State * L );
    glm::vec4* LuaUNewGLMVec4( lua_State * L );
    glm::mat4* LuaUNewGLMMat4( lua_State * L );
    glm::quat* LuaUNewGLMQuat( lua_State * L );
    
    Camera** LuaUNewCamera( lua_State * L );
}

#endif
