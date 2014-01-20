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

	void PostProcessingPainter::Initialize(GLuint FBO, GLuint dummyVAO, int screenWidth, int screenHeight)
	{
		BasePainter::Initialize(FBO, dummyVAO);

		m_LUTManager->Initialize(m_textureManager);

		m_screenHeight = screenHeight;
		m_screenWidth = screenWidth;

		m_shaderManager->CreateProgram("ColorGrading");
		m_shaderManager->LoadShader("shaders/RenderToQuad/Quad.vertex", "CGVS", GL_VERTEX_SHADER);
		m_shaderManager->LoadShader("shaders/FSQuad.geometry", "CGGS", GL_GEOMETRY_SHADER);
		m_shaderManager->LoadShader("shaders/PostProcessing/ColorGradingPS.glsl", "CGPS", GL_FRAGMENT_SHADER);
		m_shaderManager->AttachShader("CGVS", "ColorGrading");
		m_shaderManager->AttachShader("CGGS", "ColorGrading");
		m_shaderManager->AttachShader("CGPS", "ColorGrading");
		m_shaderManager->LinkProgram("ColorGrading");

		m_shaderManager->UseProgram("ColorGrading");

		m_LUTUniform = m_shaderManager->GetUniformLocation("ColorGrading", "gLut");
		m_sourceUniform = m_shaderManager->GetUniformLocation("ColorGrading", "gSourceTexture");
	}

	void PostProcessingPainter::ReloadLUT()
	{
		m_LUTManager->Reload();
	}

	void PostProcessingPainter::Render(const double& delta, const GLuint& tonemappedTexture, std::string LUT)
	{
		ColorGrading(tonemappedTexture, LUT);
		//glDisable(GL_DEPTH_TEST);
		//glEnable(GL_BLEND);
		//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		//
		//
		//glEnable(GL_DEPTH_TEST);
		//glDisable(GL_BLEND);

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