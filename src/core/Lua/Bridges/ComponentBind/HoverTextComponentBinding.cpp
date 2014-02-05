#include "HoverTextComponentBinding.hpp"

#include <SystemDef.hpp>

namespace Core
{
    ComponentGetters HoverTextComponentBinding::GetGetters()
    {
        ComponentGetters getters; 
        getters["hoverTextId"] = []( Core::Entity entity, lua_State * L )
        {
            HoverTextComponent *htc = WGETC<HoverTextComponent>( entity );

            return 1;
        };

        return getters;

    }

    HoverTextComponentBinding::ComponentSetters GetSetters()
    {
        ComponentSetters setters;

        setters["hoverTextId"] = [](Core::Entity entity, lua_State * L, int valueindex )
        {
            HoverTextComponent *htc = WGETC<HoverTextComponent>( entity );
        };

        return setters;
    }

    HoverTextComponentBinding::ComponentType GetComponentType()
    {
        return EntityHandler::GetComponentType<HoverTextComponent>();
    }

    const char* HoverTextComponentBinding::GetComponentLuaName()
    {
        return "HoverTextComponent";
    }
}

#endif
