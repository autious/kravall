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

		for (int i = 0; i < 8; i++)
		{
			m_bloomTextures.push_back(FBOTexture());
			m_bloomTextures[i].Initialize(GL_TEXTURE_2D, GL_LINEAR, GL_LINEAR, GL_CLAMP_TO_BORDER, GL_CLAMP_TO_BORDER, GL_RGBA32F, GL_RGBA);
		}
		Resize(m_screenWidth, m_screenHeight);
	}

	void PostProcessingPainter::BindTextureToFBO(FBOTexture& texture, bool ping)
	{
		if (ping)
		{
			glBindFramebuffer(GL_FRAMEBUFFER, m_pingFBO);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture.GetTextureHandle(), 0);
			glDrawBuffer(GL_COLOR_ATTACHMENT0);
		}
		else
		{
			glBindFramebuffer(GL_FRAMEBUFFER, m_pongFBO);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture.GetTextureHandle(), 0);
			glDrawBuffer(GL_COLOR_ATTACHMENT0);
		}
	}

	void PostProcessingPainter::Resize(int width, int height)
	{
		glm::vec2 size = glm::vec2(width, height);
		for (int i = 0; i < 8; i++)
		{
			m_bloomTextures[i].UpdateResolution(size.x, size.y);
			size *= 0.5f;
		}
	}

	void PostProcessingPainter::ReloadLUT()
	{
		m_LUTManager->Reload();
	}

	void PostProcessingPainter::Render(const double& delta, const GLuint& tonemappedTexture, std::string LUT, float exposure, float gamma, glm::vec3 whitePoint)
	{
		//HDRBloom(tonemappedTexture, exposure, gamma, whitePoint);
		ColorGrading(tonemappedTexture, LUT);

		//glDisable(GL_DEPTH_TEST);
		//glEnable(GL_BLEND);
		//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		//
		//
		//glEnable(GL_DEPTH_TEST);
		//glDisable(GL_BLEND);

	}

	void PostProcessingPainter::HDRBloom(const GLuint& tonemappedTexture, float exposure, float gamma, glm::vec3 whitePoint)
	{
		BasePainter::ClearFBO();
		m_shaderManager->UseProgram("BrightPass");

		glDisable(GL_DEPTH_TEST);
		glDisable(GL_BLEND);
		glDepthMask(GL_FALSE);

		TextureManager::BindTexture(tonemappedTexture, m_textureUniformBP, 0, GL_TEXTURE_2D);
		m_shaderManager->SetUniform(exposure, m_exposureUniformBP);
		m_shaderManager->SetUniform(whitePoint.x, whitePoint.y, whitePoint.z, m_whitePointUniformBP);

		glBindVertexArray(m_dummyVAO);
		glDrawArrays(GL_POINTS, 0, 1);

		m_shaderManager->ResetProgram();

	
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_BLEND);
		glDepthMask(GL_FALSE);

		for (int i = 0; i < m_bloomTextures.size(); i++)
		{
			glBindVertexArray(m_dummyVAO);
			glDrawArrays(GL_POINTS, 0, 1);
		}

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glDepthMask(GL_TRUE);


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