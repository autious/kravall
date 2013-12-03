#ifndef SRC_GFX_RENDERER_FBO_RENDERER_FBO_PAINTER_HPP
#define SRC_GFX_RENDERER_FBO_RENDERER_FBO_PAINTER_HPP

#include "../BasePainter.hpp"
#include <Shaders/ShaderManager.hpp>
#include <Buffers/UniformBufferManager.hpp>
#include "../../Textures/Texture.hpp"
#include "../FBOTexture.hpp"

namespace GFX
{

	class FBOPainter : public BasePainter
	{
	public:

		/*!
		Set ShaderManager and BufferManager pointers of the painter to point to the managers within the rendering core
		\param shaderManager Pointer to ShaderManager present in RenderCore
		\param bufferManager Pointer to BufferManager present in RenderCore
		*/
		FBOPainter(ShaderManager* shaderManager, UniformBufferManager* uniformBufferManager);

		~FBOPainter();

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
		void Render(FBOTexture* normalDepth, FBOTexture* diffuse, FBOTexture* specular, FBOTexture* glowMatID, int screenWidth, int screenHeight, int current);
		
	private:

		GLint m_alphaUniform;
		GLint m_textureUniform;
	
	};
}
#endif