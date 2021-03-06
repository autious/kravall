#include "LuaContentManagerBridge.hpp"

#include <World.hpp>

#include <Lua/Bridges/ContentLoadingBind/GnomeLoaderBridge.hpp>
#include <Lua/Bridges/ContentLoadingBind/NavigationMeshBridge.hpp>
#include <Lua/Bridges/ContentLoadingBind/MaterialLoaderBridge.hpp>
#include <Lua/Bridges/ContentLoadingBind/TTFLoaderBridge.hpp>


namespace Core
{
    typedef LuaContentManagerBridgeTemplate<GnomeLoaderBridge, NavigationMeshBridge,MaterialLoaderBridge,TTFLoaderBridge> ContentManagerBridge;
}

extern "C"
{
    static int LuaContentManagerLoad( lua_State *L )
    {
        return Core::ContentManagerBridge::Load( L );
    }

    static int LuaContentManagerFree( lua_State *L )
    {
        return Core::ContentManagerBridge::Free( L );
    }

    static int LuaLoaderToString( lua_State *L )
    {
        return Core::ContentManagerBridge::LoaderToString( L );
    }
    
    static int LuaContentHandleToString( lua_State *L )
    {
        return Core::ContentManagerBridge::ContentHandleToString( L );
    }
}

Core::LuaContentManagerBridge::LuaContentManagerBridge( lua_State * L )
{
    lua_getglobal( L, "core" );  

    int coreTableIndex = lua_gettop( L );

    if( lua_isnil( L, -1 ) == false )
    {
        lua_pushstring( L, "contentmanager" );
        lua_newtable( L );
            lua_pushstring( L, "load" );
            lua_pushcfunction( L, LuaContentManagerLoad );
            lua_settable( L, -3 );
        
            lua_pushstring( L, "free" );
            lua_pushcfunction( L, LuaContentManagerFree );
            lua_settable( L, -3 );
        lua_settable( L, coreTableIndex ); //Set contentmanager table to core
         
        if( luaL_newmetatable( L, CONTENT_LOADER_TYPE_META ) == 0 )
        {
            LOG_ERROR << "Metatable entity already exists!" << std::endl;
        }
            lua_pushstring( L, "__tostring" );
            lua_pushcfunction( L, LuaLoaderToString );
            lua_settable( L, -3 );
        lua_pop( L , 1 ); //Pop the entity meta table. remember to remove if something else ends up removing the entity from the stack.


        if( luaL_newmetatable( L, CONTENT_HANDLE_META ) == 0 )
        {
            LOG_ERROR << "Metatable entity already exists!" << std::endl;
        }
            lua_pushstring( L, "__tostring" );
            lua_pushcfunction( L, LuaContentHandleToString );
            lua_settable( L, -3 );

            lua_pushstring( L, "__index" );
            lua_getfield( L, coreTableIndex, "contentmanager" );
            lua_settable( L, -3 );

            lua_pushstring( L, "__gc" );
            lua_pushcfunction( L, LuaContentManagerFree );
            lua_settable( L, -3 );
        lua_pop( L , 1 ); //Pop the entity meta table. remember to remove if something else ends up removing the entity from the stack.

        //Register all content loaders into the lua environment 
        lua_pushstring( L, "loaders" );
        lua_newtable( L );
        std::array<std::pair<const char*, Core::ContentLoaderBridgeType>,
            ContentManagerBridge::CONTENT_LOAD_BINDER_COUNT> names
            = Core::ContentManagerBridge::GetBinderNameTypes();

        //Sanity check, check for duplicate loader types or names.
        //It's an easy mistake to make with all the copy pasting 
        //going on a daily basis.
        for( int i = 0; i < ContentManagerBridge::CONTENT_LOAD_BINDER_COUNT-1; i++  )
        {
            for( int k = i+1; k < ContentManagerBridge::CONTENT_LOAD_BINDER_COUNT; k++  )
            {
                assert( strcmp( names[i].first, names[k].first ) != 0 );
                assert( names[i].second == names[i].second ); 
            }
        }

        for( int i = 0; i < ContentManagerBridge::CONTENT_LOAD_BINDER_COUNT; i++ )
        {
            lua_pushstring( L, names[i].first );
            Core::ContentLoaderBridgeType *type 
                = (Core::ContentLoaderBridgeType*)lua_newuserdata
                (  
                    L, sizeof( Core::ContentLoaderBridgeType ) 
                );

            *type = names[i].second;
            luaL_getmetatable( L, CONTENT_LOADER_TYPE_META );
            lua_setmetatable( L, -2 ); //set metatable to userdata
            lua_settable( L, -3 );
        }
        lua_settable( L, coreTableIndex );
    }
    else
    {
        LOG_ERROR << __FUNCTION__ << ": Missing core table" << std::endl; 
    }

    lua_pop( L, 1 );
}
