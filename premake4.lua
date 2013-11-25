translateOS = { windows="win32", linux="linux"}

solution "RiotGame"

    configurations {"Debug", "Release"}
        flags{ "Unicode", "ExtraWarnings", "NoPCH" } 
        libdirs { translateOS[os.get()] .. "/lib" }
        includedirs { translateOS[os.get()] .. "/deps", "deps", "include"}
    
    local location_path = "build"

    if os.is( "linux" ) then
        buildoptions { "-std=c++11" }
        location_path = location_path .. "/make/"
    elseif(os.is("windows")) then
        location_path = location_path .."/win32/" .. _ACTION
        location ( location_path )
        location_path = location_path .. "/projects"
    end
    
    configuration "Debug"
        defines { "DEBUG" }
        flags { "Symbols" }
        
        targetdir ( "bin/" .. translateOS[os.get()] .. "/debug" )

    configuration "Release"
        defines { "NDEBUG", "RELEASE" }
        flags { "Optimize", "FloatFast" }
        targetdir ( "bin/" .. translateOS[os.get()] .. "/release" )   

    project "core"
        debugdir ""
        location ( location_path )
        language "C++"
        kind "ConsoleApp"
        files { "src/core/**.hpp", "src/core/**.h", "src/core/**.cpp" }
        includedirs { "src/core", "src/include" }
        --links { "gfx", "sfx", "contentmanager" }     
        links { "glfw3", "glew32", "glfw3dll", "opengl32" }
        targetname ( "RiotGame" )

--    project "gfx"
--        location ( location_path )
--        language "C++"
--        kind "SharedLib"
--        files { "src/gfx/**.hpp", "src/gfx/**.h", "src/gfx/**.cpp" }
--        includedirs { "src/gfx" }       
--
--        configurations "Release"
--            defines { "DLL_EXPORT" }
--
--    project "sfx"
--        location ( location_path )
--        language "C++"
--        kind "SharedLib"        
--        files { "src/sfx/**.hpp", "src/sfx/**.h", "src/sfx/**.cpp" }
--        includedirs { "src/sfx" }        
--
--        configurations "Release"
--            defines { "DLL_EXPORT" }
--
--    project "contentmanager"
--        location ( location_path )
--        language "C++"
--        kind "SharedLib"        
--        files { "src/contentmanager/**.hpp", "src/contentmanager/**.h", "src/contentmanager/**.cpp" }
--        includedirs { "src/contentmanager" }

        configurations "Release"
            defines { "DLL_EXPORT" }
