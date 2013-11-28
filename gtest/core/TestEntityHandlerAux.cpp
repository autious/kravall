#ifdef RUN_GTEST

#include "TestEntityHandlerAux.hpp"

namespace Core
{
    int AuxFunction( EntityHandler& handler )
    {
        return handler.GetComponentTypeId<Component1>(); 
    }
}

#endif
