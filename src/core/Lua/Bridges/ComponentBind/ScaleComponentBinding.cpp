#include "ScaleComponentBinding.hpp"

#include <World.hpp>

Core::ComponentGetters Core::ScaleComponentBinding::GetGetters()
{
    ComponentGetters getters;

    getters["scale"] = []( Core::Entity entity, lua_State *L )
    {
        ScaleComponent *sc = WGETC<ScaleComponent>( entity ); 

        lua_pushnumber( L, sc->scale );   

        return 1;
    };

    return getters;

}

Core::ComponentSetters Core::ScaleComponentBinding::GetSetters()
{
    ComponentSetters setters;

    setters["scale"] = []( Core::Entity entity, lua_State * L, int valueindex )
    {
        ScaleComponent *sc = WGETC<ScaleComponent>( entity ); 
        
        if( lua_isnumber(  L, valueindex ) )
        {
            sc->scale = static_cast<float>(lua_tonumber( L, valueindex ));
        }
        else
        {
            luaL_error( L, "Unable to set scale for ent %d, value is not number", entity );
        }
    };

    return setters;
}


Core::ComponentType Core::ScaleComponentBinding::GetComponentType()
{
    return EntityHandler::GetComponentType<ScaleComponent>();
}

const char* Core::ScaleComponentBinding::GetComponentLuaName()
{
    return "ScaleComponent";
}
