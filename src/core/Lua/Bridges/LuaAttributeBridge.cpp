#include "LuaAttributeBridge.hpp"

#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

#include <Components/AttributeComponent.hpp>

#include <logger/Logger.hpp>
#include <cassert>


extern "C"
{

}

static void PushPoliceObjectType( lua_State * L, const unsigned int value, const char * name, int table )
{
        lua_pushstring( L, name ); 
        unsigned int* uvalue= (unsigned int*)lua_newuserdata( L, sizeof( Core::PoliceStance ) );
        *uvalue = value;

            luaL_newmetatable( L, ATTRIBUTE_POLICE_COMPONENT_TYPE_META );
            lua_setmetatable( L, -2 );
        lua_settable( L, table );
}

static void PushRioterObjectType( lua_State * L, const unsigned int value, const char * name, int table )
{
        lua_pushstring( L, name );
        unsigned int* uvalue= (unsigned int*)lua_newuserdata( L, sizeof( Core::PoliceStance ) );
        *uvalue = value;

            luaL_newmetatable( L, ATTRIBUTE_RIOTER_COMPONENT_TYPE_META );
            lua_setmetatable( L, -2 );
        lua_settable( L, table );
}

void Core::LuaAttributeComponentBridge::OpenLibs( lua_State * L )
{
	int stackpos = lua_gettop( L );

    lua_getglobal( L, "core" );

    int coreTableIndex = lua_gettop( L );

    if( lua_isnil( L, -1 ) == false )
    {
        lua_pushstring( L, "PoliceStance" );
        lua_newtable( L ); // new table
        int policeStanceTable = lua_gettop( L );

			PushPoliceObjectType( L, Core::PoliceStance::Passive, "Passive" , policeStanceTable );
			PushPoliceObjectType( L, Core::PoliceStance::Defensive, "Defensive" , policeStanceTable );
			PushPoliceObjectType( L, Core::PoliceStance::Aggressive, "Aggressive" , policeStanceTable );
		
			lua_settable( L, coreTableIndex );


		lua_pushstring( L, "RioterAlignment" );
        lua_newtable( L ); // new table
        int rioterAlignmentTable = lua_gettop( L );

			PushRioterObjectType( L, Core::RioterAlignment::Anarchist, "Anarchist" , rioterAlignmentTable );
			PushRioterObjectType( L, Core::RioterAlignment::Pacifist, "Pacifist" , rioterAlignmentTable );
		
			lua_settable( L, coreTableIndex );

        //lua_settable( L, coreTableIndex );
    }
    else
    {
        LOG_ERROR << "Missing core table" << std::endl; 
    }

	lua_pop( L, 1 );

	assert( stackpos == lua_gettop( L ) );
}

