#ifndef SRC_GFX_RENDERER_RENDER_CORE_HPP
#define SRC_GFX_RENDERER_RENDER_CORE_HPP

#include <Shaders/ShaderManager.hpp>
#include <Buffers/UniformBufferManager.hpp>

#include "DeferredRenderer/FBOTexture.hpp"
#include "DeferredRenderer/DeferredPainter.hpp"
#include "DeferredRenderer/LightPainter.hpp"

#include "Console/ConsolePainter.hpp"
#include "OverlayRenderer/OverlayPainter.hpp"
#include "DebugRenderer/DebugPainter.hpp"
#include "TextRenderer/TextPainter.hpp"
#include "SplashRenderer/SplashPainter.hpp"
#include "FBORenderer/FBOPainter.hpp"

#include "TextRenderer/TextManager.hpp"
#include "DebugRenderer/DebugManager.hpp"
#include "RenderJobManager.hpp"
#include "../Buffers/MeshManager.hpp"
#include "../Textures/TextureManager.hpp"
#include "../Material/MaterialManager.hpp"

#include <iostream>
#include <array>

#include "../Utility/Timer.hpp"

#define GFX_CHECKTIME(x, y)\
{\
	Timer().Start(); \
	x; \
	Timer().Stop(); \
	std::chrono::microseconds ms = Timer().GetDelta(); \
	m_subsystemTimes.push_back(std::pair<const char*, std::chrono::microseconds>(y, ms)); \
}

namespace GFX
{
	class RenderCore
	{
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
		void SetProjMatrix(glm::mat4 proj);

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

		void DeleteMesh(unsigned long long id);
		void LoadStaticMesh(unsigned int& meshID, const int& sizeVerts, const int& sizeIndices, StaticVertex* verts, int* indices);

		void LoadTexture(unsigned int& id, unsigned char* data, int width, int height);
		void DeleteTexture(unsigned long long int id);
		
		void CreateMaterial(unsigned long long int& id);
		void DeleteMaterial(const unsigned long long int& id);
		int AddTextureToMaterial(const unsigned long long int& materialID, const unsigned long long int& textureID);
		void RemoveTextureFromMaterial(const unsigned long long int& materialID, const unsigned long long int& textureID);
        int GetShaderId(unsigned int& shaderId, const char* shaderName);
		int SetShaderToMaterial(const unsigned long long int& materialID, const unsigned int& shaderID);

        /*!
        Sets the font used for rendering SubSystem statistics.
        \param font The font used for rendering.
        */
        inline void SetStatisticsFont(GFX::FontData* font) { m_font = font; }
		inline void ShowStatistics(bool enabled){ m_showStatistics = enabled; }
		inline void ShowFBO(int which){ m_showFBO = ( which >= 0 && which <= 5 ) ? which : 0; }
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


		int m_windowWidth;
		int m_windowHeight;

		GLuint m_FBO;

		FBOTexture* m_depthBuffer;
		FBOTexture* m_normalDepth;
		FBOTexture* m_diffuse;
		FBOTexture* m_specular;
		FBOTexture* m_glowMatID;

		GLuint m_dummyVAO;

		UniformBufferManager*	m_uniformBufferManager;
		ShaderManager*			m_shaderManager;
		RenderJobManager*		m_renderJobManager;
		MeshManager*			m_meshManager;
		TextureManager*			m_textureManager;
		MaterialManager*		m_materialManager;


		DeferredPainter* m_deferredPainter;
		LightPainter* m_lightPainter;
		TextPainter* m_textPainter;
		DebugPainter* m_debugPainter;
		ConsolePainter* m_consolePainter;
		SplashPainter* m_splashPainter;
		FBOPainter* m_fboPainter;
        OverlayPainter* m_overlayPainter;
		
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

		bool m_playSplash;

	};

	/*!
	Function to get singleton
	\return RenderCore Returns the RenderCore instance
	*/
	RenderCore& Renderer();
}

#endif
