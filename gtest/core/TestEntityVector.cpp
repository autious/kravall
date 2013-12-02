#ifdef RUN_GTEST
#include <gtest/gtest.h>

#include "TestEntityHandlerAux.hpp"
#include <ComponentFramework/EntityVector.hpp>

namespace Core
{
    TEST( EntityVectorTest, TestCreation )
    {
        EntityVector<64,16,Component1,Component2,Component3> ev;

    }

    TEST( EntityVectorTest, TestAllocation )
    {
        EntityVector<64,16,Component1,Component2,Component3> ev;

        for( int i = 0; i < 30; i++ )
        {
            ev.Alloc();
        } 

        ASSERT_EQ( 30, ev.GetCount() );
        ASSERT_EQ( 64, ev.GetAllocation() );
    }

    TEST( EntityVectorTest, TestReAllocation )
    {
        EntityVector<64,16,Component1,Component2,Component3> ev;

        for( int i = 0; i < 30; i++ )
        {
            ev.Alloc();
        } 

        for( int i = 0; i < 30; i++ )
        {
            ev.Release( ev.Alloc() );
        }

        for( int i = 0; i < 30; i++ )
        {
            ev.Alloc();
        }

        ASSERT_EQ( 60, ev.GetCount() );
        ASSERT_EQ( 64, ev.GetAllocation() );

        for( int i = 0; i < 30; i++ )
        {
            ev.Alloc();
        }

        ASSERT_EQ( 60+30, ev.GetCount() );
        ASSERT_EQ( 64+16+16, ev.GetAllocation() );
    }

    TEST( EntityVectorTest, SetComponentId )
    {
        EntityVector<64,16,Component1,Component2,Component3> ev;

        Entity ent = ev.Alloc();

        ev.SetComponentId<Component2>( ent, 12 );
        ev.SetComponentId<Component1>( ent, 11 );
        ev.SetComponentId<Component3>( ent, 13 );

        Entity ent2 = ev.Alloc();

        ev.SetComponentId<Component1>( ent2, 14 );
        ev.SetComponentId<Component2>( ent2, 15 );
        ev.SetComponentId<Component3>( ent2, 16 );

        EXPECT_EQ( 11, ev.GetComponentId<Component1>( ent ) );
        EXPECT_EQ( 12, ev.GetComponentId<Component2>( ent ) ); 
        EXPECT_EQ( 13, ev.GetComponentId<Component3>( ent ) ); 
        EXPECT_EQ( 14, ev.GetComponentId<Component1>( ent2 ) ); 
        EXPECT_EQ( 15, ev.GetComponentId<Component2>( ent2 ) ); 
        EXPECT_EQ( 16, ev.GetComponentId<Component3>( ent2 ) ); 
    }

    TEST( EntityVectorTest, VerifyDefaultEntity )
    {
        EntityVector<64,16,Component1,Component2,Component3> ev;

        Entity ent = ev.Alloc();
       
        ASSERT_LT( ev.GetComponentId<Component1>(ent), 0 );
        ASSERT_LT( ev.GetComponentId<Component2>(ent), 0 );
        ASSERT_LT( ev.GetComponentId<Component3>(ent), 0 );

        ent = ev.Alloc();
       
        ASSERT_LT( ev.GetComponentId<Component1>(ent), 0 );
        ASSERT_LT( ev.GetComponentId<Component2>(ent), 0 );
        ASSERT_LT( ev.GetComponentId<Component3>(ent), 0 );

        ent = ev.Alloc();
       
        ASSERT_LT( ev.GetComponentId<Component1>(ent), 0 );
        ASSERT_LT( ev.GetComponentId<Component2>(ent), 0 );
        ASSERT_LT( ev.GetComponentId<Component3>(ent), 0 );
    }

    TEST( EntityVectorTest, GetAspect)
    {
        EntityVector<64,16,Component1,Component2,Component3> ev;

        Entity ent = ev.Alloc();
        ev.SetComponentId<Component1>( ent, 1 );
        ev.SetComponentId<Component2>( ent, 1 );

        Aspect asp1 = 1ULL << 0 | 1ULL << 1;
        Aspect asp2 = ev.GetAspect( ent );

        EXPECT_EQ( asp1, asp2 );

        Entity ent2 = ev.Alloc();
        ev.SetComponentId<Component3>( ent2, 1 );
        ev.SetComponentId<Component1>( ent2, 1 );

        asp1 = 1ULL << 0 | 1ULL << 2;
        asp2 = ev.GetAspect( ent2 );

        EXPECT_EQ( asp1, asp2 );
    }
}
#endif
