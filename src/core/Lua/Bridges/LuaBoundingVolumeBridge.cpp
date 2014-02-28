#include "LuaBoundingVolumeBridge.hpp"

#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

#include <logger/Logger.hpp>
#include <Components/BoundingVolumeComponent.hpp>
#include <cassert>



static void PushCollisionModelObjectType( lua_State * L, const unsigned int value, const char * name, int table )
{
        lua_pushstring( L, name ); 
        unsigned int* uvalue= (unsigned int*)lua_newuserdata( L, sizeof( Core::BoundingVolumeCollisionModel ) );
        *uvalue = value;

            luaL_newmetatable( L, BOUNDING_VOLUME_COMPONENT_COLLISIONMODEL_TYPE_META );
            lua_setmetatable( L, -2 );
        lua_settable( L, table );
}

static void PushTypeObjectType( lua_State * L, const unsigned int value, const char * name, int table )
{
        lua_pushstring( L, name ); 
        unsigned int* uvalue= (unsigned int*)lua_newuserdata( L, sizeof( Core::BoundingVolumeType ) );
        *uvalue = value;

            luaL_newmetatable( L, BOUNDING_VOLUME_COMPONENT_TYPE_TYPE_META );
            lua_setmetatable( L, -2 );
        lua_settable( L, table );
}


Core::LuaBoundingVolumeComponentBridge::LuaBoundingVolumeComponentBridge( lua_State * L )
{
    int stackpos = lua_gettop( L );

    lua_getglobal( L, "core" );

    int coreTableIndex = lua_gettop( L );

    if( lua_isnil( L, -1 ) == false )
    {
        lua_pushstring( L, "BoundingVolumeCollisionModel" );
        lua_newtable( L ); // new table
        int policeStanceTable = lua_gettop( L );

			PushCollisionModelObjectType( L, Core::BoundingVolumeCollisionModel::DynamicResolution, "DynamicResolution" , policeStanceTable );
			PushCollisionModelObjectType( L, Core::BoundingVolumeCollisionModel::NoResolution, "NoResolution" , policeStanceTable );
			PushCollisionModelObjectType( L, Core::BoundingVolumeCollisionModel::StaticResolution, "StaticResolution" , policeStanceTable );
		
			lua_settable( L, coreTableIndex );


		lua_pushstring( L, "BoundingVolumeType" );
        lua_newtable( L ); // new table
        int rioterAlignmentTable = lua_gettop( L );

			PushTypeObjectType( L, Core::BoundingVolumeType::AABBBoundingType, "AABBBoundingType" , rioterAlignmentTable );
			PushTypeObjectType( L, Core::BoundingVolumeType::NoBoundingVolume, "NoBoundingVolume" , rioterAlignmentTable );
			PushTypeObjectType( L, Core::BoundingVolumeType::SphereBoundingType, "SphereBoundingType" , rioterAlignmentTable );
		
			lua_settable( L, coreTableIndex );
    }
    else
    {
        LOG_ERROR << "Missing core table" << std::endl; 
    }

	lua_pop( L, 1 );

	assert( stackpos == lua_gettop( L ) );
}

