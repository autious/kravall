#include "ScaleComponentBinding.hpp"

#include <World.hpp>

Core::ComponentGetters Core::ScaleComponentBinding::GetGetters()
{
    ComponentGetters getters;

    getters["scale"] = []( Core::Entity entity, lua_State *L )
    {
        ScaleComponent *sc = WGETC<ScaleComponent>( entity ); 

        lua_newtable( L );
        for( int i = 0; i < 3; i++ )
        {
            lua_pushnumber( L, sc->scale[i] );
            lua_rawseti( L, -2, i+1 );
        }

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
            sc->scale[0] = luau_tofloat( L, valueindex );
            sc->scale[1] = luau_tofloat( L, valueindex );
            sc->scale[2] = luau_tofloat( L, valueindex );
        }
        else if( lua_istable( L, valueindex ) )
        {
            for( int i = 0; i < 3; i++ )
            {
                lua_rawgeti( L, valueindex, i+1 ); 
                sc->scale[i] = luau_checkfloat( L, valueindex );
                lua_pop( L, 1 );
            }   
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
