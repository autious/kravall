core = core or {}

core.config = {
showSplash = false,
vsync = 0,
windowResizable = true,
initScreenWidth = 1280,
initScreenHeight = 720,
showSystems = false,
showFramebuffers = false
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

function showFBO()
    if core.config.showFramebuffers then
        core.config.showFramebuffers = false
    else
        core.config.showFramebuffers = true
    end
end

function help()
    print( "The following functions exist:" )
    print( "showSys()" )
    print( "showFBO()" )
    print( "printAlexStatus()" )
end
