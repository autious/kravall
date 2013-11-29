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
        PVector pvec(64,16, sizeof( DataType ) );

        ASSERT_EQ( 64, pvec.GetAllocation() );
    }

    TEST( PVectorTest, CorrectResizing )
    {
        PVector pvec(64,16, sizeof( DataType ));

        for( int i = 0; i < 65; i++ )
        {
            pvec.Alloc();
        }

        ASSERT_EQ( 64+16, pvec.GetAllocation() );

        ASSERT_EQ( 65, pvec.GetCount() );
    }

    TEST( PVectorTest, CorrectReuse )
    {
        PVector pvec(64,16, sizeof( DataType ));

        for( int i = 0; i < 50; i++ )
        {
            pvec.Alloc();
        }

        for( int i = 0; i < 30; i++ )
        {
            pvec.Release(pvec.Alloc());
        }

        for( int i = 0; i < 50; i++ )
        {
            pvec.Alloc();
        }

        ASSERT_EQ( 64+16, pvec.GetAllocation() );

        ASSERT_EQ( 64, pvec.GetCount() );

    }

    TEST( PVectorTest, CorrectGetData )
    {
        PVector pvec(64,16, sizeof( DataType ));
        
        for( int i = 0; i < 10; i++ )
        {
            pvec.Get<DataType>(pvec.Alloc())->data = i;
        }
    
        int index = pvec.Alloc(); 
        DataType* d = pvec.Get<DataType>(index);
        d->data = 30;

        for( int i = 0; i < 10; i++ )
        {
            pvec.Get<DataType>(pvec.Alloc())->data = i;
        }

        d = pvec.Get<DataType>(index);

        ASSERT_EQ( 30, d->data );
    }
}

#endif
