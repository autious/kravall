#ifndef SRC_CORE_WINDOWHANDLING_INITIALIZE_GLFW_HPP
#define SRC_CORE_WINDOWHANDLING_INITIALIZE_GLFW_HPP

#include <iostream>
#include <GL/glew.h>

#include "GLFWInclude.hpp"

namespace Core
{

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

		// assign callback functions...
		glfwSetFramebufferSizeCallback((*window), framebuffer_size_callback);

		// init glew
		glewExperimental = true;
		GLenum err = glewInit();

		if (GLEW_OK != err)
		{
			std::cout << "Unable to init GLEW." << std::endl;
			return -1;
		}

		return 0;
	}
}
#endif
