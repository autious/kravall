#ifndef SRC_GFX_RENDERER_RENDER_CORE_HPP
#define SRC_GFX_RENDERER_RENDER_CORE_HPP

#include <Shaders/ShaderManager.hpp>
#include <Buffers/UniformBufferManager.hpp>

#include "FBOTexture.hpp"
#include "DeferredPainter.hpp"
#include "Console/ConsolePainter.hpp"
#include "DebugRenderer/DebugPainter.hpp"
#include "TextRenderer/TextPainter.hpp"
#include "SplashRenderer/SplashPainter.hpp"
#include "FBORenderer/FBOPainter.hpp"

#include "TextRenderer/TextManager.hpp"
#include "DebugRenderer/DebugManager.hpp"

#include <iostream>
#include <gfx/Material.hpp>

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
		void Render();

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

		void AddRenderJob(const GLuint& ibo, const GLuint& vao, const int& size, Material* m);

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

		FBOTexture* m_normalDepth;
		FBOTexture* m_diffuse;
		FBOTexture* m_specular;
		FBOTexture* m_glowMatID;

		GLuint m_dummyVAO;

		UniformBufferManager*	m_uniformBufferManager;
		ShaderManager*			m_shaderManager;
		

		DeferredPainter* m_deferredPainter;
		TextPainter* m_textPainter;
		DebugPainter* m_debugPainter;
		ConsolePainter* m_consolePainter;
		SplashPainter* m_splashPainter;
		FBOPainter* m_fboPainter;
		glm::mat4 m_viewMatrix;
		glm::mat4 m_projMatrix;

		bool m_playSplash;

	};

	/*!
	Function to get singleton
	\return RenderCore Returns the RenderCore instance
	*/
	RenderCore& Renderer();
}

#endif
