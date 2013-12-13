#ifdef RUN_GTEST
#include <gtest/gtest.h>
#include <gfx/BitmaskDefinitions.hpp>

namespace
{
	TEST(RenderingBitmaskTest, CorrectPackingUnpacking)
	{
		GFX::GFXBitmask bitmask;

		GFX::SetBitmaskValue(bitmask, GFX::BT_OBJECT_TYPE, 0);
		GFX::SetBitmaskValue(bitmask, GFX::BT_VIEWPORT_ID, 0);
		GFX::SetBitmaskValue(bitmask, GFX::BT_LAYER, 0);
		GFX::SetBitmaskValue(bitmask, GFX::BT_TRANSLUCENCY_TYPE, 0);
		GFX::SetBitmaskValue(bitmask, GFX::BT_MESH_ID, 0);
		GFX::SetBitmaskValue(bitmask, GFX::BT_MATERIAL_ID, 0);
		GFX::SetBitmaskValue(bitmask, GFX::BT_DEPTH, 0);

		ASSERT_EQ(bitmask, 0x0000000000000000ULL);
		
		GFX::SetBitmaskValue(bitmask, GFX::BT_OBJECT_TYPE, 1);
		GFX::SetBitmaskValue(bitmask, GFX::BT_VIEWPORT_ID, 1);
		GFX::SetBitmaskValue(bitmask, GFX::BT_LAYER, 1);
		GFX::SetBitmaskValue(bitmask, GFX::BT_TRANSLUCENCY_TYPE, 1);
		GFX::SetBitmaskValue(bitmask, GFX::BT_MESH_ID, 1);
		GFX::SetBitmaskValue(bitmask, GFX::BT_MATERIAL_ID, 1);
		GFX::SetBitmaskValue(bitmask, GFX::BT_DEPTH, 1);
		
		ASSERT_EQ(bitmask, 0x4940008001000001ULL);

		GFX::SetBitmaskValue(bitmask, GFX::BT_MESH_ID, 1337);
		GFX::SetBitmaskValue(bitmask, GFX::BT_OBJECT_TYPE, 2);
		GFX::SetBitmaskValue(bitmask, GFX::BT_DEPTH, 4200);
		GFX::SetBitmaskValue(bitmask, GFX::BT_VIEWPORT_ID, 5);
		GFX::SetBitmaskValue(bitmask, GFX::BT_LAYER, 6);
		GFX::SetBitmaskValue(bitmask, GFX::BT_TRANSLUCENCY_TYPE, 2);
		GFX::SetBitmaskValue(bitmask, GFX::BT_MATERIAL_ID, 9001);
		
		unsigned int value;
		value = GFX::GetBitmaskValue(bitmask, GFX::BT_OBJECT_TYPE);
		ASSERT_EQ(value, 2);
		value = GFX::GetBitmaskValue(bitmask, GFX::BT_VIEWPORT_ID);
		ASSERT_EQ(value, 5);
		value = GFX::GetBitmaskValue(bitmask, GFX::BT_LAYER);
		ASSERT_EQ(value, 6);
		value = GFX::GetBitmaskValue(bitmask, GFX::BT_TRANSLUCENCY_TYPE);
		ASSERT_EQ(value, 2);
		value = GFX::GetBitmaskValue(bitmask, GFX::BT_MESH_ID);
		ASSERT_EQ(value, 1337);
		value = GFX::GetBitmaskValue(bitmask, GFX::BT_MATERIAL_ID);
		ASSERT_EQ(value, 9001);
		value = GFX::GetBitmaskValue(bitmask, GFX::BT_DEPTH);
		ASSERT_EQ(value, 4200);

	}
}

#endif