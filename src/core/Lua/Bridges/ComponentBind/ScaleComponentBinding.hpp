#ifndef SRC_CORE_LUA_BRIDGES_COMPONENTBIND_SCALECOMPONENTBINDING_H
#define SRC_CORE_LUA_BRIDGES_COMPONENTBIND_SCALECOMPONENTBINDING_H

#include <Components/ScaleComponent.hpp>
#include <ComponentFramework/SystemTypes.hpp>

#include <Lua/Bridges/LuaEntityBridgeTypes.hpp>

#include <Lua/LuaUtility.hpp>

namespace Core
{
    class ScaleComponentBinding
    {
    public:
        static ComponentGetters GetGetters();
        static ComponentSetters GetSetters();

        static ComponentType GetComponentType();
        static const char* GetComponentLuaName();
    };
}

#endif
