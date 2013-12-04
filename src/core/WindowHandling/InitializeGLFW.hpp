#ifndef SRC_CORE_WINDOWHANDLING_INITIALIZE_GLFW_HPP
#define SRC_CORE_WINDOWHANDLING_INITIALIZE_GLFW_HPP

#include <iostream>

#include "GLFWInclude.hpp"
#include <gfx/GFXInterface.hpp>
#include <World.hpp>

namespace Core
{
	enum WindowMode
	{
		WMODE_WINDOWED = 0,
		WMODE_WINDOWED_BORDERLESS,
		WMODE_FULLSCREEN,
		WMODE_FULLSCREEN_BORDERLESS
	};
	void framebuffer_size_callback(GLFWwindow* window, int width, int height)
	{
		GFX::Resize(width, height);
	}

	int InitializeGLFW(GLFWwindow** window, int width, int height, enum WindowMode wMode )
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
        glfwWindowHint(GLFW_RESIZABLE, Core::world.m_config.GetBool( "windowResizable", true ) );

		GLFWmonitor* pMonitor = glfwGetPrimaryMonitor();

		const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

		int windowWidth = mode->width;
		int windowHeight = mode->height;

		switch (wMode)
		{
			case WMODE_WINDOWED:
			{
				glfwWindowHint(GLFW_DECORATED, GL_TRUE);
				/* Create a windowed mode window and its OpenGL context */
				(*window) = glfwCreateWindow(width, height, "RIOT", nullptr, nullptr);

				glfwSetWindowPos(*window, windowWidth / 2 - width / 2, windowHeight / 2 - height / 2);
			}
			break;

			case WMODE_WINDOWED_BORDERLESS:
			{
				glfwWindowHint(GLFW_DECORATED, GL_FALSE);
				/* Create a windowed mode window and its OpenGL context */
				(*window) = glfwCreateWindow(width, height, "RIOT", nullptr, nullptr);

				glfwSetWindowPos(*window, windowWidth / 2 - width / 2, windowHeight / 2 - height / 2);
			}
			break;

			case WMODE_FULLSCREEN:
			{
				/* Create a windowed mode window and its OpenGL context */
				(*window) = glfwCreateWindow(width, height, "RIOT", pMonitor, nullptr);
			}
			break;

			case WMODE_FULLSCREEN_BORDERLESS:
			{
				glfwWindowHint(GLFW_DECORATED, GL_FALSE);
				/* Create a windowed mode window and its OpenGL context */
				(*window) = glfwCreateWindow(windowWidth, windowHeight, "RIOT", nullptr, nullptr);
			}
			break;
		}


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
		glfwSwapInterval( Core::world.m_config.GetInt( "vsync", 0 ));

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
