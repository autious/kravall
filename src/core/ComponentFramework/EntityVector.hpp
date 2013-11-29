#ifndef SRC_CORE_COMPONENTFRAMEWORK_ENTITYVECTOR_H
#define SRC_CORE_COMPONENTFRAMEWORK_ENTITYVECTOR_H


#include "ComponentType.hpp"

#include <cstdlib>
#include <cassert>
#include <cstring>

#include <vector>

#define ONE_ENT_SIZE sizeof( Entity ) * COMPONENT_COUNT

namespace Core
{
    typedef unsigned int Entity;
    typedef uint64_t Aspect;
    
    template<size_t Initial, size_t Step, typename... Components>
    class EntityVector
    {
    private:
        std::vector<Entity> m_removed;
        int *m_entities;
        size_t m_count;
        size_t m_size;
        static const int COMPONENT_COUNT = sizeof...(Components);
    public:
        EntityVector( )
        {
            m_count = 0;
            m_size = Initial;

            m_entities = (int*)malloc( m_size * ONE_ENT_SIZE );
        }

        ~EntityVector()
        {
            free( m_entities );
        }

        Entity Alloc()
        {
            Entity id = 0;
            if( m_count >= m_size )
            {
                m_size += Step;
                m_entities = (int*)realloc( m_entities, m_size * ONE_ENT_SIZE );
                
                assert( m_entities != nullptr );
            }

            if( m_removed.size() > 0 )
            {
                id = m_removed.back();
                m_removed.pop_back();
            }
            else
            {
                id = m_count; 
            }

            m_count++;

            memset( &m_entities[id*COMPONENT_COUNT], 255, ONE_ENT_SIZE );

            return id;
        }

        void Release( Entity id )
        {
            //Reset all component variables
            for( int i = 0; i < COMPONENT_COUNT; i++ )
                m_entities[COMPONENT_COUNT*id+i] = -1; 

            m_removed.push_back( id );
            m_count--;
        }

        template<typename Component>
        void SetComponentId( Entity id, int componentId )
        {
            assert( id > 0 && id < m_size );
            int index = Index<Component,std::tuple<Components...>>::value;
            assert( componentType > 0 &&  index < COMPONENT_COUNT );
            m_entities[COMPONENT_COUNT*id+index] = componentId;
        }

        template<typename Component>
        int GetComponentId( Entity id )
        {
            return m_entities[COMPONENT_COUNT*id+Index<Component,std::tuple<Components...>>::value];
        }

        /*!
            Gets the currently active count of enities
        */
        size_t GetCount()
        {
            return m_count;
        }

        /*!
            Gets the currently memory allocated slots for entties
        */
        size_t GetAllocation()
        {
            return m_size;
        }

        /*!
            Calculates (in runtime) the given entities aspect and returns it
        */
        Aspect GetAspect( Entity id )
        {
            Aspect asp;
            for( int i = 0; i < COMPONENT_COUNT; i++ )
            {
                if( m_entities[COMPONENT_COUNT*id+i] >= 0 )
                {
                    asp |=  1ULL << i;
                }
            }

            return asp;
        }
    };
}

#undef ONE_ENT_SIZE
#endif
