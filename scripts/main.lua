--require "strict"
local Camera = require "rts_camera" 
local MainMenu = require "gui/MainMenu"
local viewport = require "gui/viewport"
local image = require "factories/image"
local viewport = require "gui/viewport" 
local toolTipHandler = require "gui/tooltip/ToolTipHandler"
require "console"

local current_scenario = nil
local current_scenario_name = ""

local input = require "input"
local createTT = require "gui/tooltip/ToolTip"

input.registerOnKey( function( key, scancode, action )
    if key == core.input.keyboard.key.F5 
        and action == core.input.action.Press 
        and current_scenario_name then
            openscenario( current_scenario_name )
    end
end)

function reopenScenario()
    openscenario( current_scenario_name )
end

--For initial camera placment.
camera = Camera.new()

-- Called on each frame
function core.update( delta )
    if camera ~= nil then
        camera:update( delta )
    end
    camera = nil
    if current_scenario ~= nil then
        current_scenario:update( delta )
    end
	
	if core.input.keyboard.isKeyDown( core.input.keyboard.key.K ) then
		toolTipHandler:register( createTT:new({
										handler=toolTipHandler
								   })
					 )
	end
	
	toolTipHandler:update()
end

--logo = nil
current_scenario = nil
current_scenario_name = nil
-- Called when program starts
function core.init() 
    print( "Program starting in lua" )
    --logo = image( 10,10, "assets/texture/ui/ekswaised-logo_00.material", false )

    if core.config.init then
        core.config.init()
    end

    if current_scenario == nil then 
        openscenario( "main_menu" )    
    end
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
        err, errstr = pcall(current_scenario.load, current_scenario)
        if err == false then
            core.log.error( "Unable to load scenario\"" .. name .."\": " .. errstr )
            _, errstr = pcall( current_scenario.destroy, current_scenario ) 
            if errstr then core.log.error( errstr ) end
            current_scenario = nil
        end
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

function deepPrint( table, counter )
    counter =  counter or 0
    for i,v in pairs ( table ) do
        print( counter, ".:", i,":", v )
        if type(v) == "table" then
            deepPrint(v, counter+1)
        elseif type(v) == "function" then
            print( counter,".:",i,"():",v()) 
            if type( v() ) == "table" then
                deepPrint( v(), counter+1)
            end 
        end
    end
end

function core.saveHomeConfiguration()
    local filename = core.configuration.getConfigurationPath()

    if filename then 
        local configFile,err = io.open( filename, "w")
        if configFile then
            configFile:write( "return {\n" )
            for _,v in  pairs(core.config.locallySavedVars) do
                configFile:write( v .. " = " )

                local t = type(core.config[v])

                if t == "string" then
                    configFile:write("[[")
                    configFile:write(core.config[v])
                    configFile:write("]]")
                elseif t == "number" then
                    configFile:write(core.config[v])
                elseif t == "boolean" then
                    if core.config[v] then
                        configFile:write( "true" )
                    else
                        configFile:write( "false" )
                    end

                    configFile:write( ",\n" )
                else
                    core.log.error( "Unable to save parameter: " .. v .. " there is no writer for type")
                    configFile:write("nil,\n")
                end
            end
            configFile:write("}\n")

            configFile:close()

            core.log.info( "Saving configuration to: " .. filename )
        else
            core.log.error("Unable to open configuration file for saving: " .. err )
        end

    else
        core.log.error("Unable to save configuration file into user home folder")
    end
    
end

function core.loadHomeConfiguration()
    local filename = core.configuration.getConfigurationPath()

    core.log.info( "Loading home configuration file" )
    if filename then
        local success, val = pcall( dofile, filename )    

        if success then
            -- Loop through locallySavedVars to protect environment to be polluted unintentionally.
            for i,v in pairs(core.config.locallySavedVars) do
                print( v )
                if val[v] ~= nil  then
                    core.config[v] = val[v]
                    core.log.info( "Loaded :" .. v .. ":" .. tostring( val[v] ) )
                end
            end  
            core.log.info( "Loaded configuration file " .. filename )
        else
            core.log.warning("Unable to load configuration file: " .. val )
        end
    else
        core.log.error( "Unable to load config file, can't find config path" )
    end 
end
