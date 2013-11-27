#ifdef RUN_GTEST
#include <gtest/gtest.h>
#include <ComponentFramework/SystemHandler.hpp>
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

    TEST( SystemHandlerTest, VariadicTemplateSize )
    {
        int count = SystemHandler<System1,System2>::SYSTEM_COUNT;
        ASSERT_EQ( 2, count );
    }
}
#endif
