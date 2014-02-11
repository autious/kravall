#include "LightPainter.hpp"
#include <logger/Logger.hpp>
#include "../ShadowData.hpp"
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


	void LightPainter::Initialize(GLuint FBO, GLuint dummyVAO, int screenWidth, int screenHeight)
	{
		BasePainter::Initialize(FBO, dummyVAO);
		m_screenWidth = screenWidth;
		m_screenHeight = screenHeight;


		m_shaderManager->CreateProgram("ComputeLighting");
		m_shaderManager->LoadShader("shaders/ComputeLighting.glsl", "Compute", GL_COMPUTE_SHADER);
		m_shaderManager->AttachShader("Compute", "ComputeLighting");
		m_shaderManager->LinkProgram("ComputeLighting");

		m_shaderManager->UseProgram("ComputeLighting");
		
		m_invProjViewUniform = m_shaderManager->GetUniformLocation("ComputeLighting", "invProjView");
		m_viewUniform = m_shaderManager->GetUniformLocation("ComputeLighting", "view");
		m_projUniform = m_shaderManager->GetUniformLocation("ComputeLighting", "proj");
		m_frambufferSizeUniform = m_shaderManager->GetUniformLocation("ComputeLighting", "framebufferDim");

		m_eyePosUniform = m_shaderManager->GetUniformLocation("ComputeLighting", "eyePos");

		m_gammaUniform = m_shaderManager->GetUniformLocation("ComputeLighting", "gGamma");
		m_exposureUniform = m_shaderManager->GetUniformLocation("ComputeLighting", "gExposure");
		m_whitePointUniform = m_shaderManager->GetUniformLocation("ComputeLighting", "gWhitePoint");

		m_numActiveLightsUniform = m_shaderManager->GetUniformLocation("ComputeLighting", "numActiveLights");

		m_numPointLightsUniform = m_shaderManager->GetUniformLocation("ComputeLighting", "numPointLights");
		m_numSpotLightsUniform = m_shaderManager->GetUniformLocation("ComputeLighting", "numSpotLights");
		m_numDirLightsUniform = m_shaderManager->GetUniformLocation("ComputeLighting", "numDirLights");
		m_numAmbientLightsUniform = m_shaderManager->GetUniformLocation("ComputeLighting", "numAmbientLights");

		m_numPointShadowsUniform = m_shaderManager->GetUniformLocation("ComputeLighting", "numPointShadows");
		m_numSpotShadowsUniform = m_shaderManager->GetUniformLocation("ComputeLighting", "numSpotShadows");
		m_numDirShadowsUniform = m_shaderManager->GetUniformLocation("ComputeLighting", "numDirShadows");

		glUniform1i(m_shaderManager->GetUniformLocation("ComputeLighting", "destTex"), 0);

		m_alphaUniform = m_shaderManager->GetUniformLocation("TQ", "alphaIN");
		m_textureUniform = m_shaderManager->GetUniformLocation("TQ", "textureIN");

		Resize(m_screenWidth, m_screenHeight);
		

		m_lights = new LightData[MAXIMUM_LIGHTS];

		glGenBuffers(1, &m_pointLightBuffer);
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_pointLightBuffer);
		glBufferData(GL_SHADER_STORAGE_BUFFER, MAXIMUM_LIGHTS * sizeof(LightData), NULL, GL_STREAM_COPY);
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 5, m_pointLightBuffer);


		glGenBuffers(1, &m_shadowDataBuffer);
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_shadowDataBuffer);
		glBufferData(GL_SHADER_STORAGE_BUFFER, MAXIMUM_SHADOWCASTERS * sizeof(ShadowData), NULL, GL_STREAM_COPY);
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 8, m_shadowDataBuffer);

	}

	void LightPainter::Resize(int screenWidth, int screenHeight)
	{
		glDeleteTextures(1, &m_textureHandle);

		m_screenWidth = screenWidth;
		m_screenHeight = screenHeight;

		glGenTextures(1, &m_textureHandle);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_textureHandle);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, m_screenWidth, m_screenHeight, 0, GL_RGBA, GL_FLOAT, NULL);
		glBindImageTexture(0, m_textureHandle, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);
	}



	void LightPainter::Render(unsigned int& renderIndex, FBOTexture* depthBuffer, FBOTexture* normalDepth, FBOTexture* diffuse, FBOTexture* specular, FBOTexture* glowMatID, FBOTexture* SSDOTexture,
		FBOTexture* shadowMap, glm::mat4 viewMatrix, glm::mat4 projMatrix, float exposure, float gamma, glm::vec3 whitePoint, GLuint& toneMappedTexture)
	{
		m_shaderManager->UseProgram("ComputeLighting");

		glm::vec3 cameraPos = -glm::vec3(viewMatrix[3]) * glm::mat3(viewMatrix);
		m_shaderManager->SetUniform(1, cameraPos, m_eyePosUniform);

		glm::mat4 invProjView = glm::inverse(projMatrix * viewMatrix);
		m_shaderManager->SetUniform(1, projMatrix, m_projUniform);
		m_shaderManager->SetUniform(GLfloat(m_screenWidth), GLfloat(m_screenHeight), m_frambufferSizeUniform);
		m_shaderManager->SetUniform(1, viewMatrix, m_viewUniform);
		m_shaderManager->SetUniform(1, invProjView, m_invProjViewUniform);

		m_shaderManager->SetUniform(exposure, m_exposureUniform);
		m_shaderManager->SetUniform(gamma, m_gammaUniform);
		m_shaderManager->SetUniform(1, whitePoint, m_whitePointUniform);

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

		unsigned int totalNumLights[GFX::LIGHT_TYPES::COUNT];
		memset(totalNumLights, 0, sizeof(totalNumLights));
		
		unsigned int numLights = 0;
		unsigned int totalDrawCalls = 0;
		GFXBitmask bitmask;
		bool ok = true;

		for (i = renderIndex; i < renderJobs.size(); i++)
		{

			bitmask = renderJobs[i].bitmask;

			objType = GetBitmaskValue(bitmask, BITMASK::TYPE);

			// Break if not a light object
			if (objType != GFX::OBJECT_TYPES::LIGHT)
			{
				break;
			}

			lightType = GetBitmaskValue(bitmask, BITMASK::LIGHT_TYPE);
			// Do point and directional lights only
			if (lightType != GFX::LIGHT_TYPES::POINT && lightType != GFX::LIGHT_TYPES::DIR 
				&& lightType != GFX::LIGHT_TYPES::AMBIENT && lightType != GFX::LIGHT_TYPES::SPOT && lightType != GFX::LIGHT_TYPES::DIR_SHADOW)
			{
				//if (lightType == GFX::LIGHT_TYPES::POINT_SHADOW || lightType == GFX::LIGHT_TYPES::SPOT_SHADOW ||
				//	lightType == GFX::LIGHT_TYPES::DIR_SHADOW)
				//	continue;
				//else
					break;
			}

			viewport = GetBitmaskValue(bitmask, BITMASK::VIEWPORT_ID);
			layer = GetBitmaskValue(bitmask, BITMASK::LAYER);
			translucency = GetBitmaskValue(bitmask, BITMASK::TRANSLUCENCY_TYPE);
			meshID = GetBitmaskValue(bitmask, BITMASK::MESH_ID);
			depth = GetBitmaskValue(bitmask, BITMASK::DEPTH);
			

			if (numLights < MAXIMUM_LIGHTS)
			{


				m_lights[numLights] = *reinterpret_cast<LightData*>(renderJobs[i].value);

				// Count light types
				totalNumLights[lightType]++;
				numLights++;
			}
			else
			{
				ok = false;
			}
			totalDrawCalls++;
		}
		if (!ok)
		{
			LOG_GFXSPECIAL << "Too many lights. Light draw calls: " << totalDrawCalls << " Maximum is: " << MAXIMUM_LIGHTS << "\n";
			LogSystem::Mute(LOG_GFXSPECIAL.GetPrefix());
		}
		else
		{
			LogSystem::Unmute(LOG_GFXSPECIAL.GetPrefix());
		}
		renderIndex = i;

		m_shaderManager->SetUniform((GLuint)numLights, m_numActiveLightsUniform);

		m_shaderManager->SetUniform((GLuint)totalNumLights[LIGHT_TYPES::POINT], m_numPointLightsUniform);
		m_shaderManager->SetUniform((GLuint)totalNumLights[LIGHT_TYPES::SPOT], m_numSpotLightsUniform);
		m_shaderManager->SetUniform((GLuint)totalNumLights[LIGHT_TYPES::DIR], m_numDirLightsUniform);
		m_shaderManager->SetUniform((GLuint)totalNumLights[LIGHT_TYPES::AMBIENT], m_numAmbientLightsUniform);

		m_shaderManager->SetUniform((GLuint)ShadowDataContainer::numPointLights, m_numPointShadowsUniform);
		m_shaderManager->SetUniform((GLuint)ShadowDataContainer::numSpotLights, m_numSpotShadowsUniform);
		m_shaderManager->SetUniform((GLuint)ShadowDataContainer::numDirLights, m_numDirShadowsUniform);

		//TextureManager::BindTexture(diffuse->GetTextureHandle(), m_shaderManager->GetUniformLocation("ComputeLighting", "normal"), 0, GL_TEXTURE_2D);
		glBindImageTexture(0, m_textureHandle, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);
		glBindImageTexture(1, normalDepth->GetTextureHandle(), 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA32F);
		glBindImageTexture(2, diffuse->GetTextureHandle(), 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA32F);
		glBindImageTexture(3, specular->GetTextureHandle(), 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA32F);
		glBindImageTexture(4, glowMatID->GetTextureHandle(), 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA32F);
		glBindImageTexture(5, SSDOTexture->GetTextureHandle(), 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA32F);

		
		m_shadowMapUniform = m_shaderManager->GetUniformLocation("ComputeLighting", "shadowMap");
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, shadowMap->GetTextureHandle());
		//glBindImageTexture(6, shadowMap->GetTextureHandle(), 0, GL_FALSE, 0, GL_READ_ONLY, GL_RG32F);


		// Transfer Lights to GPU
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_pointLightBuffer);
		LightData* lightData = (LightData*)glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, MAXIMUM_LIGHTS * sizeof(LightData), 
			GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT | GL_MAP_UNSYNCHRONIZED_BIT);

		memcpy(lightData, m_lights, numLights * sizeof(LightData));

		glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);

		// Transfer Shadow Data to GPU
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_shadowDataBuffer);
		ShadowData* shadowData = (ShadowData*)glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, MAXIMUM_SHADOWCASTERS * sizeof(ShadowData),
			GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT | GL_MAP_UNSYNCHRONIZED_BIT);

		memcpy(shadowData, ShadowDataContainer::data, MAXIMUM_SHADOWCASTERS * sizeof(ShadowData));

		glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);

		// Run the shader
		glDispatchCompute(GLuint((m_screenWidth + WORK_GROUP_SIZE - 1) / float(WORK_GROUP_SIZE)), GLuint((m_screenHeight + WORK_GROUP_SIZE - 1) / float(WORK_GROUP_SIZE)), 1);
		
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);




		//m_shaderManager->UseProgram("TQ");
		//
		//glDisable(GL_DEPTH_TEST);
		//glDisable(GL_BLEND);
		//glDepthMask(GL_FALSE);
		//
		//m_shaderManager->SetUniform(1.0f, m_alphaUniform);
		//
		//TextureManager::BindTexture(m_textureHandle, m_textureUniform, 0, GL_TEXTURE_2D);
		//
		//glBindVertexArray(m_dummyVAO);
		//glDrawArrays(GL_POINTS, 0, 1);
		//
		//m_shaderManager->ResetProgram();
		//
		//glEnable(GL_DEPTH_TEST);
		//glEnable(GL_BLEND);
		//glDepthMask(GL_TRUE);




		BasePainter::ClearFBO();

		TextureManager::UnbindTexture();

		toneMappedTexture = m_textureHandle;
		renderIndex = 0;
	}
}
