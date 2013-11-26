#include <iostream>

#include "WindowHandling/InitializeGLFW.hpp"

#include <gfx/GFXInterface.hpp>


int main(int argc, char** argv)
{
	GLFWwindow* window;
	
	Core::InitializeGLFW(&window, 1280, 720);

<<<<<<< HEAD
	if (GFX::Init(1280,720) == GFX_FAIL)
=======
	if (GFX::Init(1280, 720) == GFX_FAIL)
>>>>>>> 4c02ab93e2b69a6035395b0acdb936ae07de9576
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
