#ifndef SRC_GFX_RENDERER_RENDER_CORE_HPP
#define SRC_GFX_RENDERER_RENDER_CORE_HPP

#include "..\Shaders\ShaderManager.hpp"
#include "FBOTexture.hpp"
#include "DeferredPainter.hpp"

#include <iostream>

namespace GFX
{
	class RenderCore
	{
	public:
		friend RenderCore& Renderer();

		void Initialize(int windowWidth, int windowHeight);
		void Render();

	private:
		RenderCore();
		~RenderCore();

		void InitializeDummyVAO();
		void InitializeGBuffer();

		void ResizeGBuffer();

		int m_windowWidth;
		int m_windowHeight;

		GLuint m_FBO;

		FBOTexture* m_normalDepth;
		FBOTexture* m_diffuse;
		FBOTexture* m_specular;
		FBOTexture* m_glowMatID;

		GLuint m_dummyVAO;

		ShaderManager* m_shaderManager;

		DeferredPainter* m_deferredPainter;
	};

	RenderCore& Renderer();
}

#endif