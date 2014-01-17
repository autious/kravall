#ifndef SRC_CORE_LUA_BRIDGES_COMPONENTBIND_AREACOMPONENTBINDING_H
#define SRC_CORE_LUA_BRIDGES_COMPONENTBIND_AREACOMPONENTBINDING_H

#include <Components/AreaComponent.hpp>
#include <ComponentFramework/SystemTypes.hpp>

#include <Lua/Bridges/LuaEntityBridgeTypes.hpp>

#include <Lua/LuaUtility.hpp>

namespace Core
{
    class AreaComponentBinding
    {
    public:
        static ComponentGetters GetGetters();
        static ComponentSetters GetSetters();    

        static ComponentType GetComponentType();
        static const char* GetComponentLuaName();
    };
}

#endif
