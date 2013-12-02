#include "SplashPainter.hpp"

namespace GFX
{
	SplashPainter::SplashPainter(ShaderManager* shaderManager, BufferManager* bufferManager, UniformBufferManager* uniformBufferManager)
		: BasePainter(shaderManager, bufferManager, uniformBufferManager)
	{

		}

	SplashPainter::~SplashPainter()
	{

	}

	void SplashPainter::Initialize(GLuint FBO, GLuint dummyVAO)
	{
		m_logoTexture = new Texture();
		m_logoTexture->LoadFromFile("assets/xyz_logo.png", GL_TEXTURE_2D, GL_RGBA, GL_RGBA, GL_LINEAR, GL_LINEAR, 
			GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);

		m_logoTextTexture = new Texture();
		m_logoTextTexture->LoadFromFile("assets/xyz_logo_text.png", GL_TEXTURE_2D, GL_RGBA, GL_RGBA, GL_LINEAR, GL_LINEAR,
			GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);
	}

	
	void SplashPainter::Render()
	{

	}
}