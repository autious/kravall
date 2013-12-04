#include "RenderCore.hpp"

namespace GFX
{
	RenderCore& Renderer()
	{
		static RenderCore r;

		return r;
	}

	RenderCore::RenderCore()
	{
	}

	RenderCore::~RenderCore()
	{
	
	}

	void RenderCore::Delete()
	{
		delete(m_normalDepth);
		delete(m_diffuse);
		delete(m_specular);
		delete(m_glowMatID);

		delete(m_uniformBufferManager);
		delete(m_shaderManager);

		delete(m_deferredPainter);
		delete(m_textPainter);
		delete(m_debugPainter);
		delete(m_consolePainter);
		delete(m_splashPainter);
		delete(m_fboPainter);
	}

	void RenderCore::Initialize(int windowWidth, int windowHeight)
	{
		m_windowWidth = 0;
		m_windowHeight = 0;

		m_normalDepth = new FBOTexture();
		m_diffuse = new FBOTexture();
		m_specular = new FBOTexture();
		m_glowMatID = new FBOTexture();

		m_shaderManager = new ShaderManager();
		m_uniformBufferManager = new UniformBufferManager();

		m_deferredPainter = new DeferredPainter(m_shaderManager, m_uniformBufferManager);
		m_debugPainter = new DebugPainter(m_shaderManager, m_uniformBufferManager);
		m_textPainter = new TextPainter(m_shaderManager, m_uniformBufferManager);
		m_consolePainter = new ConsolePainter(m_shaderManager, m_uniformBufferManager);
		m_splashPainter = new SplashPainter(m_shaderManager, m_uniformBufferManager);
		m_fboPainter = new FBOPainter(m_shaderManager, m_uniformBufferManager);
		m_playSplash = false;


		m_windowWidth = windowWidth;
		m_windowHeight = windowHeight;

		InitializeDummyVAO();
		InitializeGBuffer();


		m_deferredPainter->Initialize(m_FBO, m_dummyVAO);
		m_debugPainter->Initialize(m_FBO, m_dummyVAO);
		m_textPainter->Initialize(m_FBO, m_dummyVAO);
		m_consolePainter->Initialize(m_FBO, m_dummyVAO);
		m_splashPainter->Initialize(m_FBO, m_dummyVAO);
		m_fboPainter->Initialize(m_FBO, m_dummyVAO);

		// Set console width
		m_consolePainter->SetConsoleHeight(m_windowHeight);

	}

	void RenderCore::Resize(int width, int height)
	{
		m_windowWidth = width;
		m_windowHeight = height;

		glViewport(0, 0, m_windowWidth, m_windowHeight);
		ResizeGBuffer();

		// Set console width
		m_consolePainter->SetConsoleHeight(m_windowHeight);
	}

	
	void RenderCore::AddRenderJob(const GLuint& ibo, const GLuint& vao, const int& iboSize, const int& shaderID, Material* m, glm::mat4* matrix)
	{
		m_deferredPainter->AddRenderJob(ibo, vao, iboSize, shaderID, matrix, m);
	}

	void RenderCore::Render()
	{

		if (m_playSplash)
		{
			m_splashPainter->Render(m_windowWidth, m_windowHeight);
			if (m_splashPainter->IsDone())
				m_playSplash = false;
			return;
		}

		m_deferredPainter->Render(m_normalDepth, m_diffuse, m_specular, m_glowMatID, m_viewMatrix, m_projMatrix);
		
		m_fboPainter->Render(m_normalDepth, m_diffuse, m_specular, m_glowMatID, m_windowWidth, m_windowHeight, 2);

		// Render debug
		m_debugPainter->Render(m_viewMatrix, m_projMatrix);
		
		// Render console
		m_consolePainter->Render();
		
		// Render text
		m_textPainter->Render();
	}

	void RenderCore::InitializeGBuffer()
	{
		//Generate the FBO
		glGenFramebuffers(1, &m_FBO);
		glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);

		//Initialize GBuffer textures, second to last parameter should be configurable through settings
		m_normalDepth->Initialize(GL_TEXTURE_2D, GL_NEAREST, GL_NEAREST, GL_CLAMP_TO_BORDER, GL_CLAMP_TO_BORDER, GL_RGBA32F, GL_RGBA);
		m_diffuse->Initialize(GL_TEXTURE_2D, GL_NEAREST, GL_NEAREST, GL_CLAMP_TO_BORDER, GL_CLAMP_TO_BORDER, GL_RGBA32F, GL_RGBA);
		m_specular->Initialize(GL_TEXTURE_2D, GL_NEAREST, GL_NEAREST, GL_CLAMP_TO_BORDER, GL_CLAMP_TO_BORDER, GL_RGBA32F, GL_RGBA);
		m_glowMatID->Initialize(GL_TEXTURE_2D, GL_NEAREST, GL_NEAREST, GL_CLAMP_TO_BORDER, GL_CLAMP_TO_BORDER, GL_RGBA32F, GL_RGBA);

		ResizeGBuffer();

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_normalDepth->GetTextureHandle(), 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, m_diffuse->GetTextureHandle(), 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, m_specular->GetTextureHandle(), 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D, m_glowMatID->GetTextureHandle(), 0);

		// define outputs
		GLenum drawBuffers[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
		glDrawBuffers(4, drawBuffers);

		GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		if (status != GL_FRAMEBUFFER_COMPLETE)
			std::cout << "Error initialising the deferred system\n";

		//reset the framebuffer
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void RenderCore::ResizeGBuffer()
	{
		m_normalDepth->UpdateResolution(m_windowWidth, m_windowHeight);
		m_diffuse->UpdateResolution(m_windowWidth, m_windowHeight);
		m_specular->UpdateResolution(m_windowWidth, m_windowHeight);
		m_glowMatID->UpdateResolution(m_windowWidth, m_windowHeight);
	}

	void RenderCore::InitializeDummyVAO()
	{
		float dummy = 1;

		GLuint VBO;
		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float), &dummy, GL_STATIC_DRAW);

		glGenVertexArrays(1, &m_dummyVAO);
		glBindVertexArray(m_dummyVAO);

		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glVertexAttribPointer(0, 1, GL_FLOAT, GL_FALSE, 0, (GLubyte*)NULL);

		glBindVertexArray(0);
	}

	void RenderCore::SetViewMatrix(glm::mat4 view)
	{
		m_viewMatrix = view;
	}

	void RenderCore::SetProjMatrix(glm::mat4 proj)
	{
		m_projMatrix = proj;
	}

	bool RenderCore::GetConsoleVisible()
	{
		return m_consolePainter->GetConsoleVisible();
	}

	void RenderCore::SetConsoleVisible(bool visible)
	{
		m_consolePainter->SetConsoleVisible(visible);
	}
	void RenderCore::SetSplash(bool splash)
	{
		m_playSplash = splash;
	}
}