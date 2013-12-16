#include "RenderCore.hpp"
#include <sstream>
#include <iomanip>
#include <utility/Colors.hpp>

namespace GFX
{
	RenderCore& Renderer()
	{
		static RenderCore r;

		return r;
	}

	RenderCore::RenderCore()
	{
		m_lastUpdateTime = 0;
		m_curTime = 0;
		m_showStatistics = false;
        m_font = nullptr;
		m_showFBO = false;
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
		delete(m_renderJobManager);
		delete(m_textureManager);
		delete(m_materialManager);

		delete(m_deferredPainter);
		delete(m_lightPainter);
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
		m_depthBuffer = new FBOTexture();

		m_shaderManager = new ShaderManager();
		m_uniformBufferManager = new UniformBufferManager();
		m_renderJobManager = new RenderJobManager();
		m_meshManager = new MeshManager();
		m_textureManager = new TextureManager();
		m_materialManager = new MaterialManager();

		m_deferredPainter = new DeferredPainter(m_shaderManager, m_uniformBufferManager, 
			m_renderJobManager, m_meshManager, m_textureManager, m_materialManager);

		m_lightPainter = new LightPainter(m_shaderManager, m_uniformBufferManager, m_renderJobManager);

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
		m_lightPainter->Initialize(m_FBO, m_dummyVAO);
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

	
	void RenderCore::AddRenderJob(GFXBitmask bitmask, void* value)
	{
		m_renderJobManager->AddRenderJob(bitmask, value);
	}

	void RenderCore::DeleteMesh(unsigned long long id)
	{
		m_meshManager->DeleteMesh(id);
	}
	
	void RenderCore::LoadStaticMesh(unsigned int& meshID, const int& sizeVerts, const int& sizeIndices, StaticVertex* verts, int* indices)
	{
		m_meshManager->LoadStaticMesh(meshID, sizeVerts, sizeIndices, verts, indices);
	}

	void RenderCore::LoadTexture(unsigned int& id, unsigned char* data, int width, int height)
	{
		m_textureManager->LoadTexture(id, data, width, height);
	}

	void RenderCore::DeleteTexture(unsigned long long int id)
	{
		m_textureManager->DeleteTexture(id);
	}

	
	void RenderCore::CreateMaterial(unsigned long long int& id)
	{
		m_materialManager->CreateMaterial(id);
	}

	void RenderCore::DeleteMaterial(const unsigned long long int& id)
	{
		m_materialManager->DeleteMaterial(id);
	}

	void RenderCore::AddTextureToMaterial(const unsigned long long int& materialID, const unsigned long long int& textureID)
	{
		m_materialManager->AddTexture(materialID, textureID);
	}

	void RenderCore::RemoveTextureFromMaterial(const unsigned long long int& materialID, const unsigned long long int& textureID)
	{
		m_materialManager->RemoveTexture(materialID, textureID);
	}

	void RenderCore::SetShaderToMaterial(const unsigned long long int& materialID, const unsigned int& shaderID)
	{
		m_materialManager->SetShader(materialID, shaderID);
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

		// Render last frame statistics
		bool updateStats = false;

		if (m_showStatistics)
		{
			SubSystemTimeRender();
			m_curTime = Timer().GetTotal() + 1024;
			if (m_curTime - m_lastUpdateTime > 200)
			{
				m_lastUpdateTime = m_curTime;
				m_subsystemTimes.clear();
				updateStats = true;
			}
		}

		//* Build GBuffers for all geometry										\
		//* When a call to light source is next in the render jobs list			|
		//	- Save index of last geometry/first light in the render jobs list	 > DeferredPainter
		//	- Break the loop													|
		//																		/
		//* For each light with shadow in the render jobs list, starting at     \
		//	the index obtained from the previous step.                        |
		//	- Assign and build depth buffer atlas for each light with shadow   > LightBuilder
		//	- Break when first light without shadow is encountered            |
		//																	  /
		//
		//* Apply lighting for lights with shadow
		//* Apply lighting for lights without shadow

		if (updateStats && m_showStatistics)
		{
			GFX_CHECKTIME(glFinish(), "glFinish");

			GFX_CHECKTIME(m_renderJobManager->Sort(), "Sorting");
			GFX_CHECKTIME(m_deferredPainter->Render(m_depthBuffer, m_normalDepth, m_diffuse, m_specular, m_glowMatID, m_viewMatrix, m_projMatrix), "Geometry");
			//GFX_CHECKTIME(m_lightPainter->Render(m_depthBuffer, m_normalDepth, m_diffuse, m_specular, m_glowMatID, m_viewMatrix, m_projMatrix), "Lighting");

			//Render FBO
			if (m_showFBO != -1)
				GFX_CHECKTIME(m_fboPainter->Render(m_normalDepth, m_diffuse, m_specular, m_glowMatID, m_windowWidth, m_windowHeight, m_showFBO), "FBO");

			GFX_CHECKTIME(m_debugPainter->Render(m_viewMatrix, m_projMatrix), "Debug");
			GFX_CHECKTIME(m_consolePainter->Render(), "Console");
			GFX_CHECKTIME(m_textPainter->Render(), "Text");
		}
		else
		{
			m_renderJobManager->Sort();
			m_deferredPainter->Render(m_depthBuffer, m_normalDepth, m_diffuse, m_specular, m_glowMatID, m_viewMatrix, m_projMatrix);
			//m_lightPainter->Render(m_depthBuffer, m_normalDepth, m_diffuse, m_specular, m_glowMatID, m_viewMatrix, m_projMatrix);
			
			//Render FBO
			if (m_showFBO != -1)
				m_fboPainter->Render(m_normalDepth, m_diffuse, m_specular, m_glowMatID, m_windowWidth, m_windowHeight, m_showFBO);

			m_debugPainter->Render(m_viewMatrix, m_projMatrix);
			m_consolePainter->Render();
			m_textPainter->Render();
		}

		m_renderJobManager->Clear();

	}

	void RenderCore::SubSystemTimeRender()
	{
		if( m_showStatistics && m_font)
		{

			for( int i = 0; i < (int)m_subsystemTimes.size(); i++ )
			{
				std::stringstream ss;
            
				ss << m_subsystemTimes[i].first << ": " << std::fixed << std::setw( 7 ) << std::setprecision(4) << std::setfill( '0' ) << m_subsystemTimes[i].second.count() / 1000.0f << "ms";
				glm::vec2 position = glm::vec2(m_windowWidth-200+5, m_windowHeight + 12 - 20 * m_subsystemTimes.size() + 20 * i);

				Text t(position.x, position.y, 1.0f, 1.0f, m_font, Colors::White, ss.str().c_str(), m_windowWidth, m_windowHeight);
			    GetTextManager().AddText(t);
			}

			glm::vec2 position = glm::vec2(m_windowWidth-200, m_windowHeight - 5 - 20 * m_subsystemTimes.size());
			glm::vec2 dimensions = glm::vec2(200, 20 * m_subsystemTimes.size());

			DebugRect r;
			r.color = glm::vec4( 0.5f,0.5f,0.5f,0.5f);
			r.position = glm::vec3(
				position.x / float(Renderer().GetWindowWidth() / 2) - 1.0f,
				1.0f - position.y / float(Renderer().GetWindowHeight() / 2), 0.0f);
			r.dimensions = glm::vec3(
				dimensions.x / float(Renderer().GetWindowWidth())*2,
				dimensions.y / float(Renderer().GetWindowHeight())*2, 0.0f);
			DebugDrawing().AddRect(r, true);

		}
	}
	void RenderCore::InitializeGBuffer()
	{
		//Generate the FBO
		glGenFramebuffers(1, &m_FBO);
		glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);

		//Initialize GBuffer textures, second to last parameter should be configurable through settings
		m_depthBuffer->Initialize(GL_TEXTURE_2D, GL_NEAREST, GL_NEAREST, GL_CLAMP_TO_BORDER, GL_CLAMP_TO_BORDER, GL_DEPTH_COMPONENT16, GL_DEPTH_COMPONENT);
		m_normalDepth->Initialize(GL_TEXTURE_2D, GL_NEAREST, GL_NEAREST, GL_CLAMP_TO_BORDER, GL_CLAMP_TO_BORDER, GL_RGBA32F, GL_RGBA);
		m_diffuse->Initialize(GL_TEXTURE_2D, GL_NEAREST, GL_NEAREST, GL_CLAMP_TO_BORDER, GL_CLAMP_TO_BORDER, GL_RGBA32F, GL_RGBA);
		m_specular->Initialize(GL_TEXTURE_2D, GL_NEAREST, GL_NEAREST, GL_CLAMP_TO_BORDER, GL_CLAMP_TO_BORDER, GL_RGBA32F, GL_RGBA);
		m_glowMatID->Initialize(GL_TEXTURE_2D, GL_NEAREST, GL_NEAREST, GL_CLAMP_TO_BORDER, GL_CLAMP_TO_BORDER, GL_RGBA32F, GL_RGBA);

		ResizeGBuffer();

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_depthBuffer->GetTextureHandle(), 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_normalDepth->GetTextureHandle(), 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, m_diffuse->GetTextureHandle(), 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, m_specular->GetTextureHandle(), 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D, m_glowMatID->GetTextureHandle(), 0);

		// define outputs
		GLenum drawBuffers[] = { GL_NONE, GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
		glDrawBuffers(5, drawBuffers);

		GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		if (status != GL_FRAMEBUFFER_COMPLETE)
			std::cout << "Error initialising the deferred system\n";

		//reset the framebuffer
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void RenderCore::ResizeGBuffer()
	{
		m_depthBuffer->UpdateResolution(m_windowWidth, m_windowHeight);
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
