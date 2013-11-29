#ifndef GFXINTERFACE_INCLUDE_HPP
#define GFXINTERFACE_INCLUDE_HPP

#ifdef _WIN32
#ifdef GFX_DLL_EXPORT
#define DLL_API __declspec(dllexport)
#else
#define DLL_API __declspec(dllimport)
#endif
#else
#define DLL_API 
#endif

#include <glm/glm.hpp>

#define GFX_SUCCESS 0
#define GFX_FAIL 1

typedef glm::vec2 GFXVec2;
typedef glm::vec3 GFXVec3;
typedef glm::vec4 GFXVec4;
typedef glm::mat4x4 GFXMat4x4;

typedef GFXVec4 GFXColor;

namespace GFX
{


	/*!
	Initializes the graphics engine on the currently bound context.
	\return Returns #GFX_SUCCESS if successful, else returns #GFX_FAIL
	*/
	DLL_API int Init(int windowWidth, int windowHeight);

	/*!
	Executes all draw calls made to the graphics engine and
	renders it to the screen.
	*/
	DLL_API void Render();

	/*!
	Resizes the graphics to render on with specified dimensions.
	\param width Width to use for rendering
	\param height Height to use for rendering
	*/
	DLL_API void Resize(int width, int height);

	/*!
	Sets the view matrix used by the main camera.
	\param matrix Pointer to a 4x4 matrix
	*/
	DLL_API void SetViewMatrix(GFXMat4x4 matrix);

	/*!
	Sets the projection matrix used by the main camera.
	\param matrix Pointer to a 4x4 matrix
	*/
	DLL_API void SetProjectionMatrix(GFXMat4x4 matrix);



	//-----##----------##----####----########------//
	//-----##----##----##-----##-----##-----##-----//
	//-----##---####---##-----##-----########------//
	//------##-##--##-##------##-----##------------//
	//-------###----###------####----##------------//
	//Functions below are not specified yet, and thus cannot be implemented

	/*!
	Issues a draw command to the graphics engine. 
	\param bitmask The bitmask containing the type of draw call to be queued.
	\param data A pointer to the data used for rendering
	*/
	DLL_API void Draw(unsigned int bitmask, void* data);


	namespace Debug
	{

		/*!
		Draws a point on the screen.
		\param point The screen space position where to draw the point.
		\param color The color of the point
		*/
		DLL_API void DrawPoint(GFXVec2 point, GFXColor color);

		/*!
		Draws a point on the screen.
		\param point The screen space position where to draw the point.
		\param color The color of the point
		\param size Size of the point
		*/
		DLL_API void DrawPoint(GFXVec2 point, GFXColor color, float size);

		/*!
		Draws a line on the screen.
		\param p1 World space position of the starting point of the line
		\param p2 World space position of the end point of the line
		\param color Color of the line
		*/
		DLL_API void DrawLine(GFXVec3 p1, GFXVec3 p2, GFXColor color);

		/*!
		Draws a line on the screen.
		\param p1 The world space position of the starting point of the line
		\param p2 The world space position of the end point of the line
		\param color Color of the line
		\param thickness Thickness of the line
		*/
		DLL_API void DrawLine(GFXVec3 p1, GFXVec3 p2, GFXColor color, float thickness);

		/*!
		Draws a line on the screen.
		\param p1 Screen space position of the starting point of the line
		\param p2 Screen space position of the end point of the line
		\param color Color of the line
		*/
		DLL_API void DrawLine(GFXVec2 p1, GFXVec2 p2, GFXColor color);

		/*!
		Draws a line on the screen.
		\param p1 The screen space position of the starting point of the line
		\param p2 The screen space position of the end point of the line
		\param color Color of the line
		\param thickness Thickness of the line
		*/
		DLL_API void DrawLine(GFXVec2 p1, GFXVec2 p2, GFXColor color, float thickness);

