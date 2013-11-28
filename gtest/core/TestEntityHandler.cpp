#ifdef RUN_GTEST
#include <gtest/gtest.h>
#include <ComponentFramework/SystemHandlerTemplate.hpp>
#include <ComponentFramework/BaseSystem.hpp>
#include <ComponentFramework/EntityHandlerTemplate.hpp>

#include "TestEntityHandlerAux.hpp"

namespace Core
{
    TEST( EntityHandlerTest, VariadicTemplateTest )
    {
        EntityHandler instance; 
        
        ASSERT_EQ( 0, instance.GetComponentTypeId<Component1>() );
        ASSERT_EQ( 1, instance.GetComponentTypeId<Component2>() );
        ASSERT_EQ( 2, instance.GetComponentTypeId<Component3>() );

        //This should always generate a static assert error.
        //instance.GetComponentTypeId<NotComponent1>();

        AuxFunction( instance );
    }

    TEST( EntityHandlerTest, UnorderedAspectGeneration )
    {
        EntityHandler instance; 

        Aspect asp1 = instance.GenerateAspect<Component1,Component2,Component3>();
        Aspect asp2 = instance.GenerateAspect<Component2,Component1,Component3>();


        ASSERT_EQ(asp1, asp2);

    }
}

#endif
