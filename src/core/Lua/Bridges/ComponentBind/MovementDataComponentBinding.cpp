#include "MovementDataComponentBinding.hpp"

#include <World.hpp>

Core::ComponentGetters Core::MovementDataComponentBinding::GetGetters()
{
    ComponentGetters getters;

	getters["prevDt"] = []( Core::Entity entity, lua_State * L )
    {
        MovementDataComponent *mdc = WGETC<MovementDataComponent>( entity );
        
        lua_newtable( L );  

        lua_pushnumber( L, mdc->movedThisFrame );
        
        return 1;
    };

	getters["prevPos"] = []( Core::Entity entity, lua_State * L )
    {
        MovementDataComponent *mdc = WGETC<MovementDataComponent>( entity );
        
        lua_newtable( L );  

        for( int i = 0; i < 3; i++ )
        {
            lua_pushinteger( L, i+1 );
            lua_pushnumber( L, mdc->prevPos[i] );
            lua_settable( L, -3 );
        }
        
        return 1;
    };

	getters["oldPFDir"] = []( Core::Entity entity, lua_State * L )
    {
        MovementDataComponent *mdc = WGETC<MovementDataComponent>( entity );
        
        lua_newtable( L );  

        for( int i = 0; i < 3; i++ )
        {
            lua_pushinteger( L, i+1 );
            lua_pushnumber( L, mdc->oldPFDir[i] );
            lua_settable( L, -3 );
        }
        
        return 1;
    };

	getters["movedThisFrame"] = []( Core::Entity entity, lua_State * L )
    {
        MovementDataComponent *mdc = WGETC<MovementDataComponent>( entity );
        
        lua_newtable( L );  

        lua_pushboolean( L, mdc->movedThisFrame );
        
        return 1;
    };

    return getters;

}

Core::ComponentSetters Core::MovementDataComponentBinding::GetSetters()
{
    ComponentSetters setters;

	setters["prevPos"] = [](Core::Entity entity, lua_State * L, int valueindex)
	{
		MovementDataComponent *mdc = WGETC<MovementDataComponent>( entity );

		if (lua_istable(L, valueindex))
		{
			for (int i = 0; i < 3; i++)
			{
				lua_pushinteger(L, i + 1);
				lua_gettable(L, valueindex);

				if (lua_isnumber(L, -1))
				{
					mdc->prevPos[i] = static_cast<float>(lua_tonumber(L, -1));
				}

				lua_pop(L, 1);
			}
		}
	};

	setters["oldPFDir"] = [](Core::Entity entity, lua_State * L, int valueindex)
	{
		MovementDataComponent *mdc = WGETC<MovementDataComponent>( entity );

		if (lua_istable(L, valueindex))
		{
			for (int i = 0; i < 3; i++)
			{
				lua_pushinteger(L, i + 1);
				lua_gettable(L, valueindex);

				if (lua_isnumber(L, -1))
				{
					mdc->oldPFDir[i] = static_cast<float>(lua_tonumber(L, -1));
				}

				lua_pop(L, 1);
			}
		}
	};

	setters["prevDt"] = []( Core::Entity entity, lua_State * L, int valueindex )
    {
        MovementDataComponent *mdc = WGETC<MovementDataComponent>( entity );
        
        if( lua_isnumber(  L, valueindex ) )
        {
            mdc->prevDt = static_cast<float>(lua_tonumber( L, valueindex ));
        }
        else
        {
            luaL_error( L, "Unable to set speed for ent %d, value is not number", entity );
        }
    };

	setters["movedThisFrame"] = []( Core::Entity entity, lua_State * L, int valueindex )
    {
        MovementDataComponent *mdc = WGETC<MovementDataComponent>( entity );
        
        if( lua_isboolean(  L, valueindex ) )
        {
            mdc->movedThisFrame = static_cast<bool>(lua_toboolean( L, valueindex ));
        }
        else
        {
            luaL_error( L, "Unable to set speed for ent %d, value is not number", entity );
        }
    };

    return setters;
}


Core::ComponentType Core::MovementDataComponentBinding::GetComponentType()
{
    return EntityHandler::GetComponentType<MovementDataComponent>();
}

const char* Core::MovementDataComponentBinding::GetComponentLuaName()
{
    return "MovementDataComponent";
}
