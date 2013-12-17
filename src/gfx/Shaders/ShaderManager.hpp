#ifndef SRC_GFX_SHADERS_SHADER_MANAGER_HPP
#define SRC_GFX_SHADERS_SHADER_MANAGER_HPP

#include "ShaderBaseModel.hpp"
#include <Utility/TextFile.hpp>

#include <glm/glm.hpp>

#include <iostream>

namespace GFX
{
	class ShaderManager
	{
	private:
		ShaderBaseModel* m_shaderData;
		TextFile* m_textFile;

	public:

		ShaderManager();
		~ShaderManager();

		/*!
		Load and compile a shader from a file, giving it a name and type
		\param  filepath Path to the shader file
		\param shaderKey Name given to the shader
		\param type What type of shader it is
		*/
		void LoadShader(const char* filepath, std::string shaderKey, GLenum type);

		/*!
		Attach a shader to a shader program
		\param  shaderKey What shader to attach
		\param shaderProgramKey What shader program to attach it to
		*/
		void AttachShader(std::string shaderKey, std::string shaderProgramKey);

		/*!
		Detach a shader from a shader program
		\param  shaderKey What shader to detach
		\param shaderProgramKey What shader program to detach it from
		*/
		void DetachShader(std::string shaderKey, std::string shaderProgramKey);

		/*!
		Create an OpenGL shader program with a given name
		\param shaderProgramKey Name given to the shader program
		*/
		void CreateProgram(std::string shaderProgramKey);

		/*!
		Tell the GPU to use a specific shader program
		\param shaderProgramKey Name of the program
		*/
		void UseProgram(std::string shaderProgramKey);

		/*!
		Link the attached shaders within a program
		\param shaderProgramKey The program to be linked
		*/
		void LinkProgram(std::string shaderProgramKey);

		/*!
		Get the ID of a shader
		\param shaderKey Name of the shader
		*/
		GLuint GetShaderID(std::string shaderKey);

		/*!
		Get the ID of a shader program
		\param shaderProgramKey Name of the shader
		*/
		GLuint GetShaderProgramID(std::string shaderProgramKey);

		/*!
		Delete a shader program
		\param shaderProgramKey Name of the shader
		*/
		void DeleteProgram(std::string shaderProgramKey);


		/*!
		Delete a shader
		\param shaderKey Name of the shader
		*/
		void DeleteShader(std::string shaderKey);

		/*!
		Reset the GPU to use no program
		*/
		void ResetProgram();


		/*!
		Get the location of a uniform in a program
		\param shaderProgramKey Name of the program to look for the uniform
		\param uniformName Name of the uniform to look for
		\return GLuint Location of the uniform
		*/
		GLint GetUniformLocation(std::string shaderProgramKey, std::string uniformName);

		GLint GetUniformBlockLocation(std::string shaderProgramKey, std::string uniformBlockName);

		void SetUniform(GLuint x, GLuint uniformLocation);
		void SetUniform(GLfloat x, GLuint uniformLocation);
		void SetUniform(GLfloat x, GLfloat y, GLuint uniformLocation);
		void SetUniform(GLfloat x, GLfloat y, GLfloat z, GLuint uniformLocation);
		void SetUniform(GLfloat x, GLfloat y, GLfloat z, GLfloat w, GLuint uniformLocation);

		void SetUniform(int count, glm::vec2 vec, GLuint uniformLocation);
		void SetUniform(int count, glm::vec3 vec, GLuint uniformLocation);
		void SetUniform(int count, glm::vec4 vec, GLuint uniformLocation);
		void SetUniform(int count, glm::mat2 mat, GLuint uniformLocation);
		void SetUniform(int count, glm::mat3 mat, GLuint uniformLocation);
		void SetUniform(int count, glm::mat4 mat, GLuint uniformLocation);
	};
}

#endif
