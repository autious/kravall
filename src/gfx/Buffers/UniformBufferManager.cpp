#include "UniformBufferManager.hpp"

namespace GFX
{
	UniformBufferManager::UniformBufferManager()
	{

	}

	UniformBufferManager::~UniformBufferManager()
	{

	}
	

	void UniformBufferManager::CreateExampleBuffer(GLuint programID)
	{
		GLuint bindingPoint = 0;
		GLuint blockIndex;

		ExampleBuffer eb;
		eb.colorOne = glm::vec4(0.0f);
		eb.colorTwo = glm::vec4(0.0f);
		eb.floatOne = 0.0f;
		eb.floatTwo = 0.0f;

		blockIndex = glGetUniformBlockIndex(programID, "ExampleBlock");
		glUniformBlockBinding(programID, blockIndex, bindingPoint);

		glGenBuffers(1, &m_exampleUBO);
		glBindBuffer(GL_UNIFORM_BUFFER, m_exampleUBO);
		
		glBufferData(GL_UNIFORM_BUFFER, sizeof(ExampleBuffer), &eb, GL_DYNAMIC_DRAW);
		glBindBufferBase(GL_UNIFORM_BUFFER, bindingPoint, m_exampleUBO);
	}

	void UniformBufferManager::SetExampleBufferData(const ExampleBuffer& eb)
	{
		glBindBuffer(GL_UNIFORM_BUFFER, m_exampleUBO);
		glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(ExampleBuffer), &eb);
	}

	GLuint UniformBufferManager::GetExampleBuffer()
	{
		return m_exampleUBO;
	}
}