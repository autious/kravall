function dist_name()
    if os.is( "linux" ) then
        if os.is64bit() then 
            return "linux_amd64" 
        else
            error( "Build script does not support 32-bit linux machines" ) 
        end
    elseif os.is( "windows" ) then
        return "win32"
    end
end

solution "RiotGame"
    configurations {"DebugTest","Debug", "Release", "ReleaseTest", "PureReleaseTest", "PureDebugTest"}
        flags{ "Unicode", "NoPCH" } 
        libdirs { dist_name() .. "/lib" }
        includedirs { dist_name() .. "/deps", "deps", "include"}
    
        local location_path = "build"

        -- GMAKE people are lazy, populate root folder with build files
        -- Also give extra flags to the GCC compiler
        if _ACTION and _ACTION == "gmake" then 
            buildoptions { "-std=c++11 -Wall -Werror=return-type"}-- -Wconversion"} -- -Wno-unused-parameter" }
            --Add the lib folder as an rpath.
            linkoptions { "-Wl,-rpath=" .. dist_name() .. "/lib" }
            location_path = "" 
        elseif _ACTION then
            --Disable warnings about using completely standard (and normal) c functions.
            --In visual studio
	        defines { "_CRT_SECURE_NO_WARNINGS", "NOMINMAX" }

            location_path = location_path .."/" .. dist_name() .. "/" .. _ACTION
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
        targetdir ( "bin/" .. dist_name() .. "/debug" )

    configuration { "Release" } 
        targetdir ( "bin/" .. dist_name() .. "/release" )   

    configuration { "DebugTest" }
        targetdir ( "bin/" .. dist_name() .. "/debugtest" )

    configuration { "ReleaseTest" } 
        targetdir ( "bin/" .. dist_name() .. "/releasetest" )   
    
    configuration { "PureReleaseTest" } 
        targetdir ( "bin/" .. dist_name() .. "/purereleasetest" )   

    configuration { "PureDebugTest" } 
        targetdir ( "bin/" .. dist_name() .. "/puredebugtest" )   

    project "core"
        targetname "RiotGame" 
        debugdir ""
        location ( location_path )
        language "C++"
        kind "ConsoleApp"
        files { "gtest/core/**.cpp", "src/core/**.hpp", "src/core/**.h", "src/core/**.cpp", "include/gfx/**.hpp" ,"include/utility/**.hpp" }
        includedirs { "src/core", "include", "include/gfx" }

        if os.is( "linux" ) then
            includedirs { "/usr/include/lua5.1" }
            includedirs { "/usr/include/freetype2" }
        end

        --Idiotic non-matching naming convenstions.
        if( os.is( "linux" ) ) then
           links { "luajit-5.1" } 
        else
            links { "lua51" }
        end

        links { "glfw3", "gfx", "logger", "freetype" }
        configuration{ "*Test" }
            links { "gtest" }
        configuration{ "windows" }
            links { "glew32", "glfw3dll", "opengl32" }
        configuration{ "linux" }
            links { "GLEW", "GL" }
        configuration{ "Debug*" }
            defines{ "LUA_USE_APICHECK" }
        configuration{ "PureDebugTest" }
            defines{ "LUA_USE_APICHECK" }
    

    project "gfx"
        location ( location_path )
        language "C++"
        kind "SharedLib"
        files { "gtest/gfx/**.cpp", "src/gfx/**.hpp", "src/gfx/**.h", "src/gfx/**.cpp", "include/gfx/**.hpp" ,"include/utility/**.hpp", "shaders/**.vertex", "shaders/**.geometry", "shaders/**.fragment", "shaders/**.compute", "shaders/**.glsl" }

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

        if os.is( "linux" ) then
            defines { "LOGGER_LINUX_CONSOLE" } --Activate color output for linux console
        end

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
