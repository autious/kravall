#include <iostream>

#include "WindowHandling/InitializeGLFW.hpp"

#include <gfx/GFXInterface.hpp>


int main(int argc, char** argv)
{
	GLFWwindow* window;
	
	Core::InitializeGLFW(&window, 1280, 720);

	if (GFX::Init(1280, 720) == GFX_FAIL)
		return -1;


	while (!glfwWindowShouldClose(window))
	{
		if (glfwGetKey(window, GLFW_KEY_ESCAPE))
			break;

		GFX::Render();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}
