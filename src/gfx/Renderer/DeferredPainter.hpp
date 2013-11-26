#ifndef SRC_GFX_RENDERER_DEFERRED_PAINTER_HPP
#define SRC_GFX_RENDERER_DEFERRED_PAINTER_HPP

#include "Utility/GLFWInclude.hpp"
#include "BasePainter.hpp"
#include <Shaders/ShaderManager.hpp>

#include "FBOTexture.hpp"

namespace GFX
{
	class DeferredPainter : public BasePainter
	{
	public:
		DeferredPainter(ShaderManager* shaderManager);
		~DeferredPainter();

		void Initialize(GLuint FBO, GLuint dummyVAO);
		void Render(FBOTexture* normalDepth, FBOTexture* diffuse, FBOTexture* specular, FBOTexture* glowMatID);

	private:
		void BindGBuffer(FBOTexture* normalDepth, FBOTexture* diffuse, FBOTexture* specular, FBOTexture* glowMatID);

	};
}

#endif
