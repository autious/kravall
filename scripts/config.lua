core = core or {}

core.config = {
showSplash = false,
vsync = 0,
windowResizable = true,
initScreenWidth = 1280,
initScreenHeight = 720,
showSystems = true,
showFramebuffers = 1,
initCameraFieldOfView = 45.0,
initCameraNearClipDistance = 1.0,
initCameraFarClipDistance = 1000.0,
consoleFont = "assets/Fonts/ConsoleFont.font"
}

function printAlexStatus()
    print( "He is still himself.")
end

function showSys()
    if core.config.showSystems then
        core.config.showSystems = false
    else
        core.config.showSystems = true
    end
end

-- Toggles which fbo to render
-- Fullscreen lit scene (final mode) : -1
-- Fullscreen lit scene + miniature render targets: 0
-- Fullscreen rendertarget n : (1-4)
function showFBO(which)
    core.config.showFramebuffers = which
end

function help()
    print( "The following functions exist:" )
    print( "showSys()" )
    print( "showFBO(which)" )
    print( "printAlexStatus()" )
end

-- Loads a scenario from the scenario folder and returns the assembly.
function doscenario( name )
    return dofile( "scripts/scenarios/" .. name .. ".lua" ) 
end
