#ifndef SRC_CORE_LUA_BRIDGES_COMPONENTBIND_LIGHTCOMPONENTBINDING_H
#define SRC_CORE_LUA_BRIDGES_COMPONENTBIND_LIGHTCOMPONENTBINDING_H

#include <Components/LightComponent.hpp>
#include <ComponentFramework/SystemTypes.hpp>

#include <Lua/Bridges/LuaEntityBridgeTypes.hpp>

#include <Lua/LuaUtility.hpp>

namespace Core
{
    class LightComponentBinding
    {
    public:
        static ComponentGetters GetGetters();
        static ComponentSetters GetSetters();    

        static ComponentType GetComponentType();
        static const char* GetComponentLuaName();
    };
}

#endif
