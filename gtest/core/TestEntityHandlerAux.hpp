#ifndef GTEST_CORE_TESTENTITYHANDLERAUX_H
#define GTEST_CORE_TESTENTITYHANDLERAUX_H

#include <ComponentFramework/EntityHandlerTemplate.hpp>

namespace Core
{
    struct Component1
    {
        int value;
    };

    struct Component2
    {
        float value2;
    };

    struct Component3
    {
        Component2 comp;
    };

    typedef EntityHandlerTemplate<Component1,Component2,Component3> EntityHandler;

    int AuxFunction( EntityHandler& );
}
#endif
