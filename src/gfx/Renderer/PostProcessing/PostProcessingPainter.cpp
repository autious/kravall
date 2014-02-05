#include "PostProcessingPainter.hpp"

namespace GFX
{
	PostProcessingPainter::PostProcessingPainter(ShaderManager* shaderManager, UniformBufferManager* uniformBufferManager, TextureManager* textureManager)
		: BasePainter(shaderManager, uniformBufferManager)
	{
		m_textureManager = textureManager;
		m_LUTManager = new LUTManager();
	}

	PostProcessingPainter::~PostProcessingPainter()
	{
	}

	void PostProcessingPainter::Initialize(GLuint FBO, GLuint dummyVAO, int screenWidth, int screenHeight, BlurPainter* blurPainter)
	{
		BasePainter::Initialize(FBO, dummyVAO);

		m_LUTManager->Initialize(m_textureManager);

		m_blurPainter = blurPainter;

		m_screenHeight = screenHeight;
		m_screenWidth = screenWidth;

		m_shaderManager->CreateProgram("ColorGrading");
		m_shaderManager->LoadShader("shaders/RenderToQuad/Quad.vertex", "CGVS", GL_VERTEX_SHADER);
		m_shaderManager->LoadShader("shaders/FSQuadGS.glsl", "CGGS", GL_GEOMETRY_SHADER);
		m_shaderManager->LoadShader("shaders/PostProcessing/ColorGradingPS.glsl", "CGPS", GL_FRAGMENT_SHADER);
		m_shaderManager->AttachShader("CGVS", "ColorGrading");
		m_shaderManager->AttachShader("CGGS", "ColorGrading");
		m_shaderManager->AttachShader("CGPS", "ColorGrading");
		m_shaderManager->LinkProgram("ColorGrading");

		m_shaderManager->UseProgram("ColorGrading");

		m_LUTUniform = m_shaderManager->GetUniformLocation("ColorGrading", "gLut");
		m_sourceUniform = m_shaderManager->GetUniformLocation("ColorGrading", "gSourceTexture");

		m_shaderManager->ResetProgram();

		m_shaderManager->CreateProgram("BrightPass");
		m_shaderManager->LoadShader("shaders/RenderToQuad/Quad.vertex", "BPVS", GL_VERTEX_SHADER);
		m_shaderManager->LoadShader("shaders/FSQuadGS.glsl", "BPGS", GL_GEOMETRY_SHADER);
		m_shaderManager->LoadShader("shaders/PostProcessing/BrightPassFS.glsl", "BPFS", GL_FRAGMENT_SHADER);
		m_shaderManager->AttachShader("BPVS", "BrightPass");
		m_shaderManager->AttachShader("BPGS", "BrightPass");
		m_shaderManager->AttachShader("BPFS", "BrightPass");
		m_shaderManager->LinkProgram("BrightPass");
		m_shaderManager->UseProgram("BrightPass");

		m_whitePointUniformBP = m_shaderManager->GetUniformLocation("BrightPass", "gWhitePoint");
		m_exposureUniformBP = m_shaderManager->GetUniformLocation("BrightPass", "gExposure");
		m_textureUniformBP = m_shaderManager->GetUniformLocation("BrightPass", "gSourceTexture");

		m_brightPassTexture = new FBOTexture();
		m_brightPassTexture->Initialize(GL_TEXTURE_2D, GL_LINEAR, GL_LINEAR, GL_CLAMP_TO_BORDER, GL_CLAMP_TO_BORDER, GL_RGBA32F, GL_RGBA);

		m_intermediateTexture = new FBOTexture();
		m_intermediateTexture->Initialize(GL_TEXTURE_2D, GL_LINEAR, GL_LINEAR, GL_CLAMP_TO_BORDER, GL_CLAMP_TO_BORDER, GL_RGBA32F, GL_RGBA);

		m_shaderManager->CreateProgram("Composite");
		m_shaderManager->LoadShader("shaders/RenderToQuad/Quad.vertex", "CVS", GL_VERTEX_SHADER);
		m_shaderManager->LoadShader("shaders/FSQuadGS.glsl", "CGS", GL_GEOMETRY_SHADER);
		m_shaderManager->LoadShader("shaders/PostProcessing/CompositeFS.glsl", "CFS", GL_FRAGMENT_SHADER);
		m_shaderManager->AttachShader("CVS", "Composite");
		m_shaderManager->AttachShader("CGS", "Composite");
		m_shaderManager->AttachShader("CFS", "Composite");
		m_shaderManager->LinkProgram("Composite");
		m_shaderManager->UseProgram("Composite");

		m_whitePointUniformC = m_shaderManager->GetUniformLocation("Composite", "gWhitePoint");
		m_exposureUniformC = m_shaderManager->GetUniformLocation("Composite", "gExposure");
		m_textureUniformC = m_shaderManager->GetUniformLocation("Composite", "gSourceTexture");
		m_gammaUniformC = m_shaderManager->GetUniformLocation("Composite", "gGamma");

		for (int i = 0; i < 8; i++)
		{
			m_bloomTextures.push_back(new FBOTexture());
			m_bloomTextures[i]->Initialize(GL_TEXTURE_2D, GL_LINEAR, GL_LINEAR, GL_CLAMP_TO_BORDER, GL_CLAMP_TO_BORDER, GL_RGBA32F, GL_RGBA);

			m_intermediateBlurTextures.push_back(new FBOTexture());
			m_intermediateBlurTextures[i]->Initialize(GL_TEXTURE_2D, GL_LINEAR, GL_LINEAR, GL_CLAMP_TO_BORDER, GL_CLAMP_TO_BORDER, GL_RGBA32F, GL_RGBA);

			GLint temp = m_shaderManager->GetUniformLocation("Composite", "gBloomTextures[" + std::to_string(i) + "]");
			m_bloomSamplerUniforms.push_back(temp);
		}
		Resize(m_screenWidth, m_screenHeight);

		InitFBO();
	}

