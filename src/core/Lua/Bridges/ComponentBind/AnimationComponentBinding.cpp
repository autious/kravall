#include "AnimationComponentBinding.hpp"
#include <World.hpp>

#include <cstring>

#include <logger/Logger.hpp>

#include <Lua/Bridges/LuaGFXBridge.hpp>

Core::ComponentGetters Core::AnimationComponentBinding::GetGetters()
{
    ComponentGetters getters; 
	
    getters["animationID"] = []( Core::Entity entity, lua_State * L )
    {
        AnimationComponent *ac = WGETC<AnimationComponent>( entity );

		lua_pushinteger( L, ac->animationID );
        
        return 1;
    };

    getters["queuedAnimationID"] = []( Core::Entity entity, lua_State * L )
    {
        AnimationComponent *ac = WGETC<AnimationComponent>( entity );

		lua_pushinteger( L, ac->queuedAnimationID );
        
        return 1;
    };
	
    getters["currentFrame"] = []( Core::Entity entity, lua_State * L )
    {
        AnimationComponent *ac = WGETC<AnimationComponent>( entity );

		lua_pushinteger( L, static_cast<int>(ac->currentFrame) );
        return 1;
    };
	
    getters["currentTime"] = []( Core::Entity entity, lua_State * L )
    {
        AnimationComponent *ac = WGETC<AnimationComponent>( entity );

		lua_pushnumber( L, ac->currentTime );
        
        return 1;
    };
	
    getters["speed"] = []( Core::Entity entity, lua_State * L )
    {
        AnimationComponent *ac = WGETC<AnimationComponent>( entity );

		lua_pushnumber( L, ac->speed );
        
        return 1;
    };
	
    getters["loop"] = []( Core::Entity entity, lua_State * L )
    {
        AnimationComponent *ac = WGETC<AnimationComponent>( entity );

		lua_pushboolean( L, ac->loop );
        
        return 1;
    };
	
    getters["playing"] = []( Core::Entity entity, lua_State * L )
    {
        AnimationComponent *ac = WGETC<AnimationComponent>( entity );

		lua_pushboolean( L, ac->playing );
        
        return 1;
    };

    return getters;
}

Core::ComponentSetters Core::AnimationComponentBinding::GetSetters()
{
    ComponentSetters setters;

    setters["animationID"] = [](Core::Entity entity, lua_State * L, int valueindex )
    {
        AnimationComponent *ac = WGETC<AnimationComponent>( entity );
            
        if( lua_isnumber( L, valueindex ) )
        {
            ac->animationID = static_cast<int>(lua_tointeger( L, valueindex ));
        }  
        else
        {
            luaL_error( L, "Unable to set animationID, given parameter is not number value" );
        }
    };

    setters["queuedAnimationID"] = [](Core::Entity entity, lua_State * L, int valueindex )
    {
        AnimationComponent *ac = WGETC<AnimationComponent>( entity );
            
        if( lua_isnumber( L, valueindex ) )
        {
            ac->queuedAnimationID = static_cast<int>(lua_tointeger( L, valueindex ));
        }  
        else
        {
            luaL_error( L, "Unable to set queuedAnimationID, given parameter is not number value" );
        }
    };

    setters["currentFrame"] = [](Core::Entity entity, lua_State * L, int valueindex )
    {
        AnimationComponent *ac = WGETC<AnimationComponent>( entity );
            
        if( lua_isnumber( L, valueindex ) )
        {
            ac->currentFrame = static_cast<unsigned int>(lua_tointeger( L, valueindex ));
        }  
        else
        {
            luaL_error( L, "Unable to set currentFrame, given parameter is not number value" );
        }
    };

    setters["currentTime"] = [](Core::Entity entity, lua_State * L, int valueindex )
    {
        AnimationComponent *ac = WGETC<AnimationComponent>( entity );
            
        if( lua_isnumber( L, valueindex ) )
        {
            ac->currentTime = static_cast<float>(lua_tonumber( L, valueindex ));
        }  
        else
        {
            luaL_error( L, "Unable to set currentTime, given parameter is not number value" );
        }
    };

    setters["speed"] = [](Core::Entity entity, lua_State * L, int valueindex )
    {
        AnimationComponent *ac = WGETC<AnimationComponent>( entity );
            
        if( lua_isnumber( L, valueindex ) )
        {
            ac->speed = static_cast<float>(lua_tonumber( L, valueindex ));
        }  
        else
        {
            luaL_error( L, "Unable to set speed, given parameter is not number value" );
        }
    };

    setters["loop"] = [](Core::Entity entity, lua_State * L, int valueindex )
    {
        AnimationComponent *ac = WGETC<AnimationComponent>( entity );
            
        if( lua_isboolean( L, valueindex ) )
        {
            ac->loop = static_cast<bool>(lua_toboolean( L, valueindex ));
        }  
        else
        {
            luaL_error( L, "Unable to set loop, given parameter is not number value" );
        }
    };

    setters["playing"] = [](Core::Entity entity, lua_State * L, int valueindex )
    {
        AnimationComponent *ac = WGETC<AnimationComponent>( entity );
            
        if( lua_isboolean( L, valueindex ) )
        {
            ac->playing = static_cast<bool>(lua_toboolean( L, valueindex ));
        }  
        else
        {
            luaL_error( L, "Unable to set playing, given parameter is not number value" );
        }
    };


    return setters;
}

Core::ComponentType Core::AnimationComponentBinding::GetComponentType()
{
    return EntityHandler::GetComponentType<AnimationComponent>();
}

const char* Core::AnimationComponentBinding::GetComponentLuaName()
{
    return "AnimationComponent";
}
