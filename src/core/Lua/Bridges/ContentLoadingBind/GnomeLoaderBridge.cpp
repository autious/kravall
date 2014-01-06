#include "GnomeLoaderBridge.hpp"

#include <ContentManagement/ContentManager.hpp>
#include <Lua/Bridges/ContentLoading/ContentHandle.hpp>
#include <World.hpp>

#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>


int Core::GnomeLoaderBridge::Load( lua_State * L )
{
    //Push the callback to the registry for later callback
    lua_pushvalue( L, 3 );
    int callbackRegIndex = luaL_ref( L, LUA_REGISTRYINDEX );

    Core::world.m_contentManager.Load<Core::GnomeLoader>(lua_tostring(L,2), [callbackRegIndex]
    (Core::BaseAssetLoader* baseLoader, Core::AssetHandle handle)
    {
        Core::GnomeLoader* gnomeLoader = dynamic_cast<Core::GnomeLoader*>(baseLoader);
        const Core::ModelData* data = gnomeLoader->getData(handle);
        
        lua_State * L = Core::world.m_luaState.GetState();
        lua_rawgeti( L, LUA_REGISTRYINDEX, callbackRegIndex );
        lua_pushinteger( L, data->meshID );
        int error = lua_pcall(L, 1,0,0 );

        if( error )
        {
            LOG_ERROR << __FUNCTION__ << "Unable to callback from GnomeLoader:" << lua_tostring( L, -1 ) << std::endl;

            //Pop the error.
            lua_pop(L,1 );
        }

        luaL_unref( L, LUA_REGISTRYINDEX, callbackRegIndex );
    }, lua_toboolean( L, 4 ) );

    ContentHandle *contentHandle = (ContentHandle*)lua_newuserdata( L, sizeof( ContentHandle ) );
    luaL_getmetatable( L, CONTENT_HANDLE_META );
    lua_setmetatable( L, -2 );
     
    contentHandle->assetHash = Core::world.m_contentManager.GetHash<Core::GnomeLoader>( lua_tostring(L,2) );
    contentHandle->loaderId = *(ContentLoaderBridgeType*)luaL_checkudata(L,1,CONTENT_LOADER_TYPE_META );

    return 1;
}

int Core::GnomeLoaderBridge::Free( const AssetHash assetHash, lua_State * L )
{
    Core::world.m_contentManager.Free<Core::GnomeLoader>( assetHash );
    return 0;
}

const char * Core::GnomeLoaderBridge::GetLoaderLuaName()
{
    return "GnomeLoader";
}
