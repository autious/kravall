#ifndef SRC_CORE_COMPONENTFRAMEWORK_ENTITYHANDLER_H
#define SRC_CORE_COMPONENTFRAMEWORK_ENTITYHANDLER_H

#include <typeinfo>
#include <tuple>
#include <cstddef>
#include <cstdint>

#define SA_COMPONENT_USE "Component doesn't exist in EntityHandler. Maybe you forgot to add it?"

namespace Core
{
    typedef uint64_t Aspect;
    typedef unsigned int Entity;

    template<typename... Components>
    class EntityHandlerTemplate
    {
        /*!
            This is a template hack that allows us to calculate a static
            Components id in compile-time
        */
        template <class T, class Tuple>
        struct Index;

        template <class T, class... Types>
        struct Index<T, std::tuple<T, Types...>> {
            static const std::size_t value = 0;
        };

        template <class T, class U, class... Types>
        struct Index<T, std::tuple<U, Types...>> {
            static const std::size_t value = 1 + Index<T, std::tuple<Types...>>::value;
        };

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

        int *m_entityIds[COMPONENT_COUNT];

        /*!
            size increase value in component amount if an array of components runs out
        */
        int m_componentStepSize = 64;

        template<typename AB> 
        void GenerateAspectInternal( Aspect aspect )
        {
            aspect |= (1UL << GetComponentTypeId<AB>());
        }

        template<typename AB, typename... AspectComponents>
        void GenerateAspectInternal( Aspect aspect )
        {
            //GenerateAspectInternal<AspectComponents...>( aspect );
            aspect |= (1UL << GetComponentTypeId<AB>());
        }

    public:
        EntityHandlerTemplate()
        {
            for( int i = 0; i < COMPONENT_COUNT; i++ )
            {

            } 
        }
    
        template<typename ...EntityComponents>
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
        Aspect GenerateAspect( )
        {
            Aspect asp = 0ULL;
            GenerateAspectInternal<AspectComponents...>(asp);
        }
    };
}
#endif
