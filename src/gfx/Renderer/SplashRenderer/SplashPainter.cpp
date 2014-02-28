#include "SplashPainter.hpp"

namespace GFX
{
	SplashPainter::SplashPainter(ShaderManager* shaderManager, UniformBufferManager* uniformBufferManager)
		: BasePainter(shaderManager, uniformBufferManager)
	{

		}

	SplashPainter::~SplashPainter()
	{
	}

	void SplashPainter::Initialize(GLuint FBO, GLuint dummyVAO)
	{
		BasePainter::Initialize(FBO, dummyVAO);

		//load shader
		m_shaderManager->LoadShader("shaders/RenderToQuad/Quad.vertex", "QuadVS", GL_VERTEX_SHADER);
		m_shaderManager->LoadShader("shaders/RenderToQuad/Quad.geometry", "QuadGS", GL_GEOMETRY_SHADER);
		m_shaderManager->LoadShader("shaders/RenderToQuad/Quad.fragment", "QuadFS", GL_FRAGMENT_SHADER);
		m_shaderManager->CreateProgram("TexturedQuad");
		
		m_shaderManager->AttachShader("QuadVS", "TexturedQuad");
		m_shaderManager->AttachShader("QuadGS", "TexturedQuad");
		m_shaderManager->AttachShader("QuadFS", "TexturedQuad");

		m_shaderManager->LinkProgram("TexturedQuad");

		//load logo textures
	
		m_logoTexture = TextureManager::LoadFromFile("assets/Logo/xyz_logo.png", GL_TEXTURE_2D, GL_RGBA, GL_RGBA, GL_LINEAR, GL_LINEAR, 
			GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);

		m_logoTextTexture = TextureManager::LoadFromFile("assets/Logo/xyz_logo_text.png", GL_TEXTURE_2D, GL_RGBA, GL_RGBA, GL_LINEAR, GL_LINEAR,
			GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);

		m_alphaUniform				= m_shaderManager->GetUniformLocation("TexturedQuad", "alphaIN");
		m_textureUniform			= m_shaderManager->GetUniformLocation("TexturedQuad", "textureIN");
		m_positionUniform			= m_shaderManager->GetUniformLocation("TexturedQuad", "posIN");
		m_sizeUniform				= m_shaderManager->GetUniformLocation("TexturedQuad", "sizeIN");
		m_uvXUniform				= m_shaderManager->GetUniformLocation("TexturedQuad", "uvXIN");
		m_screenDimensionUniform	= m_shaderManager->GetUniformLocation("TexturedQuad", "screenDimenionIN"); 

		m_logoAlpha		= -1.0f;
		m_logoTextAlpha = 0.0f;

		m_done = false;

		m_clearColor = 0.0f;
	}

	
	void SplashPainter::Render(const int& screenWidth, const int& screenHeight, const double& delta)
	{
		glDisable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		m_shaderManager->UseProgram("TexturedQuad");

		if (m_logoAlpha <= -1.0f)
		{
			FadeToWhite(screenWidth, screenHeight, delta);
		}

		if (m_clearColor >= 1.0f)
		{
			RenderSplash(screenWidth, screenHeight, delta);
		}
		
		if (m_logoAlpha >= 3.5f)
		{
			FadeToBlack(screenWidth, screenHeight, delta);
			RenderSplash(screenWidth, screenHeight, delta);
		}

		m_shaderManager->ResetProgram();
		glEnable(GL_DEPTH_TEST);
		glDisable(GL_BLEND);


		if (m_clearColor < 0.0f)
			m_done = true;
	}


	void SplashPainter::RenderSplash(const int& screenWidth, const int& screenHeight, const double& delta)
	{
	
		if (m_logoAlpha >= 3.5f)
		{
			m_shaderManager->SetUniform(m_clearColor, m_alphaUniform);
		}
		else
			m_shaderManager->SetUniform(m_logoAlpha += 1.01f * (float)delta, m_alphaUniform);

		m_shaderManager->SetUniform(1, glm::vec2(screenWidth / 2.0f, screenHeight / 2.0f + 100.0f), m_positionUniform);
		m_shaderManager->SetUniform(1, glm::vec2(180.0f, 110.0f), m_sizeUniform);
		m_shaderManager->SetUniform(1, glm::vec2(screenWidth, screenHeight), m_screenDimensionUniform);
		m_shaderManager->SetUniform(1.0f, m_uvXUniform);

		TextureManager::BindTexture(m_logoTexture, m_textureUniform, 0, GL_TEXTURE_2D);

		glBindVertexArray(m_dummyVAO);
		glDrawArrays(GL_POINTS, 0, 1);

		TextureManager::UnbindTexture();

		if (m_logoAlpha <= 2.0f)
		{
			m_shaderManager->SetUniform(0.0f, m_alphaUniform);
		}
		else if (m_logoAlpha >= 3.5f)
		{
			m_shaderManager->SetUniform(m_clearColor, m_alphaUniform);
		}
		else if (m_logoTextAlpha >= 1.0f)
		{
			m_logoTextAlpha = 1.0f;
		}
		else
		{
			m_shaderManager->SetUniform(m_logoTextAlpha += 0.01f, m_alphaUniform);
		}

		m_shaderManager->SetUniform(1.0f, m_uvXUniform);

		m_shaderManager->SetUniform(1, glm::vec2(screenWidth / 2.0f, screenHeight / 2.0f - 30.0f), m_positionUniform);
		m_shaderManager->SetUniform(1, glm::vec2(700.0f, 90.0f), m_sizeUniform);
		m_shaderManager->SetUniform(1, glm::vec2(screenWidth, screenHeight), m_screenDimensionUniform);

		TextureManager::BindTexture(m_logoTextTexture, m_textureUniform, 0, GL_TEXTURE_2D);
		glBindVertexArray(m_dummyVAO);
		glDrawArrays(GL_POINTS, 0, 1);
		TextureManager::UnbindTexture();
	}

	void SplashPainter::FadeToBlack(const int& screenWidth, const int& screenHeight, const double& delta)
	{
		m_clearColor -= 1.01f * (float)delta;
		glClearColor(m_clearColor, m_clearColor, m_clearColor, m_clearColor);
	}

	void SplashPainter::FadeToWhite(const int& screenWidth, const int& screenHeight, const double& delta)
	{
		m_clearColor += 1.01f * (float)delta;
		glClearColor(m_clearColor, m_clearColor, m_clearColor, m_clearColor);
	}

}