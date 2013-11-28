#ifdef RUN_GTEST
#include <gtest/gtest.h>
#include <ComponentFramework/SystemHandler.hpp>
#include <ComponentFramework/BaseSystem.hpp>
#include <ComponentFramework/EntityHandler.hpp>

#include "TestEntityHandlerAux.hpp"

namespace Core
{
    TEST( EntityHandlerTest, VariadicTemplateTest )
    {
        SEntityHandler instance; 
        
        ASSERT_EQ( 0, instance.GetComponentType<Component1>() );
        ASSERT_EQ( 1, instance.GetComponentType<Component2>() );
        ASSERT_EQ( 2, instance.GetComponentType<Component3>() );

        AuxFunction( instance );
    }
}

#endif
