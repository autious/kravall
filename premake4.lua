translateOS = { windows="win32", linux="linux"}

solution "RiotGame"
    configurations {"DebugTest","Debug", "Release", "ReleaseTest", "PureReleaseTest", "PureDebugTest"}
        flags{ "Unicode", "NoPCH" } 
        libdirs { translateOS[os.get()] .. "/lib" }
        includedirs { translateOS[os.get()] .. "/deps", "deps", "include"}
    
    local location_path = "build"

    if os.is( "linux" ) then
        buildoptions { "-std=c++11 -Wall"} -- -Wno-unused-parameter" }
        location_path = "" 
    elseif(os.is("windows")) then
        location_path = location_path .."/win32/" .. _ACTION
        location ( location_path )
        location_path = location_path .. "/projects"
    end
    
    configuration { "Debug or DebugTest or PureDebugTest" }
        defines { "DEBUG" }
        flags { "Symbols" }
        
    configuration { "Release or ReleaseTest or PureReleaseTest" }
        defines { "NDEBUG", "RELEASE" }
        flags { "Optimize", "FloatFast" }

    configuration { "*Test" }
        defines { "RUN_GTEST" }

    configuration { "Pure*Test" }
        defines { "SKIP_RUN" }

    configuration { "Debug" }
        targetdir ( "bin/" .. translateOS[os.get()] .. "/debug" )

    configuration { "Release" } 
        targetdir ( "bin/" .. translateOS[os.get()] .. "/release" )   

    configuration { "DebugTest" }
        targetdir ( "bin/" .. translateOS[os.get()] .. "/debugtest" )

    configuration { "ReleaseTest" } 
        targetdir ( "bin/" .. translateOS[os.get()] .. "/releasetest" )   
    
    configuration { "PureReleaseTest" } 
        targetdir ( "bin/" .. translateOS[os.get()] .. "/purereleasetest" )   

    configuration { "PureDebugTest" } 
        targetdir ( "bin/" .. translateOS[os.get()] .. "/puredebugtest" )   

    project "core"
        targetname "RiotGame" 
        debugdir "" -- fungerade inte för utskrift av textfiler, lämnar detta så det syns utifall något faller iom. det / John
        location ( location_path )
        language "C++"
        kind "ConsoleApp"
        files { "gtest/core/**.cpp", "src/core/**.hpp", "src/core/**.h", "src/core/**.cpp", "include/gfx/**.hpp" ,"include/utility/**.hpp" }
        includedirs { "src/core", "include", "include/gfx" }
        links { "glfw3", "gfx", "logger" }
        configuration{ "*Test" }
            links { "gtest" }
        configuration{ "windows" }
            links { "glew32", "glfw3dll", "opengl32" }
        configuration{ "linux" }
            links { "GLEW", "GL" }

    project "gfx"
        location ( location_path )
        language "C++"
        kind "SharedLib"
        files { "gtest/gfx/**.cpp", "src/gfx/**.hpp", "src/gfx/**.h", "src/gfx/**.cpp", "include/gfx/**.hpp" ,"include/utility/**.hpp", "shaders/**.vertex", "shaders/**.geometry", "shaders/**.fragment", "shaders/**.compute" }

		includedirs { "src/gfx", "include/gfx", "include/utility", "shaders", "include", "deps" }       
        
        if os.is( "linux" ) then -- This is in accordance to pkg-config --cflags freetype2
            includedirs { "/usr/include/freetype2" }
        end
            
		defines { "GFX_DLL_EXPORT" }
		
        links { "glfw3", "freetype" }
        configuration{ "*Test" }
            links { "gtest" }
        configuration{ "windows" }
            links { "glew32", "glfw3dll", "opengl32", "logger" }
        configuration{ "linux" }
            links { "GLEW", "GL" }
            
			
	project "logger"
      location ( location_path )
      language "C++"
		kind "SharedLib"
        files { "gtest/logger/**.cpp", "src/logger/**.hpp", "src/logger/**.h", "src/logger/**.cpp", "include/logger/**.hpp" }
		includedirs { "include/logger", "src/logger" }       
		defines { "LOGGER_DLL_EXPORT" }
        configuration{ "*Test" }
            links { "gtest" }
 
--    project "sfx"
--        location ( location_path )
--        language "C++"
--        kind "SharedLib"        
--        files { "gtest/sfx/**.cpp", "src/sfx/**.hpp", "include/sfx/**.h", "src/sfx/**.cpp" }
--        includedirs { "src/sfx" }        
--        defines { "SFX_DLL_EXPORT" }
--
--        configuration{ "*Test" }
--            links { "gtest" }
--
