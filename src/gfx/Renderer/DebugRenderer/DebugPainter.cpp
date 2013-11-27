#include "DebugPainter.hpp"
#include "DebugManager.hpp"

namespace GFX
{
	DebugPainter::DebugPainter(ShaderManager* shaderManager, BufferManager* bufferManager, UniformBufferManager* uniformBufferManager)
		: BasePainter(shaderManager, bufferManager, uniformBufferManager)
	{
	}

	DebugPainter::~DebugPainter()
	{
	}

	void DebugPainter::Initialize(GLuint FBO, GLuint dummyVAO)
	{
		BasePainter::Initialize(FBO, dummyVAO);


		// Load shared shaders (vertex and fragment)
		m_shaderManager->LoadShader("shaders/debug/Debug.vertex", "DebugVS", GL_VERTEX_SHADER);
		m_shaderManager->LoadShader("shaders/debug/Debug.fragment", "DebugFS", GL_FRAGMENT_SHADER);

		// Load specific geometry shaders
		m_shaderManager->LoadShader("shaders/debug/DebugLine.geometry", "DebugLineGS", GL_GEOMETRY_SHADER);

		// Create and attach point debug shaders
		m_shaderManager->CreateProgram("DebugPoint");
		m_shaderManager->AttachShader("DebugVS", "DebugPoint");
		m_shaderManager->AttachShader("DebugFS", "DebugPoint");
		m_shaderManager->LinkProgram("DebugPoint");

		// Create and attach line debug shaders
		m_shaderManager->CreateProgram("DebugLine");
		m_shaderManager->AttachShader("DebugVS", "DebugLine");
		m_shaderManager->AttachShader("DebugLineGS", "DebugLine");
		m_shaderManager->AttachShader("DebugFS", "DebugLine");
		m_shaderManager->LinkProgram("DebugLine");

		m_pointPositionUniform = m_shaderManager->GetUniformLocation("DebugPoint", "pointPosition");
		m_colorUniform = m_shaderManager->GetUniformLocation("DebugPoint", "pointColor");
		m_sizeUniform = m_shaderManager->GetUniformLocation("DebugPoint", "pointSize");

		m_uniformBufferManager->CreateBasicCameraUBO(m_shaderManager->GetShaderProgramID("DebugPoint"));
	}

	void DebugPainter::Render(glm::mat4 viewMatrix, glm::mat4 projMatrix)
	{
		BasePainter::Render();

		//DetachTextures();
		GLenum err;
		err = glGetError();

		// Draw 2D
		BasicCamera bc;
		bc.viewMatrix = glm::mat4x4(1.0f);
		bc.projMatrix = glm::mat4x4(1.0f);

		m_uniformBufferManager->SetBasicCameraUBO(bc);
		
		glBindVertexArray(m_dummyVAO);
		glDisable(GL_PROGRAM_POINT_SIZE);

		//Disable depth for debug
		glDisable(GL_DEPTH_TEST);

		glEnable(GL_BLEND);
		err = glGetError();

		// Draw points
		m_shaderManager->UseProgram("DebugPoint");
		while (!DebugDrawing().GetPoints().empty())
		{
			DebugPoint dp = DebugDrawing().GetPoints().back();
			DebugDrawing().GetPoints().pop_back();

			m_shaderManager->SetUniform(1, dp.color, m_colorUniform);
			m_shaderManager->SetUniform(1, dp.position, m_pointPositionUniform);
			m_shaderManager->SetUniform(1, dp.size, m_sizeUniform);

			glPointSize(dp.size);
			glDrawArrays(GL_POINTS, 0, 1);
		}
		glPointSize(1.0f);

		m_shaderManager->ResetProgram();

		// Draw lines
		//while (!DebugDrawing().GetLines().empty())
		//{
		//	DebugLine dl = DebugDrawing().GetLines().back();
		//	DebugDrawing().GetPoints().pop_back();
		//
		//	m_shaderManager->SetUniform(1, dl.color, m_colorUniform);
		//	m_shaderManager->SetUniform(1, dl.position, m_pointPositionUniform);
		//	m_shaderManager->SetUniform(1, dl.size, m_sizeUniform);
		//
		//	glLineWidth(dp.size);
		//	glDrawArrays(GL_POINTS, 0, 1);
		//}
		//glLineWidth(1.0f);
		//
		//m_shaderManager->ResetProgram();

		glEnable(GL_DEPTH_TEST);

		// Draw 3D
		bc.viewMatrix = viewMatrix;
		bc.projMatrix = projMatrix;

		m_uniformBufferManager->SetBasicCameraUBO(bc);

		glDisable(GL_BLEND);

		ClearFBO();
	}


	void DebugPainter::DetachTextures()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, 0, 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, 0, 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, 0, 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D, 0, 0);

		// define outputs
		GLenum drawBuffers[] = { GL_NONE, GL_NONE, GL_NONE, GL_NONE };
		glDrawBuffers(4, drawBuffers);
	}
}
