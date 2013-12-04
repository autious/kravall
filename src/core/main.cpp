
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
#include <gfx/Material.hpp>
#include <utility/Colors.hpp>
#include "Camera/Camera.hpp"
#include <ComponentFramework/SystemHandlerTemplate.hpp>
#include <ComponentFramework/EntityHandlerTemplate.hpp>

#include <utility/Colors.hpp>

#include "GLFWInput.hpp"
#include <World.hpp>

#include "console/console.hpp"
#include "BGnomeImporter.hpp"

#include "console/clop.hpp"
#include <sstream>

#include <iomanip>

#include <ContentManagement/ContentManager.hpp>
#include <logger/Logger.hpp>

#include <Lua/LuaState.hpp>

// Just an example of a clop function
// This function gets registred in Init with clop::Register("exit", ClopCloseWindow);
// And the command is sent to the command line by pressing 'E' (as seen in run()) with Core::Console().SetInputLine("exit");
void ClopCloseWindow(clop::ArgList args)
{
	exit(0);
}

GLFWwindow* init( int argc, char** argv )
{
	GLFWwindow* window;

    Core::world.m_luaState.Execute( "scripts/config.lua" );

    for( int i = 0; i < argc-1; i++ )
    {
        if( strcmp( argv[i], "--conf" ) == 0 )
        {
            Core::world.m_luaState.DoBlock( argv[i+1] ); 
        }
    }

	Core::InitializeGLFW(&window, 1280, 720, Core::WindowMode::WMODE_WINDOWED);

	clop::Register("exit", ClopCloseWindow);

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

void SystemTimeRender()
{
        std::vector<std::pair<const char *,std::chrono::microseconds>> times = Core::world.m_systemHandler.GetFrameTime();

        for( int i = 0; i < times.size(); i++ )
        {
            std::stringstream ss;
            
            ss << times[i].first << ": " << std::fixed << std::setw( 7 ) << std::setprecision(4) << std::setfill( '0' ) << times[i].second.count() / 1000.0f << "ms";
	        GFX::RenderText(glm::vec2(5, GFX::GetScreenHeight()-5-20*times.size()+20*i), glm::vec2(8, 12), Colors::White, ss.str().c_str());
        }

	    GFX::Debug::DrawRectangle(glm::vec2(0,GFX::GetScreenHeight()-5-20-17*times.size() ), 
            glm::vec2(500, 20*times.size()), true, glm::vec4( 0.5f,0.5f,0.5f,0.5f) );
}

void run( GLFWwindow * window )
{

    Core::ContentManager CM;

    LOG_INFO << "Starting program" << std::endl;

	Core::Camera* gCamera;
	gCamera = new Core::Camera(45.0f, 1.0f, 2000.0f);
	gCamera->CalculateProjectionMatrix(1280, 720);
	gCamera->SetPosition(glm::vec3(0.0f, 0.0f, -500.0f));

	GFX::SetProjectionMatrix(gCamera->GetProjectionMatrix());

	Core::GLFWInput* input = new Core::GLFWInput(window);
	GFX::RenderSplash(false);
	bool fs = false;

	BGnomeImporter* BGI = new BGnomeImporter();
    Entity ent1 = Core::world.m_entityHandler.CreateEntity<Core::ExampleComponent1,Core::ExampleComponent2>( Core::ExampleComponent1::D1(),
                                                                                   Core::ExampleComponent2::D2() );

	Entity e2 = Core::world.m_entityHandler.CreateEntity<Core::GraphicsComponent, Core::WorldPositionComponent, Core::RotationComponent, Core::ScaleComponent>
		(Core::GraphicsComponent(), Core::WorldPositionComponent(), Core::RotationComponent(), Core::ScaleComponent());

	GFX::StaticVertex* vs = nullptr;
	GLuint IBO;
	GLuint VAO;
	int vSize;
	int iSize;
	BGI->Go("assets/flag.GNOME", vs, vSize);

	int* indices = new int[vSize];
	iSize = vSize;
	for (int i = 0; i < vSize; i++)
	{
		indices[i] = i;
	}
	GFX::Content::LoadStaticMesh(IBO, VAO, vSize, iSize, vs, indices);

	std::cout << IBO << std::endl;
	std::cout << VAO << std::endl;
    

	GFX::Material* m = new GFX::Material();
	m->diffuse = GFX::Content::LoadTexture2DFromFile("assets/GDM.png");
	std::cout << GFX::GetScreenWidth() << " " << GFX::GetScreenHeight() << " ";
	while (!glfwWindowShouldClose(window))
	{
		input->UpdateInput();
		
		if (input->IsKeyPressedOnce(GLFW_KEY_ESCAPE))
			Core::Console().ClearInput();
		//	break;

		if (input->IsKeyPressedOnce(GLFW_KEY_TAB))
			Core::Console().Toggle();
		if (input->IsKeyPressedOnce(GLFW_KEY_UP))
			Core::Console().LastHistory();
		if (input->IsKeyPressedOnce(GLFW_KEY_DOWN))
			Core::Console().NextHistory();
		if (input->IsKeyPressedOnce(GLFW_KEY_PAGE_UP))
			Core::Console().Scroll(1);
		if (input->IsKeyPressedOnce(GLFW_KEY_PAGE_DOWN))
			Core::Console().Scroll(-1);
		if (input->IsKeyPressedOnce(GLFW_KEY_F))
			Core::Console().SetInputLine("Command " + std::to_string(rand()));
		if (input->IsKeyPressedOnce(GLFW_KEY_E))
			Core::Console().SetInputLine("exit");
		if (input->IsKeyPressedOnce(GLFW_KEY_C))
			Core::Console().SetInputLine("clear");
		if (input->IsKeyPressedOnce(GLFW_KEY_G))
			Core::Console().Add();
		Core::Console().Update();

		if (input->IsKeyPressedOnce(GLFW_KEY_ENTER))
		{
			glfwDestroyWindow(window);
			GFX::DeleteGFX();

			GLFWmonitor* pMonitor = glfwGetPrimaryMonitor();

			const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

			int windowWidth = mode->width;
			int windowHeight = mode->height;


			if (!fs)
			{
				Core::InitializeGLFW(&window, 1280, 720, Core::WindowMode::WMODE_FULLSCREEN_BORDERLESS);
				GFX::Init(windowWidth, windowHeight);
			}
			else
			{
				Core::InitializeGLFW(&window, 1280, 720, Core::WindowMode::WMODE_WINDOWED_BORDERLESS);
				GFX::Init(1280, 720);
			}

			fs = !fs;
		}

		//gCamera->CalculateViewMatrix();
		gCamera->LookAt(glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		GFX::SetViewMatrix(gCamera->GetViewMatrix());

		TestRendering();
		GFX::Draw(IBO, VAO, vSize, m);
		GFX::Render();

        Core::world.m_systemHandler.Update( 0.1f );
        SystemTimeRender();

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
	GLFWwindow* window = init( argc, argv );
	if( window == nullptr )
		return -1; 
#endif
#ifdef RUN_GTEST
    int gtestReturn = RUN_ALL_TESTS();
#ifdef _WIN32 
	std::cin.get();
#endif
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

/*! \mainpage Our Index Page

    \section intro_sec Introduction

     [Main Wiki](http://kronosept.comproj.bth.se:5001/)

    \section install_sec Installation

    \subsection step1 Step 1: Opening the box

*/
