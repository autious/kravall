#ifndef SRC_GFX_RENDERER_RENDER_CORE_HPP
#define SRC_GFX_RENDERER_RENDER_CORE_HPP

#include <Shaders/ShaderManager.hpp>
#include <Buffers/UniformBufferManager.hpp>
#include <Animation/AnimationManagerGFX.hpp>

#include "DeferredRenderer/FBOTexture.hpp"
#include "DeferredRenderer/DeferredPainter.hpp"
#include "DeferredRenderer/LightPainter.hpp"

#include "Console/ConsolePainter.hpp"
#include "OverlayRenderer/OverlayPainter.hpp"
#include "OverlayRenderer/BoxPainter.hpp"
#include "DebugRenderer/DebugPainter.hpp"
#include "TextRenderer/TextPainter.hpp"
#include "SplashRenderer/SplashPainter.hpp"
#include "FBORenderer/FBOPainter.hpp"
#include "PostProcessing/PostProcessingPainter.hpp"
#include "GlobalIlluminationRenderer/GIPainter.hpp"
#include "PostProcessing/BlurPainter.hpp"
#include "DeferredRenderer/ShadowPainter.hpp"

#include "TextRenderer/TextManager.hpp"
#include "DebugRenderer/DebugManager.hpp"
#include "RenderJobManager.hpp"
#include "../Buffers/MeshManager.hpp"
#include "../Textures/TextureManager.hpp"
#include "../Material/MaterialManager.hpp"

#include <GFXSettings.hpp>

#include <GL/glew.h>
#include <iostream>
#include <array>
#include <vector>
#include <glm/glm.hpp>
#include <BitmaskDefinitions.hpp>
#include "../Utility/Timer.hpp"

namespace GFX
{
   // class AnimationManagerGFX;
    class UniformBufferManager;
    class ShaderManager;
    class RenderJobManager;
    class MeshManager;
    class TextureManager;
    class MaterialManager;


    class DeferredPainter;
    class LightPainter;
    class TextPainter;
    class DebugPainter;
    class ConsolePainter;
    class SplashPainter;
    class FBOPainter;
    class OverlayPainter;
    class PostProcessingPainter;
    class GIPainter;
    class BlurPainter;
	class DecalPainter;

    class FontData;
    class Vertex;
    class FBOTexture;
}                 

namespace GFX
{
	class RenderCore
	{
	private:
		/* Holds settings data for the graphics system */
		unsigned int m_settings[GFX_SETTINGS_COUNT];

	public:
		/*!     
		Friend function to get singleton
		\return RenderCore Returns the RenderCore instance
		*/
		friend RenderCore& Renderer();

		/*!
		Initializes the rendering core, FBOTextures and painters
		\param windowWidth Height of the window when the RenderCore is initialized
		\param windowHeight Width of the window when the RenderCore is initialized
		*/
		void Initialize(int windowWidth, int windowHeight);

		/*!
		Sets a setting to the specified value
		\return Returns either GFX_SUCCESS or GFX_FAIL
		*/
		int SetConfiguration(const int setting, const int value);

		/*!
		Gets the value of a setting
		\return Returns either GFX_SUCCESS or GFX_FAIL
		*/
		int GetConfiguration(const int setting, int& out_value);


		/*!
		Main rendering loop, calls all painters
		*/
		void Render(const double& delta);

		/*!
		Resize function to handle window resizing, resizes GBuffer and viewport.
		\param width New window width
		\param height New window height
		*/
		void Resize(int width, int height);

		/*!
		Set the local view matrix
		\param view New view matrix
		*/
		void SetViewMatrix(glm::mat4 view);

		/*!
		Set the local projection matrix
		\param proj New projection matrix
		*/
		void SetProjMatrix(glm::mat4 proj, float nearZ, float farZ);

        /*!
        Sets the view matrix used by the overlay camera
        */
        void SetOverlayViewMatrix( glm::mat4 view );

        /*!
        Sets the projection matrix used by the overlay camera
        */
        void SetOverlayProjMatrix( glm::mat4 proj );

		/*!
		Get window width
		\return window width 
		*/
		inline int GetWindowWidth() const { return m_windowWidth; }

		/*!
		Get window height
		\return window height
		*/
		inline int GetWindowHeight() const { return m_windowHeight; }

		/*!
		Sets the console to show or hide
		\param visible A boolean value specifying whether the console should shown, if true the console is visible
		*/
		void SetConsoleVisible(bool visible);

		/*!
		Gets whether the console is showing or not
		\return Returns true if the console is visible, else returns false
		*/
		bool GetConsoleVisible();

		void SetSplash(bool splash);

		void Delete();

		void AddRenderJob(GFXBitmask bitmask, void* value);

		void SetLUT(std::string LUT);
		void ReloadLUT();

		void SetExposure(float exposure);
		void SetGamma(float gamma);
		void SetWhitepoint(glm::vec3 whitePoint);

		void DeleteMesh(unsigned long long id);
		void LoadMesh(unsigned int& meshID, int& sizeVerts, int& sizeIndices, GFX::Vertex* verts, int* indices);

