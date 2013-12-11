#include "LightPainter.hpp"

namespace GFX
{
	LightPainter::LightPainter(ShaderManager* shaderManager, UniformBufferManager* uniformBufferManager, RenderJobManager* renderJobManager)
		: BasePainter(shaderManager, uniformBufferManager)
	{
		m_renderJobManager = renderJobManager;
	}

	LightPainter::~LightPainter()
	{

	}

	
	void LightPainter::Initialize(GLuint FBO, GLuint dummyVAO)
	{
		BasePainter::Initialize(FBO, dummyVAO);

		m_shaderManager->CreateProgram("ComputeTest");
		m_shaderManager->LoadShader("shaders/ComputeTest.glsl", "Compute", GL_COMPUTE_SHADER);
		m_shaderManager->AttachShader("Compute", "ComputeTest");
		m_shaderManager->LinkProgram("ComputeTest");

		m_shaderManager->UseProgram("ComputeTest");

		glUniform1i(m_shaderManager->GetUniformLocation("ComputeTest", "destTex"), 0);

		glGenTextures(1, &textureHandle);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureHandle);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, 1280, 720, 0, GL_RGBA, GL_FLOAT, NULL);
		glBindImageTexture(0, textureHandle, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);
	}

	
	void LightPainter::Render(FBOTexture* depthBuffer, FBOTexture* normalDepth, FBOTexture* diffuse, FBOTexture* specular, FBOTexture* glowMatID, glm::mat4 viewMatrix, glm::mat4 projMatrix)
	{
		m_shaderManager->UseProgram("ComputeTest");
		m_shaderManager->SetUniform(1.0f, m_shaderManager->GetUniformLocation("ComputeTest", "roll"));

		TextureManager::BindTexture(normalDepth->GetTextureHandle(), m_shaderManager->GetUniformLocation("ComputeTest", "normal"), 0, GL_TEXTURE_2D);

		glDispatchCompute(1280 / 16, 720 / 16, 1);
	}
}