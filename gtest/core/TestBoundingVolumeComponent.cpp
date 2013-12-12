#ifdef RUN_GTEST
#include <gtest/gtest.h>
//#include "TestEntityHandlerAux.hpp"
#include <World.hpp>

namespace Core {

	/*
    #define TS1_INC Component1,Component4
    #define TS1_EXC Component2

    #define TS2_INC Component3

    class TestSystem1;
    class TestSystem2;

    typedef SystemHandlerTemplate<TestSystem1,TestSystem2> SH_L;
    typedef EntityHandlerTemplate<SH_L,Component1,Component2,Component3,Component4> EH_L;
	*/


    TEST( PickingSystem, BoundingVolumeComponentTest )
    {

		EXPECT_TRUE( MAX_SIZE_OF_BOUNDING_STRUCTS >= sizeof( Core::BoundingSphere ) );
		EXPECT_TRUE( MAX_SIZE_OF_BOUNDING_STRUCTS >= sizeof( Core::AABB ) );

		{
			Core::BoundingVolumeComponent aa = Core::BoundingVolumeComponent( Core::BoundingSphere( 5.0f, 4.0f, 3.0f, 2.0f ) );
			Core::BoundingSphere* data = (Core::BoundingSphere*)aa.m_data;
			/*
			EXPECT_TRUE( data->m_radius 
			EXPECT_TRUE( data->m_radius 
			EXPECT_TRUE( data->m_radius 
			EXPECT_TRUE( data->m_radius 
			*/
		}








    }
}
#endif
