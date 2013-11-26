#ifndef SRC_GFX_RENDERER_DEFERRED_PAINTER_HPP
#define SRC_GFX_RENDERER_DEFERRED_PAINTER_HPP

#include "Utility/GLFWInclude.hpp"
#include "..\Shaders\ShaderManager.hpp"

#include "FBOTexture.hpp"

namespace GFX
{
	class DeferredPainter
	{
	public:
		DeferredPainter(ShaderManager* shaderManager);
		~DeferredPainter();

		void Initialize();
		void Render(GLuint FBO, FBOTexture* normalDepth, FBOTexture* diffuse, FBOTexture* specular, FBOTexture* glowMatID);

	private:
		void BindGBuffer(GLuint FBO, FBOTexture* normalDepth, FBOTexture* diffuse, FBOTexture* specular, FBOTexture* glowMatID);

		ShaderManager* m_shaderManager;
	};
}

#endif