#ifndef SRC_CORE_LUA_BRIDGES_COMPONENTBIND_BOUNDINGVOLUMECOMPONENTBINDING_HPP
#define SRC_CORE_LUA_BRIDGES_COMPONENTBIND_BOUNDINGVOLUMECOMPONENTBINDING_HPP

#include <Components/BoundingVolumeComponent.hpp>
#include <ComponentFramework/SystemTypes.hpp>

#include <Lua/Bridges/LuaEntityBridgeTypes.hpp>

#include <Lua/LuaUtility.hpp>

namespace Core
{
    class BoundingVolumeComponentBinding
    {
    public:
        static ComponentGetters GetGetters();
        static ComponentSetters GetSetters();    

        static ComponentType GetComponentType();
        static const char* GetComponentLuaName();
    };
}

#endif
