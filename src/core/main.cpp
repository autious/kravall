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
	gCamera->SetPosition(glm::vec3(0.0f, 0.0f, 10.0f));

	GFX::SetProjectionMatrix(gCamera->GetProjectionMatrix());

	while (!glfwWindowShouldClose(window))
	{
		if (glfwGetKey(window, GLFW_KEY_ESCAPE))
			break;

		//gCamera->CalculateViewMatrix();
		gCamera->LookAt(glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		GFX::SetViewMatrix(gCamera->GetViewMatrix());

		GFX::Debug::DrawBox(glm::vec3(2, 0, 0), glm::vec3(2, 2, 1), false, glm::vec4(0, 0, 0, 1));
		GFX::Debug::DrawBox(glm::vec3(2, 0, 0), glm::vec3(2, 2, 1), true, glm::vec4(0, 0, 0, 0.5f));

		GFX::Debug::DrawLine(glm::vec2(100, 50), glm::vec2(1200, 600), glm::vec4(1, 0, 0, 1));
		GFX::Debug::DrawPoint(glm::vec2(100, 50), glm::vec4(0, 1, 0, 1), 10);
		GFX::Debug::DrawPoint(glm::vec2(1200, 600), glm::vec4(0, 1, 0, 1), 10);
		
		GFX::Debug::DrawRectangle(glm::vec2(200, 200), glm::vec2(100, 20), true, glm::vec4(0, 1, 1, 0.5f));
		GFX::Debug::DrawRectangle(glm::vec2(200, 300), glm::vec2(100, 20), false, glm::vec4(0, 1, 1, 1.0f));

		GFX::RenderText(glm::vec2(0, 100), glm::vec2(8, 12), glm::vec4(0, 0, 0, 1), "The Quick Brown Fox Jumps Over The Lazy Dog");
		GFX::RenderText(glm::vec2(10, 120), glm::vec2(7, 11), glm::vec4(0, 0, 1, 1), "The Quick Brown Fox Jumps Over The Lazy Dog");
		GFX::RenderText(glm::vec2(20, 140), glm::vec2(6, 10), glm::vec4(0, 1, 0, 1), "The Quick Brown Fox Jumps Over The Lazy Dog");
		GFX::RenderText(glm::vec2(30, 160), glm::vec2(5, 9), glm::vec4(1, 0, 0, 1), "The Quick Brown Fox Jumps Over The Lazy Dog");
		GFX::RenderText(glm::vec2(40, 180), glm::vec2(4, 8), glm::vec4(1, 1, 1, 1), "The Quick Brown Fox Jumps Over The Lazy Dog");

		GFX::RenderText(glm::vec2(0, 200), glm::vec2(8, 12), glm::vec4(1, 1, 1, 1), "ABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890ÅÄÖ*'^&%#!?");
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
