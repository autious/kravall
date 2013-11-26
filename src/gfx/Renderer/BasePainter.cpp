#include "BasePainter.hpp"

namespace GFX
{
	BasePainter::BasePainter(ShaderManager* shaderManager)
	{
		m_shaderManager = shaderManager;
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