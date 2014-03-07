#ifndef SRC_GFX_RENDERER_RENDER_SETTINGS_HPP
#define SRC_GFX_RENDERER_RENDER_SETTINGS_HPP

#include <GFXSettings.hpp>           

namespace GFX
{
	struct RenderSettings
	{
		/* Holds settings data for the graphics system */
		static unsigned int settings[GFX_SETTINGS_COUNT];
	};
}

#endif
