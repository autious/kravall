#ifndef SRC_CORE_COMPONENTFRAMEWORK_ENTITYHANDLER_H
#define SRC_CORE_COMPONENTFRAMEWORK_ENTITYHANDLER_H

#include "PVector.hpp"
#include "EntityVector.hpp"
#include "ComponentType.hpp"
#include "CheckTemplatePresence.hpp"

#include <cstdint>
#include <cassert>
#include <array>
#include <iostream>

#define SA_COMPONENT_USE "Component doesn't exist in EntityHandler. Maybe you forgot to add it?"

namespace Core
{
    template<typename SystemHandlerT, typename... Components>
    class EntityHandlerTemplate
    {
    private:
        static const int COMPONENT_COUNT = sizeof...(Components);

        EntityVector<1024,64,Components...> m_entities;
        std::array<PVector,sizeof...(Components)> m_components = {{PVector(1024,64,sizeof(Components))...}};

        SystemHandlerT *m_systemHandler;
    public:
        EntityHandlerTemplate( SystemHandlerT *systemHandler)
        {
            m_systemHandler = systemHandler;
        }

        /*!
            Create an entity instance with the given components
            NOTE: Will need a future function that can add components with runtime data. (for ex, lua)
        */
        template<typename... EntityComponents>
        Entity CreateEntity( EntityComponents... c )
        {
            Entity ent = m_entities.Alloc();        

            AddComponent<EntityComponents...>( ent, c... );

            m_systemHandler->CallChangedEntity( ent, 0ULL, GenerateAspect<EntityComponents...>() );
            return ent;
        }

        /*!
            Release an entity from allocation. Entity id'n are reused, so make sure to never reference
            an entity after calling this function as the old id might end up pointing to new, unexpected data
        */
        void DestroyEntity( Entity id )
        {
            m_systemHandler->CallChangedEntity( id, GetEntityAspect( id ), 0ULL );
            m_entities.Release( id );
        }

        /*!
            Retrieves entities current aspect
        */
        Aspect GetEntityAspect( Entity id )
        {
            return m_entities.GetAspect( id );
        }

        /*!
            Returns the static type id of the given component, used mainly internally.
            Calculated in compile-time making this function basically "free"
        */
        template<typename Component>
        static size_t constexpr GetComponentTypeId( )
        {
            static_assert( std::is_pod<Component>::value, "Components must be Pure Data Objects" );
            static_assert( Match<Component,Components...>::exists, SA_COMPONENT_USE );
            return Index<Component,std::tuple<Components...>>::value;
        }
    
        /*!
            Returns a pointer to component. The pointer is invalidated as soon as 
            a manipulating function is called (like release component or add component).
            or anything that might trigger a sorting of the component lists. This function 
            does not trigger any of these.
        */
        template<typename Component>
        Component* GetComponentTmpPointer( Entity entity )
        {
            static_assert( Match<Component,Components...>::exists, SA_COMPONENT_USE );

            static const int componentType = GetComponentTypeId<Component>();

            int componentId = m_entities.GetComponentId( entity );
            
            assert( componentId >= 0 );

            return (Component*)m_components[componentType].Get( componentId );
        }

        /*!
            Generates an aspect for the given group of components. An aspect is a generic single bitmask
            representing a group of components. This aspect can be used for either inclusive or 
            exclusive filtering. 

            This function is calculated in compile-time making this function "free". 
        */
        template<typename... AspectComponents>
        static Aspect GenerateAspect( )
        {
            static const size_t ids[] = { GetComponentTypeId<AspectComponents>()... };
            return GenerateAspect( ids, Aspect(), 0, sizeof...(AspectComponents) ); 
        }

    private:

        static Aspect constexpr GenerateAspect( const size_t *id, Aspect asp, int i, int size )
        {
            return asp |= (1ULL << id[i] | (i < size-1 ? GenerateAspect(id,asp,i+1,size) : 0ULL )); 
        }

        template<typename Component, typename... RComponents>
        void AddComponent( Entity ent, Component comp, RComponents... r  )
        {
            int componentType = GetComponentTypeId<Component>();
            //Check so that this component doesn't overwrite an existing one.
            assert( m_entities.GetComponentId(ent, componentType ) < 0 );

            int compId  = m_components[componentType].Alloc();

            m_components[componentType].Set( compId, &comp );
            
            m_entities.SetComponentId( ent, compId, componentType );

            AddComponent<RComponents...>(ent, r...);
        }

        template<typename Component>
        void AddComponent( Entity ent, Component comp )
        {
            int componentType = GetComponentTypeId<Component>();
            //Check so that this component doesn't overwrite an existing one.
            assert( m_entities.GetComponentId(ent, componentType ) < 0 );

            int compId  = m_components[componentType].Alloc();
            m_components[componentType].Set( compId, &comp );

            m_entities.SetComponentId( ent, compId, componentType );
        }
    };
}
#endif
