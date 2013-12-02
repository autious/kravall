#ifndef SRC_GFX_RENDERER_SPLASH_RENDERER_SPLASH_PAINTER_HPP
#define SRC_GFX_RENDERER_SPLASH_RENDERER_SPLASH_PAINTER_HPP

#include "../BasePainter.hpp"
#include <Shaders/ShaderManager.hpp>
#include <Buffers/UniformBufferManager.hpp>
#include "../../Textures/Texture.hpp"

namespace GFX
{

	class SplashPainter : public BasePainter
	{
	public:

		/*!
		Set ShaderManager and BufferManager pointers of the painter to point to the managers within the rendering core
		\param shaderManager Pointer to ShaderManager present in RenderCore
		\param bufferManager Pointer to BufferManager present in RenderCore
		*/
		SplashPainter(ShaderManager* shaderManager, BufferManager* bufferManager, UniformBufferManager* uniformBufferManager);

		~SplashPainter();

		/*!
		Initialization function which sets the dummyVAO and FBO for later use in the painter.
		Loads all shaders associated with deferred rendering.
		\param FBO ID of FBO used for rendertargets
		\param dummyVAO ID of an empty VAO used for screenspace rendering
		*/
		void Initialize(GLuint FBO, GLuint dummyVAO);

		/*!
		Main rendering loop
		*/
		void Render();

	private:
		Texture* m_logoTexture;
		Texture* m_logoTextTexture;

		GLint m_alphaUniform;
		GLint m_textureUniform;
		GLint m_positionUniform;
		GLint m_sizeUniform;
		GLint m_uvXUniform;

		bool m_playSplash;
	};
}
#endif