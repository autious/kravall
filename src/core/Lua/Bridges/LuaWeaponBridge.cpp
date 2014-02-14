#include "LuaWeaponBridge.hpp"

#include <Lua/LuaUtility.hpp>
#include <Lua/LuaMetatableTypes.hpp>

//#include <World.hpp>
#include<GameUtility/GameData.hpp>

extern "C"
{

	static int LuaPushWeaponType(lua_State * L)
    {
		float range						= static_cast<float>(luaL_checknumber( L, 1 ));
		float healthDmg					= static_cast<float>(luaL_checknumber( L, 2 ));
		float moraleDmg					= static_cast<float>(luaL_checknumber( L, 3 ));
		float moralDmgOnMiss			= static_cast<float>(luaL_checknumber( L, 4 ));
		float staminacost				= static_cast<float>(luaL_checknumber( L, 5 ));
		float animationDmgDealingTime	= static_cast<float>(luaL_checknumber( L, 6 ));
		int animationID					= luaL_checkint( L,						  7 );

		int weapon = Core::GameData::PushWeaponData( range, healthDmg, moraleDmg, moralDmgOnMiss, staminacost, animationDmgDealingTime, animationID );
        lua_pushnumber(L, weapon );

        return 1;
    }

}

namespace Core
{
    LuaWeaponBridge::LuaWeaponBridge(lua_State* L)
    {
		int stackpos = lua_gettop( L );

		lua_getglobal( L, "core" );
		int coreTableIndex = lua_gettop( L );

		if( lua_isnil( L, -1 ) == false )
		{
			lua_pushstring( L, "weaponData" );
			lua_newtable( L ); // new table
			int movementDataTable = lua_gettop( L );

				luau_setfunction( L, "pushWeapon", LuaPushWeaponType );

			lua_settable( L, coreTableIndex );
		}
		else
		{
			LOG_ERROR << "Missing core table" << std::endl; 
		}

		lua_pop( L, 1 );

		assert( stackpos == lua_gettop( L ) );
    }

}
