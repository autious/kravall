#ifdef RUN_GTEST 
    #include <gtest/gtest.h>
#endif

#ifdef _WIN32
    #include <Windows.h>
#endif

#include <iostream>

#include <WindowHandling/GLFWInclude.hpp>
#include "WindowHandling/InitializeGLFW.hpp"

#include <gfx/GFXInterface.hpp>
#include <utility/Colors.hpp>
#include "Camera/Camera.hpp"

#include <Utility/Colors.hpp>

#include "GLFWInput.hpp"

GLFWwindow* init()
{
	GLFWwindow* window;

	Core::InitializeGLFW(&window, 1280, 720);

	if (GFX::Init(1280,720) == GFX_FAIL)
		return nullptr;

    return window;
}

void TestRendering()
{
	GFX::Debug::DrawBox(glm::vec3(2, 0, 0), glm::vec3(2, 2, 10), false, Colors::Black);
	GFX::Debug::DrawBox(glm::vec3(2, 0, 0), glm::vec3(2, 2, 10), true, glm::vec4(Colors::Black.x, Colors::Black.y, Colors::Black.z, 0.5f));
	GFX::Debug::DrawSphere(glm::vec3(2, 0, 0), 2.0f, Colors::Black);

	GFX::Debug::DrawLine(glm::vec2(100, 50), glm::vec2(1200, 600), Colors::CornflowerBlue);
	GFX::Debug::DrawPoint(glm::vec2(100, 50), Colors::Green, 10);
	GFX::Debug::DrawPoint(glm::vec2(1200, 600), Colors::Green, 10);

	GFX::Debug::DrawRectangle(glm::vec2(0, 0), glm::vec2(200, 20), true, Colors::Aquamarine);
	GFX::Debug::DrawRectangle(glm::vec2(100, 20), glm::vec2(100, 40), false, Colors::Chocolate);

	GFX::RenderText(glm::vec2(0, 100), glm::vec2(8, 12), Colors::Black, "The Quick Brown Fox Jumps Over The Lazy Dog");
	GFX::RenderText(glm::vec2(10, 120), glm::vec2(7, 11), Colors::Blue, "The Quick Brown Fox Jumps Over The Lazy Dog");
	GFX::RenderText(glm::vec2(20, 140), glm::vec2(6, 10), Colors::Green, "The Quick Brown Fox Jumps Over The Lazy Dog");
	GFX::RenderText(glm::vec2(30, 160), glm::vec2(5, 9), Colors::CornflowerBlue, "The Quick Brown Fox Jumps Over The Lazy Dog");
	GFX::RenderText(glm::vec2(40, 180), glm::vec2(8, 12), Colors::White, "The Quick Brown Fox Jumps Over The Lazy Dog????");

	GFX::RenderText(glm::vec2(0, 200), glm::vec2(8, 12), Colors::Gold, "ABCDEFGHIJKLMNOPQRSTUVWXYZASIUHDOIASHUDIOASHDA1234567890*'^&%#!?");
}

void run( GLFWwindow * window )
{
	
	Core::Camera* gCamera;
	gCamera = new Core::Camera(45.0f, 1.0f, 1000.0f);
	gCamera->CalculateProjectionMatrix(1280, 720);
	gCamera->SetPosition(glm::vec3(0.0f, 0.0f, 10.0f));

	GFX::SetProjectionMatrix(gCamera->GetProjectionMatrix());

	Core::GLFWInput* input = new Core::GLFWInput(window);
	GFX::RenderSplash(true);

	while (!glfwWindowShouldClose(window))
	{
		input->UpdateInput();
		
		if (input->IsKeyPressedOnce(GLFW_KEY_ESCAPE))
			break;

		if (input->IsKeyPressedOnce(GLFW_KEY_TAB))
			GFX::ToggleConsole();

		//gCamera->CalculateViewMatrix();
		gCamera->LookAt(glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		GFX::SetViewMatrix(gCamera->GetViewMatrix());

		TestRendering();

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
