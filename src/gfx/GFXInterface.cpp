#include <GFXInterface.hpp>

#include <iostream>

namespace GFX
{
	int Init(GLFWwindow* window)
	{
		// TODO: Init graphics engine
		return GFX_SUCCESS;
	}

	void Render(GLFWwindow* window)
	{
		// TODO: implement render function
		std::cout << "I'm a rendered picture!\n";
		glfwSwapBuffers(window);
	}
}