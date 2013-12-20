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
        
        float color[3];
        float intensity;
        
        union
        {
            struct 
            {
                float angle;
            } spotLight;
        } lightSpecific;

		LightComponent()
		{
			bitmask = 0;
			type = GFX::LIGHT_TYPES::POINT;
			color[0] = 0;
			color[1] = 0;
			color[2] = 0;
            intensity = 0;
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
