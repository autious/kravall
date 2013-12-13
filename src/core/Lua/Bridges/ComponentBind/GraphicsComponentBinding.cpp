#include "GraphicsComponentBinding.hpp"
#include <World.hpp>

#include <cstring>

#include <logger/Logger.hpp>

Core::ComponentGetters Core::GraphicsComponentBinding::GetGetters()
{
    ComponentGetters getters; 
    getters["mesh"] = []( Core::Entity entity, lua_State * L )
    {
        GraphicsComponent *gc = WGETC<GraphicsComponent>( entity );
        
        lua_pushnumber( L, GFX::GetBitmaskValue( gc->bitmask, GFX::BT_MESH_ID ) );
        
        return 1;
    };

    return getters;
}

Core::ComponentSetters Core::GraphicsComponentBinding::GetSetters()
{
    ComponentSetters setters;

    setters["mesh"] = [](Core::Entity entity, lua_State * L, int valueindex )
    {
        GraphicsComponent *gc = WGETC<GraphicsComponent>( entity );

        if( lua_isnumber( L, valueindex ) )
        {
            GFX::SetBitmaskValue( gc->bitmask, GFX::BT_MESH_ID, lua_tointeger( L, valueindex ) );
        }  
        else
        {
            luaL_error( L, "Unable to set mesh, given parameter is not integer value" );
        }
    };

    return setters;
}

Core::ComponentType Core::GraphicsComponentBinding::GetComponentType()
{
    return EntityHandler::GetComponentType<GraphicsComponent>();
}

const char* Core::GraphicsComponentBinding::GetComponentLuaName()
{
    return "GraphicsComponent";
}
