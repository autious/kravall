#include "FBOPainter.hpp"

namespace GFX
{

	FBOPainter::FBOPainter(ShaderManager* shaderManager, UniformBufferManager* uniformBufferManager)
		:BasePainter(shaderManager, uniformBufferManager)
	{
	}

	FBOPainter::~FBOPainter()
	{

	}

	
	void FBOPainter::Initialize(GLuint FBO, GLuint dummyVAO)
	{
		BasePainter::Initialize(FBO, dummyVAO);

		//load shader
		m_shaderManager->LoadShader("shaders/RenderToQuad/Quad.vertex", "QVS", GL_VERTEX_SHADER);
		m_shaderManager->LoadShader("shaders/FSQuadGS.glsl", "QGS", GL_GEOMETRY_SHADER);
		m_shaderManager->LoadShader("shaders/RenderToQuad/Quad.fragment", "QFS", GL_FRAGMENT_SHADER);
		m_shaderManager->CreateProgram("TQ");

		m_shaderManager->AttachShader("QVS", "TQ");
		m_shaderManager->AttachShader("QGS", "TQ");
		m_shaderManager->AttachShader("QFS", "TQ");

		m_shaderManager->LinkProgram("TQ");

		m_alphaUniform = m_shaderManager->GetUniformLocation("TQ", "alphaIN");
		m_textureUniform = m_shaderManager->GetUniformLocation("TQ", "textureIN");
	}

	void FBOPainter::Render(FBOTexture* normalDepth, FBOTexture* diffuse, FBOTexture* specular, FBOTexture* glowMatID, int screenWidth, int screenHeight, FBOTexture** shadowMaps, int current )
	{
		m_shaderManager->UseProgram("TQ");

		glDisable(GL_DEPTH_TEST);
		glDisable(GL_BLEND);

		m_shaderManager->SetUniform(1.0f, m_alphaUniform);

		if (current > 1 && current <= 5)
		{
			glDepthMask(GL_FALSE);
			switch (current)
			{
			case 2:
				TextureManager::BindTexture(normalDepth->GetTextureHandle(), m_textureUniform, 0, GL_TEXTURE_2D);
				break;
			case 3:
				TextureManager::BindTexture(diffuse->GetTextureHandle(), m_textureUniform, 0, GL_TEXTURE_2D);
				break;
			case 4:
				TextureManager::BindTexture(specular->GetTextureHandle(), m_textureUniform, 0, GL_TEXTURE_2D);
				break;
			case 5:
				TextureManager::BindTexture(glowMatID->GetTextureHandle(), m_textureUniform, 0, GL_TEXTURE_2D);
				break;
			}
			glBindVertexArray(m_dummyVAO);
			glDrawArrays(GL_POINTS, 0, 1);
			glDepthMask(GL_TRUE);
		}

		else if (current == 1)
		{
			int x = screenWidth / 4;
			int y = screenHeight / 4;

			glViewport(0, 0, x, y);
			TextureManager::BindTexture(normalDepth->GetTextureHandle(), m_textureUniform, 0, GL_TEXTURE_2D);
			glBindVertexArray(m_dummyVAO);
			glDrawArrays(GL_POINTS, 0, 1);

			glViewport(x, 0, x, y);
			TextureManager::BindTexture(diffuse->GetTextureHandle(), m_textureUniform, 0, GL_TEXTURE_2D);
			glBindVertexArray(m_dummyVAO);
			glDrawArrays(GL_POINTS, 0, 1);

			glViewport(2 * x, 0, x, y);
			TextureManager::BindTexture(specular->GetTextureHandle(), m_textureUniform, 0, GL_TEXTURE_2D);
			glBindVertexArray(m_dummyVAO);
			glDrawArrays(GL_POINTS, 0, 1);

			glViewport(3 * x, 0, x, y);
			TextureManager::BindTexture(glowMatID->GetTextureHandle(), m_textureUniform, 0, GL_TEXTURE_2D);
			glBindVertexArray(m_dummyVAO);
			glDrawArrays(GL_POINTS, 0, 1);

			glViewport(0, 0, screenWidth, screenHeight);

		}
		else if (current == 6) // Draw shadowmap
		{
			glDepthMask(GL_FALSE);
			glViewport(0, 0, 128, 128);
			TextureManager::BindTexture(shadowMaps[0]->GetTextureHandle(), m_textureUniform, 0, GL_TEXTURE_2D);
			glBindVertexArray(m_dummyVAO);
			glDrawArrays(GL_POINTS, 0, 1);

			
			glViewport(128, 0, 128, 128);
			TextureManager::BindTexture(shadowMaps[1]->GetTextureHandle(), m_textureUniform, 0, GL_TEXTURE_2D);
			glBindVertexArray(m_dummyVAO);
			glDrawArrays(GL_POINTS, 0, 1);
			
			
			glViewport(256, 0, 128, 128);
			TextureManager::BindTexture(shadowMaps[2]->GetTextureHandle(), m_textureUniform, 0, GL_TEXTURE_2D);
			glBindVertexArray(m_dummyVAO);
			glDrawArrays(GL_POINTS, 0, 1);
			
			
			glViewport(384, 0, 128, 128);
			TextureManager::BindTexture(shadowMaps[3]->GetTextureHandle(), m_textureUniform, 0, GL_TEXTURE_2D);
			glBindVertexArray(m_dummyVAO);
			glDrawArrays(GL_POINTS, 0, 1);
			
			glDepthMask(GL_TRUE);
			glViewport(0, 0, screenWidth, screenHeight);
		}
		
		BasePainter::ClearFBO();
		TextureManager::UnbindTexture();
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		m_shaderManager->ResetProgram();
	}

}
