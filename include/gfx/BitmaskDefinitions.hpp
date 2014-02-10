#ifndef BITMASK_DEFINITIONS_HPP
#define BITMASK_DEFINITIONS_HPP

namespace GFX
{
	typedef unsigned long long int GFXBitmask;

	namespace BITMASK
	{
		static const GFXBitmask TYPE = 0xC000000000000000ULL;
		static const GFXBitmask VIEWPORT_ID = 0x3800000000000000ULL;
		static const GFXBitmask LAYER = 0x0700000000000000ULL;
		static const GFXBitmask TRANSLUCENCY_TYPE = 0x00C0000000000000ULL;
		static const GFXBitmask MESH_ID = 0x003FFF8000000000ULL;
		static const GFXBitmask LIGHT_TYPE = MESH_ID;
		static const GFXBitmask MATERIAL_ID = 0x0000007FFF000000ULL;
		static const GFXBitmask DEPTH = 0x0000000000FFFFFFULL;
	}

	namespace LAYER_TYPES
	{
		static const unsigned int MESH_LAYER = 3;
		static const unsigned int OUTLINE_LAYER = 2;
		static const unsigned int DECAL_LAYER = 1;
	}
	
	namespace OBJECT_TYPES
	{
		static const unsigned int OPAQUE_GEOMETRY = 3;
		static const unsigned int LIGHT = 2;
		static const unsigned int TRANSPARENT_GEOMETRY = 1;
		static const unsigned int OVERLAY_GEOMETRY = 0;
	}

	namespace LIGHT_TYPES
	{
		static const unsigned int POINT_SHADOW = 6;
		static const unsigned int SPOT_SHADOW = 5;
		static const unsigned int DIR_SHADOW = 4;
		static const unsigned int POINT = 3;
		static const unsigned int SPOT = 2;
		static const unsigned int DIR = 1;
		static const unsigned int AMBIENT = 0;

		static const unsigned int COUNT = 7;
	}

	inline static void SetBitmaskValue(GFX::GFXBitmask& bitmask, GFX::GFXBitmask type, unsigned int value)
	{
		// Clear bits at mask
		bitmask &= ~type;
		switch (type)
		{
			// Type of object to draw: Mesh/pointlight/spotlight/directional light [2b]
		case GFX::BITMASK::TYPE:
			bitmask |= (GFX::GFXBitmask(value) << 62) & GFX::BITMASK::TYPE;
			break;

			// Viewport ID [3b]
		case GFX::BITMASK::VIEWPORT_ID:
			bitmask |= (GFX::GFXBitmask(value) << 59) & GFX::BITMASK::VIEWPORT_ID;
			break;

			// Layer [3b]
		case GFX::BITMASK::LAYER:
			bitmask |= (GFX::GFXBitmask(value) << 56) & GFX::BITMASK::LAYER;
			break;

			// Translucency type: opaque/add/sub/mul [2b]
		case GFX::BITMASK::TRANSLUCENCY_TYPE:
			bitmask |= (GFX::GFXBitmask(value) << 54) & GFX::BITMASK::TRANSLUCENCY_TYPE;
			break;

			// Mesh ID [15b]
		case GFX::BITMASK::MESH_ID:
			bitmask |= (GFX::GFXBitmask(value) << 39) & GFX::BITMASK::MESH_ID;
			break;

			// Material ID [15b]
		case GFX::BITMASK::MATERIAL_ID:
			bitmask |= (GFX::GFXBitmask(value) << 24) & GFX::BITMASK::MATERIAL_ID;
			break;

			// Depth [24b]
		case GFX::BITMASK::DEPTH:
			bitmask |= (GFX::GFXBitmask(value) << 0) & GFX::BITMASK::DEPTH;
			break;
		}
	}

	inline static unsigned int GetBitmaskValue(const GFX::GFXBitmask& bitmask, GFX::GFXBitmask type)
	{
		unsigned int value = 0;
		switch (type)
		{
			// Type of object to draw: Mesh/pointlight/spotlight/directional light [2b]
		case GFX::BITMASK::TYPE:
			value = static_cast<unsigned int>((bitmask & GFX::BITMASK::TYPE) >> (64 - 2));
			break;

			// Viewport ID [3b]
		case GFX::BITMASK::VIEWPORT_ID:
			value = static_cast<unsigned int>((bitmask & GFX::BITMASK::VIEWPORT_ID) >> (64 - 2 - 3));
			break;

			// Layer [3b]
		case GFX::BITMASK::LAYER:
			value = static_cast<unsigned int>((bitmask & GFX::BITMASK::LAYER) >> (64 - 2 - 3 - 3));
			break;

			// Translucency type: opaque/add/sub/mul [2b]
		case GFX::BITMASK::TRANSLUCENCY_TYPE:
			value = static_cast<unsigned int>((bitmask & GFX::BITMASK::TRANSLUCENCY_TYPE) >> (64 - 2 - 3 - 3 - 2));
			break;

			// Mesh ID [15b]
		case GFX::BITMASK::MESH_ID:
			value = static_cast<unsigned int>((bitmask & GFX::BITMASK::MESH_ID) >> (64 - 2 - 3 - 3 - 2 - 15));
			break;

			// Material ID [15b]
		case GFX::BITMASK::MATERIAL_ID:
			value = static_cast<unsigned int>((bitmask & GFX::BITMASK::MATERIAL_ID) >> (64 - 2 - 3 - 3 - 2 - 15 - 15));
			break;

			// Depth [24b]
		case GFX::BITMASK::DEPTH:
			value = static_cast<unsigned int>((bitmask & GFX::BITMASK::DEPTH) >> (64 - 2 - 3 - 3 - 2 - 15 - 15 - 24));
			break;
		}
		return value;
	}
}
#endif
