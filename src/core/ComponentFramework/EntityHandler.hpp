#ifndef SRC_CORE_COMPONENTFRAMEWORK_ENTITYHANDLER_H
#define SRC_CORE_COMPONENTFRAMEWORK_ENTITYHANDLER_H

#include <typeinfo>
#include <tuple>
#include <cstddef>
#include <cstdint>

namespace Core
{
    typedef uint64_t Aspect;
    typedef unsigned int Entity;

    template<typename... Components>
    class EntityHandler
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
        
        static const int COMPONENT_COUNT = sizeof...(Components);

        void *m_components[COMPONENT_COUNT];
        int m_componentSizes[COMPONENT_COUNT];
        int m_componentCounts[COMPONENT_COUNT];

        int m_initialComponentSize = 1024;
        int m_componentStepSize = 64;

      
        template<typename A, typename... AspectComponents>
        Aspect constexpr GenerateAspectInternal( Aspect aspect )
        {
            return (1UL << GetComponentTypeId<A>()) | GenerateAspectInternal<AspectComponents...>( aspect );
        }

        template<typename A> 
        Aspect constexpr GenerateAspectInternal( Aspect aspect )
        {
            return (1UL << GetComponentTypeId<A>());
        }

    public:
        EntityHandler()
        {
            
        }

        template<typename Component>
        size_t constexpr GetComponentTypeId( )
        {
            return Index<Component,std::tuple<Components...>>::value;
        }
    
        template<typename Component>
        Component* GetComponentTmpPointer( Entity entity )
        {
            
        }

        template<typename... AspectComponents>
        Aspect contexpr GenerateAspect( )
        {
            return GenerateAspectInternal<AspectComponents...>(0ULL);
        }
    };
}

#endif
