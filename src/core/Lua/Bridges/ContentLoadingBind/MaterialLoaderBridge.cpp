#include "MaterialLoaderBridge.hpp"

#include <ContentManagement/ContentManager.hpp>
#include <Lua/Bridges/ContentLoading/ContentHandle.hpp>
#include <World.hpp>

#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

int Core::MaterialLoaderBridge::Load( lua_State * L )
{
    lua_pushvalue( L, 3 );
    int callbackRegIndex = luaL_ref( L, LUA_REGISTRYINDEX ); 

    Core::world.m_contentManager.Load<Core::MaterialLoader>( lua_tostring(L,2), 
    [callbackRegIndex] (Core::BaseAssetLoader* baseLoader, Core::AssetHandle handle)
            {
                Core::MaterialData* data = static_cast<Core::MaterialData*>(handle);

                lua_State * L = Core::world.m_luaState.GetState();
                lua_rawgeti( L, LUA_REGISTRYINDEX, callbackRegIndex );
                lua_pushinteger( L, data->materialId );

                int error = lua_pcall( L, 1,0,0 );

                if( error )
                {
                    LOG_ERROR << __FUNCTION__ << "Unable to callback from MaterialLoader:" << lua_tostring( L, -1 ) << std::endl;
                    //Pop the error.
                    lua_pop(L,1 );
                }

                luaL_unref( L, LUA_REGISTRYINDEX, callbackRegIndex );
            }, lua_toboolean( L, 4 ));

    ContentHandle *contentHandle = (ContentHandle*)lua_newuserdata( L, sizeof( ContentHandle ) );
    luaL_getmetatable( L, CONTENT_HANDLE_META );
    lua_setmetatable( L, -2 );
     
    contentHandle->assetHash = Core::world.m_contentManager.GetHash<Core::MaterialLoader>( lua_tostring(L,2) );
    contentHandle->loaderId = *(ContentLoaderBridgeType*)luaL_checkudata(L,1,CONTENT_LOADER_TYPE_META );

    return 1;
}

int Core::MaterialLoaderBridge::Free( const AssetHash assetHash, lua_State * L )
{
    Core::world.m_contentManager.Free<Core::MaterialLoader>( assetHash );
    return 0;
}

const char * Core::MaterialLoaderBridge::GetLoaderLuaName()
{
    return "MaterialLoader";
}
