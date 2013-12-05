#include "ConsolePainter.hpp"

#include "../RenderCore.hpp"

namespace GFX
{
	ConsolePainter::ConsolePainter(ShaderManager* shaderManager, UniformBufferManager* uniformBufferManager)
		: BasePainter(shaderManager, uniformBufferManager)
	{
		m_showConsole = false;
		m_consoleHeight = 380;
		m_consoleColor = glm::vec4(0.1f, 0.18f, 0.1f, 0.75f);
	}

	ConsolePainter::~ConsolePainter()
	{
	}

	void ConsolePainter::Initialize(GLuint FBO, GLuint dummyVAO)
	{
		BasePainter::Initialize(FBO, dummyVAO);


		// Load shared shaders (vertex and fragment)
		m_shaderManager->LoadShader("shaders/debug_shaders/Debug.vertex", "DebugVS", GL_VERTEX_SHADER);
		m_shaderManager->LoadShader("shaders/debug_shaders/Debug.fragment", "DebugFS", GL_FRAGMENT_SHADER);

		// Load specific geometry shaders
		m_shaderManager->LoadShader("shaders/debug_shaders/DebugRect.geometry", "DebugRectGS", GL_GEOMETRY_SHADER);

		// Create and attach rect debug shaders
		m_shaderManager->CreateProgram("ConsoleRect");
		m_shaderManager->AttachShader("DebugVS", "ConsoleRect");
		m_shaderManager->AttachShader("DebugRectGS", "ConsoleRect");
		m_shaderManager->AttachShader("DebugFS", "ConsoleRect");
		m_shaderManager->LinkProgram("ConsoleRect");

		m_rectPosUniform = m_shaderManager->GetUniformLocation("ConsoleRect", "pointPosition");
		m_rectDimUniform = m_shaderManager->GetUniformLocation("ConsoleRect", "pointPosition2");
		m_rectColorUniform = m_shaderManager->GetUniformLocation("ConsoleRect", "inColor");

		m_uniformBufferManager->CreateBasicCameraUBO(m_shaderManager->GetShaderProgramID("ConsoleRect"));


	}

	void ConsolePainter::Render()
	{
		// Render console
		if (m_showConsole)
		{
			BasePainter::Render();

			GLenum err;
			glBindVertexArray(m_dummyVAO);
			glDisable(GL_PROGRAM_POINT_SIZE);

			glEnable(GL_BLEND);
			BasicCamera bc;
			
			glDisable(GL_DEPTH_TEST);
			// Draw filled rectangles
			m_shaderManager->UseProgram("DebugRect");

			m_shaderManager->SetUniform(1, m_consoleRect.color, m_rectColorUniform);
			m_shaderManager->SetUniform(1, m_consoleRect.position, m_rectPosUniform);
			m_shaderManager->SetUniform(1, m_consoleRect.dimensions, m_rectDimUniform);

			glDrawArrays(GL_POINTS, 0, 1);

			m_shaderManager->SetUniform(1, m_consoleInputRect.color, m_rectColorUniform);
			m_shaderManager->SetUniform(1, m_consoleInputRect.position, m_rectPosUniform);
			m_shaderManager->SetUniform(1, m_consoleInputRect.dimensions, m_rectDimUniform);

			glDrawArrays(GL_POINTS, 0, 1);

			m_shaderManager->ResetProgram();
		}

		glDisable(GL_BLEND);

		glEnable(GL_DEPTH_TEST);

		ClearFBO();

		DebugDrawing().Clear();
	}
	void ConsolePainter::SetConsoleHeight(int height)
	{ 
		m_consoleRect.position = glm::vec3(-1.0f, 1.0f, 0.0f);
		m_consoleRect.color = m_consoleColor;
		m_consoleRect.dimensions = glm::vec3(2.0f, (m_consoleHeight/static_cast<float>(height)) * 2.0f, 0.0f);

		m_consoleInputRect.position = glm::vec3(-1.0f, 1.0f - ((m_consoleHeight+1.0f)/static_cast<float>(height)) * 2.0f, 0.0f);
		m_consoleInputRect.color = m_consoleColor;
		m_consoleInputRect.dimensions = glm::vec3(2.0f, (20.0f/static_cast<float>(height)) * 2.0f, 0.0f);

	}
	void ConsolePainter::SetConsoleVisible(bool visible)
	{
		m_showConsole = visible;
	}
	bool ConsolePainter::GetConsoleVisible()
	{
		return m_showConsole;
	}
	DebugRect ConsolePainter::GetConsoleRect()
	{
		return m_consoleRect;
	}
}