	void PostProcessingPainter::InitFBO()
	{
		glGenFramebuffers(1, &m_pingFBO);
		glBindFramebuffer(GL_FRAMEBUFFER, m_pingFBO);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_bloomTextures.at(0)->GetTextureHandle(), 0);

		GLenum drawBuffers[] = { GL_COLOR_ATTACHMENT0 };
		glDrawBuffers(1, drawBuffers);

		//reset the framebuffer
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		glGenFramebuffers(1, &m_pongFBO);
		glBindFramebuffer(GL_FRAMEBUFFER, m_pongFBO);
		
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_bloomTextures.at(1)->GetTextureHandle(), 0);
		
		GLenum drawBuffers2[] = { GL_COLOR_ATTACHMENT0 };
		glDrawBuffers(1, drawBuffers2);
		
		//reset the framebuffer
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

	}

	void PostProcessingPainter::BindTextureToFBO(FBOTexture* texture, bool ping)
	{
		if (ping)
		{
			glBindFramebuffer(GL_FRAMEBUFFER, m_pingFBO);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture->GetTextureHandle(), 0);
			glDrawBuffer(GL_COLOR_ATTACHMENT0);
		}
		else
		{
			glBindFramebuffer(GL_FRAMEBUFFER, m_pongFBO);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture->GetTextureHandle(), 0);
			glDrawBuffer(GL_COLOR_ATTACHMENT0);
		}
	}

	void PostProcessingPainter::Resize(int width, int height)
	{
		glm::vec2 size = glm::vec2(width, height);
		for (int i = 0; i < m_bloomTextures.size(); i++)
		{
			m_bloomTextures[i]->UpdateResolution(size.x, size.y);
			m_intermediateBlurTextures[i]->UpdateResolution(size.x, size.y);
			size *= 0.5f;
		}

		m_brightPassTexture->UpdateResolution(width, height);
		m_intermediateTexture->UpdateResolution(width, height);
	}

	void PostProcessingPainter::ReloadLUT()
	{
		m_LUTManager->Reload();
	}

	void PostProcessingPainter::Render(const double& delta, const GLuint& tonemappedTexture, std::string LUT, float exposure, float gamma, glm::vec3 whitePoint)
	{
		HDRBloom(tonemappedTexture, exposure, gamma, whitePoint);
		Composite(tonemappedTexture, exposure, gamma, whitePoint);
		ColorGrading(m_brightPassTexture->GetTextureHandle(), LUT);
	}

	void PostProcessingPainter::Composite(const GLuint& tonemappedTexture, float exposure, float gamma, glm::vec3 whitePoint)
	{
		BasePainter::ClearFBO();

		BindTextureToFBO(m_brightPassTexture, false);
		glClear(GL_COLOR_BUFFER_BIT);

		m_shaderManager->UseProgram("Composite");

		glDisable(GL_DEPTH_TEST);
		glDisable(GL_BLEND);
		glDepthMask(GL_FALSE);

		m_shaderManager->SetUniform(1, whitePoint, m_whitePointUniformC);
		m_shaderManager->SetUniform(exposure, m_exposureUniformC);
		m_shaderManager->SetUniform(gamma, m_gammaUniformC);
		TextureManager::BindTexture(tonemappedTexture, m_textureUniformBP, 0, GL_TEXTURE_2D);
		
		for (int i = 0; i < m_bloomTextures.size(); i++)
		{
			TextureManager::BindTexture(m_bloomTextures[i]->GetTextureHandle(), m_bloomSamplerUniforms[i], i + 1, GL_TEXTURE_2D);
		}

		glBindVertexArray(m_dummyVAO);
		glDrawArrays(GL_POINTS, 0, 1);

		BasePainter::ClearFBO();
	}

	void PostProcessingPainter::HDRBloom(const GLuint& tonemappedTexture, float exposure, float gamma, glm::vec3 whitePoint)
	{
		BasePainter::ClearFBO();
		m_shaderManager->UseProgram("BrightPass");

		BindTextureToFBO(m_brightPassTexture, false);

		glDisable(GL_DEPTH_TEST);
		glDisable(GL_BLEND);
		glDepthMask(GL_FALSE);

		TextureManager::BindTexture(tonemappedTexture, m_textureUniformBP, 0, GL_TEXTURE_2D);
		m_shaderManager->SetUniform(exposure, m_exposureUniformBP);
		m_shaderManager->SetUniform(whitePoint.x, whitePoint.y, whitePoint.z, m_whitePointUniformBP);

		glBindVertexArray(m_dummyVAO);
		glDrawArrays(GL_POINTS, 0, 1);

		m_shaderManager->ResetProgram();

		BasePainter::ClearFBO();
		glm::vec2 size = glm::vec2(m_screenWidth, m_screenHeight);
		
		for (int i = 0; i < m_bloomTextures.size(); i++)
		{
			glViewport(0, 0, size.x, size.y);
			
			BindTextureToFBO(m_intermediateBlurTextures[i], true);

			glClear(GL_COLOR_BUFFER_BIT);

			m_shaderManager->UseProgram("GaussianBlurHorizontal");

			TextureManager::BindTexture(m_brightPassTexture->GetTextureHandle(), m_shaderManager->GetUniformLocation("GaussianBlurHorizontal", "gTexture"), 0, GL_TEXTURE_2D);
			m_shaderManager->SetUniform((GLfloat)size.x, (GLfloat)size.y, m_shaderManager->GetUniformLocation("GaussianBlurHorizontal", "gScreenDimensions"));

			glBindVertexArray(m_dummyVAO);
			glDrawArrays(GL_POINTS, 0, 1);

			m_shaderManager->ResetProgram();

			BindTextureToFBO(m_bloomTextures[i], false);
			m_shaderManager->UseProgram("GaussianBlurVertical");
			glClear(GL_COLOR_BUFFER_BIT);

			TextureManager::BindTexture(m_intermediateBlurTextures[i]->GetTextureHandle(), m_shaderManager->GetUniformLocation("GaussianBlurVertical", "gTexture"), 0, GL_TEXTURE_2D);
			m_shaderManager->SetUniform((GLfloat)size.x, (GLfloat)size.y, m_shaderManager->GetUniformLocation("GaussianBlurHorizontal", "gScreenDimensions"));

			glBindVertexArray(m_dummyVAO);
			glDrawArrays(GL_POINTS, 0, 1);

			BasePainter::ClearFBO();
			m_shaderManager->ResetProgram();

			size *= 0.5f;
		}
		
		glViewport(0, 0, m_screenWidth, m_screenHeight);

	}

	void PostProcessingPainter::ColorGrading(const GLuint& tonemappedTexture, std::string LUT)
	{
		BasePainter::ClearFBO();
		m_shaderManager->UseProgram("ColorGrading");

		glDisable(GL_DEPTH_TEST);
		glDisable(GL_BLEND);
		glDepthMask(GL_FALSE);
		TextureManager::BindTexture(tonemappedTexture, m_sourceUniform, 0, GL_TEXTURE_2D);
		TextureManager::BindTexture(m_textureManager->GetTexture(m_LUTManager->GetLUTHandle(LUT)).textureHandle, m_LUTUniform, 1, GL_TEXTURE_3D);

		glBindVertexArray(m_dummyVAO);
		glDrawArrays(GL_POINTS, 0, 1);

		m_shaderManager->ResetProgram();

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glDepthMask(GL_TRUE);

	}

	void PostProcessingPainter::OutputTexture(const GLuint& texturehandle)
	{
		BasePainter::ClearFBO();

		TextureManager::UnbindTexture();
	}
}