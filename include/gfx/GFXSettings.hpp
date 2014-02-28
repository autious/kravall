#ifndef GFX_SETTINGS_HPP
#define GFX_SETTINGS_HPP
namespace GFX
{
	enum GFX_SETTING
	{
		GFX_SHADOW_RESOLUTION = 0,
		GFX_SHADOW_QUALITY,

		GFX_SETTINGS_COUNT
	};
		

	const int GFX_SHADOWS_BASIC			= 0;
	const int GFX_SHADOWS_VARIANCE		= 1;
	const int GFX_SHADOWS_DISABLED		= 3;

	const int GFX_AA_2X 				= 0;
	const int GFX_AA_4X					= 1;
	const int GFX_AA_8X					= 2;
	const int GFX_AA_16X				= 3;
	const int GFX_AA_DISABLED			= 4;

	const int GFX_SSAO_LOW				= 0;
	const int GFX_SSAO_MED				= 1;
	const int GFX_SSAO_HIGH				= 2;
	const int GFX_SSAO_HBAO				= 3;
	const int GFX_SSAO_DISABLED			= 4;

	const int GFX_TEXTURES_LOW			= 0;
	const int GFX_TEXTURES_MED			= 1;
	const int GFX_TEXTURES_HIGH			= 2;

	const int GFX_PARTICLES_LOW			= 0;
	const int GFX_PARTICLES_MED			= 1;
	const int GFX_PARTICLES_HIGH		= 2;
	const int GFX_PARTICLES_VERY_HIGH	= 3;
	const int GFX_PARTICLES_EXTREME		= 4;
	const int GFX_PARTICLES_DISABLED	= 5;
}

#endif