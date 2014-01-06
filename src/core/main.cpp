#include "main.hpp"
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

#include "GLFWInput.hpp"
#include <World.hpp>

#include "Console/Console.hpp"
#include "BGnomeImporter.hpp"

#include "Console/CLOP.hpp"
#include <sstream>

#include <iomanip>

#include <ContentManagement/ContentManager.hpp>
#include <logger/Logger.hpp>
#include <logger/Handlers.hpp>

#include <Lua/LuaState.hpp>

#include <gfx/BitmaskDefinitions.hpp>

static bool killProgram = false;

static GFX::FontData* fontData;

#include <DebugRendering.hpp>
#include <DebugCreators.hpp>
#include <CLOPLoggerCallback.hpp>

// Just an example of a clop function
// This function gets registred in Init with clop::Register("exit", ClopCloseWindow);
// And the command is sent to the command line by pressing 'E' (as seen in run()) with Core::Console().SetInputLine("exit");
void ClopCloseWindow(clop::ArgList args)
{
	killProgram = true;
}

int initScreenHeight;
int initScreenWidth;
GLFWwindow* init( int argc, char** argv )
{
	GLFWwindow* window;


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

    RegisterCLOPLogger();

    return window;
}


void CreateRioter(std::vector<Core::Entity>* rioterList, int meshID, unsigned int materialID, float posX, float posY, float posZ)
{
	int index = rioterList->size(); // Size before add will be the index of the added entity.
	double pi = 3.141529;
	double angle = 0.0; // pi * 0.25;

	/*
	Core::BoundingVolumeCollisionModel aa = Core::BoundingVolumeCollisionModel::DynamicResolution;

	glm::vec3 direction( 0.0f, 0.0f, 0.0f );
	if( posX )
		//direction = glm::normalize( glm::vec3( 0.0f, 0.0f, posY ) );
		direction = glm::normalize( glm::vec3( posX, 0.0f, posZ ) );
		//direction = glm::normalize( glm::vec3( posX, 0.0f, posY ) );	
	else 
	{
		aa = Core::BoundingVolumeCollisionModel::StaticResolution;
		posZ += 0.7f;
	}
	*/
	rioterList->push_back(Core::world.m_entityHandler.CreateEntity<Core::GraphicsComponent, 
		Core::WorldPositionComponent, Core::RotationComponent, Core::ScaleComponent, Core::UnitTypeComponent,
		Core::MovementComponent, Core::AttributeComponent, Core::BoundingVolumeComponent>
		(Core::GraphicsComponent(), 
		 Core::WorldPositionComponent(posX, posY, posZ),
		 Core::RotationComponent(),
		 Core::ScaleComponent(1.0f),
		 Core::UnitTypeComponent(Core::UnitType::Rioter),
		 Core::MovementComponent(0.0f, 0.0f, 0.0f, 2.0f, 6.0f),
		 //Core::MovementComponent( -direction.x, 0, -direction.z, 21.1f, 5.0f),
		 Core::AttributeComponent(),
		 //Core::BoundingVolumeComponent( Core::BoundingSphere( 3.0f, 0.0f, 0.0f, 0.0f ), aa )
		 Core::BoundingVolumeComponent(Core::BoundingSphere(1.0f, 0.0f, 0.0f, 0.0f), 
		 Core::BoundingVolumeCollisionModel::DynamicResolution)));

	Core::GraphicsComponent* gc = WGETC <Core::GraphicsComponent>(rioterList->at(index));
	GFX::SetBitmaskValue(gc->bitmask, GFX::BITMASK::MESH_ID, meshID);
    GFX::SetBitmaskValue(gc->bitmask, GFX::BITMASK::MATERIAL_ID, materialID);
	GFX::SetBitmaskValue(gc->bitmask, GFX::BITMASK::TYPE, GFX::OBJECT_TYPES::OPAQUE_GEOMETRY);
}

