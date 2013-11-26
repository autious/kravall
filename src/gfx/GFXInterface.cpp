#include <GFXInterface.hpp>

#include <iostream>
#include "Shaders/ShaderManager.hpp"

namespace GFX
{

	GLuint dummyVAO;
	ShaderManager* sm = new ShaderManager();
	void InitDummyVAO()
	{
		float dummy = 1;

		GLuint VBO;
		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float), &dummy, GL_STATIC_DRAW);

		glGenVertexArrays(1, &dummyVAO);
		glBindVertexArray(dummyVAO);

		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glVertexAttribPointer(0, 1, GL_FLOAT, GL_FALSE, 0, (GLubyte*)NULL);

		glBindVertexArray(0);
	}


	int Init(GLFWwindow** window)
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
		(*window) = glfwCreateWindow(1280, 720, "RIOT", nullptr, nullptr);

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
		//glDebugMessageCallbackARB(glErrorCallback, NULL);
		//glfwSetFramebufferSizeCallback((*window), framebuffer_size_callback);

		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);
		glEnable(GL_CULL_FACE);

		glCullFace(GL_BACK);
		glFrontFace(GL_CW);
		glBlendEquation(GL_FUNC_ADD);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);

		// TODO: Init graphics engine

		
		sm->LoadShader("shaders/PassThrough.vertex", "vsTest", GL_VERTEX_SHADER);
		sm->LoadShader("shaders/FSQuad.geometry", "gsTest", GL_GEOMETRY_SHADER);
		sm->LoadShader("shaders/WaveRipple.fragment", "fsTest", GL_FRAGMENT_SHADER);

		sm->CreateProgram("test");
		sm->AttachShader("vsTest", "test");
		sm->AttachShader("gsTest", "test");
		sm->AttachShader("fsTest", "test");
		sm->LinkProgram("test");

		InitDummyVAO();
		return GFX_SUCCESS;
	}

	void Render(GLFWwindow* window)
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// TODO: implement render function

		sm->UseProgram("test");
		glBindVertexArray(dummyVAO);
		glDrawArrays(GL_POINTS, 0, 1);

		glfwSwapBuffers(window);
	}
}