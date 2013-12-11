
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

#include "Console/Console.hpp"
#include "BGnomeImporter.hpp"

#include "Console/CLOP.hpp"
#include <sstream>

#include <iomanip>

#include <ContentManagement/ContentManager.hpp>
#include <logger/Logger.hpp>
#include <logger/internal/ClopHandler.hpp>

#include <Lua/LuaState.hpp>

void clopLoggerCallback( LogSystem::LogType m_type, const char * message )
{
    switch( m_type )
    {
    case LogSystem::LogType::logType_debug :
        Core::Console().PrintLine(std::string( message ), Colors::White );
        break;

    case LogSystem::LogType::logType_error :
        Core::Console().PrintLine(std::string( message ), Colors::Red );
        break;

    case LogSystem::LogType::logType_fatal :
        Core::Console().PrintLine(std::string( message ), Colors::Red );
        break;

    case LogSystem::LogType::logType_warning :
        Core::Console().PrintLine(std::string( message ), Colors::Yellow);
        break;
    }

}

// Just an example of a clop function
// This function gets registred in Init with clop::Register("exit", ClopCloseWindow);
// And the command is sent to the command line by pressing 'E' (as seen in run()) with Core::Console().SetInputLine("exit");
void ClopCloseWindow(clop::ArgList args)
{
	exit(0);
}

int initScreenHeight;
int initScreenWidth;

GLFWwindow* init( int argc, char** argv )
{
	GLFWwindow* window;

    LogSystem::RegisterLogHandler( LogSystem::debugHandler,     new ClopHandler( clopLoggerCallback, LogSystem::LogType::logType_debug ) );
    LogSystem::RegisterLogHandler( LogSystem::fatalHandler,     new ClopHandler( clopLoggerCallback, LogSystem::LogType::logType_fatal ) );
    LogSystem::RegisterLogHandler( LogSystem::errorHandler,     new ClopHandler( clopLoggerCallback, LogSystem::LogType::logType_error ) );
    LogSystem::RegisterLogHandler( LogSystem::warningHandler,   new ClopHandler( clopLoggerCallback, LogSystem::LogType::logType_warning) );

    Core::world.m_luaState.Execute( "scripts/config.lua" );

    for( int i = 0; i < argc-1; i++ )
    {
        if( strcmp( argv[i], "--conf" ) == 0 )
        {
            Core::world.m_luaState.DoBlock( argv[i+1] ); 
        }
    }

    initScreenWidth = Core::world.m_config.GetInt( "initScreenWidth", 1280 );
    initScreenHeight = Core::world.m_config.GetInt( "initScreenHeight", 720 );
    
	Core::InitializeGLFW(&window, initScreenWidth, initScreenHeight, Core::WindowMode::WMODE_WINDOWED);

	clop::Register("exit", ClopCloseWindow);

	if (GFX::Init(initScreenWidth,initScreenHeight) == GFX_FAIL)
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

	GFX::RenderText(glm::vec2(0, 100), 1.0f, Colors::Black, "The Quick Brown Fox Jumps Over The Lazy Dog");
	GFX::RenderText(glm::vec2(10, 120), 1.0f, Colors::Blue, "The Quick Brown Fox Jumps Over The Lazy Dog");
	GFX::RenderText(glm::vec2(20, 140), 1.0f, Colors::Green, "The Quick Brown Fox Jumps Over The Lazy Dog");
	GFX::RenderText(glm::vec2(30, 160), 1.0f, Colors::CornflowerBlue, "The Quick Brown Fox Jumps Over The Lazy Dog");
	GFX::RenderText(glm::vec2(40, 180), 1.0f, Colors::White, "The Quick Brown Fox Jumps Over The Lazy Dog????");

	GFX::RenderText(glm::vec2(0, 200), 1.0f, Colors::Gold, "ABCDEFGHIJKLMNOPQRSTUVWXYZASIUHDOIASHUDIOASHDA1234567890*'^&%#!?");
}

void SystemTimeRender()
{
    if( Core::world.m_config.GetBool( "showSystems", false ) )
    {
        std::vector<std::pair<const char *,std::chrono::microseconds>> times = Core::world.m_systemHandler.GetFrameTime();

        for( int i = 0; i < (int)times.size(); i++ )
        {
            std::stringstream ss;
            
            ss << times[i].first << ": " << std::fixed << std::setw( 7 ) << std::setprecision(4) << std::setfill( '0' ) << times[i].second.count() / 1000.0f << "ms";
	        GFX::RenderText(glm::vec2(5, GFX::GetScreenHeight()-5-20*times.size()+20*i), 1.0f, Colors::White, ss.str().c_str());
        }

	    GFX::Debug::DrawRectangle(glm::vec2(0,GFX::GetScreenHeight()-5-20-17*times.size() ), 
            glm::vec2(500, 20*times.size()), true, glm::vec4( 0.5f,0.5f,0.5f,0.5f) );
    }
}

