#include "ShaderBaseModel.hpp"

namespace GFX
{
	ShaderBaseModel::ShaderBaseModel()
	{
		m_shader = new std::map<std::string, GLuint>();
		m_shaderProgram = new std::map<std::string, GLuint>();
	}

	ShaderBaseModel::~ShaderBaseModel()
	{
		for (std::map<std::string, GLuint>::iterator it = m_shaderProgram->begin(); it != m_shaderProgram->end(); it++)
			glDeleteProgram(it->second);

		for (std::map<std::string, GLuint>::iterator it = m_shader->begin(); it != m_shader->end(); it++)
			glDeleteShader(it->second);

		delete (m_shaderProgram);
		delete (m_shader);
	}

	GLuint ShaderBaseModel::GetShaderID(std::string shaderKey)
	{
		std::map<std::string, GLuint>::iterator it = m_shader->find(shaderKey);

		if (it == m_shader->end()){
			return 0;
		}
		else
		{
			return it->second;
		}
	}
	GLuint ShaderBaseModel::GetShaderProgram(std::string shaderProgramKey)
	{
		std::map<std::string, GLuint>::iterator it = m_shaderProgram->find(shaderProgramKey);

		if (it == m_shaderProgram->end())
		{
			return 0;
		}
		else
		{
			return it->second;
		}
	}

	bool ShaderBaseModel::SaveShader(std::string shaderKey, GLuint shaderID)
	{
		return m_shader->insert(std::pair<std::string, GLuint>(shaderKey, shaderID)).second;
	}

	bool ShaderBaseModel::SaveShaderProgram(std::string shaderProgramKey, GLuint shaderProgramID)
	{
		return m_shaderProgram->insert(std::pair<std::string, GLuint>(shaderProgramKey, shaderProgramID)).second;
	}

	void ShaderBaseModel::DeleteShader(std::string shaderKey)
	{
		m_shader->erase(shaderKey);
	}

	void ShaderBaseModel::DeleteShaderProgram(std::string shaderProgramKey)
	{
		m_shaderProgram->erase(shaderProgramKey);
	}
}
