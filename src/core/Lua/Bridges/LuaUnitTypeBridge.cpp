#include "LuaUnitTypeBridge.hpp"

#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

#include <Components/UnitTypeComponent.hpp>

#include <logger/Logger.hpp>
#include <cassert>
#include <Lua/LuaUtility.hpp>
#include <Lua/LuaMetatableTypes.hpp>

extern "C"
{
    static int LuaEq( lua_State * L )
    {
        Core::UnitType* lhs = luau_checkunittype(L, 1);
        Core::UnitType* rhs = luau_checkunittype(L, 2);

        lua_pushboolean(L, *lhs == *rhs);        
        return 1;      
    }
}

static void PushObjectType( lua_State * L, const unsigned int value, const char * name, int table )
{
        lua_pushstring( L, name ); 
        unsigned int* uvalue= (unsigned int*)lua_newuserdata( L, sizeof( Core::UnitType ) );
        *uvalue = value;

            luaL_newmetatable( L, UNIT_TYPE_OBJECT_META_TYPE );
                luau_setfunction( L, "__eq", LuaEq ); 
            lua_setmetatable( L, -2 );
        lua_settable( L, table );
}

Core::LuaUnitTypeComponentBridge::LuaUnitTypeComponentBridge( lua_State * L )
{
	int stackpos = lua_gettop( L );

    lua_getglobal( L, "core" );

    int coreTableIndex = lua_gettop( L );

    if( lua_isnil( L, -1 ) == false )
    {
        lua_pushstring( L, "UnitType" );
        lua_newtable( L ); // new table
        int unitTypeTable = lua_gettop( L );

            PushObjectType( L, Core::UnitType::Object, "Object" , unitTypeTable );
            PushObjectType( L, Core::UnitType::Police, "Police" , unitTypeTable );
			PushObjectType( L, Core::UnitType::Rioter, "Rioter" , unitTypeTable );

        lua_settable( L, coreTableIndex );
    }
    else
    {
        LOG_ERROR << "Missing core table" << std::endl; 
    }

	lua_pop( L, 1 );

	assert( stackpos == lua_gettop( L ) );
}

