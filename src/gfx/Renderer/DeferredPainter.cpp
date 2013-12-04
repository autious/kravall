#include "DeferredPainter.hpp"

namespace GFX
{
	DeferredPainter::DeferredPainter(ShaderManager* shaderManager, UniformBufferManager* uniformBufferManager)
		: BasePainter(shaderManager, uniformBufferManager)
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
		
		m_shaderManager->AttachShader("StaticMeshVS", "StaticMesh");
		m_shaderManager->AttachShader("StaticMeshFS", "StaticMesh");

		m_shaderManager->LinkProgram("StaticMesh");


		m_diffuseUniform = m_shaderManager->GetUniformLocation("StaticMesh", "diffuseMap");
		m_normalUniform = m_shaderManager->GetUniformLocation("StaticMesh", "normalMap");
		m_specularUniform = m_shaderManager->GetUniformLocation("StaticMesh", "specularMap");
		m_glowUniform = m_shaderManager->GetUniformLocation("StaticMesh", "glowMap");

		m_uniformBufferManager->CreateBasicCameraUBO(m_shaderManager->GetShaderProgramID("StaticMesh"));
	}

	void DeferredPainter::AddRenderJob(const GLuint& ibo, const GLuint& vao, const int& size, Material* mat)
	{
		RenderJob rj;
		rj.ibo = ibo;
		rj.vao = vao;
		rj.size = size;
		rj.m = mat;
		m_renderJobs.push_back(rj);
	}

	void DeferredPainter::Render(FBOTexture* normalDepth, FBOTexture* diffuse, FBOTexture* specular, FBOTexture* glowMatID, glm::mat4 viewMatrix, glm::mat4 projMatrix)
	{
		BasePainter::Render();

		glEnable(GL_DEPTH_TEST);
		glDisable(GL_BLEND);
		BindGBuffer(normalDepth, diffuse, specular, glowMatID);
		m_shaderManager->UseProgram("StaticMesh");
		
		BasicCamera bc;
		bc.viewMatrix = viewMatrix;
		bc.projMatrix = projMatrix;
		
		m_uniformBufferManager->SetBasicCameraUBO(bc);
		
		for (unsigned int i = 0; i < m_renderJobs.size(); i++)
		{
			Texture::BindTexture(m_renderJobs.at(i).m->diffuse, m_diffuseUniform, 0, GL_TEXTURE_2D);
			Texture::BindTexture(m_renderJobs.at(i).m->normal, m_normalUniform, 1, GL_TEXTURE_2D);
			Texture::BindTexture(m_renderJobs.at(i).m->specular, m_specularUniform, 2, GL_TEXTURE_2D);
			Texture::BindTexture(m_renderJobs.at(i).m->glow, m_glowUniform, 3, GL_TEXTURE_2D);

			glBindVertexArray(m_renderJobs.at(i).vao);
			glDrawArrays(GL_TRIANGLES, 0, m_renderJobs.at(i).size);
		}

		m_renderJobs.clear();
		//glBindVertexArray(m_dummyVAO);
		//glDrawArrays(GL_TRIANGLES, 0, 8127);

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
