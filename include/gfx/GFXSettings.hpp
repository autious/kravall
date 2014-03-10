#ifndef GFX_SETTINGS_HPP
#define GFX_SETTINGS_HPP
namespace GFX
{
	enum GFX_SETTING
	{
		GFX_SHADOW_RESOLUTION = 0,
		GFX_SHADOW_QUALITY,

		GFX_MIPMAPS,

		GFX_SETTINGS_COUNT
	};
		

	const int GFX_SHADOWS_VARIANCE_2C	= 0;
	const int GFX_SHADOWS_VARIANCE_4C	= 1;
	const int GFX_SHADOWS_DISABLED		= 3;
	
	const int GFX_MIPMAPS_OFF			= 0;
	const int GFX_MIPMAPS_ON			= 1;
	const int GFX_MIPMAPS_AF			= 2;
}

#endif