		/*!
		Draws a rectangle on the screen.
		\param position The screen space position for the top left corner
		\param dimensions Rectangle dimensions
		\param solid If true, the rectangle will be filled, else only outlines will be shown
		\param color Color of the rectangle
		*/
		DLL_API void DrawRectangle(GFXVec2 position, GFXVec2 dimensions, bool solid, GFXColor color);

		/*!
		Draws an axis-aligned box on the screen.
		\param position The world space position for the center of the box
		\param dimensions Box dimensions
		\param solid If true, the box will be filled, else only outlines will be shown
		\param color Color of the rectangle
		*/
		DLL_API void DrawBox(GFXVec3 position, GFXVec3 dimensions, bool solid, GFXColor color);

		/*!
		Draws a sphere on the screen.
		\param position World space position for the center of the sphere
		\param radius Sphere radius
		\param color Color of the sphere
		*/
		DLL_API void DrawSphere(GFXVec3 position, float radius, GFXColor color);

		/*!
		Draws a screen space circle on the screen.
		\param position Screen space position for the center of the circle
		\param radius Circle radius
		\param lineWidth Width of the outline line, the circle will be filled if this value is 0
		\param color Color of the circle
		*/
		DLL_API void DrawCircle(GFXVec2 position, float radius, unsigned int lineWidth, GFXColor color);
	} // namespace Debug

	namespace Settings
	{
		enum GFXAAMode
		{
			AA_2X = 0,
			AA_4X,
			AA_8X,
			AA_16X,
			AA_DISABLED
		};

		enum GFXSSAOMode
		{
			SSAO_LOW = 0,
			SSAO_MED,
			SSAO_HIGH,
			SSAO_HBAO,
			SSAO_DISABLED
		};

		enum GFXShadowQuality
		{
			SHADOWS_LOW = 0,
			SHADOWS_MED,
			SHADOWS_HIGH,
			SHADOWS_DISABLED
		};

		enum GFXTextureQuality
		{
			TEXTURES_LOW = 0,
			TEXTURES_MED,
			TEXTURES_HIGH
		}; 
		
		enum GFXParticleDensity
		{
			PARTICLES_LOW = 0,
			PARTICLES_MED,
			PARTICLES_HIGH,
			PARTICLES_VERY_HIGH,
			PARTICLES_EXTREME,
			PARTICLES_DISABLED
		};

		/*!
		Sets antialiasing.
		\param mode Anti-Aliasing mode to use.
		*/
		DLL_API void SetAntiAliasing(GFXAAMode mode);

		/*!
		Enables or disables anisotropic filtering.
		\param enabled If true, anisotropic is set to ON, else anisotropic filtering is disabled
		*/
		DLL_API void SetAnisotropic(bool enabled);

		/*!
		Sets SSAO mode.
		\param mode SSAO mode to use
		*/
		DLL_API void SetSSAO(GFXSSAOMode mode);

		/*!
		Sets the shadowmapping quality.
		\param quality The quality of shadow maps
		*/
		DLL_API void SetShadowQuality(GFXShadowQuality quality);

		/*!
		Sets texture quality. Lower quality should only use lower mip levels of textures.
		\param quality The texture quality to be used
		*/
		DLL_API void SetTextureQuality(GFXTextureQuality quality);

		/*!
		Sets the density of particles.
		\param density Density to use when particles are rendered
		*/
		DLL_API void SetParticleDensity(GFXParticleDensity density);

		/*!
		Enables or disables High Dynamic Range shading. 
		\param enabled If true, HDR is set to ON, else HDR is disabled
		*/
		DLL_API void SetHDR(bool enabled);

		/*!
		Enables or disables Depth of Field shading. 
		\param enabled If true, Depth of Field will be enabled, else Depth of Field is disabled
		*/
		DLL_API void SetDoF(bool enabled);

	} // namespace Settings

} // namespace GFX

#endif //GFXINTERFACE_INCLUDE_HPP