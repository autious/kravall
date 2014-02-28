#ifndef SRC_CORE_COMPONENTS_LIGHT_COMPONENT_HPP
#define SRC_CORE_COMPONENTS_LIGHT_COMPONENT_HPP

#include <gfx/BitmaskDefinitions.hpp>
#include <gfx/LightData.hpp>

namespace Core
{
	struct LightComponent
	{
		GFX::GFXBitmask bitmask;
        
        float color[3];
		float specColor[3];
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
			color[0] = 0.0f;
			color[1] = 0.0f;
			color[2] = 0.0f;
			specColor[0] = 0.0f;
			specColor[1] = 0.0f;
			specColor[2] = 0.0f;
            intensity = 0.0f;
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
