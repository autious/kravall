core.config = {
showSplash = false,
vsync = 0,
windowResizable = true,
initScreenWidth = 1280,
initScreenHeight = 720,
showSystems = false,
entityMemoryOutputLevel = "partial", -- full, short or partial
-- List of components to render debug data for.
entityMemoryOutputComponents = { "WorldPositionComponent", 
                                 "GraphicsComponent", 
                                 "NameComponent",
                                 "AreaComponent" },
showFramebuffers = 0,
initCameraFieldOfView = 45.0,
initCameraNearClipDistance = 5.0,
initCameraFarClipDistance = 2500.0,
consoleFont = "assets/Fonts/ConsoleFont.font",
defaultNrFlowfields = 20,
debugRenderAreas = true,
debugLightVolumes = false
}

function toggleLightVolumes()
    if core.config.debugLightVolumes then
        core.config.debugLightVolumes = false
    else
        core.config.debugLightVolumes = true
    end
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
    print "The following functions exist:" 
    print "showSys()" 
    print "showFBO(which)" 
	print "showMesh()" 
    print "print_env()" 
    print "toggleMenu()"
end

-- Loads a scenario from the scenario folder and returns the assembly.
function doscenario( name )
    print( "Use 'openscenario()' instead:" )
    --return dofile( "scripts/scenarios/" .. name .. ".lua" ) 
end

core.config.current_scenario = nil
core.config.current_scenario_name = "No Scenario Loaded"

function currentscenario()
    print( core.config.current_scenario_name )
end

function openscenario( name )
    closescenario()
    core.config.current_scenario = dofile( "scripts/scenarios/" .. name .. ".lua" )
    core.config.current_scenario_name = name

    collectgarbage() --For niceness, always good to do right after loading a scenario as the
                     --assembly files are quite large.
end

function closescenario()
    if core.config.current_scenario ~= nil then
        print( "DESTROY" )
        core.config.current_scenario:destroy()
        core.config.current_scenario = nil
        core.config.current_scenario_name = "No Scenario Loaded"
		core.memory.clear_level_memory()
    end

    collectgarbage() --For niceness, always good to do right after loading a scenario as the
                     --assembly files are quite large.
end
    
-- Prints all keys and sub-keys for given table, useful if you want to see what core looks like.
function print_env( table, prefix)
    if prefix == nil then
        prefix = ""
    end

    if table == nil then
       table = core
       prefix = "core"
    end  

    for k,v in  pairs( table ) do
        if type( v ) == "function" then
            print( prefix .. "." .. k .. "()" )
        elseif type( v ) == "table" then
            print( prefix .. "." .. k .. " [table]")
                print_env( v, prefix .. "." .. k )
        else
            print( prefix .. "." .. k )
        end
    end

end

function print_loaded()
    for k,v in pairs( package.loaded ) do
        print( k )
    end
end
