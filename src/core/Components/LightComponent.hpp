#ifndef SRC_CORE_COMPONENTS_LIGHT_COMPONENT_HPP
#define SRC_CORE_COMPONENTS_LIGHT_COMPONENT_HPP

#include <gfx/BitmaskDefinitions.hpp>
#include <gfx/Lights.hpp>

namespace Core
{
	struct LightComponent
	{
		GFX::GFXBitmask bitmask;
		unsigned int type;
		void* LightData;

		LightComponent()
		{
			bitmask = 0;
			type = GFX::LIGHT_TYPES::POINT;
			LightData = 0;
		}

		static LightComponent GC()
		{
			LightComponent gc;
			return gc;
		}

        inline static const char* GetName()
        {
            return "LightComponent";
        }
	};
}

#endif
