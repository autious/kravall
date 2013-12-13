#ifndef BITMASK_DEFINITIONS_HPP
#define BITMASK_DEFINITIONS_HPP

namespace GFX
{
	typedef unsigned long long int GFXBitmask;

	const GFXBitmask BT_OBJECT_TYPE = 0xC000000000000000ULL;
	const GFXBitmask BT_VIEWPORT_ID = 0x3800000000000000ULL;
	const GFXBitmask BT_LAYER = 0x0700000000000000ULL;
	const GFXBitmask BT_TRANSLUCENCY_TYPE = 0x00C0000000000000ULL;
	const GFXBitmask BT_SHADOWCASTER = BT_TRANSLUCENCY_TYPE;
	const GFXBitmask BT_MESH_ID = 0x003FFF8000000000ULL;
	const GFXBitmask BT_MATERIAL_ID = 0x0000007FFF000000ULL;
	const GFXBitmask BT_DEPTH = 0x0000000000FFFFFFULL;

	static void SetBitmaskValue(GFX::GFXBitmask& bitmask, GFX::GFXBitmask type, unsigned int value)
	{
		// Clear bits at mask
		bitmask &= ~type;
		switch (type)
		{
			// Type of object to draw: Mesh/pointlight/spotlight/directional light [2b]
		case GFX::BT_OBJECT_TYPE:
			bitmask |= (GFX::GFXBitmask(value) << 62) & GFX::BT_OBJECT_TYPE;
			break;

			// Viewport ID [3b]
		case GFX::BT_VIEWPORT_ID:
			bitmask |= (GFX::GFXBitmask(value) << 59) & GFX::BT_VIEWPORT_ID;
			break;

			// Layer [3b]
		case GFX::BT_LAYER:
			bitmask |= (GFX::GFXBitmask(value) << 56) & GFX::BT_LAYER;
			break;

			// Translucency type: opaque/add/sub/mul [2b]
		case GFX::BT_TRANSLUCENCY_TYPE:
			bitmask |= (GFX::GFXBitmask(value) << 54) & GFX::BT_TRANSLUCENCY_TYPE;
			break;

			// Mesh ID [15b]
		case GFX::BT_MESH_ID:
			bitmask |= (GFX::GFXBitmask(value) << 39) & GFX::BT_MESH_ID;
			break;

			// Material ID [15b]
		case GFX::BT_MATERIAL_ID:
			bitmask |= (GFX::GFXBitmask(value) << 24) & GFX::BT_MATERIAL_ID;
			break;

			// Depth [24b]
		case GFX::BT_DEPTH:
			bitmask |= (GFX::GFXBitmask(value) << 0) & GFX::BT_DEPTH;
			break;
		}
	}
	static unsigned int GetBitmaskValue(GFX::GFXBitmask& bitmask, GFX::GFXBitmask type)
	{
		unsigned int value = 0;
		switch (type)
		{
			// Type of object to draw: Mesh/pointlight/spotlight/directional light [2b]
		case GFX::BT_OBJECT_TYPE:
			value = static_cast<unsigned int>((bitmask & GFX::BT_OBJECT_TYPE) >> (64 - 2));
			break;

			// Viewport ID [3b]
		case GFX::BT_VIEWPORT_ID:
			value = static_cast<unsigned int>((bitmask & GFX::BT_VIEWPORT_ID) >> (64 - 2 - 3));
			break;

			// Layer [3b]
		case GFX::BT_LAYER:
			value = static_cast<unsigned int>((bitmask & GFX::BT_LAYER) >> (64 - 2 - 3 - 3));
			break;

			// Translucency type: opaque/add/sub/mul [2b]
		case GFX::BT_TRANSLUCENCY_TYPE:
			value = static_cast<unsigned int>((bitmask & GFX::BT_TRANSLUCENCY_TYPE) >> (64 - 2 - 3 - 3 - 2));
			break;

			// Mesh ID [15b]
		case GFX::BT_MESH_ID:
			value = static_cast<unsigned int>((bitmask & GFX::BT_MESH_ID) >> (64 - 2 - 3 - 3 - 2 - 15));
			break;

			// Material ID [15b]
		case GFX::BT_MATERIAL_ID:
			value = static_cast<unsigned int>((bitmask & GFX::BT_MATERIAL_ID) >> (64 - 2 - 3 - 3 - 2 - 15 - 15));
			break;

			// Depth [24b]
		case GFX::BT_DEPTH:
			value = static_cast<unsigned int>((bitmask & GFX::BT_DEPTH) >> (64 - 2 - 3 - 3 - 2 - 15 - 15 - 24));
			break;
		}
		return value;
	}
}
#endif
