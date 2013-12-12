#include "LuaEntityBridge.hpp"

#include <logger/Logger.hpp>

#include <Lua/Bridges/ComponentBind/WorldPositionComponentBinding.hpp>

namespace Core
{
    typedef LuaEntityBridgeTemplate<WorldPositionComponentBinding> EntityBridge;
}

/*************/

static Core::EntityBridge entityBridge;

extern "C" 
{

    static int LuaEntitySet( lua_State * L )
    {
        return entityBridge.Set( L );    
    }

    
    static int LuaEntityGet( lua_State * L )
    {
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
}

void Core::LuaEntityBridge::OpenLibs( lua_State * L )
{

    if( luaL_newmetatable( L, ENTITY_META_TYPE ) == 0 )
    {
        LOG_ERROR << "Metatable entity already exists!" << std::endl;
    }
    lua_pop( L , 1 ); //Pop the entity meta table. remove if something else ends up removing the entity from the stack.

    if( luaL_newmetatable( L, COMPONENT_META_TYPE ) == 0 )
    {
        LOG_ERROR << "Metatable entity already exists!" << std::endl;
    }

    lua_pop( L , 1 ); //Pop the entity meta table. remove if something else ends up removing the entity from the stack.

    lua_getglobal( L, "core" );  

    if( lua_isnil( L, -1 ) == false )
    {
        // Set the component types
        lua_pushstring( L, "componentType" );
        lua_newtable( L ); 

        std::array<std::pair<const char*, Core::ComponentType>,EntityBridge::COMPONENT_HANDLER_COUNT> names = entityBridge.GetBinderNameTypes();

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
        lua_settable( L, -3 );

    }
    else
    {
        LOG_ERROR << "Missing core table" << std::endl; 
    }

    lua_pop( L, 1 );
}
