#ifndef SRC_CORE_LUA_BRIDGES_LUAENTITYBRIDGES_H
#define SRC_CORE_LUA_BRIDGES_LUAENTITYBRIDGES_H

#include "LuaEntityBridgeTypes.hpp"

#include <logger/Logger.hpp>

#include <array>
#include <vector>
#include <utility>
#include <limits>

#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

#include <Lua/LuaMetatableTypes.hpp>

#include <TemplateUtility/TemplateIndex.hpp>
#include <TemplateUtility/TemplatePresence.hpp>
#include <World.hpp>

namespace Core
{
    /*!
        EntityBridge middle class, contains only the openlibs function for LuaState.
        Functions as a front end for the template class version LuaEntityBridgeTemplate.
    */
    class LuaEntityBridge
    {
    public:
        LuaEntityBridge( lua_State *state );
    };

    /*! 
        Auxiliary function for LuaEntityBridgeTemplate
    */
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
                        componentSetter( entity, L, lua_gettop( L ) );
                    }
                    catch( const std::out_of_range& orr )
                    {
                        LOG_WARNING << __FUNCTION__ << ": " << Handler::GetComponentLuaName() << "Binding" << ": Ignoring parameter: " << name << std::endl;
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

    /*! 
        Auxiliary function for LuaEntityBridgeTemplate
    */
    template<typename Handler>
    int HandlerCallGet( Entity entity, lua_State* L )
    {
        ComponentGetters getters = Handler::GetGetters();  

        lua_newtable( L );
        for( ComponentGetters::iterator it = getters.begin();
                it != getters.end();
                it++ )
        {
            lua_pushstring( L, it->first.c_str() ); 
            if( it->second(entity, L) == 1 )
            {
               lua_settable( L, -3 ); 
            }
            else
            {
                return luaL_error( L, "%s: Internal error on parameter %s for component type %s", 
                                        __FUNCTION__, it->first.c_str(), Handler::GetComponentLuaName() );
            }
        }
        return 1;
    }


    /*! 
        Auxiliary template struct for LuaEntityBridgeTemplate
            
        Enables the Get/Set on variadic template list of multiple component type binders.
    */
    template<typename... Handlers>
    struct CallST;

    /*! 
        Auxiliary template struct for LuaEntityBridgeTemplate

        Enables the Get/Set on variadic template list of multiple component type binders.
    */
    template<typename Handler>
    struct CallST<Handler>
    {
        static void Set( Entity entity, ComponentType componentType, lua_State * L, int tableIndex )
        {
            if( Handler::GetComponentType() == componentType )
            {
                HandlerCallSet<Handler>( entity, L, tableIndex );
            }
            else
            {
                LOG_FATAL << __FUNCTION__ << ": You shouldn't be here, you've tried to set data to a component that doesn't exist." << std::endl;
                assert( false );
            }
        }

        static int Get( Entity entity, ComponentType componentType, lua_State * L )
        {
            if( Handler::GetComponentType() == componentType )
            {
                return HandlerCallGet<Handler>( entity, L  );
            }
            else
            {
                LOG_FATAL << __FUNCTION__ << ": You shouldn't be here, you've tried to get data from a component that doesn't exist." << std::endl;
                assert( false );
                return 0;
            }
                
        }
    };

    /*! 
        Auxiliary template struct for LuaEntityBridgeTemplate

        Enables the Get/Set on variadic template list of multiple component type binders.
    */
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

        static int Get( Entity entity, ComponentType componentType, lua_State * L )
        {
            if( Handler::GetComponentType() == componentType )
                return HandlerCallGet<Handler>( entity, L );
            else
                return CallST<Handlers...>::Get( entity, componentType, L );
        }
    };

    /*!
        Bridge between entity handler and lua, can create, destroy,
        place component, set component and get component data 
    */
    template<typename... ComponentHandlers>
    class LuaEntityBridgeTemplate
    {
    public:
        static const int COMPONENT_HANDLER_COUNT = sizeof...(ComponentHandlers);
    private:

    public:
        /*!
            Entity creation function, takes a series of component types and creates an entity 
            with that aspect configuration, component data is undefined
        */
        static int CreateEntity( lua_State* L )
        {
            int parameterCount = lua_gettop( L );

            Entity ent = Core::world.m_entityHandler.CreateEntity();            

            Aspect asp = 0ULL;
        
            for( int i = 1; i <= parameterCount; i++ )
            {
                if( lua_isuserdata( L, i ) )
                {
                    ComponentType type = *(ComponentType*)lua_touserdata( L, i );
                    asp |= Core::world.m_entityHandler.GenerateAspect( type );
                }
                else
                {
                    return luaL_error( L, "%s: Parameter %d is not a userdata" ,__FUNCTION__ , i );
                }
            }

            Core::world.m_entityHandler.AddComponentsAspect( ent, asp ); 

            Entity * userDataEntity = LuaUNewEntity( L );

            *userDataEntity = ent;
            return 1;
        }

        /*!
            Destroy entity function, takes an entity id and destroys is directly via the entity
            handler.
        */
        static int DestroyEntity( lua_State* L )
        {
            int paramCount = lua_gettop(L);

            for( int i = 1; i <= paramCount; i++ )
            {
                if( lua_isuserdata( L, i ) )
                {
                    Entity *entity = (Entity*)luaL_checkudata( L, i, ENTITY_META_TYPE );
                    
                    LOG_DEBUG << "Destroy call on entity: " <<  *entity << std::endl;

                    if( *entity != std::numeric_limits<Entity>::max() )
                    {
                        if( Core::world.m_entityHandler.DestroyEntity( *entity ) == false )
                        {
                            return luaL_error( L, "%s: Unable to remove given entity, parameter %d", __FUNCTION__, i );
                        }
                        else
                        {
                            *entity = std::numeric_limits<Entity>::max(); //Mark userdata as deleted.
                        }
                    }
                    else
                    {
                        LOG_DEBUG << "Ignored destruction call on non-entity value." << std::endl;
                    }
                }
                else
                {
                    return luaL_error( L, "%s: Parameter %d is not userdata", __FUNCTION__, i );
                }
            }

            return 0;
        }

        static int IsValid( lua_State * L )
        {
            Entity *entity = (Entity*)luaL_checkudata( L, 1, ENTITY_META_TYPE );
            lua_pushboolean( L, *entity != std::numeric_limits<Entity>::max() );
            return 1;
        }

        /*!
            Lua component data set function. takes entity id, component type and a table with data
            and sets it to the given entities component via component binding class object.
        */
        static int Set( lua_State * L )
        {
            assert( lua_gettop( L ) == 3 );

            if( lua_isuserdata( L, 1 ) && lua_isuserdata( L, 2 ) )
            {
                Entity entity = *((Entity*)luaL_checkudata( L, 1, ENTITY_META_TYPE ));
                ComponentType componentType = *(ComponentType*)luaL_checkudata( L, 2, COMPONENT_META_TYPE );

                if( Core::world.m_entityHandler.HasComponent( entity, componentType ) )
                {
                    CallST<ComponentHandlers...>::Set( entity, componentType, L, 3 );                      
                }
                else
                {
                    return luaL_error( L, "Unable to set component data, entity doesn't have component type" );
                }
            }
            else
            {
                return luaL_error( L, "Unable to Set component data, one of parameters are not userdata" );
            }

            return 0;
        }

        /*!
            Lua environment get function,  takes entity and component type.
            Will return table to lua containing all the data for a component.
        */
        static int Get( lua_State * L )
        {
            if( lua_isuserdata( L, 1 ) && lua_isuserdata( L, 2 ) )
            {
                Entity entity = *((Entity*)luaL_checkudata( L, 1, ENTITY_META_TYPE ));
                ComponentType componentType = *(ComponentType*)luaL_checkudata( L, 2, COMPONENT_META_TYPE );

                if( Core::world.m_entityHandler.HasComponent( entity, componentType ) )
                {
                    return CallST<ComponentHandlers...>::Get( entity, componentType, L );                      
                }
                else
                {
                    return luaL_error( L, "Unable to get component data, entity doesn't have component type" );
                }

            }
            else
            {
                return luaL_error( L, "Unable to Get component data, incorrect first two parameters" );
            }
        }

        /*!
            Returns entity integer value used internally. Exists mostly for debugging purposes
        */
        static int EntityToString( lua_State *L )
        {
            Entity entity = *(Entity*)luaL_checkudata( L, 1, ENTITY_META_TYPE );

            lua_pushinteger( L, entity );  
            
            return 1;
        }

        /*!
            Component type __tostring meta function. returns component type id.
            Debugging function mainly.
        */
        static int ComponentTypeToString( lua_State *L )
        {
            ComponentType componentType = *(ComponentType*)luaL_checkudata( L, 1, COMPONENT_META_TYPE );
            
            lua_pushinteger( L, componentType );

            return 1;
        }

        static std::array<std::pair<const char*, Core::ComponentType>,COMPONENT_HANDLER_COUNT> GetBinderNameTypes( )
        {
            return {{ std::pair<const char*, Core::ComponentType>( ComponentHandlers::GetComponentLuaName(), ComponentHandlers::GetComponentType() )... }};
        }
    private:
    };
}

#endif

