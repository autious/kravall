#ifndef SRC_GFX_SHADERS_SHADER_MANAGER_HPP
#define SRC_GFX_SHADERS_SHADER_MANAGER_HPP

#include "ShaderBaseModel.hpp"
#include <Utility/TextFile.hpp>

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

		void LoadShader(const char* filepath, std::string shaderKey, GLenum type);
		void AttachShader(std::string shaderKey, std::string shaderProgramKey);
		void DetachShader(std::string shaderKey, std::string shaderProgramKey);

		void CreateProgram(std::string shaderProgramKey);
		void UseProgram(std::string shaderProgramKey);
		void LinkProgram(std::string shaderProgramKey);

		GLuint GetShaderID(std::string shaderKey);
		GLuint GetShaderProgramID(std::string shaderProgramKey);


		void DeleteProgram(std::string shaderProgramKey);
		void DeleteShader(std::string shaderKey);

		void ResetProgram();
	};
}

#endif
