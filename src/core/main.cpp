#ifdef RUN_GTEST 
    #include <gtest/gtest.h>
#endif

#include <Windows.h>

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

	GFX::SetProjectionMatrix(gCamera->GetProjectionMatrix());

	while (!glfwWindowShouldClose(window))
	{
		if (glfwGetKey(window, GLFW_KEY_ESCAPE))
			break;
		GFX::Debug::DrawPoint(glm::vec2(20.0f, 20.0f), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), 10.0f);
		GFX::Debug::DrawPoint(glm::vec2(640.0f, 360.0f), glm::vec4(1.0f, 0.0f, 1.0f, 0.5f), 100.0f);
		GFX::Debug::DrawPoint(glm::vec2(800.0f, 600.0f), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), 10.0f);


		gCamera->CalculateViewMatrix();
		GFX::SetViewMatrix(gCamera->GetViewMatrix());

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
    GLFWwindow* window = init();
    if( window == nullptr )
       return -1; 
#ifdef RUN_GTEST
    int gtestReturn = RUN_ALL_TESTS();
    if( gtestReturn != 0 )
        return gtestReturn;
#endif
#ifndef SKIP_RUN
    run( window );
#endif
#ifdef RUN_GTEST
    return gtestReturn;
#else
	return 0;
#endif
}
