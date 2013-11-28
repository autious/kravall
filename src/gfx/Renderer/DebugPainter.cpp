#include "DebugPainter.hpp"

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

		m_shaderManager->CreateProgram("StaticMesh");

		//m_shaderManager->LoadShader("shaders/StaticMesh.vertex", "StaticMeshVS", GL_VERTEX_SHADER);
		//m_shaderManager->LoadShader("shaders/StaticMesh.fragment", "StaticMeshFS", GL_FRAGMENT_SHADER);

		m_shaderManager->LoadShader("shaders/PassThrough.vertex", "StaticMeshVS", GL_VERTEX_SHADER);
		m_shaderManager->LoadShader("shaders/FSQuad.geometry", "StaticMeshGS", GL_GEOMETRY_SHADER);
		m_shaderManager->LoadShader("shaders/WaveRipple.fragment", "StaticMeshFS", GL_FRAGMENT_SHADER);

		m_shaderManager->AttachShader("StaticMeshVS", "StaticMesh");
		m_shaderManager->AttachShader("StaticMeshGS", "StaticMesh");
		m_shaderManager->AttachShader("StaticMeshFS", "StaticMesh");

		m_shaderManager->LinkProgram("StaticMesh");

		debugUniform = m_shaderManager->GetUniformLocation("StaticMesh", "inputColor");

		m_uniformBufferManager->CreateExampleBuffer(m_shaderManager->GetShaderProgramID("StaticMesh"));
		m_uniformBufferManager->CreateBasicCameraUBO(m_shaderManager->GetShaderProgramID("StaticMesh"));
	}

	void DebugPainter::Render(glm::mat4 viewMatrix, glm::mat4 projMatrix)
	{
		BasePainter::Render();

		//DetachTextures()

		m_shaderManager->UseProgram("StaticMesh");
		m_shaderManager->SetUniform(1, glm::vec4(1, 0, 0, 1), debugUniform);

		BasicCamera bc;
		bc.viewMatrix = viewMatrix;
		bc.projMatrix = projMatrix;

		ExampleBuffer eb;
		eb.colorOne = glm::vec4(1.0f);
		eb.colorTwo = glm::vec4(0.05f);
		eb.floatOne = 0.0f;
		eb.floatTwo = 1.0f;

		m_uniformBufferManager->SetBasicCameraUBO(bc);
		m_uniformBufferManager->SetExampleBufferData(eb);

		glBindVertexArray(m_dummyVAO);

		glDrawArrays(GL_POINTS, 0, 1);

		m_shaderManager->ResetProgram();

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
