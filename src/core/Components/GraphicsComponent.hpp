#ifndef SRC_CORE_COMPONENTS_GRAPHICS_COMPONENT_HPP
#define SRC_CORE_COMPONENTS_GRAPHICS_COMPONENT_HPP

#include <gfx/BitmaskDefinitions.hpp>

namespace Core
{
	struct GraphicsComponent
	{
		GFX::GFXBitmask bitmask;
        bool render;

		float outlineColor[4];

		static GraphicsComponent GC()
		{
			GraphicsComponent gc;
            gc.render = true;
			gc.outlineColor[0] = 0.0f;
			gc.outlineColor[1] = 0.0f;
			gc.outlineColor[2] = 0.0f;
			gc.outlineColor[3] = 0.0f;
			return gc;
		}

        inline static const char* GetName()
        {
            return "GraphicsComponent";
        }
	};
}

#endif
