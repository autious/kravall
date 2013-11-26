#include <iostream>


//#include <logger/Logger.hpp>


#include "WindowHandling/InitializeGLFW.hpp"

#include <gfx/GFXInterface.hpp>

int main(int argc, char** argv)
{
	GLFWwindow* window;

	

	Core::InitializeGLFW(&window, 1280, 720);

	if (GFX::Init(window) == GFX_FAIL)
		return -1;


	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		if (glfwGetKey(window, GLFW_KEY_ESCAPE))
			break;

		GFX::Render(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}