static void ControlCamera(double delta)
{
	glm::vec2 rotation = glm::vec2(0.0f);
	glm::vec3 directions = glm::vec3(0.0f);

	if (Core::GetInput().IsMouseButtonPressed(GLFW_MOUSE_BUTTON_1))
	{

		float x = -((float)Core::GetInput().GetXPosDiff() / 1280.0f);
		float y = -((float)Core::GetInput().GetYPosDiff() / 720.0f);

		rotation.x = x * 5;
		rotation.y = y * 5;
	}

	if (Core::GetInput().IsKeyPressed(GLFW_KEY_D))
		directions.y = -1;
	if (Core::GetInput().IsKeyPressed(GLFW_KEY_A))
		directions.y = 1;
	if (Core::GetInput().IsKeyPressed(GLFW_KEY_W))
		directions.x = 1;
	if (Core::GetInput().IsKeyPressed(GLFW_KEY_S))
		directions.x = -1;
	if (Core::GetInput().IsKeyPressed(GLFW_KEY_E))
		directions.z = 1;
	if (Core::GetInput().IsKeyPressed(GLFW_KEY_Q))
		directions.z = -1;

	Core::gameCamera->UpdateView(directions, rotation, delta);
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
	Core::gameCamera->SetPosition(glm::vec3(80.0f, 70.0f, 50.0f));
	
    Core::ContentManager CM;
	
	GFX::SetProjectionMatrix(Core::gameCamera->GetProjectionMatrix());

	std::vector<Core::Entity> rioters;

	Core::GetInput().Initialize(window);

    unsigned int meshID; 
    unsigned int materialID;

    Core::world.m_contentManager.Load<Core::GnomeLoader>("assets/cube.bgnome", [&meshID](Core::BaseAssetLoader* baseLoader, Core::AssetHandle handle)
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

	//CreateRioter(&rioters, meshID, materialID, -6.0f, 0.5f, 0.0f);
	//CreateRioter(&rioters, meshID, materialID, 0.0f, 0.5f, 0.0f);
	//CreateRioter(&rioters, meshID, materialID, 6.0f, 0.5f, 0.0f);

	/*for( int i = -5; i < 5; i++ )
		CreateRioter(&rioters, meshID, materialID,  i * 16.0f, 1.0f, 0.0f);*/

    /*
	for (float i = -11.0f; i < 11.0f; ++i)
	{
		for (float j = -11.0f; j < 11.0f; ++j)
		{
			CreateRioter(&rioters, meshID, materialID, i * 2.0f + 0.5f, 0.5f, j * 2.0f + 0.5f);
		}
	}
    */


	LOG_INFO << GFX::GetScreenWidth() << " " << GFX::GetScreenHeight() << " " << std::endl;

	//inputline.resize(1);

	long long lastFrameTime = Core::Timer().GetTotal();
	long long thisFrame = Core::Timer().GetTotal();
	while (!glfwWindowShouldClose(window) && killProgram == false)
	{
		// calc delta time
		thisFrame = Core::Timer().GetTotal();
		double delta = (thisFrame - lastFrameTime) / 1000.0;
		lastFrameTime = thisFrame;

		Core::GetInput().UpdateInput();
		
		Core::Console().Update();

		//gCamera->CalculateViewMatrix();
		//Core::gameCamera->LookAt(glm::vec3(9001.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

		ControlCamera(delta);

		GFX::SetViewMatrix(Core::gameCamera->GetViewMatrix());

		Core::gameCamera->CalculateProjectionMatrix(GFX::GetScreenWidth(), GFX::GetScreenHeight());
		GFX::SetProjectionMatrix(Core::gameCamera->GetProjectionMatrix());

		//TestRendering();

	    //TODO: Timing hook
        SystemTimeRender();
		GFX::Render(delta);

        Core::world.m_contentManager.CallFinishers();
		Core::world.m_systemHandler.Update( delta );
        Core::world.m_luaState.Update( delta );

		GFX::Debug::DisplayFBO(Core::world.m_config.GetInt( "showFramebuffers", -1 ));
		Core::GetInput().ResetInput();
		glfwSwapBuffers(window);
		glfwPollEvents();

		//TODO: Timing hook
		Core::world.m_frameHeap.Rewind();
    }

    //Deregister clop before the stack starts unwinding!
    DeregisterCLOPLogger();

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
