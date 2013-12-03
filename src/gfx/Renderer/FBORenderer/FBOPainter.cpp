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
		m_shaderManager->LoadShader("shaders/PassThrough.vertex", "PTVS", GL_VERTEX_SHADER);
		m_shaderManager->LoadShader("shaders/FSQuad.geometry", "QuadGS", GL_GEOMETRY_SHADER);
		m_shaderManager->LoadShader("shaders/RenderToQuad/Quad.fragment", "QuadFS", GL_FRAGMENT_SHADER);
		m_shaderManager->CreateProgram("FSQuad");

		m_shaderManager->AttachShader("PTVS", "FSQuad");
		m_shaderManager->AttachShader("QuadGS", "FSQuad");
		m_shaderManager->AttachShader("QuadFS", "FSQuad");

		m_shaderManager->LinkProgram("FSQuad");

		
		m_textureUniform = m_shaderManager->GetUniformLocation("FSQuad", "textureIN");
	}

	void FBOPainter::Render(FBOTexture* normalDepth, FBOTexture* diffuse, FBOTexture* specular, FBOTexture* glowMatID )
	{
		m_shaderManager->UseProgram("FSQuad");

		glDisable(GL_DEPTH_TEST);
		glDisable(GL_BLEND);

		glViewport(0, 0, 500, 500);
		Texture::BindTexture(normalDepth->GetTextureHandle(), m_textureUniform, 0, GL_TEXTURE_2D );
		glBindVertexArray(m_dummyVAO);
		glDrawArrays(GL_POINTS, 0, 1);

		m_shaderManager->ResetProgram();

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);

		glViewport(0, 0, 1280, 720);
		BasePainter::ClearFBO();
	}

}