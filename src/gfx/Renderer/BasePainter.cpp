#include "BasePainter.hpp"

namespace GFX
{
	BasePainter::BasePainter(ShaderManager* shaderManager, UniformBufferManager* uniformBufferManager)
	{
		m_shaderManager			= shaderManager;
		m_uniformBufferManager	= uniformBufferManager;
	}

	BasePainter::~BasePainter()
	{

	}

	void BasePainter::Initialize(GLuint FBO, GLuint dummyVAO)
	{
		m_dummyVAO = dummyVAO;
		m_FBO = FBO;
	}

	void BasePainter::Render()
	{
	}

	void BasePainter::ClearFBO()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

}
