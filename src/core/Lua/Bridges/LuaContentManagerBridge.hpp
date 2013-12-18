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

    /*!
        Simple middle class to LuaContentManagerTemplate,
        holds the OpenLibs function for LuaState.
    */
    class LuaContentManagerBridge
    {
        public:
            static void OpenLibs( lua_State * L );   
    };

    /*!
        ContentManager lua bridge loader gluer.
        This class ties all the different instances of loader wrappers to 
        the lua environment. Each created instance has one Load function one Free and a 
        GetLoaderLuaName that defines the variable under core.loaders that matches the
        loader.
    */
    template<typename... ContentLoadBinders>
    class LuaContentManagerBridgeTemplate
    {
    public:
        static const int CONTENT_LOAD_BINDER_COUNT = sizeof...(ContentLoadBinders);
    public:

        /*!
            Lua environment load function, this function will in turn call the specific bindings
            based on loader index value given
        */
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
            return luaL_error( L, "%s: Unable to load, program flow should not reach this position.", __FUNCTION__ );
        }

        /*!
            Lua environment free call, will in turn call the specific loader type free function
            to free a given asset name 
        */
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
                if( lua_isuserdata( L, 1 ) == 0 )
                    return luaL_error( L, "%s: First parameter is not userdata.", __FUNCTION__ );
                if( lua_isstring( L, 2 ) == 0 )
                    return luaL_error( L, "%s: Second parameter is not string.", __FUNCTION__ );
            }
            return luaL_error( L, "%s: Unable to free, program flow should not reach this position.", __FUNCTION__ );
        }

        /*! 
            Lua environment __tostring function for the content loader type variables
            returns a simple integer value describing the index.
        */
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
                return luaL_error( L, "Unable to load given asset, not loader exist for type" );
            }

            static int Free( ContentLoaderBridgeType loaderType, lua_State* L  )
            {
                if( GetContentLoaderBridgeTypeId<Loader>() == loaderType )
                {
                    return Loader::Free( L );
                }
                return luaL_error( L, "Unable to free given asset, no loader exists for type" );
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
