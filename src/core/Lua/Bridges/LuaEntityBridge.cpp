#include "LuaEntityBridge.hpp"

#include <logger/Logger.hpp>
#include <World.hpp>

#include <Lua/Bridges/ComponentBind/WorldPositionComponentBinding.hpp>
#include <Lua/Bridges/ComponentBind/GraphicsComponentBinding.hpp>
#include <Lua/Bridges/ComponentBind/RotationComponentBinding.hpp>
#include <Lua/Bridges/ComponentBind/ScaleComponentBinding.hpp>
#include <Lua/Bridges/ComponentBind/LightComponentBinding.hpp>
#include <Lua/Bridges/ComponentBind/MovementComponentBinding.hpp>
#include <Lua/Bridges/ComponentBind/UnitTypeComponentBinding.hpp>
#include <Lua/Bridges/ComponentBind/AttributeComponentBinding.hpp>
#include <Lua/Bridges/ComponentBind/BoundingVolumeComponentBinding.hpp>
#include <Lua/Bridges/ComponentBind/FlowfieldComponentBinding.hpp>
#include <Lua/Bridges/ComponentBind/AreaComponentBinding.hpp>
#include <Lua/Bridges/ComponentBind/NameComponentBinding.hpp>
#include <Lua/Bridges/ComponentBind/AnimationComponentBinding.hpp>
#include <Lua/Bridges/ComponentBind/TargetingComponentBinding.hpp>
#include <Lua/Bridges/ComponentBind/SquadComponentBinding.hpp>
#include <Lua/Bridges/ComponentBind/FormationComponentBinding.hpp>
#include <Lua/Bridges/ComponentBind/HoverTextComponentBinding.hpp>

#include <cassert>

#include <Lua/LuaUtility.hpp>
#include <Lua/LuaMetatableTypes.hpp>

namespace Core
{
    typedef LuaEntityBridgeTemplate
    <WorldPositionComponentBinding,
     GraphicsComponentBinding,
     RotationComponentBinding,
     ScaleComponentBinding,
     LightComponentBinding,
	 MovementComponentBinding,
	 UnitTypeComponentBinding, 
	 AttributeComponentBinding, 
	 BoundingVolumeComponentBinding,
	 FlowfieldComponentBinding,
     AreaComponentBinding,
     NameComponentBinding,
	 AnimationComponentBinding,
	 TargetingComponentBinding,
     SquadComponentBinding,
     FormationComponentBinding,
     HoverTextComponentBinding> EntityBridge;
}

/*************/

static Core::EntityBridge entityBridge;

extern "C" 
{
    static int LuaEntitySet( lua_State * L )
    {
        LuaEntity * ent = luau_checkentity( L, 1 );
        if( ent->entity == INVALID_ENTITY )
            return luaL_error( L, "entity set on invalid entity" );
        else
            return entityBridge.Set( L );    
    }
    
    static int LuaEntityGet( lua_State * L )
    {

        LuaEntity * ent = luau_checkentity( L, 1 );
        if( ent->entity == INVALID_ENTITY )
            return luaL_error( L, "entity get on invalid entity" );
        else
            return entityBridge.Get( L );
    }

    static int LuaEntityCreate( lua_State * L )
    {
        return entityBridge.CreateEntity( L );    
    }

    static int LuaEntityDestroy( lua_State * L )
    {
        return entityBridge.DestroyEntity( L );    
    }

    static int LuaEntityGC( lua_State * L )
    {
        LuaEntity * ent = luau_checkentity( L, 1 );
          
        if( ent->light == false )
        {
            return LuaEntityDestroy( L );
        }
        else
        {
            LOG_DEBUG << "Ignoring GC on light entity." << std::endl; 
            return 0;
        }
    }

    static int LuaEntityIsValid( lua_State * L )
    {
        return entityBridge.IsValid( L );
    }

    static int LuaEntityToString( lua_State * L )
    {
        return entityBridge.EntityToString( L );
    }
    
    static int LuaEntityIsSameEntity( lua_State * L )
    {
        LuaEntity * e1 = luau_checkentity( L , 1 );
        LuaEntity * e2 = luau_checkentity( L , 2 );

        return (e1->entity == e2->entity) && (e1->entity != INVALID_ENTITY);
    }

    static int LuaComponentTypeToString( lua_State * L )
    {
        return entityBridge.ComponentTypeToString( L ); 
    }

    static int LuaGenerateAspect( lua_State * L )
    {
        Core::Aspect asp = 0ULL;
        LuaAspect* luaAsp = Core::LuaUNewAspect(L);

        int params = lua_gettop(L);
        
        for(int i=1; i <= params; ++i)
        {
            asp |= Core::EntityHandler::GenerateAspect( luau_checkcomponent(L, i));
        }

        luaAsp->aspect = asp;

        return 1;
    }
}

