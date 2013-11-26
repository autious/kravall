#ifndef SRC_GFX_RENDERER_RENDER_CORE_HPP
#define SRC_GFX_RENDERER_RENDER_CORE_HPP

#include "FBOTexture.hpp"
#include <iostream>

namespace GFX
{
	class RenderCore
	{
	public:
		RenderCore(int windowWidth, int windowHeight);
		~RenderCore();

		void Initialize();
		void Render();

	private:
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
	};
}

#endif