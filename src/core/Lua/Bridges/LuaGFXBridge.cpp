#include "LuaGFXBridge.hpp"

#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

#include <gfx/BitmaskDefinitions.hpp>
#include <GFXInterface.hpp>
#include <logger/Logger.hpp>

#include <sstream>
#include <cassert>

#include <Lua/LuaUtility.hpp>

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

	static int LuaToStringGFXLayerType(lua_State * L)
	{
		Core::GFXLayerType *ot = (Core::GFXLayerType*)luaL_checkudata(L, 1, GFX_LAYER_TYPE_META);

		std::stringstream ss;

		ss << "layerType:" << *ot;

		lua_pushstring(L, ss.str().c_str());

		return 1;
	}

	static int LuaGetGamma(lua_State* L)
	{
		lua_pushnumber(L, GFX::Settings::GetGamma());
		return 1;
	}

	static int LuaSetGamma(lua_State* L)
	{
		float gamma = luaL_checknumber(L, 1);
		GFX::Settings::SetGamma(gamma);
		return 1;
	}

	static int LuaSetLUT(lua_State* L)
	{
		std::string lut = luaL_checkstring(L, 1);
		GFX::ColorSettings::SetLUT(lut.c_str());
		return 1;
	}

	static int LuaSetExposure(lua_State* L)
	{
		float exposure = luaL_checknumber(L, 1);
		GFX::ColorSettings::SetExposure(exposure);
		return 1;
	}

	static int LuaSetWhitepoint(lua_State* L)
	{
		float wpOne = luaL_checknumber(L, 1);
		float wpTwo = luaL_checknumber(L, 2);
		float wpThree = luaL_checknumber(L, 3);
		GFX::ColorSettings::SetWhitePoint(glm::vec3(wpOne, wpTwo, wpThree));
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

static void PushLayerType(lua_State * L, const unsigned int value, const char * name, int table)
{
	lua_pushstring(L, name);
	Core::GFXLayerType* uvalue = (Core::GFXLayerType*)lua_newuserdata(L, sizeof(unsigned int));
	*uvalue = value;
	luaL_newmetatable(L, GFX_LAYER_TYPE_META);
	lua_setmetatable(L, -2);
	lua_settable(L, table);
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

			luau_setfunction(L, "setGamma", LuaSetGamma);
			luau_setfunction(L, "getGamma", LuaGetGamma);
			luau_setfunction(L, "setExposure", LuaSetExposure);
			luau_setfunction(L, "setWhitepoint", LuaSetWhitepoint);
			luau_setfunction(L, "setLUT", LuaSetLUT);

            lua_pushstring( L, "objectTypes" );
            lua_newtable( L ); //objectTypes table
                int objectTypeTable = lua_gettop(L);
                PushObjectType( L, GFX::OBJECT_TYPES::OVERLAY_GEOMETRY, "OverlayGeometry", objectTypeTable );
                PushObjectType( L, GFX::OBJECT_TYPES::OPAQUE_GEOMETRY, "OpaqueGeometry" , objectTypeTable );
				PushObjectType( L, GFX::OBJECT_TYPES::DECAL_GEOMETRY, "DecalGeometry", objectTypeTable);
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

			lua_pushstring(L, "layerTypes");
			lua_newtable(L);
				int layerTypeTable = lua_gettop(L);
				PushLayerType(L, GFX::LAYER_TYPES::MESH_LAYER, "MeshLayer", layerTypeTable);
				PushLayerType(L, GFX::LAYER_TYPES::OUTLINE_LAYER, "OutlineLayer", layerTypeTable);
			lua_settable(L, gfxTable);
                

        lua_settable( L, coreTableIndex );
    }
    else
    {
        LOG_ERROR << "Missing core table" << std::endl; 
    }

    lua_pop( L, 1); //pop core table

    assert( stackSize == lua_gettop( L ) );
}