void run( GLFWwindow * window )
{
    LOG_INFO << "Starting program" << std::endl;

	Core::Camera* gCamera;
	gCamera = new Core::Camera(45.0f, 1.0f, 1000.0f);
	gCamera->CalculateProjectionMatrix(initScreenWidth, initScreenHeight);
	gCamera->SetPosition(glm::vec3(0.0f, 0.0f, -700.0f));

	GFX::SetProjectionMatrix(gCamera->GetProjectionMatrix());

	Core::GetInput().Initialize(window);

    
    Entity ent1 = Core::world.m_entityHandler.CreateEntity<Core::ExampleComponent1,Core::ExampleComponent2>( Core::ExampleComponent1::D1(),
                                                                                   Core::ExampleComponent2::D2() );
    Core::ContentManager CM;

	GLuint IBO;
	GLuint VAO;
    GLint vSize;
    GLint iSize;

    CM.Load<Core::GnomeLoader>("assets/tomte.gnome", [&VAO, &IBO, &vSize, &iSize](Core::BaseAssetLoader* baseLoader, Core::AssetHandle handle)
            {
                Core::GnomeLoader* gnomeLoader = dynamic_cast<Core::GnomeLoader*>(baseLoader);
                const Core::ModelData* data = gnomeLoader->getData(handle);
                VAO = data->VAO;
                IBO = data->IBO;
                vSize = data->vSize;
                iSize = data->iSize;
            });
    std::cout << "ASDF" << std::endl;
   
	GFX::RenderSplash(Core::world.m_config.GetBool( "showSplash", false ));
    GFX::Material* m = new GFX::Material();
	m->diffuse = GFX::Content::LoadTexture2DFromFile("assets/GDM.png");
    /*
	for (int i = -100; i < 100; i++)
	{
		for (int j = -10; j < 10; j++)
		{
			Entity e2 = Core::world.m_entityHandler.CreateEntity<Core::GraphicsComponent, Core::WorldPositionComponent, Core::RotationComponent, Core::ScaleComponent>
				(Core::GraphicsComponent(), Core::WorldPositionComponent(), Core::RotationComponent(), Core::ScaleComponent());
	
			Core::GraphicsComponent* gc = WGETC<Core::GraphicsComponent>(e2);
	
			gc->ibo = IBO;
			gc->iboSize = vSize;
			gc->vao = VAO;
			gc->material = m;
			gc->shader = 0;
	
			Core::WorldPositionComponent* wpc = WGETC<Core::WorldPositionComponent>(e2);
			wpc->position[0] = i * 10;
			wpc->position[1] = j * 10;
	
			Core::ScaleComponent* sc = WGETC<Core::ScaleComponent>(e2);
			sc->scale = .1f;
	
			Core::RotationComponent* rc = WGETC<Core::RotationComponent>(e2);
		
			//rc->rotation[0] = sin(3.14f / 2.0f);
			//rc->rotation[1] = sin(3.14f / 2.0f);
			rc->rotation[2] = sin(3.14f);
			rc->rotation[3] = cos(3.14f / 2.0f);
		}
	}
    */

	std::cout << GFX::GetScreenWidth() << " " << GFX::GetScreenHeight() << " ";

	//inputline.resize(1);

    CM.Load<Core::GnomeLoader>("assets/flag.GNOME", [&m]
            (Core::BaseAssetLoader* baseLoader, Core::AssetHandle handle)
            {
                LOG_DEBUG << "Loading async" << std::endl;
                Core::GnomeLoader* gnomeLoader = dynamic_cast<Core::GnomeLoader*>(baseLoader);
                const Core::ModelData* data = gnomeLoader->getData(handle);

			    Entity e2 = Core::world.m_entityHandler.CreateEntity<Core::GraphicsComponent, Core::WorldPositionComponent, Core::RotationComponent, Core::ScaleComponent>
				(Core::GraphicsComponent(), Core::WorldPositionComponent(), Core::RotationComponent(), Core::ScaleComponent());
	
			    Core::GraphicsComponent* gc = WGETC<Core::GraphicsComponent>(e2);
	
		    	gc->ibo = data->IBO;
		    	gc->iboSize = data->vSize;
		    	gc->vao = data->VAO;
		    	gc->material = m;
		    	gc->shader = 0;
	
		    	Core::WorldPositionComponent* wpc = WGETC<Core::WorldPositionComponent>(e2);
		    	wpc->position[0] = 0.0f;
		    	wpc->position[1] = 0.0f;
	
		    	Core::ScaleComponent* sc = WGETC<Core::ScaleComponent>(e2);
		    	sc->scale = 0.5f;
	
		    	Core::RotationComponent* rc = WGETC<Core::RotationComponent>(e2);
		    
		    	rc->rotation[2] = 0.0f;
		    	rc->rotation[3] = 0.0f;
            }, true);


	while (!glfwWindowShouldClose(window))
	{
		Core::GetInput().UpdateInput();
		
		Core::Console().Update();

        CM.CallFinishers();

		//gCamera->CalculateViewMatrix();
		gCamera->LookAt(glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		GFX::SetViewMatrix(gCamera->GetViewMatrix());

		TestRendering();

	    GFX::Render();
        Core::world.m_systemHandler.Update( 0.1f );
        SystemTimeRender();
		Core::GetInput().ResetInput();
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
