#include "AreaComponentBinding.hpp"

#include <World.hpp>
#include <logger/Logger.hpp>
#include <Lua/Bridges/LuaGFXBridge.hpp>

namespace Core
{
    ComponentGetters AreaComponentBinding::GetGetters()
    {
        ComponentGetters getters;

        getters["vertices"] = []( Core::Entity entity, lua_State * L )
        {
            AreaComponent * ac = WGETC<AreaComponent>(entity);
            lua_newtable( L );
            for( int i = 0; i < AreaComponent::DATA_COUNT; i++ )
            {
                lua_pushnumber( L, ac->vertices[i] );
                lua_rawseti( L, -2, i+1 );
            }
            return 1;
        };

        return getters;
    }

    ComponentSetters AreaComponentBinding::GetSetters()
    {
        ComponentSetters setters;

        setters["vertices"] = []( Core::Entity entity, lua_State *L, int valueindex )
        {
            AreaComponent * ac = WGETC<AreaComponent>(entity);
            if( lua_istable( L, valueindex ) )
            {
                for( int i = 0; i < AreaComponent::DATA_COUNT; i++ )
                {
                    lua_rawgeti( L, valueindex, i+1 );
                    ac->vertices[i] = luau_checkfloat( L, -1 );        
                    lua_pop( L, 1 );
                }
            }
            else
            {
                luaL_error( L, "Invalid vertices element" );
            }
        };

        return setters;
    }

    ComponentType AreaComponentBinding::GetComponentType()
    {
        return EntityHandler::GetComponentType<AreaComponent>();
    }

    const char* AreaComponentBinding::GetComponentLuaName()
    {
        return "AreaComponent";
    }
}
