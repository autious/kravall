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

		Resize(screenWidth, screenHeight);

		m_shaderManager->CreateProgram("RadianceMap");
		m_shaderManager->LoadShader("shaders/RenderToQuad/Quad.vertex", "RMVS", GL_VERTEX_SHADER);
		m_shaderManager->LoadShader("shaders/FSQuad.geometry", "RMGS", GL_GEOMETRY_SHADER);
		m_shaderManager->LoadShader("shaders/GlobalIllumination/RadianceMapFS.glsl", "RMPS", GL_FRAGMENT_SHADER);
		m_shaderManager->AttachShader("RMVS", "RadianceMap");
		m_shaderManager->AttachShader("RMGS", "RadianceMap");
		m_shaderManager->AttachShader("RMPS", "RadianceMap");
		m_shaderManager->LinkProgram("RadianceMap");
	}

	void GIPainter::Render(const double& delta, FBOTexture* normalDepth, FBOTexture* diffuse, FBOTexture* specular)
	{

	}

	void GIPainter::Resize(int width, int height)
	{
		glDeleteTextures(1, &m_radianceTexture);

		m_screenWidth = width;
		m_screenHeight = height;

		glGenTextures(1, &m_radianceTexture);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_radianceTexture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, m_screenWidth, m_screenHeight, 0, GL_RGBA, GL_FLOAT, NULL);
		glBindImageTexture(0, m_radianceTexture, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);
	}
}