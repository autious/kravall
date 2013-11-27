#ifndef SRC_GFX_SHADERS_SHADER_BASE_MODEL_HPP
#define SRC_GFX_SHADERS_SHADER_BASE_MODEL_HPP

#include <map>
#include <string>

#include <gl/glew.h>

namespace GFX
{
	class ShaderBaseModel
	{
		friend class ShaderManager;

	private:
		std::map<std::string, GLuint>* m_shader;
		std::map<std::string, GLuint>* m_shaderProgram;

		ShaderBaseModel();
		~ShaderBaseModel();

	public:

		/*!
		Function to retrieve a shader's id
		\param shaderKey Name of the shader
		\return GLuint Returns the id of the input shader
		*/
		GLuint GetShaderID(std::string shaderKey);


		/*!
		Function to retrieve a shader program's id
		\param shaderKey Name of the shader program
		\return GLuint Returns the id of the input shader program
		*/
		GLuint GetShaderProgram(std::string shaderProgramKey);

		/*!
		Save the name and id of the shader to the list
		\param shaderKey Name of the shader
		\param shaderID ID of the shader
		\return bool Returns if the operation failed or not
		*/
		bool SaveShader(std::string shaderKey, GLuint shaderID);

		/*!
		Save the name and id of the shader program to the list
		\param shaderProgramKey Name of the shader program
		\param shaderProgramID ID of the shader program
		\return bool Returns if the operation failed or not
		*/
		bool SaveShaderProgram(std::string shaderProgramKey, GLuint shaderProgramID);

		/*!
		Remove the shader with the associated name
		\param shaderKey Name of the shader
		*/
		void DeleteShader(std::string shaderKey);

		/*!
		Remove the shader program with the associated name
		\param shaderProgramKey Name of the shader program
		*/
		void DeleteShaderProgram(std::string shaderProgramKey);
	};
}

#endif
