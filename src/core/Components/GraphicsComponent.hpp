#ifndef SRC_CORE_COMPONENTS_GRAPHICS_COMPONENT_HPP
#define SRC_CORE_COMPONENTS_GRAPHICS_COMPONENT_HPP

#include <gfx/BitmaskDefinitions.hpp>

namespace Core
{
	struct GraphicsComponent
	{
		GFX::GFXBitmask bitmask;
        bool render;
		static GraphicsComponent GC()
		{
			GraphicsComponent gc;
            gc.render = true;
			return gc;
		}

        inline static const char* GetName()
        {
            return "GraphicsComponent";
        }
	};
}

#endif
