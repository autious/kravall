#include "LuaGFXBridge.hpp"

#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

#include <gfx/BitmaskDefinitions.hpp>

#include <logger/Logger.hpp>

#include <sstream>
#include <cassert>

extern "C"
{
    static int LuaToStringGFXObjectType( lua_State * L )
    {
        Core::GFXObjectType *ot = (Core::GFXObjectType*)luaL_checkudata( L, 1, GFX_OBJECT_TYPE_META );
        
        std::stringstream ss;

        ss << "objectType:" <<  *ot;
        
        lua_pushstring( L, ss.str().c_str() );  
    
        return 1;
    }

    static int LuaToStringGFXLightType( lua_State * L )
    {
        Core::GFXLightType *ot = (Core::GFXLightType*)luaL_checkudata( L, 1, GFX_LIGHT_TYPE_META );
       
        std::stringstream ss;

        ss << "lightType:" <<  *ot;
        
        lua_pushstring( L, ss.str().c_str() );  

        return 1;
    }
}

static void PushObjectType( lua_State * L, const unsigned int value, const char * name, int table )
{
        lua_pushstring( L, name ); 
        Core::GFXObjectType* uvalue= (Core::GFXObjectType*)lua_newuserdata( L, sizeof( unsigned int ) );
        *uvalue = value;
            luaL_newmetatable( L, GFX_OBJECT_TYPE_META );
            lua_setmetatable( L, -2 );
        lua_settable( L, table );
}

static void PushLightType( lua_State * L, const unsigned int value, const char * name, int table )
{
        lua_pushstring( L, name ); 
        Core::GFXLightType* uvalue= (Core::GFXLightType*)lua_newuserdata( L, sizeof( unsigned int ) );
        *uvalue = value;
            luaL_newmetatable( L, GFX_LIGHT_TYPE_META );
            lua_setmetatable( L, -2 );
        lua_settable( L, table );
}

Core::LuaGFXBridge::LuaGFXBridge( lua_State * L )
{
    int stackSize = lua_gettop( L );
    lua_getglobal( L, "core" );

    int coreTableIndex = lua_gettop( L );

    if( lua_isnil( L, -1 ) == false )
    {
        luaL_newmetatable( L, GFX_OBJECT_TYPE_META );
            lua_pushstring( L, "__tostring" );
            lua_pushcfunction( L, LuaToStringGFXObjectType );
            lua_settable( L, -3 );
        lua_pop( L, 1 );
    
        luaL_newmetatable( L, GFX_LIGHT_TYPE_META );
            lua_pushstring( L, "__tostring" );
            lua_pushcfunction( L, LuaToStringGFXLightType );
            lua_settable( L, -3 );
        lua_pop( L, 1 );
        
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

            lua_pushstring( L, "lightTypes" );
            lua_newtable( L ) ;
                int lightTypeTable = lua_gettop(L);
                PushLightType( L, GFX::LIGHT_TYPES::POINT_SHADOW, "PointShadow" , lightTypeTable );
                PushLightType( L, GFX::LIGHT_TYPES::SPOT_SHADOW, "SpotShadow" , lightTypeTable );
                PushLightType( L, GFX::LIGHT_TYPES::DIR_SHADOW, "DirShadow" , lightTypeTable );
                PushLightType( L, GFX::LIGHT_TYPES::POINT, "Point" , lightTypeTable );
                PushLightType( L, GFX::LIGHT_TYPES::SPOT, "Spot" , lightTypeTable );
                PushLightType( L, GFX::LIGHT_TYPES::DIR, "Dir" , lightTypeTable );
                PushLightType( L, GFX::LIGHT_TYPES::AMBIENT, "Ambient" , lightTypeTable );
            lua_settable( L, gfxTable );
                

        lua_settable( L, coreTableIndex );
    }
    else
    {
        LOG_ERROR << "Missing core table" << std::endl; 
    }

    lua_pop( L, 1); //pop core table

    assert( stackSize == lua_gettop( L ) );
}

