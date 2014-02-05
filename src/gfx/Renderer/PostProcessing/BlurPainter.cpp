#include "BlurPainter.hpp"
#include <gfx/LightData.hpp>

namespace GFX
{
	BlurPainter::BlurPainter(ShaderManager* shaderManager, UniformBufferManager* uniformBufferManager)
		: BasePainter(shaderManager, uniformBufferManager)
	{
	}

	BlurPainter::~BlurPainter()
	{

	}

	void BlurPainter::Initialize(GLuint FBO, GLuint dummyVAO)
	{
		BasePainter::Initialize(FBO, dummyVAO);
		
		m_intermediateTexture = new FBOTexture();
		
		m_occlusionRadius = 3.0f;
		m_maxOcclusionDistance = 12.0f;
		
		InitFBO();
		
		m_shaderManager->ResetProgram();
		
		m_seedTexture = TextureManager::LoadFromFile("assets/texture/noise.png", GL_TEXTURE_2D, GL_RGBA, GL_RGBA, GL_LINEAR, GL_LINEAR, GL_REPEAT, GL_REPEAT);
		
		m_shaderManager->ResetProgram();
		
		m_shaderManager->CreateProgram("GaussianBlurHorizontal");
		m_shaderManager->LoadShader("shaders/RenderToQuad/Quad.vertex", "GBHVS", GL_VERTEX_SHADER);
		m_shaderManager->LoadShader("shaders/FSQuadGS.glsl", "GBHGS", GL_GEOMETRY_SHADER);
		m_shaderManager->LoadShader("shaders/PostProcessing/GaussianBlurHorizontalFS.glsl", "GBHFS", GL_FRAGMENT_SHADER);
		m_shaderManager->AttachShader("GBHVS", "GaussianBlurHorizontal");
		m_shaderManager->AttachShader("GBHGS", "GaussianBlurHorizontal");
		m_shaderManager->AttachShader("GBHFS", "GaussianBlurHorizontal");
		m_shaderManager->LinkProgram("GaussianBlurHorizontal");
		
		m_shaderManager->CreateProgram("GaussianBlurVertical");
		m_shaderManager->LoadShader("shaders/RenderToQuad/Quad.vertex", "GBVVS", GL_VERTEX_SHADER);
		m_shaderManager->LoadShader("shaders/FSQuadGS.glsl", "GBVGS", GL_GEOMETRY_SHADER);
		m_shaderManager->LoadShader("shaders/PostProcessing/GaussianBlurVerticalFS.glsl", "GBVFS", GL_FRAGMENT_SHADER);
		m_shaderManager->AttachShader("GBVVS", "GaussianBlurVertical");
		m_shaderManager->AttachShader("GBVGS", "GaussianBlurVertical");
		m_shaderManager->AttachShader("GBVFS", "GaussianBlurVertical");
		m_shaderManager->LinkProgram("GaussianBlurVertical");
		
	}

	void BlurPainter::InitFBO()
	{
		//blur FBO
		glGenFramebuffers(1, &m_blurFBO);
		glBindFramebuffer(GL_FRAMEBUFFER, m_blurFBO);
		
		m_intermediateTexture->Initialize(GL_TEXTURE_2D, GL_NEAREST, GL_NEAREST, GL_CLAMP_TO_BORDER, GL_CLAMP_TO_BORDER, GL_RGBA32F, GL_RGBA);
		
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_intermediateTexture->GetTextureHandle(), 0);
		
		GLenum drawBuffers[] = { GL_COLOR_ATTACHMENT0 };
		glDrawBuffers(1, drawBuffers);
		
		//reset the framebuffer
		glBindFramebuffer(GL_FRAMEBUFFER, 0);


	}

	void BlurPainter::GaussianBlur(FBOTexture* texture)
	{
		GLenum err;
		int texWidth = texture->GetWidth();
		int texHeight = texture->GetHeight();
		m_intermediateTexture->UpdateResolution(texWidth, texHeight);
		
		glBindFramebuffer(GL_FRAMEBUFFER, m_blurFBO);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_intermediateTexture->GetTextureHandle(), 0);
		glDrawBuffer( GL_COLOR_ATTACHMENT0 );

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		m_shaderManager->UseProgram("GaussianBlurHorizontal");
		
		TextureManager::BindTexture(texture->GetTextureHandle(), m_shaderManager->GetUniformLocation("GaussianBlurHorizontal", "gTexture"), 0, GL_TEXTURE_2D);
		m_shaderManager->SetUniform((GLfloat)texWidth, (GLfloat)texHeight, m_shaderManager->GetUniformLocation("GaussianBlurHorizontal", "gScreenDimensions"));
		err = glGetError();
		
		glBindVertexArray(m_dummyVAO);
		glDrawArrays(GL_POINTS, 0, 1);
		BasePainter::ClearFBO();
		m_shaderManager->ResetProgram();

		glBindFramebuffer(GL_FRAMEBUFFER, m_blurFBO);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture->GetTextureHandle(), 0);
		glDrawBuffer(GL_COLOR_ATTACHMENT0);
		m_shaderManager->UseProgram("GaussianBlurVertical");
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		TextureManager::BindTexture(m_intermediateTexture->GetTextureHandle(), m_shaderManager->GetUniformLocation("GaussianBlurVertical", "gTexture"), 0, GL_TEXTURE_2D);
		m_shaderManager->SetUniform((GLfloat)texWidth, (GLfloat)texHeight, m_shaderManager->GetUniformLocation("GaussianBlurVertical", "gScreenDimensions"));
		
		glBindVertexArray(m_dummyVAO);
		glDrawArrays(GL_POINTS, 0, 1);
		BasePainter::ClearFBO();
		m_shaderManager->ResetProgram();
		glBindFramebuffer(GL_FRAMEBUFFER, 0);


	}
}