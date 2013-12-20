#include "BoundingVolumeComponentBinding.hpp"

#include <World.hpp>

#include <Lua/Bridges/LuaBoundingVolumeBridge.hpp>


Core::ComponentGetters Core::BoundingVolumeComponentBinding::GetGetters()
{
    ComponentGetters getters; 
    
	getters["type"] = []( Core::Entity entity, lua_State *L )
    {
        BoundingVolumeComponent *bvc = WGETC<BoundingVolumeComponent>( entity ); 
        
        Core::BoundingVolumeType* type = (Core::BoundingVolumeType*)lua_newuserdata( L, sizeof( Core::BoundingVolumeType ) );
        *type = bvc->type;
        
        luaL_newmetatable( L, BOUNDING_VOLUME_COMPONENT_TYPE_TYPE_META );
        lua_setmetatable( L, -2 );
    
        return 1;
    };

	getters["collisionModel"] = []( Core::Entity entity, lua_State *L )
    {
        BoundingVolumeComponent *bvc = WGETC<BoundingVolumeComponent>( entity ); 
        
        Core::BoundingVolumeCollisionModel* collisionModel = (Core::BoundingVolumeCollisionModel*)lua_newuserdata( L, sizeof( Core::BoundingVolumeCollisionModel ) );
        *collisionModel = bvc->collisionModel;
        
        luaL_newmetatable( L, BOUNDING_VOLUME_COMPONENT_COLLISIONMODEL_TYPE_META );
        lua_setmetatable( L, -2 );
    
        return 1;
    };

	getters["sphereRadius"] = []( Core::Entity entity, lua_State *L )
    {
        BoundingVolumeComponent *bvc = WGETC<BoundingVolumeComponent>( entity ); 

        lua_pushnumber( L, reinterpret_cast<BoundingSphere*>(bvc->data)->radius );   
        return 1;
    };

	getters["sphereOffset"] = []( Core::Entity entity, lua_State * L )
    {
        BoundingVolumeComponent *bvc = WGETC<BoundingVolumeComponent>( entity ); 
        
        lua_newtable( L );  

        for( int i = 0; i < 3; i++ )
        {
            lua_pushinteger( L, i+1 );
            lua_pushnumber( L, reinterpret_cast<BoundingSphere*>(bvc->data)->offset[i] );
            lua_settable( L, -3 );
        }
        
        return 1;
    };

    return getters;
}

Core::ComponentSetters Core::BoundingVolumeComponentBinding::GetSetters()
{
    ComponentSetters setters;

	setters["type"] = []( Core::Entity entity, lua_State * L, int valueindex )
    {
        BoundingVolumeComponent *bvc = WGETC<BoundingVolumeComponent>( entity ); 
        bvc->type = *(Core::BoundingVolumeType*)luaL_checkudata( L, valueindex, BOUNDING_VOLUME_COMPONENT_TYPE_TYPE_META );
    };

	setters["collisionModel"] = []( Core::Entity entity, lua_State * L, int valueindex )
    {
        BoundingVolumeComponent *bvc = WGETC<BoundingVolumeComponent>( entity ); 
        bvc->collisionModel = *(Core::BoundingVolumeCollisionModel*)luaL_checkudata( L, valueindex, BOUNDING_VOLUME_COMPONENT_COLLISIONMODEL_TYPE_META );
    };

	// sphere component
	setters["sphereRadius"] = []( Core::Entity entity, lua_State * L, int valueindex )
    {
        BoundingVolumeComponent *bvc = WGETC<BoundingVolumeComponent>( entity ); 
        
        if( lua_isnumber(  L, valueindex ) )
        {
            reinterpret_cast<Core::BoundingSphere*>(bvc->data)->radius = lua_tonumber( L, valueindex );
        }
        else
        {
            luaL_error( L, "Unable to set scale for ent %d, value is not number", entity );
        }
    };

    setters["sphereOffset"] = [](Core::Entity entity, lua_State * L, int valueindex )
    {
        BoundingVolumeComponent *bvc = WGETC<BoundingVolumeComponent>( entity ); 

        if( lua_istable( L, valueindex ) )
        {
            for( int i = 0; i < 4; i++ )
            {
                lua_pushinteger( L, i+1 );
                lua_gettable( L, valueindex );
                    
                if( lua_isnumber( L, -1 ) )
                {
                    reinterpret_cast<Core::BoundingSphere*>(bvc->data)->offset[i] = lua_tonumber( L, -1 );  
                }

                lua_pop( L, 1 );
            }
        }
    };

    return setters;
}


Core::ComponentType Core::BoundingVolumeComponentBinding::GetComponentType()
{
    return EntityHandler::GetComponentType<BoundingVolumeComponent>();
}

const char* Core::BoundingVolumeComponentBinding::GetComponentLuaName()
{
    return "BoundingVolumeComponent";
}
