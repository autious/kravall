#ifndef SRC_CORE_LUA_BRIDGES_LUA_NAVMESH_BRIDGE_HPP
#define SRC_CORE_LUA_BRIDGES_LUA_NAVMESH_BRIDGE_HPP

#include <lua.h>

namespace Core
{
	class LuaNavMeshBridge
	{
	public:
		static void OpenLibs(lua_State * L);
	};
}

#endif
