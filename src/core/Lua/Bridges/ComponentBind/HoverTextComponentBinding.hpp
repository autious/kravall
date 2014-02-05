#ifndef SRC_CORE_LUA_BRIDGES_COMPONENTBIND_HOVERTEXTCOMPONENTBINDING_H
#define SRC_CORE_LUA_BRIDGES_COMPONENTBIND_HOVERTEXTCOMPONENTBINDING_H

#include <Components/HoverTextComponent.hpp>
#include <ComponentFramework/SystemTypes.hpp>

#include <Lua/Bridges/LuaEntityBridgeTypes.hpp>

#include <Lua/LuaUtility.hpp>

namespace Core
{
    class HoverTextComponentBinding
    {
    public:
        static ComponentGetters GetGetters();
        static ComponentSetters GetSetters();    

        static ComponentType GetComponentType();
        static const char* GetComponentLuaName();

    };
}

#endif
