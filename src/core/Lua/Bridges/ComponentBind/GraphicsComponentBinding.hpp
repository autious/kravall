#ifndef SRC_CORE_LUA_BRIDGES_COMPONENTBIND_GRAPHICSCOMPONENTBINDING_H
#define SRC_CORE_LUA_BRIDGES_COMPONENTBIND_GRAPHICSCOMPONENTBINDING_H

#include <Components/GraphicsComponent.hpp>
#include <ComponentFramework/SystemTypes.hpp>

#include <Lua/Bridges/LuaEntityBridgeTypes.hpp>

#include <Lua/LuaUtility.hpp>

namespace Core
{
    class GraphicsComponentBinding
    {
    public:
        static ComponentGetters GetGetters();
        static ComponentSetters GetSetters();    

        static ComponentType GetComponentType();
        static const char* GetComponentLuaName();
    };
}

#endif
