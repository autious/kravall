#ifndef SRC_CORE_COMPONENTFRAMEWORK_ENTITYHANDLER_H
#define SRC_CORE_COMPONENTFRAMEWORK_ENTITYHANDLER_H

#include "PVector.hpp"
#include "EntityVector.hpp"
#include "ComponentType.hpp"

#include <cstdint>
#include <cassert>
#include <array>

#define SA_COMPONENT_USE "Component doesn't exist in EntityHandler. Maybe you forgot to add it?"

namespace Core
{

    template<typename... Components>
    class EntityHandlerTemplate
    {

        //**********************************************************************
        
        /*!
            This template hack is for checking if a component
            given in an function exists for the handler.
        */
        template <class... R>
        struct Match;

        template <class T, class U>
        struct Match<T,U>
        {
            static const bool exists = false;
        };

        template <class T>
        struct Match<T,T>
        {
            static const bool exists = true;
        };

        template <class T, class... R> 
        struct Match<T,T,R...>
        {
            static const bool exists = true;
        };

        template <class T, class U, class... R> 
        struct Match<T,U,R...>
        {
            static const bool exists = Match<T,R...>::exists;
        };

        //**********************************************************************
        
        static const int COMPONENT_COUNT = sizeof...(Components);

        std::vector<int[COMPONENT_COUNT]> m_entityIds;
        std::vector<int> m_deletedEntities;

        std::array<PVector,sizeof...(Components)> p = {{PVector(1024,64,sizeof(Components))...}};

    public:
        EntityHandlerTemplate()
        {
        }

        template<typename... EntityComponents>
        Entity CreateEntity()
        {
            
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
        }

        template<typename... AspectComponents>
        Aspect constexpr GenerateAspect( )
        {
            static const size_t ids[] = { GetComponentTypeId<AspectComponents>()... };
            return GenerateAspect( ids, Aspect(), 0, sizeof...(AspectComponents) ); 
        }

        Aspect constexpr GenerateAspect( const size_t *id, Aspect asp, int i, int size )
        {
               return asp |= (1ULL << id[i] | (i < size ? GenerateAspect(id,asp,i+1,size) : 0ULL )); 
        }

    private:
    };
}
#endif
