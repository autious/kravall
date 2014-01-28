#ifndef SRC_CORE_LUA_BRIDGES_COMPONENTBIND_ANIMATIONCOMPONENTBINDING_H
#define SRC_CORE_LUA_BRIDGES_COMPONENTBIND_ANIMATIONCOMPONENTBINDING_H

#include <Components/AnimationComponent.hpp>
#include <ComponentFramework/SystemTypes.hpp>

#include <Lua/Bridges/LuaEntityBridgeTypes.hpp>

#include <Lua/LuaUtility.hpp>

namespace Core
{
    class AnimationComponentBinding
    {
    public:
        static ComponentGetters GetGetters();
        static ComponentSetters GetSetters();    

        static ComponentType GetComponentType();
        static const char* GetComponentLuaName();
    };
}

#endif
