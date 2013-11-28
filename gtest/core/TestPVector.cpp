#ifdef RUN_GTEST
#include <gtest/gtest.h>
#include <ComponentFramework/PVector.hpp>

namespace Core
{
    struct DataType
    {
        int data;
    };

    TEST( PVectorTest, CorrectInitialSize )
    {
        PVector<DataType> pvec(64,16);

        ASSERT_EQ( 64, pvec.GetAllocation() );
    }

    TEST( PVectorTest, CorrectResizing )
    {
        PVector<DataType> pvec(64,16);

        for( int i = 0; i < 65; i++ )
        {
            pvec.Alloc();
        }

        ASSERT_EQ( 64+16, pvec.GetAllocation() );

        ASSERT_EQ( 65, pvec.GetCount() );
    }

    TEST( PVectorTest, CorrectGetData )
    {
        PVector<DataType> pvec(64,16);

        
        for( int i = 0; i < 10; i++ )
        {
            pvec.Alloc();
        }
    
        int index = pvec.Alloc(); 
        DataType* d = pvec.Get(index);
        d->data = 30;

        for( int i = 0; i < 10; i++ )
        {
            pvec.Alloc();
        }

        d = pvec.Get(index);

        ASSERT_EQ( 30, d->data );

    }
}

#endif
