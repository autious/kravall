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

		InitFBO();

		Resize(screenWidth, screenHeight);

		m_shaderManager->ResetProgram();

		m_shaderManager->CreateProgram("SSDO");
		m_shaderManager->LoadShader("shaders/RenderToQuad/Quad.vertex", "SSDOVS", GL_VERTEX_SHADER);
		m_shaderManager->LoadShader("shaders/FSQuadGS.glsl", "SSDOGS", GL_GEOMETRY_SHADER);
		m_shaderManager->LoadShader("shaders/GlobalIllumination/DSSDOFS.glsl", "SSDOFS", GL_FRAGMENT_SHADER);
		m_shaderManager->AttachShader("SSDOVS", "SSDO");
		m_shaderManager->AttachShader("SSDOGS", "SSDO");
		m_shaderManager->AttachShader("SSDOFS", "SSDO");
		m_shaderManager->LinkProgram("SSDO");

		m_shaderManager->UseProgram("SSDO");

		m_seedTexture = TextureManager::LoadFromFile("assets/texture/noise.png", GL_TEXTURE_2D, GL_RGBA, GL_RGBA, GL_LINEAR, GL_LINEAR, GL_REPEAT, GL_REPEAT);
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
		m_shaderManager->UseProgram("SSDO");

		TextureManager::BindTexture(normalDepth->GetTextureHandle(), m_shaderManager->GetUniformLocation("SSDO", "gNormals_depth"), 0, GL_TEXTURE_2D);
		glGetError();
		TextureManager::BindTexture(m_seedTexture, m_shaderManager->GetUniformLocation("SSDO", "gNoiseTexture"), 1, GL_TEXTURE_2D);
		glGetError();

		m_shaderManager->SetUniform(12.0f, m_shaderManager->GetUniformLocation("SSDO", "gSampleRadius"));

		glGetError();

		m_shaderManager->SetUniform(4.0f, m_shaderManager->GetUniformLocation("SSDO", "gOcclusionRadius"));


		glGetError();
		m_shaderManager->SetUniform(16.0f, m_shaderManager->GetUniformLocation("SSDO", "gMaxOcclusionDistance"));
		glGetError();
		m_shaderManager->SetUniform(50.0f, m_shaderManager->GetUniformLocation("SSDO", "gMaxDistance"));
		glGetError();

		m_shaderManager->SetUniform(1280.0f, 720.0f, m_shaderManager->GetUniformLocation("SSDO", "gScreenSize"));
		glGetError();

		glm::mat3 rotMat(viewMatrix);
		glm::vec3 d(viewMatrix[3]);
		glm::mat4 invView = glm::inverse(viewMatrix);
		glm::vec4 camPos = invView[3];

		m_shaderManager->SetUniform(camPos.x, camPos.y, camPos.z, m_shaderManager->GetUniformLocation("SSDO", "gEyePosition"));
		glGetError();

		glm::mat4 invProjView = glm::inverse(projMatrix * viewMatrix);
		glGetError();

		m_shaderManager->SetUniform(1, invProjView, m_shaderManager->GetUniformLocation("SSDO", "gInvProjView"));
		glGetError();

		glBindVertexArray(m_dummyVAO);
		glDrawArrays(GL_POINTS, 0, 1);
		BasePainter::ClearFBO();
		m_shaderManager->ResetProgram();

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
	}

}