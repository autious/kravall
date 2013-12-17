#ifndef SRC_CORE_LUA_BRIDGES_COMPONENTBIND_ROTATIONCOMPONENTBINDING_H
#define SRC_CORE_LUA_BRIDGES_COMPONENTBIND_ROTATIONCOMPONENTBINDING_H

#include <Components/RotationComponent.hpp>
#include <ComponentFramework/SystemTypes.hpp>

#include <Lua/Bridges/LuaEntityBridgeTypes.hpp>

#include <Lua/LuaUtility.hpp>

namespace Core
{
    class RotationComponentBinding
    {
    public:
        static ComponentGetters GetGetters();
        static ComponentSetters GetSetters();    

        static ComponentType GetComponentType();
        static const char* GetComponentLuaName();
    };
}

#endif