		void LoadTexture(unsigned int& id, unsigned char* data, int width, int height, bool decal);
		void DeleteTexture(unsigned long long int id);
		
		void CreateMaterial(unsigned long long int& id);
		void DeleteMaterial(const unsigned long long int& id);
		int AddTextureToMaterial(const unsigned long long int& materialID, const unsigned long long int& textureID);
		void RemoveTextureFromMaterial(const unsigned long long int& materialID, const unsigned long long int& textureID);
        int GetShaderId(unsigned int& shaderId, const char* shaderName);
		int SetShaderToMaterial(const unsigned long long int& materialID, const unsigned int& shaderID);
		int CreateSkeleton(int& out_skeletonID);
		int DeleteSkeleton(const int& skeletonID);
		int GetSkeletonID(const unsigned int& meshID);
		int BindSkeletonToMesh(const unsigned int& meshID, const int& skeletonID);
		int AddAnimationToSkeleton(const int& skeletonID, glm::mat4x4* frames, const unsigned int& numFrames, const unsigned int& numBonesPerFrame);
		int GetAnimationInfo(const int& skeletonID, const int& animationID, unsigned int& out_frameCount, unsigned int& out_bonesPerFrame, unsigned int& out_animationOffset);

        /*!
        Sets the font used for rendering SubSystem statistics.
        \param font The font used for rendering.
        */
        inline void SetStatisticsFont(GFX::FontData* font) { m_font = font; }
		inline void ShowStatistics(bool enabled){ m_showStatistics = enabled; }
		inline void ShowFBO(int which){ m_showFBO = ( which >= 0 && which <= 6 ) ? which : 0; }
		
		void SetAnimationFramerate(unsigned int framerate);
		inline unsigned int GetAnimationFramerate(){ return m_animationFramerate; }

		inline void DrawSelectionbox(const glm::vec4& posdim, const glm::vec4& color){ m_selectionBoxColor = color; m_selectionBoxPosDim = posdim; m_drawSelectionbox = true; }

	private:
		RenderCore();
		~RenderCore();
		/*!
		Create an empty VBO and VAO to be used for screenspace rendering
		*/
		void InitializeDummyVAO();

		/*!
		Initialize the GBuffer, FBO and associated textures
		*/
		void InitializeGBuffer();

		/*!
		Resize all textures bound to the GBuffer
		*/
		void ResizeGBuffer();

		void LoadGPUPF();

		int m_windowWidth;
		int m_windowHeight;

		GLuint m_FBO;

		FBOTexture* m_depthBuffer;
		FBOTexture* m_normalDepth;
		FBOTexture* m_diffuse;
		FBOTexture* m_specular;
		FBOTexture* m_glowMatID;
		
		FBOTexture** m_shadowMapTextures;

		GLuint m_dummyVAO;

		UniformBufferManager*	m_uniformBufferManager;
		ShaderManager*			m_shaderManager;
		RenderJobManager*		m_renderJobManager;
		MeshManager*			m_meshManager;
		TextureManager*			m_textureManager;
		MaterialManager*		m_materialManager;
		AnimationManagerGFX*    m_animationManager;


		DeferredPainter* m_deferredPainter;
		LightPainter* m_lightPainter;
		TextPainter* m_textPainter;
		DebugPainter* m_debugPainter;
		ConsolePainter* m_consolePainter;
		SplashPainter* m_splashPainter;
		FBOPainter* m_fboPainter;
        OverlayPainter* m_overlayPainter;
		BoxPainter* m_boxPainter;
		PostProcessingPainter* m_postProcessingPainter;
		GIPainter* m_GIPainter;
		BlurPainter* m_blurPainter;
		DecalPainter* m_decalPainter;
		ShadowPainter* m_shadowPainter;

		void SubSystemTimeRender();
        std::vector<std::pair<const char*, std::chrono::microseconds>> m_subsystemTimes;
		unsigned long long m_lastUpdateTime;
		unsigned long long m_curTime;
		bool m_showStatistics;
        GFX::FontData* m_font;
		
		int m_showFBO;

		glm::mat4 m_viewMatrix;
		glm::mat4 m_projMatrix;

        glm::mat4 m_overlayViewMatrix;
        glm::mat4 m_overlayProjMatrix;

		float m_nearZ;
		float m_farZ;

		bool m_playSplash;

		float m_gamma;
		float m_exposure;
		glm::vec3 m_whitePoint;

		GLuint m_toneMappedTexture;
		std::string m_currentLUT;

		unsigned int m_animationFramerate;
		
		glm::mat4x4 m_debugLightFrustum;
		glm::mat4x4 m_debugCameraFrustum;
		glm::mat4x4 m_debugFitFrustum;

		bool m_reloadAnimationData;
		
		glm::vec4 m_selectionBoxPosDim;
		glm::vec4 m_selectionBoxColor;
		bool m_drawSelectionbox;

	};

	/*!
	Function to get singleton
	\return RenderCore Returns the RenderCore instance
	*/
	RenderCore& Renderer();
}

#endif
