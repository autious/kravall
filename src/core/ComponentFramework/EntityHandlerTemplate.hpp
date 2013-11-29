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
    template<typename... Components>
    class EntityHandlerTemplate
    {
    private:
        static const int COMPONENT_COUNT = sizeof...(Components);

        EntityVector<1024,64,Components...> m_entities;
        std::array<PVector,sizeof...(Components)> m_components = {{PVector(1024,64,sizeof(Components))...}};
        
    public:
        EntityHandlerTemplate()
        {
        }

        template<typename... EntityComponents>
        Entity CreateEntity( EntityComponents... c )
        {
            Entity ent = m_entities.Alloc();        

            AddComponent<EntityComponents...>( ent, c... );

            return ent;
        }

        void DestroyEntity( Entity id )
        {
            m_entities.Release( id );
        }

        template<typename Component>
        size_t constexpr GetComponentTypeId( )
        {
            static_assert(std::is_pod<Component>::value, "Components must be Pure Data Objects");
            static_assert( Match<Component,Components...>::exists, SA_COMPONENT_USE );
            return Index<Component,std::tuple<Components...>>::value;
        }
    
        template<typename Component>
        Component* GetComponentTmpPointer( Entity entity )
        {
            static_assert( Match<Component,Components...>::exists, SA_COMPONENT_USE );

            static const int componentType = GetComponentTypeId<Component>();

            int componentId = m_entities.GetComponentId( entity );
            
            assert( componentId >= 0 );

            return (Component*)m_components[componentType].Get( componentId );
        }

        template<typename... AspectComponents>
        Aspect constexpr GenerateAspect( )
        {
            static const size_t ids[] = { GetComponentTypeId<AspectComponents>()... };
            return GenerateAspect( ids, Aspect(), 0, sizeof...(AspectComponents) ); 
        }

    private:

        Aspect constexpr GenerateAspect( const size_t *id, Aspect asp, int i, int size )
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

            m_entities.SetComponentId( ent, compId, componentType );
        }
    };
}
#endif
