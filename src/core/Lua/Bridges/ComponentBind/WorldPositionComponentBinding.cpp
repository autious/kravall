#include "WorldPositionComponentBinding.hpp"
#include <World.hpp>

#include <cstring>

#include <logger/Logger.hpp>

Core::ComponentGetters Core::WorldPositionComponentBinding::GetGetters()
{
    ComponentGetters getters; 
    getters["position"] = []( Core::Entity entity, lua_State * L )
    {
        WorldPositionComponent *wpc = WGETC<WorldPositionComponent>( entity );
        
        lua_newtable( L );  

        for( int i = 0; i < 3; i++ )
        {
            lua_pushinteger( L, i+1 );
            lua_pushnumber( L, wpc->position[i] );
            lua_settable( L, -3 );
        }
        
        return 1;
    };

    return getters;
}

Core::ComponentSetters Core::WorldPositionComponentBinding::GetSetters()
{
    ComponentSetters setters;

	setters["position"] = [](Core::Entity entity, lua_State * L, int valueindex )
    {
        WorldPositionComponent *wpc = WGETC<WorldPositionComponent>( entity );

        if( lua_istable( L, valueindex ) )
        {
            for( int i = 0; i < 3; i++ )
            {
                lua_pushinteger( L, i+1 );
                lua_gettable( L, valueindex );
                    
                if( lua_isnumber( L, -1 ) )
                {
                    wpc->position[i] = lua_tonumber( L, -1 );  
                }

                lua_pop( L, 1 );
            }
        }
    };

    return setters;
}

Core::ComponentType Core::WorldPositionComponentBinding::GetComponentType()
{
    return EntityHandler::GetComponentType<WorldPositionComponent>();
}

const char* Core::WorldPositionComponentBinding::GetComponentLuaName()
{
    return "WorldPositionComponent";
}
