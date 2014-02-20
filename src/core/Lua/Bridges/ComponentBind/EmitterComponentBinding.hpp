#ifndef CORE_LUA_BRIDGES_COMPONENT_BIND_EMITTER_COMPONENT_BINDING_HPP
#define CORE_LUA_BRIDGES_COMPONENT_BIND_EMITTER_COMPONENT_BINDING_HPP

#include <Lua/Bridges/LuaEntityBridgeTypes.hpp>

namespace Core
{
    class EmitterComponentBinding
    {
    public:
        static ComponentGetters GetGetters();
        static ComponentSetters GetSetters();

        static ComponentType GetComponentType();
        static const char* GetComponentLuaName();
    };
}

#endif
