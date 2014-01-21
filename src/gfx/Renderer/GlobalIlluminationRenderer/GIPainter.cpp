#include "GIPainter.hpp"
#include <gfx/LightData.hpp>

namespace GFX
{
	GIPainter::GIPainter(ShaderManager* shaderManager, UniformBufferManager* uniformBufferManager, RenderJobManager* renderJobManager)
		: BasePainter(shaderManager, uniformBufferManager)
	{
		m_renderJobManager = renderJobManager;
	}

	GIPainter::~GIPainter()
	{

	}

	void GIPainter::Initialize(GLuint FBO, GLuint dummyVAO, int screenWidth, int screenHeight)
	{
		BasePainter::Initialize(FBO, dummyVAO);

		m_screenWidth = screenWidth;
		m_screenHeight = screenHeight;

		m_strength = 4.0f;
		m_singularity = 50.0f;
		m_depthBias = 1.0f;
		m_bounceStrength = 10700.0f;
		m_bounceSingularity = 2700.0f;
		m_sampleRadius = 50.0f;
		m_lightRotationAngle = 0.0f;
		m_sampleCount = 16.0f;
		m_maxSampleCount = 128;
		m_patternSize = 4;
		m_kernelSize = 4;
		m_positionThreshold = 30.0f;
		m_normalThreshold = 0.3f;
		m_maxRadiance = 4.5f;

		m_radianceTexture = new FBOTexture();
		m_SSDOTexture = new FBOTexture();

		InitFBO();

		Resize(screenWidth, screenHeight);
		GenerateSeedTexture();

		m_shaderManager->CreateProgram("RadianceMap");
		m_shaderManager->LoadShader("shaders/RenderToQuad/Quad.vertex", "RMVS", GL_VERTEX_SHADER);
		m_shaderManager->LoadShader("shaders/FSQuadGS.glsl", "RMGS", GL_GEOMETRY_SHADER);
		m_shaderManager->LoadShader("shaders/GlobalIllumination/RadianceMapFS.glsl", "RMPS", GL_FRAGMENT_SHADER);
		m_shaderManager->AttachShader("RMVS", "RadianceMap");
		m_shaderManager->AttachShader("RMGS", "RadianceMap");
		m_shaderManager->AttachShader("RMPS", "RadianceMap");
		m_shaderManager->LinkProgram("RadianceMap");

		m_shaderManager->UseProgram("RadianceMap");

		m_radDiffuseUniform = m_shaderManager->GetUniformLocation("RadianceMap", "gDiffuse");
		m_radNormalsDepthUniform = m_shaderManager->GetUniformLocation("RadianceMap", "gNormals_depth");
		m_lightDirUniform = m_shaderManager->GetUniformLocation("RadianceMap", "gLightDir");
		m_invViewProjUniform = m_shaderManager->GetUniformLocation("RadianceMap", "gInvViewProj");

		m_shaderManager->ResetProgram();

		m_shaderManager->CreateProgram("SSDO");
		m_shaderManager->LoadShader("shaders/RenderToQuad/Quad.vertex", "SSDOVS", GL_VERTEX_SHADER);
		m_shaderManager->LoadShader("shaders/FSQuadGS.glsl", "SSDOGS", GL_GEOMETRY_SHADER);
		m_shaderManager->LoadShader("shaders/GlobalIllumination/SSDOFS.glsl", "SSDOFS", GL_FRAGMENT_SHADER);
		m_shaderManager->AttachShader("SSDOVS", "SSDO");
		m_shaderManager->AttachShader("SSDOGS", "SSDO");
		m_shaderManager->AttachShader("SSDOFS", "SSDO");
		m_shaderManager->LinkProgram("SSDO");

		m_shaderManager->UseProgram("SSDO");

		m_ssdoDiffuseUniform = m_shaderManager->GetUniformLocation("SSDO", "gDiffuse");
		m_ssdoNormals_depthUniform = m_shaderManager->GetUniformLocation("SSDO", "gNormals_depth");
		m_radianceUniform = m_shaderManager->GetUniformLocation("SSDO", "gRadianceTexture");

		m_sampleRadiusUniform = m_shaderManager->GetUniformLocation("SSDO", "gSampleRadius");

		m_viewMatrixUniform = m_shaderManager->GetUniformLocation("SSDO", "gViewMatrix");
		m_projMatrixUniform = m_shaderManager->GetUniformLocation("SSDO", "gProjMatrix");
		m_ssdoInvViewProjUniform = m_shaderManager->GetUniformLocation("SSDO", "gInvViewProjMatrix");

		m_strengthUniform = m_shaderManager->GetUniformLocation("SSDO", "gStrength");
		m_singularityUniform = m_shaderManager->GetUniformLocation("SSDO", "gSingularity");
		m_depthBiasUniform = m_shaderManager->GetUniformLocation("SSDO", "gDepthBias");
		m_bounceStrengthUniform = m_shaderManager->GetUniformLocation("SSDO", "gBounceStrength");
		m_bounceSingularityUniform = m_shaderManager->GetUniformLocation("SSDO", "gBounceSingularity");

		m_sampleCountUniform = m_shaderManager->GetUniformLocation("SSDO", "gSampleCount");
		m_patternSizeUniform = m_shaderManager->GetUniformLocation("SSDO", "gPatternSize");
		m_seedTextureUniform = m_shaderManager->GetUniformLocation("SSDO", "gSeedTexture");
		m_lightRotationUniform = m_shaderManager->GetUniformLocation("SSDO", "gLightRotationAngle");

		
	}

