#include <iostream>

#include "WindowHandling/InitializeGLFW.hpp"
int main(int argc, char** argv)
{
	GLFWwindow* window;

	Core::InitializeGLFW(&window, 1280, 720);

	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		if (glfwGetKey(window, GLFW_KEY_ESCAPE))
			break;

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}