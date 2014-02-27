#include "LuaUnitOrderBridge.hpp"

#include <Lua/LuaUtility.hpp>
#include <Lua/LuaMetatableTypes.hpp>

#include <World.hpp>

extern "C"
{
    static int LuaAttackGroup(lua_State * L)
    {
		//float range						= static_cast<float>(luaL_checknumber( L, 1  ));
		//float graceDistance				= static_cast<float>(luaL_checknumber( L, 2  ));
		//float healthDmg					= static_cast<float>(luaL_checknumber( L, 3  ));
		//float moraleDmg					= static_cast<float>(luaL_checknumber( L, 4  ));
		//float moralDmgOnMiss			= static_cast<float>(luaL_checknumber( L, 5  ));
		//float rageBuff					= static_cast<float>(luaL_checknumber( L, 6  ));
		//float pressureBuff				= static_cast<float>(luaL_checknumber( L, 7  ));
		//float staminacost				= static_cast<float>(luaL_checknumber( L, 8  ));
		//float animationDmgDealingTime	= static_cast<float>(luaL_checknumber( L, 9  ));
		//std::string animationName		= luaL_checkstring( L,					  10 );
		//
		//int weapon = Core::GameData::PushWeaponData( range, graceDistance, healthDmg, moraleDmg, moralDmgOnMiss, rageBuff, pressureBuff, staminacost, animationDmgDealingTime, animationName );
        //lua_pushnumber(L, weapon );
		//
        //return 1;

		return 0;
    }

}

namespace Core
{
    LuaUnitOrderBridge::LuaUnitOrderBridge(lua_State* L)
    {
		int stackpos = lua_gettop( L );

		lua_getglobal( L, "core" );
		int coreTableIndex = lua_gettop( L );

		if( lua_isnil( L, -1 ) == false )
		{
			lua_pushstring( L, "orders" );
			lua_newtable( L ); // new table
			int movementDataTable = lua_gettop( L );

				luau_setfunction( L, "attackGroup", LuaAttackGroup );

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
