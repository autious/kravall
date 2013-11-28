#ifndef SRC_CORE_COMPONENTFRAMEWORK_ENTITYHANDLER_H
#define SRC_CORE_COMPONENTFRAMEWORK_ENTITYHANDLER_H

#include <typeinfo>
#include <tuple>
#include <cstddef>

namespace Core
{
    typedef unsigned int Entity;

    template<typename... Components>
    class EntityHandler
    {
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

    public:
        EntityHandler( )
        {

        }

        template<typename Component>
        size_t GetComponentType( )
        {
            return Index<Component,std::tuple<Components...>>::value;
        }

        void *m_components[sizeof...(Components)];
        int m_componentSizes[sizeof...(Components)];
    };
}

#endif
