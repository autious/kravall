#ifdef RUN_GTEST
#include <gtest/gtest.h>
#include <ComponentFramework/PVector.hpp>

namespace Core
{
    struct DataType
    {
        float Data;
    };

    TEST( PVectorTest, CorrectInitialSize )
    {
       PVector<DataType> pvec( 64,64);
    }

    TEST( PVectorTest, CorrectResizing )
    {
    }

    TEST( PVectorTest, CorrectGetData )
    {
    }

    TEST( PVectorTest, Removal )
    {
    }

    TEST( PVectorTest, Reuse )
    {
    }
}

#endif
