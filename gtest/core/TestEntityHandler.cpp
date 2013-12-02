#ifdef RUN_GTEST
#include <gtest/gtest.h>

#include "TestEntityHandlerAux.hpp"

namespace Core
{
    TEST( EntityHandlerTest, VariadicTemplateTest )
    {
        SystemHandler system;
        EntityHandler instance(&system); 
        
        ASSERT_EQ( 0, instance.GetComponentTypeId<Component1>() );
        ASSERT_EQ( 1, instance.GetComponentTypeId<Component2>() );
        ASSERT_EQ( 2, instance.GetComponentTypeId<Component3>() );


        //This should always generate a static assert error.
        //instance.GetComponentTypeId<NotComponent1>();
    }

    TEST( EntityHandlerTest, UnorderedAspectGeneration )
    {
        SystemHandler system;
        EntityHandler instance(&system); 

        Aspect asp1 = instance.GenerateAspect<Component1,Component2,Component3>();
        Aspect asp2 = instance.GenerateAspect<Component2,Component1,Component3>();


        ASSERT_EQ(asp1, asp2);

    }

    TEST( EntityHandlerTest, CorrectlyGeneratedAspect )
    {
        SystemHandler system;
        EntityHandler instance(&system); 

        Aspect asp1 = instance.GenerateAspect<Component1,Component2,Component3>();
        Aspect asp2 = 1ULL << 0 | 1ULL << 1 | 1ULL << 2;

        ASSERT_EQ(asp2, asp1);

        asp1 = instance.GenerateAspect<Component1,Component3>();
        asp2 = 1ULL << 0 |  1ULL << 2;

        ASSERT_EQ(asp2, asp1);

        asp1 = instance.GenerateAspect<Component3,Component1>();
        asp2 = 1ULL << 0 |  1ULL << 2;

        ASSERT_EQ(asp2, asp1);
    }
}
#endif
