#ifndef SRC_CORE_LUA_BRIDGES_LUARIOTERDATASYSTEMBRIDGE_H
#define SRC_CORE_LUA_BRIDGES_LUARIOTERDATASYSTEMBRIDGE_H

#include <lua.h>

namespace Core
{
	class LuaRioterDataSystemBridge
	{
	public:
        LuaRioterDataSystemBridge( lua_State *L );
	};
}


#endif
