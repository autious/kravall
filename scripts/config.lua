core = core or {}

core.config = {
showSplash = false,
vsync = 0,
windowResizable = true,
initScreenWidth = 1280,
initScreenHeight = 720,
showSystems = false,
showFramebuffers = 1,
initCameraFieldOfView = 45.0,
initCameraNearClipDistance = 1.0,
initCameraFarClipDistance = 1000.0
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

function showFBO(which)
    core.config.showFramebuffers = which
end

function help()
    print( "The following functions exist:" )
    print( "showSys()" )
    print( "showFBO(which)" )
    print( "printAlexStatus()" )
end
