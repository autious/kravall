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

		m_SSDOTexture = new FBOTexture();

		m_occlusionRadius = 3.0f;
		m_maxOcclusionDistance = 14.0f;

		InitFBO();

		Resize(screenWidth, screenHeight);

		m_shaderManager->ResetProgram();

		m_shaderManager->CreateProgram("DSSDO");
		m_shaderManager->LoadShader("shaders/RenderToQuad/Quad.vertex", "DSSDOVS", GL_VERTEX_SHADER);
		m_shaderManager->LoadShader("shaders/FSQuadGS.glsl", "DSSDOGS", GL_GEOMETRY_SHADER);
		m_shaderManager->LoadShader("shaders/GlobalIllumination/DSSDOFS.glsl", "DSSDOFS", GL_FRAGMENT_SHADER);
		m_shaderManager->AttachShader("DSSDOVS", "DSSDO");
		m_shaderManager->AttachShader("DSSDOGS", "DSSDO");
		m_shaderManager->AttachShader("DSSDOFS", "DSSDO");
		m_shaderManager->LinkProgram("DSSDO");
		m_shaderManager->UseProgram("DSSDO");

		m_invProjUniform = m_shaderManager->GetUniformLocation("DSSDO", "gInvProjView");
		m_eyePosUniform = m_shaderManager->GetUniformLocation("DSSDO", "gEyePosition");
		m_screenSizeUniform = m_shaderManager->GetUniformLocation("DSSDO", "gScreenSize");
		m_maxOcclusionDistanceUniform = m_shaderManager->GetUniformLocation("DSSDO", "gMaxOcclusionDistance");
		m_occlusionRadiusUniform = m_shaderManager->GetUniformLocation("DSSDO", "gOcclusionRadius");

		m_normalDepthUniform = m_shaderManager->GetUniformLocation("DSSDO", "gNormals_depth");
		m_noiseUniform = m_shaderManager->GetUniformLocation("DSSDO", "gNoiseTexture");

		m_seedTexture = TextureManager::LoadFromFile("assets/texture/noise.png", GL_TEXTURE_2D, GL_RGBA, GL_RGBA, GL_LINEAR, GL_LINEAR, GL_REPEAT, GL_REPEAT);

		m_shaderManager->ResetProgram();

		m_shaderManager->CreateProgram("SSDORadiance");
		m_shaderManager->LoadShader("shaders/RenderToQuad/Quad.vertex", "RadianceVS", GL_VERTEX_SHADER);
		m_shaderManager->LoadShader("shaders/FSQuadGS.glsl", "RadianceGS", GL_GEOMETRY_SHADER);
		m_shaderManager->LoadShader("shaders/GlobalIllumination/SSDORadianceFS.glsl", "RadianceFS", GL_FRAGMENT_SHADER);
		m_shaderManager->AttachShader("RadianceVS", "SSDORadiance");
		m_shaderManager->AttachShader("RadianceGS", "SSDORadiance");
		m_shaderManager->AttachShader("RadianceFS", "SSDORadiance");
		m_shaderManager->LinkProgram("SSDORadiance");
		m_shaderManager->UseProgram("SSDORadiance");

		m_shaderManager->CreateProgram("SSDO");
		m_shaderManager->LoadShader("shaders/RenderToQuad/Quad.vertex", "SSDOVS", GL_VERTEX_SHADER);
		m_shaderManager->LoadShader("shaders/FSQuadGS.glsl", "SSDOGS", GL_GEOMETRY_SHADER);
		m_shaderManager->LoadShader("shaders/GlobalIllumination/SSDOFS.glsl", "SSDOFS", GL_FRAGMENT_SHADER);
		m_shaderManager->AttachShader("SSDOVS", "SSDO");
		m_shaderManager->AttachShader("SSDOGS", "SSDO");
		m_shaderManager->AttachShader("SSDOFS", "SSDO");
		m_shaderManager->LinkProgram("SSDO");
		m_shaderManager->UseProgram("SSDO");
	}

	void GIPainter::InitFBO()
	{
		//SSDO FBO
		glGenFramebuffers(1, &m_SSDOFBO);
		glBindFramebuffer(GL_FRAMEBUFFER, m_SSDOFBO);

		m_SSDOTexture->Initialize(GL_TEXTURE_2D, GL_NEAREST, GL_NEAREST, GL_CLAMP_TO_BORDER, GL_CLAMP_TO_BORDER, GL_RGBA32F, GL_RGBA);
		m_SSDOTexture->UpdateResolution(m_screenWidth, m_screenHeight);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_SSDOTexture->GetTextureHandle(), 0);

		GLenum drawBuffers2[] = { GL_COLOR_ATTACHMENT0 };
		glDrawBuffers(1, drawBuffers2);

		//reset the framebuffer
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}


	void GIPainter::Resize(int width, int height)
	{
		m_screenWidth = width;
		m_screenHeight = height;

		m_SSDOTexture->UpdateResolution(width, height);

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
		glm::mat4 invProjView = glm::inverse(projMatrix * viewMatrix);
		RenderSSDO(normalDepth, diffuse, invProjView, viewMatrix, projMatrix);
	}

	void GIPainter::RenderSSDO(FBOTexture* normalDepth, FBOTexture* diffuse, glm::mat4 invViewProj, glm::mat4 viewMatrix, glm::mat4 projMatrix)
	{
		BindSSDOFBO();
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_BLEND);
		m_shaderManager->UseProgram("DSSDO");

		TextureManager::BindTexture(normalDepth->GetTextureHandle(), m_normalDepthUniform, 0, GL_TEXTURE_2D);
		TextureManager::BindTexture(m_seedTexture, m_noiseUniform, 1, GL_TEXTURE_2D);

		m_shaderManager->SetUniform(m_occlusionRadius, m_occlusionRadiusUniform);
		m_shaderManager->SetUniform(m_maxOcclusionDistance, m_maxOcclusionDistanceUniform);
		m_shaderManager->SetUniform(m_screenWidth, m_screenHeight, m_screenSizeUniform);

		glm::mat4 invView = glm::inverse(viewMatrix);
		glm::vec4 camPos = invView[3];

		m_shaderManager->SetUniform(camPos.x, camPos.y, camPos.z, m_eyePosUniform);

		glm::mat4 invProjView = glm::inverse(projMatrix * viewMatrix);
		m_shaderManager->SetUniform(1, invProjView, m_invProjUniform);

		glBindVertexArray(m_dummyVAO);
		glDrawArrays(GL_POINTS, 0, 1);
		BasePainter::ClearFBO();
		m_shaderManager->ResetProgram();

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
	}

}