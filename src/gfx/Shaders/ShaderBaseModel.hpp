#ifndef SRC_GFX_SHADERS_SHADER_BASE_MODEL_HPP
#define SRC_GFX_SHADERS_SHADER_BASE_MODEL_HPP

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <map>
#include <string>

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

		GLuint GetShaderID(std::string shaderKey);
		GLuint GetShaderProgram(std::string shaderProgramKey);
		bool SaveShader(std::string shaderKey, GLuint shaderID);
		bool SaveShaderProgram(std::string shaderProgramKey, GLuint shaderProgramID);

		void DeleteShader(std::string shaderKey);
		void DeleteShaderProgram(std::string shaderProgramKey);
	};
}

#endif
