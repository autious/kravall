#include "LuaUnitTypeBridge.hpp"

#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

#include <Components/UnitTypeComponent.hpp>

#include <logger/Logger.hpp>
#include <cassert>

extern "C"
{

}

static void PushObjectType( lua_State * L, const unsigned int value, const char * name, int table )
{
        lua_pushstring( L, name ); 
        unsigned int* uvalue= (unsigned int*)lua_newuserdata( L, sizeof( Core::UnitType ) );
        *uvalue = value;

            luaL_newmetatable( L, UNIT_TYPE_OBJECT_TYPE_META );
            lua_setmetatable( L, -2 );
        lua_settable( L, table );
}

void Core::LuaUnitTypeComponentBridge::OpenLibs( lua_State * L )
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

