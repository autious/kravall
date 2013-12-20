#ifndef SRC_CORE_LUA_BRIDGES_COMPONENTBIND_UNITTYPECOMPONENTBINDING_HPP
#define SRC_CORE_LUA_BRIDGES_COMPONENTBIND_UNITTYPECOMPONENTBINDING_HPP

#include <Components/UnitTypeComponent.hpp>
#include <ComponentFramework/SystemTypes.hpp>

#include <Lua/Bridges/LuaEntityBridgeTypes.hpp>

#include <Lua/LuaUtility.hpp>

namespace Core
{
    class UnitTypeComponentBinding
    {
    public:
        static ComponentGetters GetGetters();
        static ComponentSetters GetSetters();

        static ComponentType GetComponentType();
        static const char* GetComponentLuaName();
    };
}

#endif
