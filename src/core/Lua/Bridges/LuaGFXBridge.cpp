#include "LuaGFXBridge.hpp"

#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

#include <gfx/BitmaskDefinitions.hpp>

#include <logger/Logger.hpp>


extern "C"
{

}

static void PushObjectType( lua_State * L, const unsigned int value, const char * name, int table )
{
        lua_pushstring( L, name ); 
        unsigned int* uvalue= (unsigned int*)lua_newuserdata( L, sizeof( unsigned int ) );
        *uvalue = value;

            luaL_newmetatable( L, GFX_OBJECT_TYPE_META );
            lua_setmetatable( L, -2 );
        lua_settable( L, table );
}

void Core::LuaGFXBridge::OpenLibs( lua_State * L )
{
    lua_getglobal( L, "core" );

    int coreTableIndex = lua_gettop( L );

    if( lua_isnil( L, -1 ) == false )
    {
        lua_pushstring( L, "gfx" );
        lua_newtable( L ); //GFX table
        int gfxTable = lua_gettop( L );

            lua_pushstring( L, "objectTypes" );
            lua_newtable( L ); //objectTypes table
                int objectTypeTable = lua_gettop(L);
                PushObjectType( L, GFX::OBJECT_TYPES::OPAQUE_GEOMETRY, "OpaqueGeometry" , objectTypeTable );
                PushObjectType( L, GFX::OBJECT_TYPES::LIGHT, "Light" , objectTypeTable );
                PushObjectType( L, GFX::OBJECT_TYPES::TRANSPARENT_GEOMETRY, "TransparentGeometry" , objectTypeTable );
            lua_settable( L, gfxTable );

        lua_settable( L, coreTableIndex );
    }
    else
    {
        LOG_ERROR << "Missing core table" << std::endl; 
    }
}

