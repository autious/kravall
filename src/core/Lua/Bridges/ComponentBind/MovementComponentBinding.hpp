#ifndef SRC_CORE_LUA_BRIDGES_COMPONENTBIND_MOVEMENTCOMPONENT_HPP
#define SRC_CORE_LUA_BRIDGES_COMPONENTBIND_MOVEMENTCOMPONENT_HPP

#include <Components/MovementComponent.hpp>
#include <ComponentFramework/SystemTypes.hpp>

#include <Lua/Bridges/LuaEntityBridgeTypes.hpp>

#include <Lua/LuaUtility.hpp>

namespace Core
{
    class MovementComponentBinding
    {
    public:
        static ComponentGetters GetGetters();
        static ComponentSetters GetSetters();

        static ComponentType GetComponentType();
        static const char* GetComponentLuaName();
    };
}

#endif
