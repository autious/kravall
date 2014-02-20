#include "UniformBufferManager.hpp"

namespace GFX
{
    const GLuint UniformBufferManager::CAMERA_BINDING_INDEX = 0;
    

	UniformBufferManager::UniformBufferManager()
	{
        CreateBasicCameraUBO();
	}

	UniformBufferManager::~UniformBufferManager()
	{
		glDeleteBuffers(1, &m_basicCameraUBO);
	}

	void UniformBufferManager::CreateBasicCameraUBO()
	{
		BasicCamera bc;
		bc.projMatrix = glm::mat4();
		bc.viewMatrix = glm::mat4();

		glGenBuffers(1, &m_basicCameraUBO);
		glBindBuffer(GL_UNIFORM_BUFFER, m_basicCameraUBO);

		glBufferData(GL_UNIFORM_BUFFER, sizeof(BasicCamera), &bc, GL_DYNAMIC_DRAW);
		glBindBufferBase(GL_UNIFORM_BUFFER, CAMERA_BINDING_INDEX, m_basicCameraUBO);
	}

    void UniformBufferManager::SetUniformBlockBindingIndex(GLuint programID, const char* uniformBufferName, GLuint bindingIndex)
    {
        GLuint uniformBlockIndex = glGetUniformBlockIndex(programID, uniformBufferName);
        glUniformBlockBinding(programID, uniformBlockIndex, bindingIndex);
    }

	void UniformBufferManager::SetBasicCameraUBO(const BasicCamera& basicCamera)
	{
		glBindBuffer(GL_UNIFORM_BUFFER, m_basicCameraUBO);
		glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(BasicCamera), &basicCamera);
	}
}
