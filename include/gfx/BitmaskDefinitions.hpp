#ifndef BITMASK_DEFINITIONS_HPP
#define BITMASK_DEFINITIONS_HPP

namespace GFX
{
	typedef unsigned long long int GFXBitmask;

	namespace BITMASK
	{
		static const GFXBitmask TYPE = 0xFF00000000000000ULL;
		static const GFXBitmask VIEWPORT_ID = 0x00E0000000000000ULL;
		static const GFXBitmask LAYER = 0x001F000000000000ULL;
		static const GFXBitmask TRANSLUCENCY_TYPE = 0x0000C00000000000ULL;
		static const GFXBitmask MESH_ID = 0x00003FFF80000000ULL;
		static const GFXBitmask LIGHT_TYPE = MESH_ID;
		static const GFXBitmask MATERIAL_ID = 0x000000007FFF0000ULL;
		static const GFXBitmask DEPTH = 0x000000000000FFFFULL;
	}

	namespace LAYER_TYPES
	{
		static const unsigned int MESH_LAYER = 3;
		static const unsigned int OUTLINE_LAYER = 2;
	}
	
	namespace OBJECT_TYPES
	{
		static const unsigned int OPAQUE_GEOMETRY = 3;
        static const unsigned int PARTICLE_GEOMETRY = 4;
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
			// Type of object to draw: Mesh/pointlight/spotlight/directional light [8b]
		case GFX::BITMASK::TYPE:
			bitmask |= (GFX::GFXBitmask(value) << 56) & GFX::BITMASK::TYPE;
			break;

			// Viewport ID [3b]
		case GFX::BITMASK::VIEWPORT_ID:
			bitmask |= (GFX::GFXBitmask(value) << 53) & GFX::BITMASK::VIEWPORT_ID;
			break;

			// Layer [5b]
		case GFX::BITMASK::LAYER:
			bitmask |= (GFX::GFXBitmask(value) << 48) & GFX::BITMASK::LAYER;
			break;

			// Translucency type: opaque/add/sub/mul [2b]
		case GFX::BITMASK::TRANSLUCENCY_TYPE:
			bitmask |= (GFX::GFXBitmask(value) << 46) & GFX::BITMASK::TRANSLUCENCY_TYPE;
			break;

			// Mesh ID [15b]
		case GFX::BITMASK::MESH_ID:
			bitmask |= (GFX::GFXBitmask(value) << 31) & GFX::BITMASK::MESH_ID;
			break;

			// Material ID [15b]
		case GFX::BITMASK::MATERIAL_ID:
			bitmask |= (GFX::GFXBitmask(value) << 16) & GFX::BITMASK::MATERIAL_ID;
			break;

			// Depth [16b]
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
			// Type of object to draw: Mesh/pointlight/spotlight/directional light [8b]
		case GFX::BITMASK::TYPE:
			value = static_cast<unsigned int>((bitmask & GFX::BITMASK::TYPE) >> (64 - 8));
			break;

			// Viewport ID [3b]
		case GFX::BITMASK::VIEWPORT_ID:
			value = static_cast<unsigned int>((bitmask & GFX::BITMASK::VIEWPORT_ID) >> (64 - 8 - 3));
			break;

			// Layer [5b]
		case GFX::BITMASK::LAYER:
			value = static_cast<unsigned int>((bitmask & GFX::BITMASK::LAYER) >> (64 - 8 - 3 - 5));
			break;

			// Translucency type: opaque/add/sub/mul [2b]
		case GFX::BITMASK::TRANSLUCENCY_TYPE:
			value = static_cast<unsigned int>((bitmask & GFX::BITMASK::TRANSLUCENCY_TYPE) >> (64 - 8 - 3 - 5 - 2));
			break;

			// Mesh ID [15b]
		case GFX::BITMASK::MESH_ID:
			value = static_cast<unsigned int>((bitmask & GFX::BITMASK::MESH_ID) >> (64 - 8 - 3 - 5 - 2 - 15));
			break;

			// Material ID [15b]
		case GFX::BITMASK::MATERIAL_ID:
			value = static_cast<unsigned int>((bitmask & GFX::BITMASK::MATERIAL_ID) >> (64 - 8 - 5 - 3 - 2 - 15 - 15));
			break;

			// Depth [16b]
		case GFX::BITMASK::DEPTH:
			value = static_cast<unsigned int>((bitmask & GFX::BITMASK::DEPTH) >> (64 - 8 - 3 - 5 - 2 - 15 - 15 - 16));
			break;
		}
		return value;
	}
}
#endif
