#ifdef RUN_GTEST
#include <gtest/gtest.h>
#include "TestEntityHandlerAux.hpp"

namespace Core {

    TEST( SystemHandlerTest, VariadicTemplateSize )
    {
        SystemHandlerTemplate<System1,System2> systemHandler;
        int count = SystemHandlerTemplate<System1,System2>::SYSTEM_COUNT;
        ASSERT_EQ( 2, count );

        count = SystemHandlerTemplate<System1,System2,System3>::SYSTEM_COUNT;
        ASSERT_EQ( 3, count );
    }

    #define TS1_INC Component1,Component4
    #define TS1_EXC Component2

    #define TS2_INC Component3

    class TestSystem1;
    class TestSystem2;

    typedef SystemHandlerTemplate<TestSystem1,TestSystem2> SH_L;
    typedef EntityHandlerTemplate<SH_L,Component1,Component2,Component3,Component4> EH_L;

    class TestSystem1 : public BaseSystem
    {
        public:
            TestSystem1() : BaseSystem( EH_L::GenerateAspect<TS1_INC>(),
                                        EH_L::GenerateAspect<TS1_EXC>()){};
            virtual void Update( float delta ) override {}
            virtual ~TestSystem1() {};
            bool has( Entity id )
            {
                for( unsigned int i = 0; i < m_entities.size(); i++ )
                {
                    if( id == m_entities[i] )
                        return true;
                }  

                return false;
            }
    };

    class TestSystem2 : public BaseSystem
    {
        public:
            TestSystem2() : BaseSystem( EH_L::GenerateAspect<TS2_INC>(),
                                        0ULL ){};

            virtual void Update( float delta ) override {}
            bool has( Entity id )
            {
                for( unsigned int i = 0; i < m_entities.size(); i++ )
                {
                    if( id == m_entities[i] )
                        return true;
                }  

                return false;
            }

            virtual ~TestSystem2() {};
    };

    //Defins some component setups that match situations.
    // Exclusive on 1 inclusive match on 2
    #define TS2_OK_TS1_NO Component4,Component3,Component2,Component1
    // Match on 1, no match on 2
    #define TS2_NO_TS1_OK Component1,Component4
    //Exlusive on 1 no match on 2
    #define TS2_NO_TS1_NO Component2
    //Inclusive on both
    #define TS2_OK_TS1_OK Component1,Component3,Component4

    TEST( SystemHandlerTest, AspectCallback )
    {
        SH_L s_handler;
        EH_L e_handler( &s_handler );

        TestSystem1 *t1 = dynamic_cast<TestSystem1*>(s_handler.GetSystem(0));
        TestSystem2 *t2 = dynamic_cast<TestSystem2*>(s_handler.GetSystem(1));

        Entity e = e_handler.CreateEntity<TS2_OK_TS1_NO>( Component4(), Component3(), Component2(), Component1() );
        EXPECT_TRUE( t2->has(e) );
        EXPECT_FALSE( t1->has(e) );

        e = e_handler.CreateEntity<TS2_NO_TS1_OK>( Component1(), Component4() );
        EXPECT_FALSE( t2->has(e) );
        EXPECT_TRUE( t1->has(e) );

        e_handler.DestroyEntity(e);
        EXPECT_FALSE( t2->has(e) );
        EXPECT_FALSE( t1->has(e) );

        e = e_handler.CreateEntity<TS2_NO_TS1_NO>( Component2() );
        EXPECT_FALSE( t2->has(e) );
        EXPECT_FALSE( t1->has(e) );

        e = e_handler.CreateEntity<TS2_OK_TS1_OK>( Component1(), Component3(), Component4() );
        EXPECT_TRUE( t2->has(e) );
        EXPECT_TRUE( t1->has(e) );

        e_handler.DestroyEntity(e);
        EXPECT_FALSE( t2->has(e) );
        EXPECT_FALSE( t1->has(e) );
    }
}
#endif
