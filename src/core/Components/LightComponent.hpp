#ifndef SRC_CORE_COMPONENTS_LIGHT_COMPONENT_HPP
#define SRC_CORE_COMPONENTS_LIGHT_COMPONENT_HPP

#include <gfx/BitmaskDefinitions.hpp>
#include <gfx/Lights.hpp>

namespace Core
{
	struct LightComponent
	{
		GFX::GFXBitmask bitmask;
        
        float color[3];
        float intensity;
        
        union
        {
            struct 
            {
                float angle;
                float penumbraAngle;
            } spotLight;
        } lightSpecific;

		LightComponent()
		{
			bitmask = 0;
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

        inline static const unsigned int GetLightType( const LightComponent &lc )
        {
            return GFX::GetBitmaskValue( lc.bitmask, GFX::BITMASK::LIGHT_TYPE );
        }

        inline static const char* GetName()
        {
            return "LightComponent";
        }
	};
}

#endif
