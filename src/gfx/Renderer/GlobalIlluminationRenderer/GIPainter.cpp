#include "GIPainter.hpp"

namespace GFX
{
	GIPainter::GIPainter(ShaderManager* shaderManager, UniformBufferManager* uniformBufferManager)
		: BasePainter(shaderManager, uniformBufferManager)
	{

	}

	GIPainter::~GIPainter()
	{

	}

	void GIPainter::Initialize(GLuint FBO, GLuint dummyVAO, int screenWidth, int screenHeight)
	{
		BasePainter::Initialize(FBO, dummyVAO);

		m_screenWidth = screenWidth;
		m_screenHeight = screenHeight;

		m_radianceTexture = new FBOTexture();
		m_SSDOTexture = new FBOTexture();

		InitFBO();

		Resize(screenWidth, screenHeight);

		m_shaderManager->CreateProgram("RadianceMap");
		m_shaderManager->LoadShader("shaders/RenderToQuad/Quad.vertex", "RMVS", GL_VERTEX_SHADER);
		m_shaderManager->LoadShader("shaders/FSQuadGS.glsl", "RMGS", GL_GEOMETRY_SHADER);
		m_shaderManager->LoadShader("shaders/GlobalIllumination/RadianceMapFS.glsl", "RMPS", GL_FRAGMENT_SHADER);
		m_shaderManager->AttachShader("RMVS", "RadianceMap");
		m_shaderManager->AttachShader("RMGS", "RadianceMap");
		m_shaderManager->AttachShader("RMPS", "RadianceMap");
		m_shaderManager->LinkProgram("RadianceMap");

		m_shaderManager->UseProgram("RadianceMap");

		m_diffuseUniform = m_shaderManager->GetUniformLocation("RadianceMap", "gDiffuse");
		m_normalsDepthUniform = m_shaderManager->GetUniformLocation("RadianceMap", "gNormals_depth");
		m_lightDirUniform = m_shaderManager->GetUniformLocation("RadianceMap", "gLightDiffuse");
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

	void GIPainter::Render(const double& delta, FBOTexture* normalDepth, FBOTexture* diffuse, glm::mat4 viewMatrix, glm::mat4 projMatrix)
	{

	}

}