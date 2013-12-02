#include "DeferredPainter.hpp"

namespace GFX
{
	DeferredPainter::DeferredPainter(ShaderManager* shaderManager, BufferManager* bufferManager, UniformBufferManager* uniformBufferManager)
		: BasePainter(shaderManager, bufferManager, uniformBufferManager)
	{
	}

	DeferredPainter::~DeferredPainter()
	{
	}
	
	void DeferredPainter::Initialize(GLuint FBO, GLuint dummyVAO)
	{
		BasePainter::Initialize(FBO, dummyVAO);

		m_shaderManager->CreateProgram("StaticMesh");

		m_shaderManager->LoadShader("shaders/SimpleGeometry.vertex", "StaticMeshVS", GL_VERTEX_SHADER);
		m_shaderManager->LoadShader("shaders/SimpleGeometry.fragment", "StaticMeshFS", GL_FRAGMENT_SHADER);

		//m_shaderManager->LoadShader("shaders/PassThrough.vertex", "StaticMeshVS", GL_VERTEX_SHADER);
		//m_shaderManager->LoadShader("shaders/FSQuad.geometry", "StaticMeshGS", GL_GEOMETRY_SHADER);
		//m_shaderManager->LoadShader("shaders/WaveRipple.fragment", "StaticMeshFS", GL_FRAGMENT_SHADER);
		
		m_shaderManager->AttachShader("StaticMeshVS", "StaticMesh");
		//m_shaderManager->AttachShader("StaticMeshGS", "StaticMesh");
		m_shaderManager->AttachShader("StaticMeshFS", "StaticMesh");

		m_shaderManager->LinkProgram("StaticMesh");

		exampleUniform = m_shaderManager->GetUniformLocation("StaticMesh", "inputColor");

		m_uniformBufferManager->CreateBasicCameraUBO(m_shaderManager->GetShaderProgramID("StaticMesh"));
	}

	void DeferredPainter::Render(FBOTexture* normalDepth, FBOTexture* diffuse, FBOTexture* specular, FBOTexture* glowMatID, glm::mat4 viewMatrix, glm::mat4 projMatrix)
	{
		BasePainter::Render();

		glEnable(GL_DEPTH_TEST);
		glDisable(GL_BLEND);
		//BindGBuffer(normalDepth, diffuse, specular, glowMatID);

		m_shaderManager->UseProgram("StaticMesh");
		
		BasicCamera bc;
		bc.viewMatrix = viewMatrix;
		bc.projMatrix = projMatrix;
		
		m_uniformBufferManager->SetBasicCameraUBO(bc);
		
		//glBindVertexArray(m_dummyVAO);

		glDrawArrays(GL_TRIANGLES, 0, 8127);

		m_shaderManager->ResetProgram();

		ClearFBO();
	}


	void DeferredPainter::BindGBuffer(FBOTexture* normalDepth, FBOTexture* diffuse, FBOTexture* specular, FBOTexture* glowMatID)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, normalDepth->GetTextureHandle(),	0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, diffuse->GetTextureHandle(),		0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, specular->GetTextureHandle(),		0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D, glowMatID->GetTextureHandle(),		0);

		// define outputs
		GLenum drawBuffers[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
		glDrawBuffers(4, drawBuffers);
	}
}
