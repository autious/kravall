#ifndef GTEST_CORE_TESTENTITYHANDLERAUX_H
#define GTEST_CORE_TESTENTITYHANDLERAUX_H

#include <ComponentFramework/EntityHandler.hpp>

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

    typedef EntityHandler<Component1,Component2,Component3> SEntityHandler;

    int AuxFunction( SEntityHandler& );
}
#endif
