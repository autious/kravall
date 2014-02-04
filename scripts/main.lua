local Camera = require "rts_camera" 
local MainMenu = require "gui/MainMenu"
local viewport = require "gui/viewport"
local image = require "factories/image"
local viewport = require "gui/viewport" 
require "console"

local current_scenario = nil
local current_scenario_name = ""

-- Called on each frame
camera = Camera.new()

function core.update( delta )
    if camera ~= nil then
        camera:update( delta )
    end
    camera = nil
    if current_scenario ~= nil then
        current_scenario:update( delta )
    end 
end

-- Called when program starts
function core.init() 
    print( "Program starting in lua" )
    logo = image( 10,10, "assets/material/ui/test.material", false )
    openscenario( "main_menu" )    
end

menuState = nil
function toggleMenu()
    if menuState == nil then
        menuState = MainMenu:new()
    else
        menuState:destroy()
        menuState = nil
    end
end

-- Called when program end
function core.stop()
    print "Program stopping in lua" 
    closescenario()
end

function openscenario( name )
    closescenario()
    current_scenario = dofile( "scripts/scenarios/" .. name .. ".lua" )
    current_scenario_name = name

    if type( current_scenario.load ) ~= "function" then
        --closescenario()
        core.log.warning( "A scenario now must do all entity and content loading in a function named scen:load. IF YOU LOAD IN ENTITIES FROM THE BODY OF THE SCENARIO YOU*RE DOING SOMETHING WRONG. See protoype_area for example." )
    else
        current_scenario:load()
    end

    collectgarbage() --For niceness, always good to do right after loading a scenario as the
                     --assembly files are quite large.
end

function closescenario()
    if current_scenario ~= nil then
        current_scenario:destroy()
        current_scenario = nil
        current_scenario_name = "No Scenario Loaded"
		core.memory.clear_level_memory()
    end

    collectgarbage() --For niceness, always good to do right after loading a scenario as the
                     --assembly files are quite large.
end
