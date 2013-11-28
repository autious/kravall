#ifdef RUN_GTEST

#include "TestEntityHandlerAux.hpp"

namespace Core
{
    int AuxFunction( SEntityHandler& handler )
    {
        return handler.GetComponentType<Component1>(); 
    }
}

#endif
