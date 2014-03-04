#include "LuaMovementMetaDataBridge.hpp"

#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

#include <Lua/LuaUtility.hpp>
#include <Lua/LuaMetatableTypes.hpp>

#include <GameUtility/GameData.hpp>
#include <Components/MovementComponent.hpp>

#include <logger/Logger.hpp>
#include <cassert>


static void PushObjectType( lua_State * L, const unsigned int value, const char * name, int table )
{
        lua_pushstring( L, name ); 
        unsigned int* uvalue= (unsigned int*)lua_newuserdata( L, sizeof( Core::MovementState ) );
        *uvalue = value;
		
			luaL_newmetatable( L, UNIT_MOVEMENT_META_DATA_TYPE_META );
			lua_setmetatable( L, -2 );
		
        lua_settable( L, table );
}

Core::LuaMovementMetaDataBridge::LuaMovementMetaDataBridge( lua_State * L )
{
	int stackpos = lua_gettop( L );
	
    lua_getglobal( L, "core" );
	
    int coreTableIndex = lua_gettop( L );
	
    if( lua_isnil( L, -1 ) == false )
    {
        lua_pushstring( L, "movementData" );
        lua_newtable( L ); // new table
        int movementDataTable = lua_gettop( L );
	
            PushObjectType( L, Core::MovementState::Movement_Walking, "Walking",		movementDataTable );
			PushObjectType( L, Core::MovementState::Movement_Jogging, "Jogging",	    movementDataTable );
			PushObjectType( L, Core::MovementState::Movement_Sprinting, "Sprinting",	movementDataTable );
	
		lua_settable( L, coreTableIndex );
			
	
    }
    else
    {
        LOG_ERROR << "Missing core table" << std::endl; 
    }
	
	lua_pop( L, 1 );
	
	assert( stackpos == lua_gettop( L ) );
}

