#include "RotationComponentBinding.hpp"

#include <World.hpp>

Core::ComponentGetters Core::RotationComponentBinding::GetGetters()
{
    ComponentGetters getters; 
    getters["rotation"] = []( Core::Entity entity, lua_State * L )
    {
        RotationComponent *rc = WGETC<RotationComponent>( entity );
        
        lua_newtable( L );  

        for( int i = 0; i < 4; i++ )
        {
            lua_pushinteger( L, i+1 );
            lua_pushnumber( L, rc->rotation[i] );
            lua_settable( L, -3 );
        }
        
        return 1;
    };

    return getters;
}

Core::ComponentSetters Core::RotationComponentBinding::GetSetters()
{
    ComponentSetters setters;

    setters["rotation"] = [](Core::Entity entity, lua_State * L, int valueindex )
    {
        RotationComponent *rc = WGETC<RotationComponent>( entity );

        if( lua_istable( L, valueindex ) )
        {
            for( int i = 0; i < 4; i++ )
            {
                lua_pushinteger( L, i+1 );
                lua_gettable( L, valueindex );
                    
                if( lua_isnumber( L, -1 ) )
                {
                    rc->rotation[i] = static_cast<float>(lua_tonumber( L, -1 ));  
                }

                lua_pop( L, 1 );
            }
        }
    };

    return setters;
}


Core::ComponentType Core::RotationComponentBinding::GetComponentType()
{
    return EntityHandler::GetComponentType<RotationComponent>();
}

const char* Core::RotationComponentBinding::GetComponentLuaName()
{
    return "RotationComponent";
}
