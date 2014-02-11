#include "RenderCore.hpp"
#include <sstream>
#include <algorithm>
#include <iomanip>
#include <utility/Colors.hpp>
#include <GFXDefines.hpp>
#include "ShadowData.hpp"
ShadowData ShadowDataContainer::data[MAXIMUM_SHADOWCASTERS];

unsigned int ShadowDataContainer::numDirLights = 0;
unsigned int ShadowDataContainer::numSpotLights = 0;
unsigned int ShadowDataContainer::numPointLights = 0;


#include "DeferredRenderer/FBOTexture.hpp"
#include "DeferredRenderer/LightPainter.hpp"
#include "DeferredRenderer/DeferredPainter.hpp"
#include "Console/ConsolePainter.hpp"
#include "OverlayRenderer/OverlayPainter.hpp"
#include "DebugRenderer/DebugPainter.hpp"
#include "TextRenderer/TextPainter.hpp"
#include "SplashRenderer/SplashPainter.hpp"
#include "FBORenderer/FBOPainter.hpp"
#include "PostProcessing/PostProcessingPainter.hpp"
#include "GlobalIlluminationRenderer/GIPainter.hpp"
#include "PostProcessing/BlurPainter.hpp"

#include <Shaders/ShaderManager.hpp>
#include <Buffers/UniformBufferManager.hpp>
#include "TextRenderer/TextManager.hpp"
#include "DebugRenderer/DebugManager.hpp"
#include "RenderJobManager.hpp"
#include "../Buffers/MeshManager.hpp"
#include "../Textures/TextureManager.hpp"
#include "../Material/MaterialManager.hpp"

