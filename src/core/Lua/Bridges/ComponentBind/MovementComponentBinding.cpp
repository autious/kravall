#include "MovementComponentBinding.hpp"

#include <World.hpp>

Core::ComponentGetters Core::MovementComponentBinding::GetGetters()
{
    ComponentGetters getters;

    getters["direction"] = []( Core::Entity entity, lua_State * L )
    {
        MovementComponent *mvc = WGETC<MovementComponent>( entity );
        
        lua_newtable( L );  

        for( int i = 0; i < 3; i++ )
        {
            lua_pushinteger( L, i+1 );
            lua_pushnumber( L, mvc->direction[i] );
            lua_settable( L, -3 );
        }
        
        return 1;
    };

    getters["speed"] = []( Core::Entity entity, lua_State *L )
    {
        MovementComponent *mvc = WGETC<MovementComponent>( entity );

        lua_pushnumber( L, mvc->speed );

        return 1;
    };

	getters["maxSpeed"] = []( Core::Entity entity, lua_State *L )
    {
        MovementComponent *mvc = WGETC<MovementComponent>( entity );

        lua_pushnumber( L, mvc->maxSpeed );

        return 1;
    };

	getters["goal"] = []( Core::Entity entity, lua_State * L )
    {
        MovementComponent *mvc = WGETC<MovementComponent>( entity );
        
        lua_newtable( L );  

        for( int i = 0; i < 3; i++ )
        {
            lua_pushinteger( L, i+1 );
            lua_pushnumber( L, mvc->goal[i] );
            lua_settable( L, -3 );
        }
        
        return 1;
    };

    return getters;

}

Core::ComponentSetters Core::MovementComponentBinding::GetSetters()
{
    ComponentSetters setters;

    setters["direction"] = [](Core::Entity entity, lua_State * L, int valueindex )
    {
        MovementComponent *mvc = WGETC<MovementComponent>( entity );

        if( lua_istable( L, valueindex ) )
        {
            for( int i = 0; i < 3; i++ )
            {
                lua_pushinteger( L, i+1 );
                lua_gettable( L, valueindex );
                    
                if( lua_isnumber( L, -1 ) )
                {
                    mvc->direction[i] = static_cast<float>(lua_tonumber( L, -1 ));  
                }

                lua_pop( L, 1 );
            }
        }
    };

	setters["speed"] = []( Core::Entity entity, lua_State * L, int valueindex )
    {
        MovementComponent *mvc = WGETC<MovementComponent>( entity );
        
        if( lua_isnumber(  L, valueindex ) )
        {
            mvc->speed = static_cast<float>(lua_tonumber( L, valueindex ));
        }
        else
        {
            luaL_error( L, "Unable to set speed for ent %d, value is not number", entity );
        }
    };

	setters["maxSpeed"] = []( Core::Entity entity, lua_State * L, int valueindex )
    {
        MovementComponent *mvc = WGETC<MovementComponent>( entity );
        
        if( lua_isnumber(  L, valueindex ) )
        {
            mvc->maxSpeed = static_cast<float>(lua_tonumber( L, valueindex ));
        }
        else
        {
            luaL_error( L, "Unable to set maxSpeed for ent %d, value is not number", entity );
        }
    };

	setters["goal"] = [](Core::Entity entity, lua_State * L, int valueindex )
    {
        MovementComponent *mvc = WGETC<MovementComponent>( entity );

        if( lua_istable( L, valueindex ) )
        {
            for( int i = 0; i < 3; i++ )
            {
                lua_pushinteger( L, i+1 );
                lua_gettable( L, valueindex );
                    
                if( lua_isnumber( L, -1 ) )
                {
                    mvc->goal[i] = lua_tonumber( L, -1 );  
                }

                lua_pop( L, 1 );
            }
        }
		else if( lua_isboolean( L, -1 ) )
		{
			mvc->goal[0] = FLT_MAX;
			return;
		}
    };

    return setters;
}


Core::ComponentType Core::MovementComponentBinding::GetComponentType()
{
    return EntityHandler::GetComponentType<MovementComponent>();
}

const char* Core::MovementComponentBinding::GetComponentLuaName()
{
    return "MovementComponent";
}
