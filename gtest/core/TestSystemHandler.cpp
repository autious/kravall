#ifdef RUN_GTEST
#include <gtest/gtest.h>
#include <ComponentFramework/SystemHandlerTemplate.hpp>
#include <ComponentFramework/BaseSystem.hpp>

namespace Core {
    class System1 : public BaseSystem
    {
        public:
            virtual ~System1() {};

    };

    class System2 : public BaseSystem
    {
        public:
            virtual ~System2() {};
    };

    class System3 : public BaseSystem
    {
        public:
            virtual ~System3() {};
    };

    TEST( SystemHandlerTest, VariadicTemplateSize )
    {
        int count = SystemHandlerTemplate<System1,System2>::SYSTEM_COUNT;
        ASSERT_EQ( 2, count );

        count = SystemHandlerTemplate<System1,System2,System3>::SYSTEM_COUNT;
        ASSERT_EQ( 3, count );
    }
}
#endif
