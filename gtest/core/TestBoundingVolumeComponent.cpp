#ifdef RUN_GTEST
#include <gtest/gtest.h>
#include <World.hpp>

namespace Core {

    TEST( PickingSystem, BoundingVolumeComponentTest )
    {

		EXPECT_TRUE( MAX_SIZE_OF_BOUNDING_STRUCTS >= sizeof( Core::BoundingSphere ) );
		EXPECT_TRUE( MAX_SIZE_OF_BOUNDING_STRUCTS >= sizeof( Core::AABB ) );

		{
			Core::BoundingVolumeComponent aa = Core::BoundingVolumeComponent( Core::BoundingSphere( 5.0f, 4.0f, 3.0f, 2.0f ) );
			Core::BoundingSphere* data = (Core::BoundingSphere*)aa.data;
			
			EXPECT_TRUE( std::abs(data->radius - 5.0f) < 0.0005f );
			EXPECT_TRUE( std::abs(data->offset[0] - 4.0f) < 0.0005f );
			EXPECT_TRUE( std::abs(data->offset[1] - 3.0f) < 0.0005f );
			EXPECT_TRUE( std::abs(data->offset[2] - 2.0f) < 0.0005f );
		}

    }
}
#endif
