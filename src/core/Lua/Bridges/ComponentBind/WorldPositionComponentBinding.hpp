#ifndef SRC_CORE_LUA_BRIDGES_COMPONENTGETSET_WORLDPOSITIONCOMPONENTBINDING_H
#define SRC_CORE_LUA_BRIDGES_COMPONENTGETSET_WORLDPOSITIONCOMPONENTBINDING_H

#include <Components/WorldPositionComponent.hpp>
#include <ComponentFramework/SystemTypes.hpp>

#include <Lua/Bridges/LuaEntityBridgeTypes.hpp>

#include <Lua/LuaUtility.hpp>

namespace Core
{
    class WorldPositionComponentBinding
    {
    public:
        static ComponentGetters GetGetters();
        static ComponentSetters GetSetters();    

        static ComponentType GetComponentType();
        static const char* GetComponentLuaName();
    };
}

#endif