	void GIPainter::InitFBO()
	{
		//Radiance FBO
		glGenFramebuffers(1, &m_radianceFBO);
		glBindFramebuffer(GL_FRAMEBUFFER, m_radianceFBO);

		m_radianceTexture->Initialize(GL_TEXTURE_2D, GL_NEAREST, GL_NEAREST, GL_CLAMP_TO_BORDER, GL_CLAMP_TO_BORDER, GL_RGBA32F, GL_RGBA);
		m_radianceTexture->UpdateResolution(m_screenWidth, m_screenHeight);
		
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_radianceTexture->GetTextureHandle(), 0);
		
		GLenum drawBuffers[] = { GL_COLOR_ATTACHMENT0 };
		glDrawBuffers(1, drawBuffers);
		
		GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		if (status != GL_FRAMEBUFFER_COMPLETE)
			std::cout << "Error initialising the radiance FBO\n";

		//reset the framebuffer
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		//SSDO FBO
		glGenFramebuffers(1, &m_SSDOFBO);
		glBindFramebuffer(GL_FRAMEBUFFER, m_SSDOFBO);

		m_SSDOTexture->Initialize(GL_TEXTURE_2D, GL_NEAREST, GL_NEAREST, GL_CLAMP_TO_BORDER, GL_CLAMP_TO_BORDER, GL_RGBA32F, GL_RGBA);
		m_SSDOTexture->UpdateResolution(m_screenWidth, m_screenHeight);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_SSDOTexture->GetTextureHandle(), 0);

		GLenum drawBuffers2[] = { GL_COLOR_ATTACHMENT0 };
		glDrawBuffers(1, drawBuffers2);

		status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		if (status != GL_FRAMEBUFFER_COMPLETE)
			std::cout << "Error initialising the SSDBO FBO\n";

		//reset the framebuffer
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}


	void GIPainter::Resize(int width, int height)
	{
		m_screenWidth = width;
		m_screenHeight = height;

		m_SSDOTexture->UpdateResolution(width, height);
		m_radianceTexture->UpdateResolution(width, height);

	}

	void GIPainter::BindRadianceFBO()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_radianceFBO);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_radianceTexture->GetTextureHandle(), 0);
		GLenum drawBuffers[] = { GL_COLOR_ATTACHMENT0 };
		glDrawBuffers(1, drawBuffers);
	}

	void GIPainter::BindSSDOFBO()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_SSDOFBO);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_SSDOTexture->GetTextureHandle(), 0);
		GLenum drawBuffers[] = { GL_COLOR_ATTACHMENT0 };
		glDrawBuffers(1, drawBuffers);
	}

	float GIPainter::HaltonNumber(const int base, int index)
	{
		float x = 0.0f;
		float f = 1.0f / base;

		while (index) 
		{
			x += f * (float)(index % base);
			index /= base;
			f *= 1.0f / base;
		}
		return x;
	}

	void GIPainter::GenerateSeedTexture()
	{
		int patternSizeSquared = m_patternSize * m_patternSize;

		srand(0);

		int haltonIndex = 0;
		float* const seedPixels = new float[3 * (int)m_maxSampleCount * patternSizeSquared];

		for (int i = 0; i < patternSizeSquared; i++) 
		{
			for (int j = 0; j < (int)m_maxSampleCount; j++) 
			{
				glm::vec3 sample;
				
				do {
					sample = glm::vec3(
						2.0f * HaltonNumber(2, haltonIndex) - 1.0f,
						2.0f * HaltonNumber(3, haltonIndex) - 1.0f,
						HaltonNumber(5, haltonIndex));
					haltonIndex++;

					// printf("sample dir %f %f %f\n",sample[0],sample[1],sample[2]);
				} while (glm::length(sample) > 1.0);

				seedPixels[(i * (int)m_maxSampleCount + j) * 3 + 0] = sample[0];
				seedPixels[(i * (int)m_maxSampleCount + j) * 3 + 1] = sample[1];
				seedPixels[(i * (int)m_maxSampleCount + j) * 3 + 2] = sample[2];
			}
		}

		glGenTextures(1, &m_seedTexture);
		glBindTexture(GL_TEXTURE_2D, m_seedTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F_ARB, m_maxSampleCount, patternSizeSquared, 0, GL_RGB, GL_FLOAT, seedPixels);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

		delete seedPixels;
	}

	void GIPainter::Render(const double& delta, FBOTexture* normalDepth, FBOTexture* diffuse, glm::mat4 viewMatrix, glm::mat4 projMatrix)
	{
		glm::mat4 invProjView = glm::inverse(projMatrix * viewMatrix);
		RenderRadiance(normalDepth, diffuse, invProjView);
		RenderSSDO(normalDepth, diffuse, invProjView, viewMatrix, projMatrix);

	}

	void GIPainter::RenderRadiance(FBOTexture* normalDepth, FBOTexture* diffuse, glm::mat4 invViewProj)
	{
		BindRadianceFBO();

		glDisable(GL_DEPTH_TEST);
		glDisable(GL_BLEND);

		m_shaderManager->UseProgram("RadianceMap");

		TextureManager::BindTexture(diffuse->GetTextureHandle(), m_radDiffuseUniform, 0, GL_TEXTURE_2D);
		TextureManager::BindTexture(normalDepth->GetTextureHandle(), m_radNormalsDepthUniform, 1, GL_TEXTURE_2D);
		m_shaderManager->SetUniform(1, invViewProj, m_invViewProjUniform);

		std::vector<RenderJobManager::RenderJob> renderJobs = m_renderJobManager->GetJobs();

		unsigned int objType = std::numeric_limits<decltype(objType)>::max();
		unsigned int viewport = std::numeric_limits<decltype(viewport)>::max();
		unsigned int layer = std::numeric_limits<decltype(layer)>::max();
		unsigned int translucency = std::numeric_limits<decltype(translucency)>::max();
		unsigned int meshID = std::numeric_limits<decltype(meshID)>::max();
		unsigned int lightType = std::numeric_limits<decltype(lightType)>::max();
		unsigned int depth = std::numeric_limits<decltype(depth)>::max();
		GFXBitmask bitmask;
		for (int i = 0; i < renderJobs.size(); i++)
		{
			bitmask = renderJobs[i].bitmask;

			objType = GetBitmaskValue(bitmask, BITMASK::TYPE);

			// Break if not a light object
			if (objType != GFX::OBJECT_TYPES::LIGHT)
			{
				continue;
			}

			lightType = GetBitmaskValue(bitmask, BITMASK::LIGHT_TYPE);

			if (lightType == GFX::LIGHT_TYPES::DIR)
			{
				LightData light = *reinterpret_cast<LightData*>(renderJobs[i].value);
				m_shaderManager->SetUniform(light.orientation.x, light.orientation.y, light.orientation.z, m_lightDirUniform);
			}
		}

		glBindVertexArray(m_dummyVAO);
		glDrawArrays(GL_POINTS, 0, 1);

		BasePainter::ClearFBO();
		m_shaderManager->ResetProgram();

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
	}

	void GIPainter::RenderSSDO(FBOTexture* normalDepth, FBOTexture* diffuse, glm::mat4 invViewProj, glm::mat4 viewMatrix, glm::mat4 projMatrix)
	{
		BindSSDOFBO();
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_BLEND);
		m_shaderManager->UseProgram("SSDO");

		TextureManager::BindTexture(diffuse->GetTextureHandle(), m_ssdoDiffuseUniform, 0, GL_TEXTURE_2D);
		TextureManager::BindTexture(normalDepth->GetTextureHandle(), m_ssdoNormals_depthUniform, 1, GL_TEXTURE_2D);
		TextureManager::BindTexture(m_radianceTexture->GetTextureHandle(), m_radianceUniform, 2, GL_TEXTURE_2D);
		TextureManager::BindTexture(m_seedTexture, m_seedTextureUniform, 3, GL_TEXTURE_2D);

		m_shaderManager->SetUniform((float)m_sampleRadiusUniform, m_sampleRadiusUniform);
		m_shaderManager->SetUniform(1, viewMatrix, m_viewMatrixUniform);
		m_shaderManager->SetUniform(1, projMatrix, m_projMatrixUniform);
		m_shaderManager->SetUniform(1, invViewProj, m_ssdoInvViewProjUniform);
		m_shaderManager->SetUniform(m_strength, m_strengthUniform);
		m_shaderManager->SetUniform(m_singularity, m_singularityUniform);
		m_shaderManager->SetUniform(m_depthBias, m_depthBiasUniform);
		m_shaderManager->SetUniform(m_bounceStrength, m_bounceStrengthUniform);
		m_shaderManager->SetUniform(m_bounceSingularity, m_bounceSingularityUniform);
		m_shaderManager->SetUniform(m_sampleCount, m_sampleCountUniform);
		m_shaderManager->SetUniform(m_patternSize, m_patternSizeUniform);

		std::vector<RenderJobManager::RenderJob> renderJobs = m_renderJobManager->GetJobs();

		unsigned int objType = std::numeric_limits<decltype(objType)>::max();
		unsigned int viewport = std::numeric_limits<decltype(viewport)>::max();
		unsigned int layer = std::numeric_limits<decltype(layer)>::max();
		unsigned int translucency = std::numeric_limits<decltype(translucency)>::max();
		unsigned int meshID = std::numeric_limits<decltype(meshID)>::max();
		unsigned int lightType = std::numeric_limits<decltype(lightType)>::max();
		unsigned int depth = std::numeric_limits<decltype(depth)>::max();
		GFXBitmask bitmask;
		for (int i = 0; i < renderJobs.size(); i++)
		{
			bitmask = renderJobs[i].bitmask;

			objType = GetBitmaskValue(bitmask, BITMASK::TYPE);

			// Break if not a light object
			if (objType != GFX::OBJECT_TYPES::LIGHT)
			{
				continue;
			}

			lightType = GetBitmaskValue(bitmask, BITMASK::LIGHT_TYPE);

			if (lightType == GFX::LIGHT_TYPES::DIR)
			{
				LightData light = *reinterpret_cast<LightData*>(renderJobs[i].value);
				float angle = glm::angle(light.orientation, glm::vec3(0,1,0));
				m_shaderManager->SetUniform(1.0f, m_lightRotationUniform);

				glGetError();
			}
		}

		glBindVertexArray(m_dummyVAO);

		glGetError();

		glDrawArrays(GL_POINTS, 0, 1);

		glGetError();

		BasePainter::ClearFBO();

		glGetError();

		m_shaderManager->ResetProgram();

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
	}

}