#ifndef SRC_GFX_RENDERER_BASE_PAINTER_HPP
#define SRC_GFX_RENDERER_BASE_PAINTER_HPP

#include "../Shaders/ShaderManager.hpp"
#include "../Buffers/BufferManager.hpp"
#include <gl/glew.h>

namespace GFX
{
	class BasePainter
	{
	public:
		BasePainter(ShaderManager* shaderManager, BufferManager* bufferManager);
		~BasePainter();

		virtual void Initialize(GLuint FBO, GLuint dummyVAO);
		virtual void Render();
		void ClearFBO();
	protected:

		ShaderManager* m_shaderManager;
		BufferManager* m_bufferManager;

		GLuint m_dummyVAO;
		GLuint m_FBO;
	};
}

#endif