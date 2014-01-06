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
#include <GL/glew.h>

#include <GFXDefines.hpp>

typedef glm::vec2 GFXVec2;
typedef glm::vec3 GFXVec3;
typedef glm::vec4 GFXVec4;
typedef glm::mat4x4 GFXMat4x4;

typedef GFXVec4 GFXColor;

#include <vector>
#include <iostream>
#include <gfx/Vertex.hpp>
#include <gfx/BitmaskDefinitions.hpp>
#include <gfx/FontData.hpp>

namespace GFX
{
	DLL_API void test(std::vector<float>* t);

	/*!
	Initializes the graphics engine on the currently bound context.
	\return Returns #GFX_SUCCESS if successful, else returns #GFX_FAIL
	*/
	DLL_API int Init(int windowWidth, int windowHeight);

	/*!
	Executes all draw calls made to the graphics engine and
	renders it to the screen.
	*/
	DLL_API void Render(const double& delta);

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

	/*!
	Issues a draw command to the graphics engine. 
	\param bitmask The bitmask containing the type of draw call to be queued.
	\param data A pointer to the data used for rendering
	*/
	DLL_API void Draw(GFXBitmask bitmask, void* data);

	/*!
	Issues a draw  text command to the graphics engine.
	\param position Position of the starting letter
	\param size Vertical size of each letter
	\param color The color of the text
	\param text The text to be rendered
	*/
	DLL_API void RenderText(GFX::FontData* fontData, GFXVec2 position, float size, GFXVec4 color, const char* text);

	/*!
	Shows the console window
	*/
	DLL_API void ShowConsole();
	
	/*!
	Hides the console window
	*/
	DLL_API void HideConsole();

	/*!
	Toggles the console window
	*/
	DLL_API void ToggleConsole();

	/*!
	Is this magic?
	\param renderSplash If true, the splash screen will render at start
	*/
	DLL_API void RenderSplash(bool renderSplash);

	/*!
	Deletes all of the dynamically allocated memory in GFX
	*/
	DLL_API void DeleteGFX();
	
	/*!
	\return Returns the screen width
	*/
	DLL_API int GetScreenWidth();

	/*!
	\return Returns the screen height
	*/
	DLL_API int GetScreenHeight();

	namespace Content
	{
		/*!
		Loads a 2D RGBA texture onto the GPU
		\param out_id Reference to the id created for the texture
		\param data Texture data
		\param width Width of the texture
		\param height Height of the texture
		\return Handle of the texture
		*/
		DLL_API void LoadTexture2DFromMemory(unsigned int& out_id, unsigned char* data, int width, int height);

		/*!
		Deletes a texture from the GPU
		\param textureHandle The handle of the texture to be deleted
		*/
		DLL_API void DeleteTexture(unsigned int id);

		DLL_API void LoadStaticMesh(unsigned int& meshID, int& sizeVerts, int& sizeIndices, GFX::StaticVertex* verts, int* indices);

		DLL_API void DeleteStaticMesh(unsigned int& meshID);

		/*!
		Creates an empty material
		\param out_id Reference to set material id
		*/
		DLL_API void CreateMaterial(unsigned long long int& out_id);
		
		/*!
		Deletes a material
		\param id The id of the material to remove
		*/
		DLL_API void DeleteMaterial(const unsigned long long int& id);
		
		/*!
		Adds a texture to a material
		\param materialID Id to material to attach texture to
		\param textureID Id of texture to attach
		\return Returns #GFX_SUCCESS, #GFX_INVALID_MATERIAL
		*/
		DLL_API int AddTextureToMaterial(const unsigned long long int& materialID, const unsigned long long int& textureID);
		
		/*!
		Adds a texture to a material
		\param materialID Id to material where the texture is attached
		\param textureID Id of texture to detach
		*/
		DLL_API void RemoveTextureFromMaterial(const unsigned long long int& materialID, const unsigned long long int& textureID);
	
        /*!
        Gets the shader id of the shader specified by the null terminated string.
        \param shaderId Reference to set shader id.
        \param shaderName The identifying string of the shader.
		\return Returns #GFX_SUCCESS or #GFX_INVALID_SHADER
        */
        DLL_API int GetShaderId(unsigned int& shaderId, const char* shaderName);

		/*!
		Sets a shader for a material
		\param materialID Id to material to attach shader to
		\param textureID Id of shader to attach
		\return Returns #GFX_SUCCESS, #GFX_INVALID_MATERIAL
		*/
		DLL_API int AttachShaderToMaterial(const unsigned long long int& materialID, const unsigned int& shaderID);


	}

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
		\param useDepth If true, the depth buffer is used to occlude if behind an object, else draws on top of existing geometry regardless of depth
		*/
		DLL_API void DrawLine(GFXVec3 p1, GFXVec3 p2, GFXColor color, bool useDepth);

		/*!
		Draws a line on the screen.
		\param p1 The world space position of the starting point of the line
		\param p2 The world space position of the end point of the line
		\param color Color of the line
		\param thickness Thickness of the line
		\param useDepth If true, the depth buffer is used to occlude if behind an object, else draws on top of existing geometry regardless of depth
		*/
		DLL_API void DrawLine(GFXVec3 p1, GFXVec3 p2, GFXColor color, float thickness, bool useDepth);

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
		\param useDepth If true, the depth buffer is used to occlude if behind an object, else draws on top of existing geometry regardless of depth
		*/
		DLL_API void DrawBox(GFXVec3 position, GFXVec3 dimensions, bool solid, GFXColor color, bool useDepth);

		/*!
		Draws a representation of a sphere on the screen.
		\param position World space position for the center of the sphere
		\param radius Sphere radius
		\param color Color of the sphere
		\param useDepth If true, the depth buffer is used to occlude if behind an object, else draws on top of existing geometry regardless of depth
		*/
		DLL_API void DrawSphere(GFXVec3 position, float radius, GFXColor color, bool useDepth);

		/*!
		Draws a screen space circle on the screen.
		\param position Screen space position for the center of the circle
		\param radius Circle radius
		\param lineWidth Width of the outline line, the circle will be filled if this value is 0
		\param color Color of the circle
		*/
		DLL_API void DrawCircle(GFXVec2 position, float radius, unsigned int lineWidth, GFXColor color);


        /*!
        Sets the font used for rendering the time statistics.
        \param font Poitner to GFX::FontData struct used for rendering. 
        */
        DLL_API void SetStatisticsFont(GFX::FontData* font);

		/*!
		Enables or disables display of gfx system info
		\param enabled If true, toggles graphics system info to show on the screen, false disables it
		*/
		DLL_API void DisplaySystemInfo(bool enabled);
		
		/*!
		Enables or disables display of frame buffers as miniatures
		\param which Which rendertarget to show -1: disabled, 0: miniatures, 1-4: displays target 1-4 as full screen
		*/
		DLL_API void DisplayFBO(int which);

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
