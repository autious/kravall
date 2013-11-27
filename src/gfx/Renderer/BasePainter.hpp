#ifndef SRC_GFX_RENDERER_BASE_PAINTER_HPP
#define SRC_GFX_RENDERER_BASE_PAINTER_HPP

#include "../Shaders/ShaderManager.hpp"
#include "../Buffers/BufferManager.hpp"
#include <GL/glew.h>

namespace GFX
{
	class BasePainter
	{
	public:

		/*!
		Set ShaderManager and BufferManager pointers of the painter to point to the managers within the rendering core
		\param shaderManager Pointer to ShaderManager present in RenderCore
		\param bufferManager Pointer to BufferManager present in RenderCore
		*/
		BasePainter(ShaderManager* shaderManager, BufferManager* bufferManager);

		~BasePainter();

		/*!
		Initialization function which sets the dummyVAO and FBO for later use in the painter
		\param FBO ID of FBO used for rendertargets
		\param dummyVAO ID of an empty VAO used for screenspace rendering
		*/
		virtual void Initialize(GLuint FBO, GLuint dummyVAO);

		/*!
		Empty virtual function for overriding.
		*/
		virtual void Render();

		/*!
		Resets the FBO to use the backbuffer
		*/
		void ClearFBO();
	protected:

		ShaderManager* m_shaderManager;
		BufferManager* m_bufferManager;

		GLuint m_dummyVAO;
		GLuint m_FBO;
	};
}

#endif
