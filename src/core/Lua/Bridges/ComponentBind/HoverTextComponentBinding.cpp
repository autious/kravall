#include "HoverTextComponentBinding.hpp"

#include <Lua/LuaMetatableTypes.hpp>

#include <SystemDef.hpp>
#include <World.hpp>

namespace Core
{
    ComponentGetters HoverTextComponentBinding::GetGetters()
    {
        ComponentGetters getters; 
        getters["hoverTextId"] = []( Core::Entity entity, lua_State * L )
        {
            LuaHoverText *lht = LuaUNewHoverText( L );
            HoverTextComponent *htc = WGETC<HoverTextComponent>( entity );
            
            lht->hoverTextId = htc->hoverTextId;

            return 1;
        };

        return getters;
    }

    ComponentSetters HoverTextComponentBinding::GetSetters()
    {
        ComponentSetters setters;

        setters["hoverTextId"] = [](Core::Entity entity, lua_State * L, int valueindex )
        {
            LuaHoverText *lht = luau_checkhovertext( L, 1 );
            HoverTextComponent *htc = WGETC<HoverTextComponent>( entity );

            htc->hoverTextId = lht->hoverTextId;
        };

        return setters;
    }

    ComponentType HoverTextComponentBinding::GetComponentType()
    {
        return EntityHandler::GetComponentType<HoverTextComponent>();
    }

    const char* HoverTextComponentBinding::GetComponentLuaName()
    {
        return "HoverTextComponent";
    }
}
