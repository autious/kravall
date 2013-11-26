#ifndef SRC_CORE_WINDOWHANDLING_INITIALIZE_GLFW_HPP
#define SRC_CORE_WINDOWHANDLING_INITIALIZE_GLFW_HPP

#include <iostream>
#include <glew/glew.h>

#include "GLFWInclude.hpp"

namespace Core
{
	void APIENTRY glErrorCallback(GLenum _source, GLenum _type, GLuint _id, GLenum _severity, GLsizei _length, const char* _message, void* _userParam)
	{
		const char* source = "";

		if (_source == GL_DEBUG_SOURCE_API_ARB)
			source = "The GL";
		else if (_source == GL_DEBUG_SOURCE_SHADER_COMPILER_ARB)
			source = "The GLSL Shader compiler";
		else if (_source == GL_DEBUG_SOURCE_WINDOW_SYSTEM_ARB)
			source = "The source window system arb";

		std::cout << "OPENGL_ERROR: id:" << source << ":" << _type << ":" << _message << std::endl;
	}

	void framebuffer_size_callback(GLFWwindow* window, int width, int height)
	{
		//Resize framebuffer here
	}

	int InitializeGLFW(GLFWwindow** window, int width, int height)
	{
		
		

		return 0;
	}
}
#endif