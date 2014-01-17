#include "LuaLevelHeapBridge.hpp"

#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

#include <logger/Logger.hpp>
#include <cassert>

#include <World.hpp>


extern "C"
{
	static int ResetLevelHeap( lua_State * L )
	{
		Core::world.m_levelHeap.Rewind();
		return 1;
	}

	static int LuaNewIndex(lua_State * L)
	{
		return luaL_error(L, "Read only table");
	}
}

Core::LuaLevelHeapBridge::LuaLevelHeapBridge( lua_State * L )
{
	int stackSize = lua_gettop( L );
	lua_getglobal( L, "core" );

	int coreTableIndex = lua_gettop( L );

	if (lua_isnil( L, -1 ) == false )
	{
		lua_pushstring( L, "memory" );

		lua_newtable( L );
			lua_pushstring( L, "clear_level_memory" );
			lua_pushcfunction( L, ResetLevelHeap );
		lua_settable( L, -3 );

			lua_newtable( L );
				lua_pushstring( L, "__newindex" );
				lua_pushcfunction(L, LuaNewIndex );
				lua_settable( L, - 3 );
		lua_setmetatable( L, -2 );

		lua_settable( L, -3 );
	}
	else
	{
		LOG_ERROR << "Missing core table" << std::endl;
	}

	lua_pop(L, 1); //pop core table

	assert(stackSize == lua_gettop(L));
}

