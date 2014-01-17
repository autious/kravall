#ifndef SRC_CORE_LUA_BRIDGES_COMPONENTBIND_NAMECOMPONENTBINDING_H
#define SRC_CORE_LUA_BRIDGES_COMPONENTBIND_NAMECOMPONENTBINDING_H

#include <Components/NameComponent.hpp>
#include <ComponentFramework/SystemTypes.hpp>

#include <Lua/Bridges/LuaEntityBridgeTypes.hpp>

#include <Lua/LuaUtility.hpp>

namespace Core
{
    class NameComponentBinding
    {
    public:
        static ComponentGetters GetGetters();
        static ComponentSetters GetSetters();    

        static ComponentType GetComponentType();
        static const char* GetComponentLuaName();
    };
}


#endif
