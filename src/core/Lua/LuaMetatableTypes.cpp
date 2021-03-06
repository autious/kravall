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

    LuaEntity* LuaUNewLightEntity( lua_State * L, Core::Entity entity )
    {
        LuaEntity* ent = LuaUNewEntity( L );
        ent->entity = entity;
        ent->light = true;

        return ent;
    }

    LuaAspect* LuaUNewAspect( lua_State * L )
    {
        LuaAspect* asp = (LuaAspect*)lua_newuserdata(L, sizeof(LuaAspect));
        luaL_newmetatable(L, ASPECT_META_TYPE);
        lua_setmetatable(L, -2);

        return asp;
    }

    Core::SquadFormation* LuaUNewSquadFormation(lua_State* L)
    {
        Core::SquadFormation* formation = static_cast<Core::SquadFormation*>(lua_newuserdata(L, sizeof(Core::SquadFormation)));                  
        luaL_newmetatable(L, SQUAD_FORMATION_META_TYPE);
        lua_setmetatable(L, -2);
        
        return formation;
    }

    Core::SquadAbility* LuaUNewSquadAbility( lua_State * L )
    {
        Core::SquadAbility* ability = static_cast<Core::SquadAbility*>(lua_newuserdata(L, sizeof(Core::SquadAbility)));
        luaL_newmetatable( L, SQUAD_ABILITY_META_TYPE );
        lua_setmetatable(L, -2);
        
        return ability;
    }

    Core::SquadAbility* LuaUNewSquadAbility( lua_State * L, Core::SquadAbility value )
    {
        Core::SquadAbility* ability = static_cast<Core::SquadAbility*>(lua_newuserdata(L, sizeof(Core::SquadAbility)));
        *ability = value;
        luaL_newmetatable( L, SQUAD_ABILITY_META_TYPE );
        lua_setmetatable(L, -2);
        
        return ability;
    }

    Core::PoliceStance* LuaUNewPoliceStance(lua_State* L)
    {
        Core::PoliceStance* stance = static_cast<Core::PoliceStance*>(lua_newuserdata(L, sizeof(Core::PoliceStance)));                  
        luaL_newmetatable(L, POLICE_STANCE_META_TYPE);
        lua_setmetatable(L, -2);
        
        return stance;
    }

    LuaParticleDefinition* LuaUNewParticleDefinition(lua_State* L)
    {
        LuaParticleDefinition* handle = static_cast<LuaParticleDefinition*>(lua_newuserdata(L, sizeof(LuaParticleDefinition)));
        luaL_newmetatable(L, PARTICLE_DEFINITION_META_TYPE);
        lua_setmetatable(L, -2);

        return handle;
    }

    Core::EmitterType* LuaUNewEmitterType(lua_State* L)
    {
        Core::EmitterType* type = static_cast<Core::EmitterType*>(lua_newuserdata(L, sizeof(Core::EmitterType)));
        luaL_newmetatable(L, EMITTER_TYPE_META_TYPE);
        lua_setmetatable(L, -2);

        return type;
    }

    Core::PoliceStance* LuaUNewPoliceStance(lua_State* L, Core::PoliceStance value )
    {
        Core::PoliceStance* stance = static_cast<Core::PoliceStance*>(lua_newuserdata(L, sizeof(Core::PoliceStance)));                  
        *stance = value;
        luaL_newmetatable(L, POLICE_STANCE_META_TYPE);
        lua_setmetatable(L, -2);
        
        return stance;
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
    
    LuaLog* LuaUNewLog( lua_State * L )
    {
        LuaLog* log = static_cast<LuaLog*>(lua_newuserdata( L, sizeof( LuaLog ) ));

        luaL_getmetatable( L, LOG_META_TYPE );
        lua_setmetatable( L, -2 );

        return log;
    }

    Camera** LuaUNewCamera( lua_State * L )
    {
        Camera** cam = static_cast<Camera**>(lua_newuserdata( L, sizeof( Camera* ) ) );
        *cam = nullptr;

        luaL_getmetatable( L, CAMERA_META_TYPE );
        lua_setmetatable( L, -2 ); 
       
        return cam;
    }

    LuaHoverText* LuaUNewHoverText( lua_State * L )
    {
        LuaHoverText* hover = static_cast<LuaHoverText*>( lua_newuserdata( L, sizeof( LuaHoverText) ) );
        hover->hoverTextId = -1;
        hover->light = false;
           
        luaL_getmetatable( L, HOVER_TEXT_META_TYPE );
        lua_setmetatable( L, -2 );

        return hover;
    }

    LuaHoverText* LuaUNewLightHoverText( lua_State * L, int id )
    {
        LuaHoverText* hover = static_cast<LuaHoverText*>( lua_newuserdata( L, sizeof( LuaHoverText) ) );
        hover->hoverTextId = id;
        hover->light = true;
           
        luaL_getmetatable( L, HOVER_TEXT_META_TYPE );
        lua_setmetatable( L, -2 );

        return hover;
    }

    LuaTTFFont* LuaUNewTTFFont( lua_State * L )
    {
        LuaTTFFont* font = static_cast<LuaTTFFont*>( lua_newuserdata( L, sizeof(LuaTTFFont) ) );
        font->fontPtr = nullptr;

        //Needs to be new, because no-one else will define and add functions.
        luaL_newmetatable( L, TTF_FONT_META_TYPE );
        lua_setmetatable( L, -2 );

        return font;
    }

    LuaTimer* LuaUNewTimer( lua_State * L, Core::HighresTimer * t )
    {
        LuaTimer* timer = static_cast<LuaTimer*>( lua_newuserdata( L, sizeof(LuaTimer) ) );

        timer->timer = t;

        luaL_getmetatable( L, TIMER_META_TYPE );
        lua_setmetatable( L, -2 );

        return timer;
    }
}