Core::LuaEntityBridge::LuaEntityBridge( lua_State * L  )
{
	int sanity = lua_gettop(L);
    lua_getglobal( L, "core" );  

    int coreTableIndex = lua_gettop( L );

    if( !lua_isnil( L, -1 ) )
    {
        // Bind the set and get functions
        lua_pushstring( L, "entity" );
        lua_newtable( L );
            lua_pushstring( L, "set" );
            lua_pushcfunction( L, LuaEntitySet );
            lua_settable( L, -3 );
        
            lua_pushstring( L, "get" );
            lua_pushcfunction( L, LuaEntityGet );
            lua_settable( L, -3 );

            lua_pushstring( L, "create" );
            lua_pushcfunction( L, LuaEntityCreate );  
            lua_settable( L, -3 );

            lua_pushstring( L, "destroy" );
            lua_pushcfunction( L, LuaEntityDestroy );
            lua_settable( L, -3 );
            
            luau_setfunction( L, "isValid", LuaEntityIsValid );
            luau_setfunction( L, "isSameEntity", LuaEntityIsSameEntity );
            luau_setfunction( L, "generateAspect", LuaGenerateAspect );
        lua_settable( L, -3 );

        if( luaL_newmetatable( L, ENTITY_META_TYPE ) == 0 )
        {
            LOG_ERROR << "Metatable entity already exists!" << std::endl;
        }
            lua_pushstring( L, "__tostring" ); 
            lua_pushcfunction( L, LuaEntityToString );
            lua_settable( L , -3 );

            lua_pushstring( L, "__index" );
            lua_getfield( L, coreTableIndex, "entity" ); 
            lua_settable( L, -3 );
			
			lua_pushstring( L, "__gc" );
            lua_pushcfunction( L, LuaEntityGC );
			lua_settable( L, -3 );
            
            
        lua_pop( L , 1 ); //Pop the entity meta table. remove if something else ends up removing the entity from the stack.

        if( luaL_newmetatable( L, COMPONENT_META_TYPE ) == 0 )
        {
            LOG_ERROR << "Metatable entity already exists!" << std::endl;
        }
            lua_pushstring( L, "__tostring" ); 
            lua_pushcfunction( L, LuaComponentTypeToString );
            lua_settable( L , -3 );
        lua_pop( L , 1 ); //Pop the entity meta table. remove if something else ends up removing the entity from the stack.


        // Set the component types
        lua_pushstring( L, "componentType" );
        lua_newtable( L ); 

        std::array<std::pair<const char*, Core::ComponentType>,EntityBridge::COMPONENT_HANDLER_COUNT> names = entityBridge.GetBinderNameTypes();

        //Sanity check, check for duplicate component types or names.
        //It's an easy mistake to make with all the copy pasting 
        //going on a daily basis.
        for( int i = 0; i < EntityBridge::COMPONENT_HANDLER_COUNT-1; i++  )
        {
            for( int k = i+1; k < EntityBridge::COMPONENT_HANDLER_COUNT; k++  )
            {
                // If you failed on this assert then
                // you've gone done fucked up with the names given to component, 
                // two of them share the same one! 
                assert( strcmp( names[i].first, names[k].first ) != 0 );
                // If you failed on this assert then
                // You've gone done fucked up the id you gave to a componentbinder, 
                // two of them share the same, which shouldn't be possible. 
                assert( names[i].second != names[k].second ); 
            }
             
        }

        for( int i = 0; i < EntityBridge::COMPONENT_HANDLER_COUNT; i++ )
        {
            lua_pushstring( L, names[i].first );
            Core::ComponentType *type = (Core::ComponentType*)lua_newuserdata( L, sizeof( Core::ComponentType ) );
            *type = names[i].second;
            luaL_getmetatable( L, COMPONENT_META_TYPE );
            lua_setmetatable( L, -2 ); 
            lua_settable( L, -3 );
        }

        lua_settable( L, -3 );
    }
    else
    {
        LOG_ERROR << "Missing core table" << std::endl; 
    }

    lua_pop( L, 1 );
	assert(sanity==lua_gettop(L));
}
