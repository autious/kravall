#ifndef SRC_CORE_LUA_BRIDGES_COMPONENTBIND_TARGETINGCOMPONENT_HPP
#define SRC_CORE_LUA_BRIDGES_COMPONENTBIND_TARGETINGCOMPONENT_HPP

#include <Components/TargetingComponent.hpp>
#include <ComponentFramework/SystemTypes.hpp>

#include <Lua/Bridges/LuaEntityBridgeTypes.hpp>

#include <Lua/LuaUtility.hpp>

namespace Core
{
	class TargetingComponentBinding
	{
	public:
		static ComponentGetters GetGetters();
		static ComponentSetters GetSetters();

		static ComponentType GetComponentType();
		static const char* GetComponentLuaName();
	};
}

#endif
