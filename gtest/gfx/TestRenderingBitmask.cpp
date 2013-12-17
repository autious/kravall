#ifdef RUN_GTEST
#include <gtest/gtest.h>
#include <gfx/BitmaskDefinitions.hpp>

namespace
{
	TEST(RenderingBitmaskTest, CorrectPackingUnpacking)
	{
		GFX::GFXBitmask bitmask;

		GFX::SetBitmaskValue(bitmask, GFX::BITMASK::TYPE, 0);
		GFX::SetBitmaskValue(bitmask, GFX::BITMASK::VIEWPORT_ID, 0);
		GFX::SetBitmaskValue(bitmask, GFX::BITMASK::LAYER, 0);
		GFX::SetBitmaskValue(bitmask, GFX::BITMASK::TRANSLUCENCY_TYPE, 0);
		GFX::SetBitmaskValue(bitmask, GFX::BITMASK::MESH_ID, 0);
		GFX::SetBitmaskValue(bitmask, GFX::BITMASK::MATERIAL_ID, 0);
		GFX::SetBitmaskValue(bitmask, GFX::BITMASK::DEPTH, 0);

		ASSERT_EQ(bitmask, 0x0000000000000000ULL);
		
		GFX::SetBitmaskValue(bitmask, GFX::BITMASK::TYPE, 1);
		GFX::SetBitmaskValue(bitmask, GFX::BITMASK::VIEWPORT_ID, 1);
		GFX::SetBitmaskValue(bitmask, GFX::BITMASK::LAYER, 1);
		GFX::SetBitmaskValue(bitmask, GFX::BITMASK::TRANSLUCENCY_TYPE, 1);
		GFX::SetBitmaskValue(bitmask, GFX::BITMASK::MESH_ID, 1);
		GFX::SetBitmaskValue(bitmask, GFX::BITMASK::MATERIAL_ID, 1);
		GFX::SetBitmaskValue(bitmask, GFX::BITMASK::DEPTH, 1);
		
		ASSERT_EQ(bitmask, 0x4940008001000001ULL);

		GFX::SetBitmaskValue(bitmask, GFX::BITMASK::MESH_ID, 1337);
		GFX::SetBitmaskValue(bitmask, GFX::BITMASK::TYPE, 2);
		GFX::SetBitmaskValue(bitmask, GFX::BITMASK::DEPTH, 4200);
		GFX::SetBitmaskValue(bitmask, GFX::BITMASK::VIEWPORT_ID, 5);
		GFX::SetBitmaskValue(bitmask, GFX::BITMASK::LAYER, 6);
		GFX::SetBitmaskValue(bitmask, GFX::BITMASK::TRANSLUCENCY_TYPE, 2);
		GFX::SetBitmaskValue(bitmask, GFX::BITMASK::MATERIAL_ID, 9001);
		
		unsigned int value;
		value = GFX::GetBitmaskValue(bitmask, GFX::BITMASK::TYPE);
		ASSERT_EQ(value, 2);
		value = GFX::GetBitmaskValue(bitmask, GFX::BITMASK::VIEWPORT_ID);
		ASSERT_EQ(value, 5);
		value = GFX::GetBitmaskValue(bitmask, GFX::BITMASK::LAYER);
		ASSERT_EQ(value, 6);
		value = GFX::GetBitmaskValue(bitmask, GFX::BITMASK::TRANSLUCENCY_TYPE);
		ASSERT_EQ(value, 2);
		value = GFX::GetBitmaskValue(bitmask, GFX::BITMASK::MESH_ID);
		ASSERT_EQ(value, 1337);
		value = GFX::GetBitmaskValue(bitmask, GFX::BITMASK::MATERIAL_ID);
		ASSERT_EQ(value, 9001);
		value = GFX::GetBitmaskValue(bitmask, GFX::BITMASK::DEPTH);
		ASSERT_EQ(value, 4200);

	}
}

#endif
