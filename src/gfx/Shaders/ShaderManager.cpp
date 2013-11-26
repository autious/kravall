#include "ShaderManager.hpp"

namespace GFX
{
	ShaderManager::ShaderManager()
	{
		m_shaderData = new ShaderBaseModel();
		m_textFile = new TextFile();

	}

	ShaderManager::~ShaderManager()
	{
		delete m_shaderData;
	}

	void ShaderManager::LoadShader(const char* filepath, std::string shaderKey, GLenum type)
	{
		try
		{
			m_textFile->LoadFile(filepath);
		}
		catch (std::exception e)
		{
			std::cout << (std::string("Could not load file from ") + filepath + ": " + e.what()).c_str();

		}

		glGetError();

		GLuint shaderID = glCreateShader(type);
		const char* source = m_textFile->GetCString();

		if (shaderID != 0)
		{
			m_shaderData->SaveShader(shaderKey, shaderID);
			glShaderSource(shaderID, 1, &source, NULL);
			glCompileShader(shaderID);
		}

		GLint compileStatus;
		glGetShaderiv(shaderID, GL_COMPILE_STATUS, &compileStatus);

		if (glGetError() != GL_NO_ERROR || compileStatus == GL_FALSE)
		{

			if (shaderID != 0)
			{
				m_shaderData->DeleteShader(shaderKey);
			}

			GLsizei* length = new GLsizei;
			glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, length); //Get the length of the compilation log
			char* compilationLog = new char[*length];			 //Create the needed char array

			glGetShaderInfoLog(shaderID, *length, NULL, compilationLog); //Get the compilation log

			std::cout << "ERROR: \nCompilation log of shader " + shaderKey + ":\n" + std::string(compilationLog).c_str();
		}
		else
			std::cout << "Compiled shader " + shaderKey << std::endl;
	}

	void ShaderManager::AttachShader(std::string shaderKey, std::string shaderProgramKey)
	{
		glGetError();

		GLuint shaderID = m_shaderData->GetShaderID(shaderKey);
		GLuint shaderProgramID = m_shaderData->GetShaderProgram(shaderProgramKey);

		if (shaderProgramID != 0 && shaderID != 0)
		{
			glAttachShader(shaderProgramID, shaderID);
		}
		else
		{
			std::cout << ("ERROR: No shader with associated key does exist!");
		}

		if (glGetError() != GL_NO_ERROR)
		{
			std::cout << ((std::string("ERROR: Could not attach shader ") + std::string(shaderKey) + std::string(" to ") + std::string(shaderProgramKey)).c_str());
		}
	}

	void ShaderManager::DetachShader(std::string shaderKey, std::string shaderProgramKey)
	{
		glGetError();

		GLuint shaderID = m_shaderData->GetShaderID(shaderKey);
		GLuint shaderProgramID = m_shaderData->GetShaderProgram(shaderProgramKey);

		if (shaderProgramID != 0 && shaderID != 0)
		{
			glDetachShader(shaderProgramID, shaderID);
		}
		else
		{
			std::cout << ("ERROR: No shader or shader-program with associated key does exist!");
		}

		if (glGetError() != GL_NO_ERROR)
		{
			std::cout << (("ERROR: Could not detach shader " + std::string(shaderKey) + " from " + std::string(shaderProgramKey)).c_str());
		}
	}

	void ShaderManager::CreateProgram(std::string shaderProgramKey)
	{
		glGetError();

		GLuint shaderProgramID = glCreateProgram();

		if (shaderProgramID != 0)
		{
			m_shaderData->SaveShaderProgram(shaderProgramKey, shaderProgramID);
		}

		if (glGetError() != GL_NO_ERROR)
		{

			if (shaderProgramID != 0)
			{
				m_shaderData->DeleteShaderProgram(shaderProgramKey);
			}

			std::cout << (("ERROR: Could not create shader-program " + shaderProgramKey).c_str());
		}
	}

	void ShaderManager::UseProgram(std::string shaderProgramKey)
	{
		glGetError();

		GLuint shaderProgramID = m_shaderData->GetShaderProgram(shaderProgramKey);

		if (shaderProgramID != 0)
		{
			glUseProgram(shaderProgramID);
		}
		else
		{
			std::cout << ("ERROR: No shader-program with associated key does exist!");
		}

		if (glGetError() != GL_NO_ERROR)
		{
			std::cout << "ERROR: Could not use shaderProgram " + shaderProgramKey + "!\n";
		}
	}

	void ShaderManager::LinkProgram(std::string shaderProgramKey)
	{
		glGetError();

		GLuint shaderProgramID = m_shaderData->GetShaderProgram(shaderProgramKey);

		if (shaderProgramID != 0)
		{
			glLinkProgram(shaderProgramID);
		}
		else
		{
			std::cout << ("ERROR: No shader-program with associated key does exist!");
		}

		GLint linkStatus;
		glGetProgramiv(shaderProgramID, GL_LINK_STATUS, &linkStatus);

		if (glGetError() != GL_NO_ERROR || linkStatus == GL_FALSE)
		{

			GLsizei* length = new GLsizei;
			glGetProgramiv(shaderProgramID, GL_INFO_LOG_LENGTH, length); //Get the length of the compilation log
			char* linkingLog = new char[*length];			 //Create the needed char array

			glGetProgramInfoLog(shaderProgramID, *length, NULL, linkingLog); //Get the compilation log

			std::cout << (("ERROR: \nLinker log of shader-programm " + shaderProgramKey + ":\n" + std::string(linkingLog)).c_str());

		}
	}

	GLuint ShaderManager::GetShaderID(std::string shaderKey)
	{
		return m_shaderData->GetShaderID(shaderKey);
	}

	GLuint ShaderManager::GetShaderProgramID(std::string shaderProgramKey)
	{
		return m_shaderData->GetShaderProgram(shaderProgramKey);
	}

	void ShaderManager::DeleteProgram(std::string shaderProgramKey)
	{
		glGetError();

		GLuint shaderProgramID = m_shaderData->GetShaderProgram(shaderProgramKey);

		if (shaderProgramID != 0)
		{
			glDeleteProgram(shaderProgramID);
		}
		else
		{
			std::cout << ("ERROR: No shader-program with associated key does exist!");
		}

		if (glGetError() != GL_NO_ERROR)
		{
			std::cout << (("ERROR: Could not delete shader-program " + shaderProgramKey).c_str());
		}
	}

	void ShaderManager::DeleteShader(std::string shaderKey)
	{
		glGetError();

		GLuint shaderID = m_shaderData->GetShaderID(shaderKey);

		if (shaderID != 0)
		{
			glDeleteShader(shaderID);
		}
		else
		{
			std::cout << ("ERROR: No shader-program with associated key does exist!");
		}


		if (glGetError() != GL_NO_ERROR)
		{
			std::cout << (("ERROR: Could not delete shader " + shaderKey).c_str());
		}
	}

	void ShaderManager::ResetProgram()
	{
		glUseProgram(0);
	}
}
