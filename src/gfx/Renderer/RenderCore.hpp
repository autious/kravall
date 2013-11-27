#ifndef SRC_GFX_RENDERER_RENDER_CORE_HPP
#define SRC_GFX_RENDERER_RENDER_CORE_HPP

#include <Shaders/ShaderManager.hpp>
#include <Buffers/BufferManager.hpp>
#include <Buffers/UniformBufferManager.hpp>

#include "FBOTexture.hpp"
#include "DeferredPainter.hpp"

#include <iostream>

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
		BufferManager*			m_bufferManager;
		

		DeferredPainter* m_deferredPainter;
	};

	/*!
	Function to get singleton
	\return RenderCore Returns the RenderCore instance
	*/
	RenderCore& Renderer();
}

#endif
