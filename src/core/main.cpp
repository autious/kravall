
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
#include <ComponentFramework/SystemHandlerTemplate.hpp>
#include <ComponentFramework/EntityHandlerTemplate.hpp>
#include <ComponentFramework/SystemTypes.hpp>

#include <utility/Colors.hpp>

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
#include <Input/InputManager.hpp>

#include <gfx/BitmaskDefinitions.hpp>


GFX::FontData* fontData;

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
    Core::world.m_luaState.Execute( "scripts/main.lua" );

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

	Core::world.InitWorld();

	if (GFX::Init(initScreenWidth,initScreenHeight) == GFX_FAIL)
		return nullptr;

    Core::world.m_contentManager.Load<Core::TTFLoader>(Core::world.m_config.GetString("consoleFont", "assets/Fonts/ConsoleFont.font").c_str(), [](Core::BaseAssetLoader* baseLoader, Core::AssetHandle handle)
            {
                fontData = static_cast<GFX::FontData*>(handle);
                Core::Console().Init(fontData);  
                GFX::Debug::SetStatisticsFont(fontData);
            });

    return window;
}

void TestRendering()
{
	GFX::Debug::DrawBox(glm::vec3(50, -35, 0), glm::vec3(25, 25, 25), false, Colors::YellowGreen, true);
	GFX::Debug::DrawBox(glm::vec3(50, -35, 0), glm::vec3(25, 25, 25), true, glm::vec4(Colors::YellowGreen.x, Colors::YellowGreen.y, Colors::YellowGreen.z, 0.9f), true);

	GFX::Debug::DrawBox(glm::vec3(50, 0, 0.0f), glm::vec3(25, 25, 25), false, Colors::HotPink, true);
	GFX::Debug::DrawBox(glm::vec3(50, 0, 0.0f), glm::vec3(25, 25, 25), true, glm::vec4(Colors::HotPink.x, Colors::HotPink.y, Colors::HotPink.z, 0.9f), true);

	GFX::Debug::DrawBox(glm::vec3(50, -35, -35), glm::vec3(25, 25, 25), false, Colors::HotPink, true);
	GFX::Debug::DrawBox(glm::vec3(50, -35, -35), glm::vec3(25, 25, 25), true, glm::vec4(Colors::HotPink.x, Colors::HotPink.y, Colors::HotPink.z, 0.9f), true);

	GFX::Debug::DrawSphere(glm::vec3(0, 0, 0.0f), 25.0f, Colors::Green, false);
	GFX::Debug::DrawSphere(glm::vec3(-50, 0, 0.0f), 25.0f, Colors::White, true);

	GFX::Debug::DrawLine(glm::vec3(-50.0f, -50.0f, -50.0f), glm::vec3(50.0f, 50.0f, 50.0f), Colors::CornflowerBlue, true);
	GFX::Debug::DrawLine(glm::vec3(50.0f, -50.0f, -50.0f), glm::vec3(-50.0f, 50.0f, 50.0f), Colors::CornflowerBlue, false);
	//GFX::Debug::DrawPoint(glm::vec2(100, 50), Colors::Green, 10);
	//GFX::Debug::DrawPoint(glm::vec2(1200, 600), Colors::Green, 10);

	//GFX::Debug::DrawRectangle(glm::vec2(0, 0), glm::vec2(200, 20), true, Colors::Aquamarine);
	//GFX::Debug::DrawRectangle(glm::vec2(100, 20), glm::vec2(100, 40), false, Colors::Chocolate);

	//GFX::RenderText(fontData, glm::vec2(0, 100), 1.0f, Colors::Black, "The Quick Brown Fox Jumps Over The Lazy Dog");
	//GFX::RenderText(fontData, glm::vec2(10, 120), 1.0f, Colors::Blue, "The Quick Brown Fox Jumps Over The Lazy Dog");
	//GFX::RenderText(fontData, glm::vec2(20, 140), 1.0f, Colors::Green, "The Quick Brown Fox Jumps Over The Lazy Dog");
	//GFX::RenderText(fontData, glm::vec2(30, 160), 1.0f, Colors::CornflowerBlue, "The Quick Brown Fox Jumps Over The Lazy Dog");
	//GFX::RenderText(fontData, glm::vec2(40, 180), 1.0f, Colors::White, "The Quick Brown Fox Jumps Over The Lazy Dog????");
	//
	//GFX::RenderText(fontData, glm::vec2(0, 200), 1.0f, Colors::Gold, "ABCDEFGHIJKLMNOPQRSTUVWXYZASIUHDOIASHUDIOASHDA1234567890*'^&%#!?");
}

