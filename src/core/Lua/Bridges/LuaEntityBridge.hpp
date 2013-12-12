#ifndef SRC_CORE_LUA_BRIDGES_LUAENTITYBRIDGES_H
#define SRC_CORE_LUA_BRIDGES_LUAENTITYBRIDGES_H

#include <array>
#include <ComponentFramework/SystemTypes.hpp>
#include "LuaEntityBridgeTypes.hpp"

#include <logger/Logger.hpp>

#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

namespace Core
{
    class LuaEntityBridge
    {
        static void OpenLibs( lua_State * state );
    };

    template<typename Handler>
    void HandlerCallSet( Entity entity, lua_State* L, int tableindex )
    {
        ComponentSetters setters = Handler::GetSetters();  

        if( lua_istable( L, tableindex ) )
        {
            /* Table iteration similar to luas own for k,v in pairs(table) do...*/
            lua_pushnil(L); //First key for the iterator
            while( lua_next( L , tableindex ) != 0 )
            {
                if( lua_isstring( L, -2 ) )
                {
                    const char * name = lua_tostring( L, -2 );

                    try
                    {
                        ComponentSet componentSetter = setters.at(name);
                        componentSetter( entity, L, -1 );
                    }
                    catch( const std::out_of_range& orr )
                    {
                        LOG_WARNING << Handler::GetComponentLuaName() << ": Ignoring parameter: " << name << std::endl;
                    }
                }
                /* Pops the value, leaving the key for the next iteration */
                lua_pop( L, 1 );
            } 
        }
        else
        {
            luaL_error( L, "Unable to set data for %s, given tableindex is not a table", Handler::GetComponentLuaName() );
        }   
    }

/*
    template<typename... Handler>
    void HandlerCallGet( Entity entity, lua_State* L, int tableindex )
    {
        ComponentGetters getters = Handler::GetGetters();  

        try
        {
            ComponentSet componentGetter = getters.at(name);

            componentGetter( entity, L, returntableindex );
        }
        catch( const std::out_of_range& orr )
        {
            LOG_WARNING << Handler::GetComponentLuaName() << ": Ignoring parameter: " <<  name;
        }
    }
*/

    template<typename... Handlers>
    struct CallST;

    template<typename Handler>
    struct CallST<Handler>
    {
        static void Set( Entity entity, ComponentType componentType, lua_State * L, int tableIndex )
        {
            if( Handler::GetComponentType() == componentType )
                HandlerCallSet<Handler>( entity, L, tableIndex );
        }

/*
        static int Get( Entity entity, ComponentType componentType, lua_State * L )
        {
            if( Handler::GetComponentType() == componentType )
                return Handler::Set( entity, componentType, L );
        }
*/
    };

    template<typename Handler, typename... Handlers>
    struct CallST<Handler,Handlers...>
    {
        static void Set( Entity entity, ComponentType componentType, lua_State * L, int tableIndex )
        {
            if( Handler::GetComponentType() == componentType )
                HandlerCallSet<Handler>( entity, L, tableIndex );
            else
                CallST<Handlers...>::Set( entity, componentType, L, tableIndex );
        }
/*
        static int Get( Entity entity, ComponentType componentType, lua_State * L )
        {
            if( Handler::GetComponentType() == componentType )
                return Handler::Get( entity, componentType, L );
            else
                return CallST<Handlers...>::Get( entity, componentType, L );
        }
*/
    };


    template<typename... ComponentHandlers>
    class LuaEntityBridgeTemplate
    {
    public:
        static const int COMPONENT_HANDLER_COUNT = sizeof...(ComponentHandlers);
    private:

    public:
        static int CallSet( lua_State * L )
        {
            if( lua_isuserdata( L, 1 ) && lua_isuserdata( L, 2 ) )
            {
                Entity entity = *((Entity*)lua_touserdata( L, 1 ));
                ComponentType componentType = (ComponentType)lua_touserdata( L, 2 );
            
                CallST<ComponentHandlers...>::Set( entity, componentType, L, 3 );                      
            }
            else
            {
                return luaL_error( L, "Unable to Set component data, incorrect first two parameters" );
            }

            return 0;
        }

/*
        static int CallGet( lua_State * L )
        {
            if( lua_isuserdata( L, 1 ) && lua_isuserdata( L, 2 ) )
            {
                Entity entity = (Entity)lua_touserdata( L, 1 );       
                ComponentType componentType = (ComponentType)lua_touserdata( L, 2 );

                return CallST<ComponentHandlers...>::Get( entity, componentType, L );                      
            }
            else
            {
                return luaL_error( L, "Unable to Get component data, incorrect first two parameters" );
            }
        }
*/

    private:
    };
}

#endif

