#ifdef RUN_GTEST 
    #include <gtest/gtest.h>
#endif

#include <iostream>

#include <WindowHandling/GLFWInclude.hpp>
#include "WindowHandling/InitializeGLFW.hpp"

#include <gfx/GFXInterface.hpp>

#include "Camera/Camera.hpp"



GLFWwindow* init()
{
	GLFWwindow* window;

	Core::InitializeGLFW(&window, 1280, 720);

	if (GFX::Init(1280,720) == GFX_FAIL)
		return nullptr;

    return window;
}

void run( GLFWwindow * window )
{
	Core::Camera* gCamera;
	gCamera = new Core::Camera(45.0f, 1.0f, 1000.0f);
	gCamera->CalculateProjectionMatrix(1280, 720);

	GFX::SetProjectionMatrix(gCamera->GetProjectionMatrixAsArray());

	while (!glfwWindowShouldClose(window))
	{
		if (glfwGetKey(window, GLFW_KEY_ESCAPE))
			break;

		GFX::Debug::DrawPoint(glm::vec2(100.0f, 100.0f), glm::vec4(1.0f, 1.0f, 0.0f, 1.0f));


		gCamera->CalculateViewMatrix();
		GFX::SetViewMatrix(gCamera->GetViewMatrixAsArray());

		GFX::Render();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

    glfwDestroyWindow( window );

	glfwTerminate();
}


int main(int argc, char** argv)
{
#ifdef RUN_GTEST
    ::testing::InitGoogleTest(&argc, argv);
#endif
#ifndef SKIP_RUN
	GLFWwindow* window = init();
	if( window == nullptr )
		return -1; 
#endif
#ifdef RUN_GTEST
    int gtestReturn = RUN_ALL_TESTS();
    if( gtestReturn != 0 )
        return gtestReturn;
#endif
#ifndef SKIP_RUN
    run( window );
#elif _WIN32
	std::cin.get();
#endif

#ifdef RUN_GTEST
    return gtestReturn;
#else
	return 0;
#endif
}
