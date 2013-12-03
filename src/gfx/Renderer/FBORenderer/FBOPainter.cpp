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
		m_shaderManager->LoadShader("shaders/FSQuad.geometry", "QGS", GL_GEOMETRY_SHADER);
		m_shaderManager->LoadShader("shaders/RenderToQuad/Quad.fragment", "QFS", GL_FRAGMENT_SHADER);
		m_shaderManager->CreateProgram("TQ");

		m_shaderManager->AttachShader("QVS", "TQ");
		m_shaderManager->AttachShader("QGS", "TQ");
		m_shaderManager->AttachShader("QFS", "TQ");

		m_shaderManager->LinkProgram("TQ");

		m_alphaUniform = m_shaderManager->GetUniformLocation("TQ", "alphaIN");
		m_textureUniform = m_shaderManager->GetUniformLocation("TQ", "textureIN");
	}

	void FBOPainter::Render(FBOTexture* normalDepth, FBOTexture* diffuse, FBOTexture* specular, FBOTexture* glowMatID, int screenWidth, int screenHeight, int current )
	{
		m_shaderManager->UseProgram("TQ");

		glDisable(GL_DEPTH_TEST);
		glDisable(GL_BLEND);

		m_shaderManager->SetUniform(1.0f, m_alphaUniform);

		switch (current)
		{
			case 0:
				//Draw composite to back buffer
				break;
			case 1:
				Texture::BindTexture(normalDepth->GetTextureHandle(), m_textureUniform, 0, GL_TEXTURE_2D);
				break;
			case 2:
				Texture::BindTexture(diffuse->GetTextureHandle(), m_textureUniform, 0, GL_TEXTURE_2D);
				break;
			case 3:
				Texture::BindTexture(specular->GetTextureHandle(), m_textureUniform, 0, GL_TEXTURE_2D);
				break;
			case 4:
				Texture::BindTexture(glowMatID->GetTextureHandle(), m_textureUniform, 0, GL_TEXTURE_2D);
				break;
		}
		glBindVertexArray(m_dummyVAO);
		glDrawArrays(GL_POINTS, 0, 1);

		float x = screenWidth / 4;
		glm::vec2 vpDim = glm::vec2(288, 162);
		float offset = (screenWidth - (vpDim.x * 4.0f)) / 4.0f;
		float halfOffset = offset / 2;

		
		glViewport(halfOffset, 50, vpDim.x, vpDim.y);
		Texture::BindTexture(normalDepth->GetTextureHandle(), m_textureUniform, 0, GL_TEXTURE_2D );
		glBindVertexArray(m_dummyVAO);
		glDrawArrays(GL_POINTS, 0, 1);

		glViewport(vpDim.x + offset + halfOffset, 50, vpDim.x, vpDim.y);
		Texture::BindTexture(diffuse->GetTextureHandle(), m_textureUniform, 0, GL_TEXTURE_2D);
		glBindVertexArray(m_dummyVAO);
		glDrawArrays(GL_POINTS, 0, 1);

		glViewport((vpDim.x + offset) * 2 + halfOffset, 50, vpDim.x, vpDim.y);
		Texture::BindTexture(specular->GetTextureHandle(), m_textureUniform, 0, GL_TEXTURE_2D);
		glBindVertexArray(m_dummyVAO);
		glDrawArrays(GL_POINTS, 0, 1);
		
		glViewport((vpDim.x + offset) * 3 + halfOffset, 50, vpDim.x, vpDim.y);
		Texture::BindTexture(glowMatID->GetTextureHandle(), m_textureUniform, 0, GL_TEXTURE_2D);
		glBindVertexArray(m_dummyVAO);
		glDrawArrays(GL_POINTS, 0, 1);

		m_shaderManager->ResetProgram();

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);

		glViewport(0, 0, 1280, 720);
		BasePainter::ClearFBO();
	}

}