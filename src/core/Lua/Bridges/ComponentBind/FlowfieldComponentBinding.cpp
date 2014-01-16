#include "FlowfieldComponentBinding.hpp"

#include <World.hpp>

#include <Lua/Bridges/LuaBoundingVolumeBridge.hpp>


Core::ComponentGetters Core::FlowfieldComponentBinding::GetGetters()
{
    ComponentGetters getters; 
    
	getters["node"] = []( Core::Entity entity, lua_State *L )
    {
        FlowfieldComponent *ffc = WGETC<FlowfieldComponent>( entity );

        lua_pushnumber( L, ffc->node );
        return 1;
    };

    return getters;
}

Core::ComponentSetters Core::FlowfieldComponentBinding::GetSetters()
{
    ComponentSetters setters;

	setters["node"] = []( Core::Entity entity, lua_State * L, int valueindex )
    {
        FlowfieldComponent *ffc = WGETC<FlowfieldComponent>( entity );
        
        if( lua_isnumber(  L, valueindex ) )
        {
            ffc->node = static_cast<float>(lua_tonumber( L, valueindex ));
        }
        else
        {
            luaL_error( L, "Unable to set scale for ent %d, value is not number", entity );
        }
    };

    return setters;
}


Core::ComponentType Core::FlowfieldComponentBinding::GetComponentType()
{
    return EntityHandler::GetComponentType<FlowfieldComponent>();
}

const char* Core::FlowfieldComponentBinding::GetComponentLuaName()
{
    return "FlowfieldComponent";
}
