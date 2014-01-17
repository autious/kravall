#include "NameComponentBinding.hpp"

#include <World.hpp>

#include <cstring>

#include <logger/Logger.hpp>

namespace Core
{

    ComponentGetters NameComponentBinding::GetGetters()
    {
        ComponentGetters getters;
    
        getters["name"] = []( Core::Entity entity, lua_State * L )
        {
            NameComponent *nc = WGETC<NameComponent>(entity);

            lua_pushstring( L, nc->name );

            return 1;
        };
        
        return getters;
    }

    ComponentSetters NameComponentBinding::GetSetters()
    {
        ComponentSetters setters;

        setters["name"] = []( Core::Entity entity, lua_State * L, int valueindex )
        {
            NameComponent *nc = WGETC<NameComponent>(entity);

            NameComponent::SetName( *nc, luaL_checkstring( L, valueindex ) );
        };

        return setters;
    }

    ComponentType NameComponentBinding::GetComponentType()
    {
        EntityHandler::GetComponentType<NameComponent>();
    }

    const char* NameComponentBinding::GetComponentLuaName()
    {
        return "NameComponent";
    }
}
