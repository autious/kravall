#ifdef RUN_GTEST
#include <gtest/gtest.h>
#include <dummy.hpp>

namespace {
    TEST( DummyFunctionTest, HandlesZeroInput )
    {
        EXPECT_EQ( 0, DummyFunction(0) ); 
    }

    TEST( DummyFunctionTest, HandlesPositiveInput )
    {
        EXPECT_EQ( 9, DummyFunction( 9 ) );
    }
}
#endif
