#ifndef SRC_CORE_COMPONENTFRAMEWORK_COMPONENTTYPE_H
#define SRC_CORE_COMPONENTFRAMEWORK_COMPONENTTYPE_H

#include <tuple>
#include <cstdint>

namespace Core
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
}
#endif