#include <FontData.hpp>
#include <Vertex.hpp>

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
		m_showFBO = 0;
		m_animationFramerate = 24;
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
		delete(m_animationManager);

		delete(m_deferredPainter);
		delete(m_lightPainter);
		delete(m_textPainter);
		delete(m_debugPainter);
		delete(m_consolePainter);
		delete(m_splashPainter);
		delete(m_fboPainter);
        delete(m_overlayPainter);
		delete(m_blurPainter);
		delete(m_shadowPainter);
	}

	int RenderCore::SetConfiguration(const int setting, const int value)
	{
		if (setting >= 0 && setting < GFX_SETTINGS_COUNT)
		{
			m_settings[setting] = value;
			return GFX_SUCCESS;
		}
		else
		{
			return GFX_FAIL;
		}
	}

	int RenderCore::GetConfiguration(const int setting, int& out_value)
	{
		if (setting >= 0 && setting < GFX_SETTINGS_COUNT)
		{
			out_value = m_settings[setting];
			return GFX_SUCCESS;
		}
		else
		{
			return GFX_FAIL;
		}
	}

	void RenderCore::Initialize(int windowWidth, int windowHeight)
	{
		m_windowWidth = 0;
		m_windowHeight = 0;
		
		// Set default settings
		m_settings[GFX_SHADOW_QUALITY] = GFX_SHADOWS_VARIANCE; // TODO: Implement basic shadow mapping as low res option
		m_settings[GFX_SHADOW_RESOLUTION] = 1024;

		m_normalDepth = new FBOTexture();
		m_diffuse = new FBOTexture();
		m_specular = new FBOTexture();
		m_glowMatID = new FBOTexture();
		m_depthBuffer = new FBOTexture();
		m_shadowMapTexture = new FBOTexture();

		// Fill shadowmap with reds!
		m_shadowMapTexture->CreateShadowmap(m_settings[GFX_SHADOW_RESOLUTION], m_settings[GFX_SHADOW_QUALITY]);




		m_shaderManager = new ShaderManager();
		m_uniformBufferManager = new UniformBufferManager();
		m_renderJobManager = new RenderJobManager();
		m_meshManager = new MeshManager();
		m_textureManager = new TextureManager();
		m_materialManager = new MaterialManager();
		m_animationManager = new AnimationManagerGFX();

		m_deferredPainter = new DeferredPainter(m_shaderManager, m_uniformBufferManager, 
		m_renderJobManager, m_meshManager, m_textureManager, m_materialManager);

		m_shadowPainter = new ShadowPainter(m_shaderManager, m_uniformBufferManager, m_renderJobManager, m_meshManager);

		m_lightPainter = new LightPainter(m_shaderManager, m_uniformBufferManager, m_renderJobManager);

		m_postProcessingPainter = new PostProcessingPainter(m_shaderManager, m_uniformBufferManager, m_textureManager);

		m_GIPainter = new GIPainter(m_shaderManager, m_uniformBufferManager, m_renderJobManager);
		m_blurPainter = new BlurPainter(m_shaderManager, m_uniformBufferManager);

		m_debugPainter = new DebugPainter(m_shaderManager, m_uniformBufferManager);
		m_textPainter = new TextPainter(m_shaderManager, m_uniformBufferManager);
		m_consolePainter = new ConsolePainter(m_shaderManager, m_uniformBufferManager);
		m_splashPainter = new SplashPainter(m_shaderManager, m_uniformBufferManager);
		m_fboPainter = new FBOPainter(m_shaderManager, m_uniformBufferManager);
		m_overlayPainter = new OverlayPainter(m_shaderManager, m_uniformBufferManager, 
		m_renderJobManager, m_meshManager, m_textureManager, m_materialManager);
		m_playSplash = false;


		m_windowWidth = windowWidth;
		m_windowHeight = windowHeight;

		InitializeDummyVAO();
		InitializeGBuffer();


		m_deferredPainter->Initialize(m_FBO, m_dummyVAO);
		m_shadowPainter->Initialize(m_FBO, m_dummyVAO, m_blurPainter, m_settings[GFX_SHADOW_RESOLUTION]);
		m_lightPainter->Initialize(m_FBO, m_dummyVAO, m_windowWidth, m_windowHeight);
		m_debugPainter->Initialize(m_FBO, m_dummyVAO);
		m_textPainter->Initialize(m_FBO, m_dummyVAO);
		m_consolePainter->Initialize(m_FBO, m_dummyVAO);
		m_splashPainter->Initialize(m_FBO, m_dummyVAO);
		m_fboPainter->Initialize(m_FBO, m_dummyVAO);
        m_overlayPainter->Initialize(m_FBO, m_dummyVAO);
		m_blurPainter->Initialize(m_FBO, m_dummyVAO);
		m_postProcessingPainter->Initialize(m_FBO, m_dummyVAO, m_windowWidth, m_windowHeight, m_blurPainter);
		m_GIPainter->Initialize(m_FBO, m_dummyVAO, m_windowWidth, m_windowHeight);
		

		// Set console width
		m_consolePainter->SetConsoleHeight(m_windowHeight);

		LoadGPUPF();

		m_gamma = 2.2f;
		m_exposure = 1.0f;
		m_whitePoint = glm::vec3(1.0f);

		m_currentLUT = "identity";
	}

	void RenderCore::LoadGPUPF()
	{
		m_shaderManager->CreateProgram("GPUPFCompute");
		m_shaderManager->LoadShader("shaders/GPUPF.glsl", "GPUPF", GL_COMPUTE_SHADER);
		m_shaderManager->AttachShader("GPUPF", "GPUPFCompute");
		m_shaderManager->LinkProgram("GPUPFCompute");
	}

	void RenderCore::Resize(int width, int height)
	{
		m_windowWidth = width;
		m_windowHeight = height;

		glViewport(0, 0, m_windowWidth, m_windowHeight);
		ResizeGBuffer();
		m_lightPainter->Resize(width, height);
		m_postProcessingPainter->Resize(width, height);
		// Set console width
		m_consolePainter->SetConsoleHeight(m_windowHeight);
	}

	void RenderCore::SetLUT(std::string LUT)
	{
		m_currentLUT = LUT;
	}

	void RenderCore::SetExposure(float exposure)
	{
		m_exposure = exposure;
	}

	void RenderCore::SetGamma(float gamma)
	{
		m_gamma = gamma;
	}

	void RenderCore::SetWhitepoint(glm::vec3 whitePoint)
	{
		m_whitePoint = whitePoint;
	}

	void RenderCore::ReloadLUT()
	{
		m_postProcessingPainter->ReloadLUT();
	}
	void RenderCore::SetAnimationFramerate(unsigned int framerate)
	{
		m_animationFramerate = std::max(std::min(framerate, 48U), 12U);
	}
	
	void RenderCore::AddRenderJob(GFXBitmask bitmask, void* value)
	{
		m_renderJobManager->AddRenderJob(bitmask, value);
	}

	void RenderCore::DeleteMesh(unsigned long long id)
	{
		m_meshManager->DeleteMesh(id);
	}
	
	void RenderCore::LoadMesh(unsigned int& meshID, int& sizeVerts, int& sizeIndices, GFX::Vertex* verts, int* indices)
	{
		m_meshManager->LoadMesh(meshID, sizeVerts, sizeIndices, verts, indices);
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

	int RenderCore::AddTextureToMaterial(const unsigned long long int& materialID, const unsigned long long int& textureID)//////////////////////
	{
		return m_materialManager->AddTexture(materialID, textureID);
	}

	void RenderCore::RemoveTextureFromMaterial(const unsigned long long int& materialID, const unsigned long long int& textureID)
	{
		m_materialManager->RemoveTexture(materialID, textureID);
	}

    int RenderCore::GetShaderId(unsigned int& shaderId, const char* shaderName) ////////////////////////////////////
    {
        shaderId = m_shaderManager->GetShaderProgramID(shaderName);
		if (shaderId != std::numeric_limits<unsigned int>::max())
			return GFX_SUCCESS;
		else
			return GFX_INVALID_SHADER;
    }

	int RenderCore::SetShaderToMaterial(const unsigned long long int& materialID, const unsigned int& shaderID) /////////////////////////
	{
		return m_materialManager->SetShader(materialID, shaderID);
	}


#define CT(x, y)\
{\
	if (updateStats && m_showStatistics) \
	{ \
		Timer().Start(); \
			x; \
			Timer().Stop(); \
			std::chrono::microseconds ms = Timer().GetDelta(); \
			m_subsystemTimes.push_back(std::pair<const char*, std::chrono::microseconds>(y, ms)); \
	} \
	else \
	{ \
		x; \
	} \
}


	void RenderCore::Render(const double& delta)
	{
		if (m_playSplash)
		{
			m_splashPainter->Render(m_windowWidth, m_windowHeight, delta);
			if (m_splashPainter->IsDone())
				m_playSplash = false;

			m_renderJobManager->Clear();
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

		// Build GBuffers for all geometry										\
		// When a call to light source is next in the render jobs list			|
		//	- Save index of last geometry/first light in the render jobs list	 > DeferredPainter
		//	- Break the loop													|
		//																		/
		// For each light with shadow in the render jobs list, starting at     \
		//  the index obtained from the previous step.                        |
		//	- Assign and build depth buffer atlas for each light with shadow   > LightBuilder
		//	- Break when first light without shadow is encountered            |
		//																	  /
		//
		// Apply lighting for lights with shadow
		// Apply lighting for lights without shadow

		// Reset shadow data
		ShadowDataContainer::numDirLights = 0;
		ShadowDataContainer::numSpotLights = 0;
		ShadowDataContainer::numPointLights = 0;

		// renderJobIndex is the index of the current render job
		unsigned int renderJobIndex = 0;

		// Finish gl calls to get better timings
		CT(glFinish(), "glFinish");

		// Sort the list of draw calls
		CT(m_renderJobManager->Sort(), "Sorting");

		// Draw geometry to G-buffers
		CT(m_deferredPainter->Render(m_animationManager, renderJobIndex, m_depthBuffer, m_normalDepth, m_diffuse, m_specular, m_glowMatID, m_viewMatrix, m_projMatrix, m_gamma), "Geometry");

		// Draw shadow map geometry
		CT(m_shadowPainter->Render(m_animationManager, renderJobIndex, m_depthBuffer, m_viewMatrix, m_projMatrix, 0, renderJobIndex, m_shadowMapTexture, m_windowWidth, m_windowHeight), "Shadowmap");
			

		// Draw frustum
		for (int i = 0; i < ShadowDataContainer::numDirLights; i++)
		{
			DebugLine lines[4];
			for (int e = 0; e < 4; e++)
			{
				lines[e].color = Colors::Blue;
				lines[e].thickness = 1.0f;
				lines[e].useDepth = true;
			}

			glm::mat4x4 invMat = glm::inverse(ShadowDataContainer::data[i].lightMatrix);
			for (int e = 0; e < 4; e++)
			{
				float x = -1 + 2 * (e % 2);
				float y = -1 + 2 * (e / 2);
				
				glm::vec4 start =	invMat * glm::vec4(x, y,  1.0f, 1.0f);
				glm::vec4 end =		invMat * glm::vec4(x, y, -1.0f, 1.0f);
				
				lines[e].start = glm::vec3(glm::vec3(start) / start.w);
				lines[e].end = glm::vec3(glm::vec3(end) / end.w);

				DebugDrawing().AddLineWorld(lines[e]);
			}
			for (int e = 0; e < 4; e++)
			{
				float z = -1 + 2 * (e % 2);
				float y = -1 + 2 * (e / 2);
				
				glm::vec4 start =	invMat * glm::vec4(-1, y, z, 1.0f);
				glm::vec4 end =		invMat * glm::vec4( 1, y, z, 1.0f);
				
				lines[e].start = glm::vec3(glm::vec3(start) / start.w);
				lines[e].end = glm::vec3(glm::vec3(end) / end.w);

				DebugDrawing().AddLineWorld(lines[e]);
			}
			for (int e = 0; e < 4; e++)
			{
				float z = -1 + 2 * (e % 2);
				float x = -1 + 2 * (e / 2);
				
				glm::vec4 start =	invMat * glm::vec4(x, -1, z, 1.0f);
				glm::vec4 end =		invMat * glm::vec4(x,  1, z, 1.0f);
				
				lines[e].start = glm::vec3(glm::vec3(start) / start.w);
				lines[e].end = glm::vec3(glm::vec3(end) / end.w);

				DebugDrawing().AddLineWorld(lines[e]);
			}
			for (int e = 0; e < 2; e++)
			{
				float y = -1 + 2 * e;
				
				glm::vec4 start =	invMat * glm::vec4(-0.3f, -0.3f * y, 1.0, 1.0f);
				glm::vec4 end =		invMat * glm::vec4( 0.3f,  0.3f * y, 1.0, 1.0f);
				
				lines[e].start = glm::vec3(glm::vec3(start) / start.w);
				lines[e].end = glm::vec3(glm::vec3(end) / end.w);

				DebugDrawing().AddLineWorld(lines[e]);
			}

		}
		//for (int i = 0; i < ShadowDataContainer::numSpotLights; i++)
		//{
		//}
		//for (int i = 0; i < ShadowDataContainer::numDirLights; i++)
		//{
		//}

		// Do global illumination / ssao
		CT(m_GIPainter->Render(delta, m_normalDepth, m_diffuse, m_viewMatrix, m_projMatrix), "GI");

		// Do lighting calculations
		CT(m_lightPainter->Render(renderJobIndex, m_depthBuffer, m_normalDepth, m_diffuse, m_specular, m_glowMatID, m_GIPainter->m_SSDOTexture,
			m_shadowMapTexture, m_viewMatrix, m_projMatrix, m_exposure, m_gamma, m_whitePoint, m_toneMappedTexture), "Lighting");

		// Do post processing
		CT(m_postProcessingPainter->Render(delta, m_toneMappedTexture, m_currentLUT, m_exposure, m_gamma, m_whitePoint), "PostProcessing");

		// Draw overlays/ui
		CT( m_overlayPainter->Render( renderJobIndex, m_overlayViewMatrix, m_overlayProjMatrix ), "Console");
			
		// Draw fbo previews
		if (m_showFBO != 0)
			CT(m_fboPainter->Render(m_normalDepth, m_diffuse, m_specular, m_glowMatID, m_windowWidth, m_windowHeight, m_shadowMapTexture, m_showFBO), "FBO");

		// Draw debug information
		CT(m_debugPainter->Render(m_depthBuffer, m_normalDepth, m_viewMatrix, m_projMatrix), "Debug");

		// Draw the console
		CT(m_consolePainter->Render(), "Console");

		// Draw debug text
		CT(m_textPainter->Render(), "Text");

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
		m_depthBuffer->Initialize(GL_TEXTURE_2D, GL_NEAREST, GL_NEAREST, GL_CLAMP_TO_BORDER, GL_CLAMP_TO_BORDER, GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL);

		m_normalDepth->Initialize(GL_TEXTURE_2D, GL_NEAREST, GL_NEAREST, GL_CLAMP_TO_BORDER, GL_CLAMP_TO_BORDER, GL_RGBA32F, GL_RGBA);

		m_diffuse->Initialize(GL_TEXTURE_2D, GL_NEAREST, GL_NEAREST, GL_CLAMP_TO_BORDER, GL_CLAMP_TO_BORDER, GL_RGBA32F, GL_RGBA);

		m_specular->Initialize(GL_TEXTURE_2D, GL_NEAREST, GL_NEAREST, GL_CLAMP_TO_BORDER, GL_CLAMP_TO_BORDER, GL_RGBA32F, GL_RGBA);

		m_glowMatID->Initialize(GL_TEXTURE_2D, GL_NEAREST, GL_NEAREST, GL_CLAMP_TO_BORDER, GL_CLAMP_TO_BORDER, GL_RGBA32F, GL_RGBA);


		ResizeGBuffer();

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_depthBuffer->GetTextureHandle(), 0);
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

    void RenderCore::SetOverlayViewMatrix( glm::mat4 view )
    {
        m_overlayViewMatrix = view;
    }

    void RenderCore::SetOverlayProjMatrix( glm::mat4 proj )
    {
        m_overlayProjMatrix = proj;
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

	int RenderCore::CreateSkeleton(int& out_skeletonID)
	{
		return m_animationManager->CreateSkeleton(out_skeletonID);
	}

	int RenderCore::DeleteSkeleton(const int& skeletonID)
	{
		return m_animationManager->DeleteSkeleton(skeletonID);
	}

	int RenderCore::GetSkeletonID(const unsigned int& meshID)
	{
		return m_meshManager->GetSkeletonID(meshID);
	}

	int RenderCore::BindSkeletonToMesh(const unsigned int& meshID, const int& skeletonID)
	{
		return m_meshManager->BindSkeletonToMesh(meshID, skeletonID);
	}

	int RenderCore::AddAnimationToSkeleton(const int& skeletonID, glm::mat4x4* frames, const unsigned int& numFrames, const unsigned int& numBonesPerFrame)
	{
		return m_animationManager->AddAnimationToSkeleton(skeletonID, frames, numFrames, numBonesPerFrame);
	}

	int RenderCore::GetAnimationInfo(const int& skeletonID, const int& animationID, unsigned int& out_frameCount, unsigned int& out_bonesPerFrame, unsigned int& out_animationOffset)
	{
		return m_animationManager->GetFrameInfo(skeletonID, animationID, out_frameCount, out_bonesPerFrame, out_animationOffset);
	}
}
