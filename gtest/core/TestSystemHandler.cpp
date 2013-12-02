#ifdef RUN_GTEST
#include <gtest/gtest.h>
#include "SystemHandlerAux.hpp"

namespace Core {

    TEST( SystemHandlerTest, VariadicTemplateSize )
    {
        SystemHandlerTemplate<System1,System2> systemHandler;
        int count = SystemHandlerTemplate<System1,System2>::SYSTEM_COUNT;
        ASSERT_EQ( 2, count );

        count = SystemHandlerTemplate<System1,System2,System3>::SYSTEM_COUNT;
        ASSERT_EQ( 3, count );
    }
}
#endif
