#include "LuaGameMetaDataBridge.hpp"

#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

#include <Lua/LuaUtility.hpp>
#include <Lua/LuaMetatableTypes.hpp>

#include <GameUtility/GameData.hpp>

#include <logger/Logger.hpp>
#include <cassert>

extern "C"
{
	static int LuaClearGameMetaData( lua_State * L )
    {
        Core::GameData::ClearData();
		return 0;
    }

	static int LuaRegisterEscapePoint( lua_State * L )
	{

		float x = luaL_checknumber( L, 1 );
		float y = luaL_checknumber( L, 2 );
		float z = luaL_checknumber( L, 3 );

		Core::GameData::RegisterEscapePoint( glm::vec3( x, y, z ) );
		return 0;
	}
}


Core::LuaGameMetaDataBridge::LuaGameMetaDataBridge( lua_State * L )
{
	int stackpos = lua_gettop( L );

    lua_getglobal( L, "core" );

    int coreTableIndex = lua_gettop( L );

    if( lua_isnil( L, -1 ) == false )
    {
        lua_pushstring( L, "gameMetaData" );
        lua_newtable( L ); // new table
        int movementDataTable = lua_gettop( L );

			luau_setfunction( L, "clearGameData", LuaClearGameMetaData );
			luau_setfunction( L, "registerEscapePoint", LuaRegisterEscapePoint );

		lua_settable( L, coreTableIndex );
		

    }
    else
    {
        LOG_ERROR << "Missing core table" << std::endl;
    }

	lua_pop( L, 1 );

	assert( stackpos == lua_gettop( L ) );
}