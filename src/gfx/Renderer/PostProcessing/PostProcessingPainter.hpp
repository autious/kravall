#ifndef SRC_GFX_RENDERER_POST_PROCESSING_POST_PROCESSING_PAINTER_HPP
#define SRC_GFX_RENDERER_POST_PROCESSING_POST_PROCESSING_PAINTER_HPP

#include "../BasePainter.hpp"
#include <Shaders/ShaderManager.hpp>
#include <Buffers/UniformBufferManager.hpp>
#include "../../Textures/TextureManager.hpp"

namespace GFX
{

	class PostProcessingPainter : public BasePainter
	{
	public:

		/*!
		Set ShaderManager and BufferManager pointers of the painter to point to the managers within the rendering core
		\param shaderManager Pointer to ShaderManager present in RenderCore
		\param bufferManager Pointer to BufferManager present in RenderCore
		*/
		PostProcessingPainter(ShaderManager* shaderManager, UniformBufferManager* uniformBufferManager, TextureManager* textureManager);

		~PostProcessingPainter();

		/*!
		Initialization function which sets the dummyVAO and FBO for later use in the painter.
		Loads all shaders associated with deferred rendering.
		\param FBO ID of FBO used for rendertargets
		\param dummyVAO ID of an empty VAO used for screenspace rendering
		*/
		void Initialize(GLuint FBO, GLuint dummyVAO, int screenWidth, int screenHeight);

		/*!
		Main rendering loop
		*/
		void Render(const double& delta, const GLuint& tonemappedTexture);

	private:

		void ColorGrading(const GLuint& tonemappedTexture);
		void OutputTexture(const GLuint& texturehandle);

		GLint m_alphaUniform;
		GLint m_textureUniform;

		GLint m_LUTUniform;
		GLint m_sourceUniform;
		GLuint m_currentLut;

		GLuint m_destinationTexture;
		int m_screenWidth;
		int m_screenHeight;

		TextureManager* m_textureManager;

	};
}
#endif