void CreateRioter(std::vector<Core::Entity>* rioterList, int meshID, unsigned int materialID, float posX, float posY, float posZ)
{
	int index = rioterList->size(); // Size before add will be the index of the added entity.
	double pi = 3.141529;
	double angle = 0.0; // pi * 0.25;

	Core::BoundingVolumeCollisionModel aa = Core::BoundingVolumeCollisionModel::DynamicResolution;

	glm::vec3 direction( 0.0f, 0.0f, 0.0f );
	if( posX )
		//direction = glm::normalize( glm::vec3( 0.0f, 0.0f, posY ) );
		direction = glm::normalize( glm::vec3( posX, 0.0f, 0.0f ) );
		//direction = glm::normalize( glm::vec3( posX, 0.0f, posY ) );	
	else 
	{
		aa = Core::BoundingVolumeCollisionModel::StaticResolution;
		posZ += 0.7f;
	}

	rioterList->push_back(Core::world.m_entityHandler.CreateEntity<Core::GraphicsComponent, 
		Core::WorldPositionComponent, Core::RotationComponent, Core::ScaleComponent, Core::UnitTypeComponent,
		Core::MovementComponent, Core::AttributeRioterComponent, Core::BoundingVolumeComponent>
		(Core::GraphicsComponent(), 
		 Core::WorldPositionComponent(posX, posY, posZ),
		 Core::RotationComponent(),
		 Core::ScaleComponent(0.1f),
		 Core::UnitTypeComponent(),
		 //Core::MovementComponent(0.0f, 0.0f, 1.0f, 2.0f, 6.0f),
		 Core::MovementComponent( -direction.x, 0, -direction.z, 21.1f, 5.0f),
		 Core::AttributeRioterComponent(),
		 Core::BoundingVolumeComponent( Core::BoundingSphere( 3.0f, 0.0f, 0.0f, 0.0f ), aa ) ));

	Core::GraphicsComponent* gc = WGETC <Core::GraphicsComponent>(rioterList->at(index));
	GFX::SetBitmaskValue(gc->bitmask, GFX::BITMASK::MESH_ID, meshID);
    GFX::SetBitmaskValue(gc->bitmask, GFX::BITMASK::MATERIAL_ID, materialID);
	GFX::SetBitmaskValue(gc->bitmask, GFX::BITMASK::TYPE, GFX::OBJECT_TYPES::OPAQUE_GEOMETRY);
}

void LuaInfoRender()
{
    int screenWidth = GFX::GetScreenWidth(); 
    int screenHeight = GFX::GetScreenHeight();

    std::stringstream ss;

    ss << "Memory: " << Core::world.m_luaState.GetMemoryUse() << "Kb";
    GFX::RenderText( fontData, glm::vec2( 510, screenHeight - 40 ), 1.0f, Colors::White, "Lua"  );
    GFX::RenderText( fontData, glm::vec2( 510, screenHeight - 25 ), 1.0f, Colors::White, ss.str().c_str()  );
    std::stringstream ss2;
    ss2 << "Update: " << std::fixed << std::setw( 7 ) << std::setprecision(4) << std::setfill( '0' ) << Core::world.m_luaState.GetUpdateTiming().count() / 1000.0f << "ms";
    GFX::RenderText( fontData, glm::vec2( 510, screenHeight - 10 ), 1.0f, Colors::White, ss2.str().c_str()  );

    GFX::Debug::DrawRectangle(glm::vec2( 505, screenHeight - 55 ),
            glm::vec2(175, 50), true, glm::vec4( 0.5f,0.5f,0.5f,0.5f) );

}

void SystemTimeRender()
{
    bool showSystems = Core::world.m_config.GetBool( "showSystems", false ) ;
	GFX::Debug::DisplaySystemInfo( showSystems );
    if( showSystems )
    {
        LuaInfoRender();

        std::vector<std::pair<const char *,std::chrono::microseconds>> times = Core::world.m_systemHandler.GetFrameTime();

        for( int i = 0; i < (int)times.size(); i++ )
        {
            std::stringstream ss;
            
            ss << times[i].first << ": " << std::fixed << std::setw( 7 ) << std::setprecision(4) << std::setfill( '0' ) << times[i].second.count() / 1000.0f << "ms";
	        GFX::RenderText(fontData, glm::vec2(5, GFX::GetScreenHeight()+12-20*times.size()+20*i), 1.0f, Colors::White, ss.str().c_str());
        }

	    GFX::Debug::DrawRectangle(glm::vec2(0,GFX::GetScreenHeight()-5-20*times.size() ), 
            glm::vec2(500, 20*times.size()), true, glm::vec4( 0.5f,0.5f,0.5f,0.5f) );
    }
}



