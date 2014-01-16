#ifndef SRC_CORE_LUA_BRIDGES_COMPONENTBIND_FLOWFIELDCOMPONENTBINDING_HPP
#define SRC_CORE_LUA_BRIDGES_COMPONENTBIND_FLOWFIELDCOMPONENTBINDING_HPP

#include <Components/FlowfieldComponent.hpp>
#include <ComponentFramework/SystemTypes.hpp>

#include <Lua/Bridges/LuaEntityBridgeTypes.hpp>

#include <Lua/LuaUtility.hpp>

namespace Core
{
    class FlowfieldComponentBinding
    {
    public:
        static ComponentGetters GetGetters();
        static ComponentSetters GetSetters();    

        static ComponentType GetComponentType();
        static const char* GetComponentLuaName();
    };
}

#endif
