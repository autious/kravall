#include "LightPainter.hpp"
#include <logger/Logger.hpp>
#include <limits>

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

		m_invProjViewUniform = m_shaderManager->GetUniformLocation("ComputeTest", "inv_proj_view_mat");
		m_viewUniform = m_shaderManager->GetUniformLocation("ComputeTest", "view");
		m_projUniform = m_shaderManager->GetUniformLocation("ComputeTest", "proj");
		m_frambufferSizeUniform = m_shaderManager->GetUniformLocation("ComputeTest", "framebufferDim");



		numActiveLightsUniform = m_shaderManager->GetUniformLocation("ComputeTest", "numActiveLights");
		

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


		m_pointLights = new LightData[m_maximumLights];
		//LightData p;
		//
		//p.position = glm::vec3(0.0f, 0.0f, 0.0f);
		//p.color = glm::vec3((rand() % 1000) / 1000.0f, (rand() % 1000) / 1000.0f, (rand() % 1000) / 1000.0f);
		//p.intensity = 1.0f;
		//p.radius_length = 10.0f;
		//m_pointLights[0] = p;
		//
		//for(int i = 1; i < m_maximumLights; i++)
		//{
		//	p.position = glm::vec3(-50 + 100.0 * (rand() % 1000) / 1000.0f, -75 + 150.0 * (rand() % 1000) / 1000.0f, 0.0f);
		//	p.color = glm::vec3((rand() % 1000) / 1000.0f, (rand() % 1000) / 1000.0f, (rand() % 1000) / 1000.0f);
		//	p.intensity = 1.0f;
		//	p.radius_length = 10.0f;
		//	m_pointLights[i] = p;
		//}

		glGenBuffers(1, &m_pointLightBuffer);
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_pointLightBuffer);
		//glBufferData(GL_SHADER_STORAGE_BUFFER, m_maximumLights * sizeof(LightData), m_pointLights, GL_DYNAMIC_COPY);
		glBufferData(GL_SHADER_STORAGE_BUFFER, m_maximumLights * sizeof(LightData), NULL, GL_STREAM_COPY);
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 5, m_pointLightBuffer);


	}

	static double llooollll = 0.0;
	void LightPainter::Render(unsigned int& renderIndex, FBOTexture* depthBuffer, FBOTexture* normalDepth, FBOTexture* diffuse, FBOTexture* specular, FBOTexture* glowMatID, glm::mat4 viewMatrix, glm::mat4 projMatrix)
	{
		m_shaderManager->UseProgram("ComputeTest");

		glm::mat4 invProjView = glm::inverse(projMatrix * viewMatrix);
		m_shaderManager->SetUniform(1, projMatrix, m_projUniform);
		m_shaderManager->SetUniform(1280, 720, m_frambufferSizeUniform);
		m_shaderManager->SetUniform(1, viewMatrix, m_viewUniform);
		m_shaderManager->SetUniform(1, invProjView, m_invProjViewUniform);


		std::vector<RenderJobManager::RenderJob> renderJobs = m_renderJobManager->GetJobs();

		unsigned int objType = std::numeric_limits<decltype(objType)>::max();
		unsigned int viewport = std::numeric_limits<decltype(viewport)>::max();
		unsigned int layer = std::numeric_limits<decltype(layer)>::max();
		unsigned int translucency = std::numeric_limits<decltype(translucency)>::max();
		unsigned int meshID = std::numeric_limits<decltype(meshID)>::max();
		unsigned int lightType = std::numeric_limits<decltype(lightType)>::max();
		unsigned int depth = std::numeric_limits<decltype(depth)>::max();

		//Assemble lights
		unsigned int i = 0;
		unsigned int numLights = 0;
		unsigned int totalNumLights = 0;
		GFXBitmask bitmask;
		bool ok = true;
		for (i = renderIndex; i < renderJobs.size(); i++)
		{

			bitmask = renderJobs[i].bitmask;

			objType = GetBitmaskValue(bitmask, BITMASK::TYPE);

			// Break if no opaque object
			if (objType != GFX::OBJECT_TYPES::LIGHT)
			{
				break;
			}

			lightType = GetBitmaskValue(bitmask, BITMASK::LIGHT_TYPE);
			// Do point lights only
			if (lightType != GFX::LIGHT_TYPES::POINT)
			{
				break;
			}

			viewport = GetBitmaskValue(bitmask, BITMASK::VIEWPORT_ID);
			layer = GetBitmaskValue(bitmask, BITMASK::LAYER);
			translucency = GetBitmaskValue(bitmask, BITMASK::TRANSLUCENCY_TYPE);
			meshID = GetBitmaskValue(bitmask, BITMASK::MESH_ID);
			depth = GetBitmaskValue(bitmask, BITMASK::DEPTH);

			if (numLights < m_maximumLights)
			{
				m_pointLights[numLights] = *reinterpret_cast<LightData*>(renderJobs[i].value);
				numLights++;
			}
			else
			{
				ok = false;
			}
			totalNumLights++;
		}
		if (!ok)
		{
			LOG_GFXSPECIAL << "Too many lights. Light draw calls: " << totalNumLights << " Maximum is: " << m_maximumLights << "\n";
			LogSystem::Mute(LOG_GFXSPECIAL.GetPrefix());
		}
		else
		{
			LogSystem::Unmute(LOG_GFXSPECIAL.GetPrefix());
		}
		renderIndex = i;

		m_shaderManager->SetUniform((GLuint)numLights, numActiveLightsUniform);

		//TextureManager::BindTexture(diffuse->GetTextureHandle(), m_shaderManager->GetUniformLocation("ComputeTest", "normal"), 0, GL_TEXTURE_2D);
		glBindImageTexture(0, textureHandle, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);
		glBindImageTexture(1, normalDepth->GetTextureHandle(), 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA32F);
		glBindImageTexture(2, diffuse->GetTextureHandle(), 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA32F);
		glBindImageTexture(3, specular->GetTextureHandle(), 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA32F);
		glBindImageTexture(4, glowMatID->GetTextureHandle(), 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA32F);

		LightData* pData = (LightData*)glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, m_maximumLights * sizeof(LightData), GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);
		LightData p;
		for (unsigned int i = 0; i < numLights; i++)
		{
			pData[i] = m_pointLights[i];
		}
		llooollll += 0.01;
		glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);

		glDispatchCompute(1280 / 16, 720 / 16, 1);
		//glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

		m_shaderManager->UseProgram("TQ");

		glDisable(GL_DEPTH_TEST);
		glDisable(GL_BLEND);
		glDepthMask(GL_FALSE);

		m_shaderManager->SetUniform(1.0f, alphaUniform);

		TextureManager::BindTexture(textureHandle, textureUniform, 0, GL_TEXTURE_2D);

		glBindVertexArray(m_dummyVAO);
		glDrawArrays(GL_POINTS, 0, 1);

		m_shaderManager->ResetProgram();

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glDepthMask(GL_TRUE);

		BasePainter::ClearFBO();

		TextureManager::UnbindTexture();
		renderIndex = 0;
	}
}
