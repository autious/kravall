#ifndef SRC_CORE_LUA_BRIDGES_COMPONENTBIND_ATTRIBUTECOMPONENTBINDING_HPP
#define SRC_CORE_LUA_BRIDGES_COMPONENTBIND_ATTRIBUTECOMPONENTBINDING_HPP

#include <Components/AttributeComponent.hpp>
#include <ComponentFramework/SystemTypes.hpp>

#include <Lua/Bridges/LuaEntityBridgeTypes.hpp>

#include <Lua/LuaUtility.hpp>

namespace Core
{
    class AttributeComponentBinding
    {
    public:
        static ComponentGetters GetGetters();
        static ComponentSetters GetSetters();    

        static ComponentType GetComponentType();
        static const char* GetComponentLuaName();
    };
}

#endif
