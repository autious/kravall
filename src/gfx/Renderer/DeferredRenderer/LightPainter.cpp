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

		matrixUniform = m_shaderManager->GetUniformLocation("ComputeTest", "inv_proj_view_mat");

		glUniform1i(m_shaderManager->GetUniformLocation("ComputeTest", "destTex"), 0);

		glGenTextures(1, &textureHandle);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureHandle);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, 1280, 720, 0, GL_RGBA, GL_FLOAT, NULL);
		glBindImageTexture(0, textureHandle, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);



		alphaUniform = m_shaderManager->GetUniformLocation("TQ", "alphaIN");
		textureUniform = m_shaderManager->GetUniformLocation("TQ", "textureIN");


		m_pointLights = new PointLightData[m_maximumLights];
		PointLightData p;

		p.position = glm::vec3(0.0f, 0.0f, 0.0f);
		p.color = glm::vec3((rand() % 1000) / 1000.0f, (rand() % 1000) / 1000.0f, (rand() % 1000) / 1000.0f);
		p.intensity = 0.3f;
		p.radius = 10.0f;
		m_pointLights[0] = p;

		for(int i = 1; i < m_maximumLights; i++)
		{
			p.position = glm::vec3(-50 + 100.0 * (rand() % 1000) / 1000.0f, -75 + 150.0 * (rand() % 1000) / 1000.0f, 0.0f);
			p.color = glm::vec3((rand() % 1000) / 1000.0f, (rand() % 1000) / 1000.0f, (rand() % 1000) / 1000.0f);
			p.intensity = 0.3f;
			p.radius = 10.0f;
			m_pointLights[i] = p;
		}

		glGenBuffers(1, &m_pointLightBuffer);
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_pointLightBuffer);
		glBufferData(GL_SHADER_STORAGE_BUFFER, m_maximumLights * sizeof(PointLightData), m_pointLights, GL_DYNAMIC_COPY);
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 5, m_pointLightBuffer);


	}

	static double llooollll = 0.0;
	void LightPainter::Render(FBOTexture* depthBuffer, FBOTexture* normalDepth, FBOTexture* diffuse, FBOTexture* specular, FBOTexture* glowMatID, glm::mat4 viewMatrix, glm::mat4 projMatrix)
	{
		m_shaderManager->UseProgram("ComputeTest");

		glm::mat4 invProjView = glm::inverse(projMatrix * viewMatrix);
		m_shaderManager->SetUniform(1, invProjView, matrixUniform);

		//TextureManager::BindTexture(diffuse->GetTextureHandle(), m_shaderManager->GetUniformLocation("ComputeTest", "normal"), 0, GL_TEXTURE_2D);
		glBindImageTexture(0, textureHandle, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);
		glBindImageTexture(1, normalDepth->GetTextureHandle(), 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA32F);
		glBindImageTexture(2, diffuse->GetTextureHandle(), 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA32F);
		glBindImageTexture(3, specular->GetTextureHandle(), 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA32F);
		glBindImageTexture(4, glowMatID->GetTextureHandle(), 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA32F);

		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 5, m_pointLightBuffer);
		PointLightData* pData = (PointLightData*)glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, m_maximumLights * sizeof(PointLightData), GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);
		PointLightData p;
		for (int i = 0; i < m_maximumLights; i++)
		{
			m_pointLights[i].position.x = 150.0f * sin(llooollll + (double)i);
			//m_pointLights[i].position.y = 100 - ((double)i/(double)m_maximumLights) * 200 + 100.0f * cos(llooollll + (double)i);
			p = m_pointLights[i];
			//p.position = m_pointLights[i].position;// glm::vec3(-100 + 200.0 * (rand() % 1000) / 1000.0f, -100 + 200.0 * (rand() % 1000) / 1000.0f, 0.0f);
			//p.color = glm::vec3(1.0f, 0.0f, 0.0f);// glm::vec3((rand() % 1000) / 1000.0f, (rand() % 1000) / 1000.0f, (rand() % 1000) / 1000.0f);
			//p.intensity = 1.0f;
			//p.radius = 20.0f;
			pData[i] = p;
		}
		llooollll += 0.01;
		glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);

		glDispatchCompute(1280 / 16, 720 / 16, 1);
		glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

		m_shaderManager->UseProgram("TQ");

		glDisable(GL_DEPTH_TEST);
		glDisable(GL_BLEND);

		m_shaderManager->SetUniform(1.0f, alphaUniform);

		TextureManager::BindTexture(textureHandle, textureUniform, 0, GL_TEXTURE_2D);

		glBindVertexArray(m_dummyVAO);
		glDrawArrays(GL_POINTS, 0, 1);

		m_shaderManager->ResetProgram();

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);

		BasePainter::ClearFBO();

		TextureManager::UnbindTexture();

	}
}
