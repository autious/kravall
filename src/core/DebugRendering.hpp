
#include <algorithm>
#include <World.hpp>

static void LuaInfoRender()
{
    int screenWidth = GFX::GetScreenWidth(); 
    int screenHeight = GFX::GetScreenHeight();

    std::stringstream ss;

    ss << "Memory: " << Core::world.m_luaState.GetMemoryUse() << "Kb";
    GFX::RenderText( localFontData, glm::vec2( 615, screenHeight - 55 ), 1.0f, Colors::White, "Lua"  );
    GFX::RenderText( localFontData, glm::vec2( 615, screenHeight - 40 ), 1.0f, Colors::White, ss.str().c_str()  );
    std::stringstream ss2;
    ss2 << "Update: " << std::fixed << std::setw( 7 ) << std::setprecision(4) << std::setfill( '0' ) << Core::world.m_luaState.GetUpdateTiming().count() / 1000.0f << "ms";
    GFX::RenderText( localFontData, glm::vec2( 615, screenHeight - 25 ), 1.0f, Colors::White, ss2.str().c_str()  );

    std::stringstream ss3;
    ss3 << "FPS: ";

    lua_State * L = Core::world.m_luaState.GetState();
    lua_getglobal(L, "core");
    lua_getfield(L, -1, "framesPerSecond");

    if(!lua_isnil(L, -1))
    {
        ss3 << std::fixed << std::setw(8) << std::setprecision(2) << static_cast<double>(lua_tonumber(L, -1));
    }

    GFX::RenderText( localFontData, glm::vec2(615, screenHeight - 10), 1.0f, Colors::White, ss3.str().c_str() );

    GFX::Debug::DrawRectangle(glm::vec2( 610, screenHeight - 70 ),
            glm::vec2(175, 65), true, glm::vec4( 0.5f,0.5f,0.5f,0.5f) );
    
}

static void EntityHandlerMemoryRender()
{
    Core::EntityHandler::EntityDataUseList edul = Core::world.m_entityHandler.GetDataUse();
    bool renderAllComponents = Core::world.m_config.GetString( "entityMemoryOutputLevel", "short" ) != "short";
    bool renderPartial = Core::world.m_config.GetString( "entityMemoryOutputLevel", "short" ) == "partial";

    std::vector<std::string> showComponents = Core::world.m_config.GetVectorString( "entityMemoryOutputComponents" );
       
    int totalComponentCount = 0;
    int totalComponentAllocation = 0;
    float totalMemoryUse = 0;
    float totalMemoryAllocation = 0;

    // Render all by default
    int elementCount = edul.size() + 1;

    if( renderPartial )
    {
        //Add Title and Total
        elementCount = showComponents.size() + 2;
    }

    if( renderAllComponents == false )
    {
        //Only title and entity
        elementCount = 2; 
    }

    std::stringstream ss;


    int count = 0;

    for( unsigned int i = 0; i < edul.size(); i++ )
    {
        bool renderCurrent = i == 0 
            || ((renderAllComponents) 
            && ( renderPartial == false 
                || std::find( showComponents.begin(), showComponents.end(), std::get<0>(edul[i] ) ) 
                    != showComponents.end() ) );
        ss.str("");
        ss.clear();
        ss <<  std::get<0>(edul[i]);

        if( renderCurrent )
            GFX::RenderText(localFontData, glm::vec2(10, GFX::GetScreenHeight()+12-2*20*(elementCount)+20*2*count), 1.0f, Colors::White, ss.str().c_str());

        ss.str("");
        ss.clear();
        ss << std::get<1>(edul[i]) << " ";
        ss << std::get<2>(edul[i]) << " ";
        float memUse = std::get<3>(edul[i])*0.001f;
        ss << memUse << "Kb ";
        float memAlloc = std::get<4>(edul[i])*0.001f;
        ss << memAlloc << "Kb ";

        if( renderCurrent )
            GFX::RenderText(localFontData, glm::vec2(10, GFX::GetScreenHeight()+12-2*20*(elementCount)+20*count*2+20*1), 1.0f, Colors::White, ss.str().c_str());

        if( i > 0 )
        {
            totalComponentCount += std::get<1>(edul[i]);
            totalComponentAllocation += std::get<2>(edul[i]);
        }
    
        totalMemoryUse += memUse;
        totalMemoryAllocation += memAlloc;

        if( renderCurrent )
            count++;
    }

    ss.str("");
    ss.clear();
    ss << totalComponentCount << " ";
    ss << totalComponentAllocation << " ";
    ss << totalMemoryUse << "Kb ";
    ss << totalMemoryAllocation << "Kb ";

    GFX::RenderText(localFontData, glm::vec2(10, GFX::GetScreenHeight()+12-2*20*(elementCount)+20*elementCount*2-40), 1.0f, Colors::White, "Total Components" );
    GFX::RenderText(localFontData, glm::vec2(10, GFX::GetScreenHeight()+12-2*20*(elementCount)+20*elementCount*2-20), 1.0f, Colors::White, ss.str().c_str() );

    GFX::Debug::DrawRectangle(glm::vec2(5,GFX::GetScreenHeight()-5-2*20*(elementCount) ), 
        glm::vec2(295, 2*20*(elementCount)), true, glm::vec4( 0.5f,0.5f,0.5f,0.5f) );
}

static void SystemTimeRender()
{
    bool showSystems = Core::world.m_config.GetBool( "showSystems", false ) ;
	GFX::Debug::DisplaySystemInfo( showSystems );
    if( showSystems )
    {
        EntityHandlerMemoryRender();
        LuaInfoRender();

        std::vector<std::pair<const char *,std::chrono::microseconds>> times = Core::world.m_systemHandler.GetFrameTime();

        for( int i = 0; i < (int)times.size(); i++ )
        {
            std::stringstream ss;
            
            ss << times[i].first << ": " << std::fixed << std::setw( 7 ) << std::setprecision(4) << std::setfill( '0' ) << times[i].second.count() / 1000.0f << "ms";
	        GFX::RenderText(localFontData, glm::vec2(310, GFX::GetScreenHeight()+12-20*times.size()+20*i), 1.0f, Colors::White, ss.str().c_str());
        }

	    GFX::Debug::DrawRectangle(glm::vec2(305,GFX::GetScreenHeight()-5-20*times.size() ), 
            glm::vec2(300, 20*times.size()), true, glm::vec4( 0.5f,0.5f,0.5f,0.5f) );
    }
}