void run( GLFWwindow * window )
{
    LOG_INFO << "Starting program" << std::endl;

	// init game camera...
	Core::gameCamera = Core::world.m_constantHeap.NewObject<Core::Camera>(
		Core::world.m_config.GetDouble( "initCameraFieldOfView", 45.0f ), 
		Core::world.m_config.GetDouble( "initCameraNearClipDistance", 1.0f ), 
		Core::world.m_config.GetDouble( "initCameraFarClipDistance", 1000.0f ) );
	Core::gameCamera->CalculateProjectionMatrix(initScreenWidth, initScreenHeight);
	Core::gameCamera->SetPosition(glm::vec3(0.0f, 100.0f, 200.0f));
	
    Core::ContentManager CM;

	GFX::SetProjectionMatrix(Core::gameCamera->GetProjectionMatrix());

	std::vector<Core::Entity> rioters;

	//Core::GetInput().Initialize(window);
    Core::GetInputManager().Init( window );

    unsigned int meshID; 
    unsigned int materialID;

    Core::world.m_contentManager.Load<Core::GnomeLoader>("assets/teapot.bgnome", [&meshID](Core::BaseAssetLoader* baseLoader, Core::AssetHandle handle)
            {
                Core::GnomeLoader* gnomeLoader = dynamic_cast<Core::GnomeLoader*>(baseLoader);
                const Core::ModelData* data = gnomeLoader->getData(handle);
				meshID = data->meshID;
            });

    Core::world.m_contentManager.Load<Core::MaterialLoader>("assets/material/test-material.material", [&materialID](Core::BaseAssetLoader* baseLoader, Core::AssetHandle handle)
            {
                Core::MaterialData* data = static_cast<Core::MaterialData*>(handle);
                materialID = data->materialId;
            }, false);
   
	GFX::RenderSplash(Core::world.m_config.GetBool( "showSplash", false ));	


	//for (int i = -100; i < 100; i++)
	//{
	//	for (int j = -10; j < 10; j++)
	//	{
	//		Core::Entity e2 = Core::world.m_entityHandler.CreateEntity<Core::GraphicsComponent, Core::WorldPositionComponent, Core::RotationComponent, Core::ScaleComponent>
	//			(Core::GraphicsComponent(), Core::WorldPositionComponent(), Core::RotationComponent(), Core::ScaleComponent());
	//
	//		Core::GraphicsComponent* gc = WGETC<Core::GraphicsComponent>(e2);
	//		
	//		GFX::SetBitmaskValue(gc->bitmask, GFX::BITMASK::TYPE, GFX::OBJECT_TYPES::OPAQUE_GEOMETRY);
	//		GFX::SetBitmaskValue(gc->bitmask, GFX::BITMASK::MESH_ID, meshID);
	//		
	//
	//		Core::WorldPositionComponent* wpc = WGETC<Core::WorldPositionComponent>(e2);
	//		wpc->position[0] = (float)(i * 10);
	//		wpc->position[1] = (float)(j * 10);
	//
	//		Core::ScaleComponent* sc = WGETC<Core::ScaleComponent>(e2);
	//		sc->scale = .1f;
	//
	//		Core::RotationComponent* rc = WGETC<Core::RotationComponent>(e2);
	//	
	//		//rc->rotation[0] = sin(3.14f / 2.0f);
	//		//rc->rotation[1] = sin(3.14f / 2.0f);
	//		rc->rotation[2] = sin(3.14f);
	//		rc->rotation[3] = cos(3.14f / 2.0f);
	//	}
	//}
	// Create lights
	/*
	for (int i = -50; i < 50; i++)
	{
		Core::Entity light = Core::world.m_entityHandler.CreateEntity<Core::LightComponent, Core::WorldPositionComponent, Core::RotationComponent, Core::ScaleComponent>
				(Core::LightComponent(), Core::WorldPositionComponent(), Core::RotationComponent(), Core::ScaleComponent());
	
			Core::LightComponent* lc = WGETC<Core::LightComponent>(light);

			// Create a point light on the constant heap
			GFX::PointLight* pointLight = Core::world.m_constantHeap.NewObject<GFX::PointLight>();
			pointLight->color = glm::vec3((rand()&1000)/1000.0f, (rand()&1000)/1000.0f, (rand()&1000)/1000.0f);
			pointLight->intensity = 0.3f;

			// Set the the light component to new point light 
			lc->LightData = (void*)pointLight;
			lc->type = GFX::LIGHT_TYPES::POINT;
			GFX::SetBitmaskValue(lc->bitmask, GFX::BITMASK::TYPE, GFX::OBJECT_TYPES::LIGHT);
			GFX::SetBitmaskValue(lc->bitmask, GFX::BITMASK::LIGHT_TYPE, lc->type);
	
			Core::WorldPositionComponent* wpc = WGETC<Core::WorldPositionComponent>(light);
			wpc->position[0] = (float)(-100.0f + 200.0f * (rand()&1000)/1000.0f);
			wpc->position[1] = (float)(-100.0f + 200.0f * (rand()&1000)/1000.0f);
	
			Core::ScaleComponent* sc = WGETC<Core::ScaleComponent>(light);
			sc->scale = 10.0f + 10.0f * (rand()&1000)/1000.0f;
	
			Core::RotationComponent* rc = WGETC<Core::RotationComponent>(light);
	}
	*/

	//CreateRioter(&rioters, meshID, -6.0f, -3.0f, 0.0f);
	//CreateRioter(&rioters, meshID, 0.0f, -3.0f, 0.0f);
	//CreateRioter(&rioters, meshID, 6.0f, -3.0f, 0.0f);
	//for( int i = -100; i < 100; i++ )
	for( int i = -5; i < 5; i++ )
		CreateRioter(&rioters, meshID, materialID,  i * 16.0f, 1.0f, 0.0f);
	//for( int i = -200; i < 200; i++ )
		//CreateRioter(&rioters, meshID, i * 16.0f, 1.0f, 0.0f);

	//CreateRioter(&rioters, meshID, 16.0f, 0.0f, 0.0f);
	//CreateRioter(&rioters, meshID, -16.0f, 0.0f, 0.0f);

	/*
	rioters.push_back(Core::world.m_entityHandler.CreateEntity<Core::GraphicsComponent, 
		Core::WorldPositionComponent, Core::RotationComponent, Core::ScaleComponent, Core::UnitTypeComponent,
		Core::MovementComponent, Core::AttributeRioterComponent, Core::BoundingVolumeComponent>
		(Core::GraphicsComponent(), 
		 Core::WorldPositionComponent(-16.0f, 0.0f, 0.0f),
		 Core::RotationComponent(),
		 Core::ScaleComponent(0.5f),
		 Core::UnitTypeComponent(),
		 //Core::MovementComponent(0.0f, 0.0f, 1.0f, 2.0f, 6.0f),
		 Core::MovementComponent( 1.0f, 0.0f, 0.0f, 0.0f, 5.0f),
		 Core::AttributeRioterComponent(),
		 Core::BoundingVolumeComponent( Core::BoundingSphere( 3.0f, 0.0f, 0.0f, 0.0f ), Core::BoundingVolumeCollisionModel::StaticResolution ) ));
	*/

	std::cout << GFX::GetScreenWidth() << " " << GFX::GetScreenHeight() << " ";

	//inputline.resize(1);

	long long lastFrameTime = Core::Timer().GetTotal();
	long long thisFrame = Core::Timer().GetTotal();
	while (!glfwWindowShouldClose(window))
	{
		// calc delta time
		thisFrame = Core::Timer().GetTotal();
		double delta = (thisFrame - lastFrameTime) / 1000.0;
		lastFrameTime = thisFrame;

		
		Core::Console().Update();

		//gCamera->CalculateViewMatrix();
		Core::gameCamera->LookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		GFX::SetViewMatrix(Core::gameCamera->GetViewMatrix());
		Core::gameCamera->CalculateProjectionMatrix(GFX::GetScreenWidth(), GFX::GetScreenHeight());
		GFX::SetProjectionMatrix(Core::gameCamera->GetProjectionMatrix());

		TestRendering();

	    //TODO: Timing hook
        SystemTimeRender();
		GFX::Render();

        Core::world.m_contentManager.CallFinishers();
		Core::world.m_systemHandler.Update( delta );
        Core::world.m_luaState.Update( delta );

		GFX::Debug::DisplayFBO(Core::world.m_config.GetInt( "showFramebuffers", -1 ));
		//Core::GetInput().ResetInput();
		glfwSwapBuffers(window);
		Core::GetInputManager().PollEvents();
        Core::GetInputManager().CallListeners();

		//TODO: Timing hook
		Core::world.m_frameHeap.Rewind();
    }

	Core::world.m_constantHeap.Rewind();

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
    if( gtestReturn != 0 )
	{
#ifdef _WIN32 
	std::cin.get();
#endif
        return gtestReturn;
	}
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
