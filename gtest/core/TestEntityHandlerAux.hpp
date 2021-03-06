#ifndef GTEST_CORE_TESTENTITYHANDLERAUX_H
#define GTEST_CORE_TESTENTITYHANDLERAUX_H

#include <ComponentFramework/EntityHandlerTemplate.hpp>
#include "SystemHandlerAux.hpp"

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

    struct Component4
    {
        float a,b,c,d;
    };
    
    struct NotComponent1
    {
        int data;
    };

    typedef EntityHandlerTemplate<SystemHandler,Component1,Component2,Component3> EntityHandler;
}
#endif
