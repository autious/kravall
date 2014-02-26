#ifndef SRC_GFX_RENDERER_BASE_PAINTER_HPP
#define SRC_GFX_RENDERER_BASE_PAINTER_HPP

#include "../Shaders/ShaderManager.hpp"
#include "../Buffers/UniformBufferManager.hpp"
#include <GL/glew.h>

namespace GFX
{
	struct RenderInfo
	{
		unsigned int numTris;
		unsigned int numDrawCalls;
	};
	class BasePainter
	{
	public:

		/*!
		Set ShaderManager and BufferManager pointers of the painter to point to the managers within the rendering core
		\param shaderManager Pointer to ShaderManager present in RenderCore
		\param bufferManager Pointer to BufferManager present in RenderCore
		*/
		BasePainter(ShaderManager* shaderManager, UniformBufferManager* uniformBufferManager);

		virtual ~BasePainter();

		/*!
		Initialization function which sets the dummyVAO and FBO for later use in the painter
		\param FBO ID of FBO used for rendertargets
		\param dummyVAO ID of an empty VAO used for screenspace rendering
		*/
		virtual void Initialize(GLuint FBO, GLuint dummyVAO);

		/*!
		Empty virtual function for overriding.
		\return Returns rendering info
		*/
		virtual void Render();

		/*!
		Resets the FBO to use the backbuffer
		*/
		void ClearFBO();
	protected:

		UniformBufferManager*	m_uniformBufferManager;
		ShaderManager*			m_shaderManager;

		GLuint m_dummyVAO;
		GLuint m_FBO;
	};
}

#endif
