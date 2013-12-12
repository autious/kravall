#ifndef SRC_GFX_RENDERER_DEBUG_PAINTER_HPP
#define SRC_GFX_RENDERER_DEBUG_PAINTER_HPP

#include "../BasePainter.hpp"
#include <Shaders/ShaderManager.hpp>

#include "../DeferredRenderer/FBOTexture.hpp"
#include "../../Buffers/UniformBufferManager.hpp"
#include <GL/glew.h>

namespace GFX
{
	class DebugPainter : public BasePainter
	{
	public:

		/*!
		Set ShaderManager and BufferManager pointers of the painter to point to the managers within the rendering core
		\param shaderManager Pointer to ShaderManager present in RenderCore
		\param uniformBufferManager Pointer to uniformBufferManager present in RenderCore
		*/
		DebugPainter(ShaderManager* shaderManager, UniformBufferManager* uniformBufferManager);

		~DebugPainter();

		/*!
		Initialization function which sets the dummyVAO and FBO for later use in the painter.
		Loads all shaders associated with debug rendering.
		\param FBO ID of FBO used for rendertargets
		\param dummyVAO ID of an empty VAO used for screenspace rendering
		*/
		void Initialize(GLuint FBO, GLuint dummyVAO);

		/*!
		Main debug rendering loop
		\param viewMatrix View matrix for the shader
		\param projMatrix Projection matrix for the shader
		*/
		void Render(glm::mat4 viewMatrix, glm::mat4 projMatrix);

	private:

		/*!
		Detaches all textures used in the deferred pipeline
		*/
		void DetachTextures();

		// Uniforms
		GLuint m_pointPositionUniform;
		GLuint m_pointColorUniform;
		GLuint m_pointSizeUniform;

		GLuint m_lineStartUniform;
		GLuint m_lineEndUniform;
		GLuint m_lineColorUniform;

		GLuint m_rectPosUniform;
		GLuint m_rectDimUniform;
		GLuint m_rectColorUniform;

		GLuint m_boxPosUniform;
		GLuint m_boxDimUniform;
		GLuint m_boxColorUniform;

		GLuint m_circlePosUniform;
		GLuint m_circleDimUniform;
		GLuint m_circleColorUniform;

		GLuint m_screenSizeUniform;
	};
}

#endif
