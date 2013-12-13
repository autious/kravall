#ifndef SRC_CORE_LUA_BRIDGES_LUACONTENTMANAGERBRIDGE_H
#define SRC_CORE_LUA_BRIDGES_LUACONTENTMANAGERBRIDGE_H

#include "LuaContentManagerBridge.hpp"

#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

#include <ComponentFramework/ComponentType.hpp>

#include <utility>
#include <array>
#include <tuple>

#define CONTENT_LOADER_TYPE_META "content_loader_type_meta"

namespace Core
{
    typedef unsigned int ContentLoaderBridgeType;

    class LuaContentManagerBridge
    {
        public:
            static void OpenLibs( lua_State * L );   
    };

    template<typename... ContentLoadBinders>
    class LuaContentManagerBridgeTemplate
    {
    public:

        static const int CONTENT_LOAD_BINDER_COUNT = sizeof...(ContentLoadBinders);
    public:
        static int Load( lua_State * L )
        {
            if( lua_isuserdata( L, 1 ) && lua_isstring( L, 2 ) && lua_isfunction( L, 3 ) && lua_isboolean( L, 4 ) )
            {
                ContentLoaderBridgeType clbt 
                    = *(ContentLoaderBridgeType*)luaL_checkudata( L, 1, CONTENT_LOADER_TYPE_META );

                return CallST<ContentLoadBinders...>::Load( clbt, L );
            }   
            else
            {
                if( lua_isuserdata( L, 1 ) == 0 )
                    return luaL_error( L, "%s: first parameter is not userdata", __FUNCTION__ );
                if( lua_isstring( L, 2 ) == 0 )
                    return luaL_error( L, "%s: second parameter is not string", __FUNCTION__ );
                if( lua_isfunction( L, 3 ) == 0 )
                    return luaL_error( L, "%s: third parameter is not function", __FUNCTION__ );
                if( lua_isboolean( L, 4 ) == 0 )
                    return luaL_error( L, "%s: fourth parameter is not boolen", __FUNCTION__ );
            }
        }

        static int Free( lua_State * L )
        {
            if( lua_isuserdata( L, 1 ) && lua_isstring( L, 2 ) )
            {
                ContentLoaderBridgeType clbt 
                    = *(ContentLoaderBridgeType*)luaL_checkudata( L, 1, CONTENT_LOADER_TYPE_META );

                return CallST<ContentLoadBinders...>::Free( clbt, L );
            }   
            else
            {
                if( lua_isuserdata( L, 1 ) )
                    luaL_error( L, "%s: First parameter is not userdata.", __FUNCTION__ );
                if(  lua_isstring( L, 2 ) )
                    luaL_error( L, "%s: Second parameter is not string.", __FUNCTION__ );
            }
        }

        static int LoaderToString( lua_State *L )
        {
            ContentLoaderBridgeType clbt 
                = *(ContentLoaderBridgeType*)luaL_checkudata( L, 1, CONTENT_LOADER_TYPE_META );

            lua_pushinteger( L, clbt );

            return 1;
        }

        static std::array<std::pair<const char*, Core::ContentLoaderBridgeType>,CONTENT_LOAD_BINDER_COUNT> GetBinderNameTypes()
        {
            return 
            {{ 
                std::pair<const char*, Core::ContentLoaderBridgeType>
                ( 
                    ContentLoadBinders::GetLoaderLuaName(), 
                    GetContentLoaderBridgeTypeId<ContentLoadBinders>() 
                )... 
            }};
        }
            
    private:    
        template<typename ContentLoadBind> 
        static ContentLoaderBridgeType GetContentLoaderBridgeTypeId()
        {
            return Index<ContentLoadBind,std::tuple<ContentLoadBinders...>>::value;
        }

        template<typename... Loaders>
        struct CallST;

        template<typename Loader>
        struct CallST<Loader>
        {
            static int Load( ContentLoaderBridgeType loaderType, lua_State* L  )
            {
                if( GetContentLoaderBridgeTypeId<Loader>() == loaderType )
                {
                    return Loader::Load( L );
                }
            }

            static int Free( ContentLoaderBridgeType loaderType, lua_State* L  )
            {
                if( GetContentLoaderBridgeTypeId<Loader>() == loaderType )
                {
                    return Loader::Free( L );
                }
            }
        };

        template<typename Loader, typename... Loaders>
        struct CallST<Loader,Loaders...>
        {
            static int Load( ContentLoaderBridgeType loaderType, lua_State *L )
            {
                if( GetContentLoaderBridgeTypeId<Loader>() == loaderType )
                    return Loader::Load( L );
                else
                    return CallST<Loaders...>::Load( loaderType, L );
            }

            static int Free( ContentLoaderBridgeType loaderType, lua_State *L )
            {
                if( GetContentLoaderBridgeTypeId<Loader>() == loaderType )
                    return Loader::Free( L );
                else
                    return CallST<Loaders...>::Free( loaderType, L );
            }
        };
    };
}

#endif
