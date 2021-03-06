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
    class Particle;
}

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
	DLL_API void SetProjectionMatrix(GFXMat4x4 matrix, float nearZ, float farZ);

	/*!
	Sets the view matrix used by the overlay cam
	\param matrix Pointer to a 4x4 matrix
	*/
	DLL_API void SetOverlayViewMatrix(GFXMat4x4 matrix);

	/*!
	Sets the projection matrix used by the overlay cam
	\param matrix Pointer to a 4x4 matrix
	*/
	DLL_API void SetOverlayProjectionMatrix(GFXMat4x4 matrix);

	/*!
	Issues a draw command to the graphics engine. 
	\param bitmask The bitmask containing the type of draw call to be queued.
	\param data A pointer to the data used for rendering
	*/
	DLL_API void Draw(GFXBitmask bitmask, void* data);

	/*!
	Sets the values needed to draw the selection box.
	\param posDim This is a vec4 containing position and dimension (x, y, w, h)
	\param color The color of the selectionbox
	*/
	DLL_API void DrawSelectionbox(const glm::vec4& posDim, const GFXColor& color );

	/*!
	Draws a rectangle on the screen.
	\param position The screen space position for the top left corner
	\param dimensions Rectangle dimensions
	\param solid If true, the rectangle will be filled, else only outlines will be shown
	\param color Color of the rectangle
	*/
	DLL_API void DrawFilledRect(GFXVec2 position, GFXVec2 dimensions, GFXColor color);


	/*!
	Issues a draw  text command to the graphics engine.
	\param position Position of the starting letter
	\param size Vertical size of each letter
	\param color The color of the text
	\param text The text to be rendered
	*/
	DLL_API void RenderText(GFX::FontData* fontData, GFXVec2 position, float size, GFXVec4 color, const char* text);

	/*!
	Issues a draw  text command to the graphics engine.
	\param rectangle Rectangle (x, y, w, h) in which to draw the text
	\param offset Scroll offset in pixels, if offset is zero, the text is scrolled all the way to the top
	\param size Vertical size of each letter
	\param color The color of the text
	\param text The text to be rendered
	*/
	DLL_API void RenderTextbox(GFX::FontData* fontData, GFXVec4 rectangle, float offset, float size, GFXVec4 color, const char* text);

	/*!
	Returns the actual width and height of a textbox of specified size
	\param width The width of the textbox after which text is wrapped
	\param size Vertical size of each letter
	\param text The text to use
	*/
	DLL_API void GetActualTextboxSize(GFX::FontData* fontData, float width, float size, const char* text, float& out_actualWidth, float& out_actualHeight);

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
		*/
		DLL_API void LoadTexture2DFromMemory(unsigned int& out_id, unsigned char* data, int width, int height, bool decal);

		/*!
		Deletes a texture from the GPU
		\param textureHandle The handle of the texture to be deleted
		*/
		DLL_API void DeleteTexture(unsigned int id);

		/*!
		Loads a mesh to the GPU
		\param meshID Reference to the id created for the mesh
		\param sizeVerts Number of vertices in the mesh
		\param sizeIndices Number of indices in the mesh
		\param verts Array of vertices to use for the mesh
		\param indices Array of indices to use for the mesh
		*/
		DLL_API void LoadMesh(unsigned int& meshID, int& sizeVerts, int& sizeIndices, GFX::Vertex* verts, int* indices);
		
		/*!
		Creates a skeleton on the GPU
		\param out_skeletonID Reference returning the id created for the skeleton
		\return Returns #GFX_FAIL if unable to create skeleton, else returns #GFX_SUCCESS
		*/
		DLL_API int CreateSkeleton(int& out_skeletonID);

		/*!
		Deletes a skeleton, removing its animations
		\param skeletonID The id of the skeleton to delete
		\return Returns #GFX_FAIL if unable to remove skeleton, else returns #GFX_SUCCESS
		*/
		DLL_API int DeleteSkeleton(const int& skeletonID);

		/*!
		Binds a skeleton to a mesh
		\param meshID The mesh to bind the skeleton to
		\param skeletonID The id of the skeleton which to bind to the mesh
		\return Returns #GFX_FAIL if unable bind skeleton to mesh, else returns #GFX_SUCCESS
		*/
		DLL_API int BindSkeletonToMesh(const unsigned int& meshID, const int& skeletonID);

		/*!
		Gets the skeleton ID linked to the mesh
		*/
		DLL_API int GetSkeletonID(const unsigned int& meshID);

		/*!
		Adds an animation to a skeleton. The data must contain the same number of bones for all frames
		\param skeletonID Reference id of the skeleton to bind the animation to
		\param frames Array of bone matrices sorted by frame
		\param numFrames Number of bone matrices in the array
		\param numBonesPerFrame Number of bone matrices per frame
		\return Returns the animation ID if successful, else returns #GFX_INVALID_ANIMATION or #GFX_INVALID_SKELETON
		*/
		DLL_API int AddAnimationToSkeleton(const int& skeletonID, GFXMat4x4* frames, const unsigned int& numFrames, const unsigned int& numBonesPerFrame);
		
		/*!
		Gets the info for a particular animation.
		\param skeletonID The id of the skeleton
		\param animationID The id of the animation
		\param out_frameCount The number of frames in this animation
		\return Returns #GFX_SUCCESS if successful, else returns #GFX_INVALID_ANIMATION or #GFX_INVALID_SKELETON
		*/
		DLL_API int GetAnimationInfo(const int& skeletonID, const int& animationID, unsigned int& out_frameCount, unsigned int& out_bonesPerFrame, unsigned int& out_animationOffset);
		
		/*!
		Deletes a mesh
		\param meshID The id of the mesh to delete
		*/
		DLL_API void DeleteMesh(unsigned int& meshID);

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

        /*!
        Creates a buffer that stores particles.
        \param bufferId Out parameter that will be assigned the id of the particle buffer.
        \param particleCount The number of particles that the buffer will hold.
        */
        DLL_API void CreateParticleBuffer(unsigned int& bufferId, unsigned int particleCount);

        /*!
        Deletes the specified particle buffer.
        \param bufferId The Id of the particle buffer to be deleted.
        */
        DLL_API void DeleteParticleBuffer(unsigned int bufferId);

        /*!
        Transfers data over to the given particle buffer.
        \param bufferId The buffer id to assign the data to.
        \param particleData The data to be set.
        \param particleCount The number of particles the data contains.
        */
        DLL_API void BufferParticleData(unsigned int bufferId, GFX::Particle* const data);

		DLL_API void ReloadLUT();
	}

	namespace Debug
	{
		/*!
		Sets whether to draw debug or not.
		\param enable If true, enables debug drawing.
		*/
		DLL_API void SetEnableDebug(bool enable);

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
		Draws a frustum
		\param cameraMatrix The matrix representing the frustum to draw
		\param color The color to draw the frustum
		\param useDepth If true, the depth buffer is used to occlude if behind an object, else draws on top of existing geometry regardless of depth
		*/
		DLL_API void DrawFrustum(GFXMat4x4 cameraMatrix, GFXColor color, bool useDepth);


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
		/*!
		Sets gamma
		\param gamma
		*/
		DLL_API void SetGamma(float gamma);

		/*!
		Gets gamma
		*/
		DLL_API float GetGamma();
		
		/*!
		Sets a setting to the specified value
		\return Returns either GFX_SUCCESS or GFX_FAIL
		*/
		DLL_API int SetConfiguration(const int setting, const int value);

		/*!
		Gets the value of a setting
		\return Returns either GFX_SUCCESS or GFX_FAIL
		*/
		DLL_API int GetConfiguration(const int setting, int& out_value);


		/*!
		Gets the animation play framerate.
		\return Returns animation play framerate
		*/
		DLL_API unsigned int GetAnimationFramerate();

		/*!
		Sets the animation play framerate.
		\param framerate The target framerate for animations (clamped between 12 and 48 fps)
		*/
		DLL_API void SetAnimationFramerate(unsigned int framerate);


	} // namespace Settings

	namespace ColorSettings
	{
		/*!
		Sets the white point which means a pixel should be full-brighted
		\param whitePoint
		*/
		DLL_API void SetWhitePoint(GFXVec3 whitePoint);

		/*!
		Sets exposure
		\param exposure
		*/
		DLL_API void SetExposure(float exposure);

		DLL_API void SetLUT(const char* LUT);
	}

} // namespace GFX

#endif //GFXINTERFACE_INCLUDE_HPP
