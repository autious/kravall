#ifndef SRC_CORE_LUA_BRIDGES_LUA_GROUPS_BRIDGE_HPP
#define SRC_CORE_LUA_BRIDGES_LUA_GROUPS_BRIDGE_HPP

#include <lua.h>

namespace Core
{
	class LuaGroupsBridge
	{
	public:
        LuaGroupsBridge( lua_State *L );
	};
}

#endif
