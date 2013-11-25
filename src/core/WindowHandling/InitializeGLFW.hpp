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
		/* Initialize the library */
		if (!glfwInit())
		{
			std::cout << "Unable to init GLFW." << std::endl;
			return -1;
		}

		//Set context to version 4.4, not forward compatible (for compute shader support)
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_FALSE);
		glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

		/* Create a windowed mode window and its OpenGL context */
		(*window) = glfwCreateWindow(width, height, "RIOT", nullptr, nullptr);

		if (!(*window))
		{
			glfwTerminate();
			std::cout << "Unable to create window" << std::endl;
			return -1;
		}

		/* Make the window's context current */
		glfwMakeContextCurrent(*window);

		std::cout << "OpenGL version: " << glfwGetWindowAttrib((*window), GLFW_CONTEXT_VERSION_MAJOR) << "." << glfwGetWindowAttrib((*window), GLFW_CONTEXT_VERSION_MINOR) << std::endl;

		// if 1 then limits system to max 60 fps!
		glfwSwapInterval(0);
		glClearColor(100.0f / 255.0f, 149.0f / 255.0f, 237.0f / 255.0f, 1.0f);

		// init glew
		glewExperimental = true;
		GLenum err = glewInit();

		if (GLEW_OK != err)
		{
			std::cout << "Unable to init GLEW." << std::endl;
			return -1;
		}

		// assign callback functions...
		glDebugMessageCallbackARB(glErrorCallback, NULL);
		glfwSetFramebufferSizeCallback((*window), framebuffer_size_callback);

		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);
		glEnable(GL_CULL_FACE);

		glCullFace(GL_BACK);
		glFrontFace(GL_CW);
		glBlendEquation(GL_FUNC_ADD);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);

		return 0;
	}
}
#endif