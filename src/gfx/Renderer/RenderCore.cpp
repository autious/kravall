#include "RenderCore.hpp"
namespace GFX
{

	RenderCore::RenderCore(int windowWidth, int windowHeight)
	{
		m_windowWidth	= windowWidth;
		m_windowHeight	= windowHeight;

		m_normalDepth	= new FBOTexture();
		m_diffuse		= new FBOTexture();
		m_specular		= new FBOTexture();
		m_glowMatID		= new FBOTexture();
	}

	RenderCore::~RenderCore()
	{

	}

	void RenderCore::Initialize()
	{
		InitializeDummyVAO();
	}

	void RenderCore::Render()
	{

	}

	void RenderCore::InitializeGBuffer()
	{
		//Generate the FBO
		glGenFramebuffers(1, &m_FBO);
		glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);

		//Initialize GBuffer textures, second to last parameter should be configurable through settings
		m_normalDepth->Initialize(GL_TEXTURE_2D, GL_NEAREST, GL_NEAREST, GL_CLAMP_TO_BORDER, GL_CLAMP_TO_BORDER, GL_RGBA32F, GL_RGBA);
		m_diffuse->Initialize(GL_TEXTURE_2D, GL_NEAREST, GL_NEAREST, GL_CLAMP_TO_BORDER, GL_CLAMP_TO_BORDER, GL_RGBA32F, GL_RGBA);
		m_specular->Initialize(GL_TEXTURE_2D, GL_NEAREST, GL_NEAREST, GL_CLAMP_TO_BORDER, GL_CLAMP_TO_BORDER, GL_RGBA32F, GL_RGBA);
		m_glowMatID->Initialize(GL_TEXTURE_2D, GL_NEAREST, GL_NEAREST, GL_CLAMP_TO_BORDER, GL_CLAMP_TO_BORDER, GL_RGBA32F, GL_RGBA);

		ResizeGBuffer();
	}

	void RenderCore::ResizeGBuffer()
	{
		m_normalDepth->UpdateResolution(m_windowWidth, m_windowHeight);
		m_diffuse->UpdateResolution(m_windowWidth, m_windowHeight);
		m_specular->UpdateResolution(m_windowWidth, m_windowHeight);
		m_glowMatID->UpdateResolution(m_windowWidth, m_windowHeight);
	}

	void RenderCore::InitializeDummyVAO()
	{
		float dummy = 1;

		GLuint VBO;
		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float), &dummy, GL_STATIC_DRAW);

		glGenVertexArrays(1, &m_dummyVAO);
		glBindVertexArray(m_dummyVAO);

		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glVertexAttribPointer(0, 1, GL_FLOAT, GL_FALSE, 0, (GLubyte*)NULL);

		glBindVertexArray(0);
	}
}