#include <iostream>

#include "WindowHandling/InitializeGLFW.hpp"

int main(int argc, char** argv)
{
	GLFWwindow* window;

	InitializeGLFW(&window, 1280, 720);

	std::cout << "Hello world.\n";
	system("pause");

	return 0;
}
