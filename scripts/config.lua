core = core or {}

core.config = {
showSplash = true,
vsync = 0,
windowResizable = true,
initScreenWidth = 800,
initScreenHeight = 600,
showSystems = false
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

function help()
    print( "The following functions exist:" )
    print( "showSys()" )
    print( "printAlexStatus()" )
end
