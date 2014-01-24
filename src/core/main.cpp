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

#include <utility/Colors.hpp>
#include "Camera/Camera.hpp"
#include <ComponentFramework/SystemHandlerTemplate.hpp>
#include <ComponentFramework/EntityHandlerTemplate.hpp>
#include <ComponentFramework/SystemTypes.hpp>

#include <utility/Colors.hpp>

#include <World.hpp>

#include "Console/Console.hpp"
#include "Console/CLOP.hpp"
#include <sstream>

#include <iomanip>

#include <ContentManagement/ContentManager.hpp>
#include <logger/Logger.hpp>
#include <logger/Handlers.hpp>

#include <Lua/LuaState.hpp>
#include <Input/InputManager.hpp>

#include <gfx/BitmaskDefinitions.hpp>

static bool killProgram = false;

#include <DebugRendering.hpp>
#include <DebugCreators.hpp>
#include <CLOPLoggerCallback.hpp>

GLFWwindow *mainWindow = nullptr;

GFX::FontData* localFontData;

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

    Core::world.m_luaState.OpenLibs();
    bool worked = Core::world.m_luaState.Execute( "scripts/config.lua" ) && Core::world.m_luaState.Execute( "scripts/main.lua" );
    assert( worked );

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
                localFontData = static_cast<GFX::FontData*>(handle);
                Core::Console().Init(localFontData);  
                GFX::Debug::SetStatisticsFont(localFontData);
            });

    RegisterCLOPLogger();
	return window;
}

void run( GLFWwindow * window )
{
    LOG_INFO << "Starting program" << std::endl;

    Core::ContentManager CM;
	
	GFX::SetProjectionMatrix(Core::gameCamera.GetProjectionMatrix());

	std::vector<Core::Entity> rioters;

    Core::GetInputManager().Init( window );

    unsigned int meshID; 
    unsigned int copMaterialID;
	unsigned int rioterMaterialID;

    Core::world.m_contentManager.Load<Core::GnomeLoader>("assets/model/animated/police/cop/police-female_00.bgnome", [&meshID](Core::BaseAssetLoader* baseLoader, Core::AssetHandle handle)
            {
                Core::GnomeLoader* gnomeLoader = dynamic_cast<Core::GnomeLoader*>(baseLoader);
                const Core::ModelData* data = gnomeLoader->getData(handle);
				meshID = data->meshID;
            });

	Core::world.m_contentManager.Load<Core::MaterialLoader>("assets/material/cop.material", [&copMaterialID](Core::BaseAssetLoader* baseLoader, Core::AssetHandle handle)
            {
                Core::MaterialData* data = static_cast<Core::MaterialData*>(handle);
				copMaterialID = static_cast<unsigned int>(data->materialId);
            }, false);

	Core::world.m_contentManager.Load<Core::MaterialLoader>("assets/material/rioter.material", [&rioterMaterialID](Core::BaseAssetLoader* baseLoader, Core::AssetHandle handle)
			{ 
				Core::MaterialData* data = static_cast<Core::MaterialData*>(handle);
				rioterMaterialID = static_cast<unsigned int>(data->materialId);
			}, false);
   
	GFX::RenderSplash(Core::world.m_config.GetBool( "showSplash", false ));	

	clop::Register( "showMesh", Core::ToggleDrawOfNavigationMesh );

	LOG_INFO << GFX::GetScreenWidth() << " " << GFX::GetScreenHeight() << " " << std::endl;

    Core::world.m_luaState.Init();

	//inputline.resize(1);
	Core::HighresTimer timer;
	long long lastFrameTime = timer.GetTotal();
	long long thisFrame = timer.GetTotal();
    double timeAccumulator = 0.0;
    const double MAXIMUM_TIME_STEP = 1.0/15.0;
    const double MINIMUM_TIME_STEP = 1.0/120.0;

    const int FPS_COUNTERS_SIZE = 20;
    double fpsCounters[FPS_COUNTERS_SIZE];
    int fpsCounterIndex = 0;

	while (!glfwWindowShouldClose(window) && killProgram == false)
	{
		// calc delta time
		thisFrame = timer.GetTotal();
		double delta = (thisFrame - lastFrameTime) / 1000.0;
		lastFrameTime = thisFrame;

        double minimumTimeStep = Core::world.m_config.GetDouble("timeStepMinimum", MINIMUM_TIME_STEP);
        double maximumTimeStep = Core::world.m_config.GetDouble("timeStepMaximum", MAXIMUM_TIME_STEP);

        timeAccumulator += delta;

        if(timeAccumulator > maximumTimeStep)
        {
            timeAccumulator = maximumTimeStep;
        }

        if(timeAccumulator > minimumTimeStep)
        {
            // NOCOMMIT
            //delta = 0.01f;
            
            Core::Console().Update();

            GFX::SetOverlayViewMatrix( Core::overlayCamera.GetViewMatrix() );
            GFX::SetOverlayProjectionMatrix( Core::overlayCamera.GetProjectionMatrix() );
            GFX::SetViewMatrix(Core::gameCamera.GetViewMatrix());
            GFX::SetProjectionMatrix(Core::gameCamera.GetProjectionMatrix());

            //TestRendering();

            //TODO: Timing hook
            SystemTimeRender();
            GFX::Render(timeAccumulator);

            Core::world.m_contentManager.CallFinishers();
            Core::world.m_systemHandler.Update(static_cast<float>(timeAccumulator));
            Core::world.m_luaState.Update(static_cast<float>(timeAccumulator));

            Core::DrawToggledNavigationMesh();

            GFX::Debug::DisplayFBO(Core::world.m_config.GetInt( "showFramebuffers", -1 ));
            glfwSwapBuffers(window);
            Core::GetInputManager().PollEvents();
            Core::GetInputManager().CallListeners();

            //TODO: Timing hook
            Core::world.m_frameHeap.Rewind();

            fpsCounters[fpsCounterIndex++] = 1.0 / timeAccumulator;
            if(fpsCounterIndex == FPS_COUNTERS_SIZE)
            {
                double averageFps = 0.0;
                for(int i=0; i < FPS_COUNTERS_SIZE; ++i)
                {
                    averageFps += fpsCounters[i];
                }

                averageFps /= FPS_COUNTERS_SIZE;
                LOG_DEBUG << "FPS: " << static_cast<int>(averageFps) << std::endl;
                fpsCounterIndex = 0;

                lua_State* L = Core::world.m_luaState.GetState();
                lua_getglobal(L, "core");
                    lua_pushstring(L, "framesPerSecond");
                    lua_pushnumber(L, averageFps);
                lua_settable(L, -3);
                lua_pop(L, 1);
            }

            timeAccumulator = 0.0;
        }
    }

    Core::world.m_luaState.Stop();

    //Deregister clop before the stack starts unwinding!
    DeregisterCLOPLogger();

    Core::world.m_luaState.CloseLibs();
	Core::world.m_constantHeap.Rewind();

    Core::GLFWWindowCallbackHandler::Free();
    glfwDestroyWindow( window );

	glfwTerminate();
}


int main(int argc, char** argv)
{
#ifdef RUN_GTEST
    ::testing::InitGoogleTest(&argc, argv);
#endif
#ifndef SKIP_RUN
	mainWindow = init( argc, argv );
	if( mainWindow == nullptr )
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
    run( mainWindow );
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
