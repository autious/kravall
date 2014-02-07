local input = require "input" 
local MainMenu = require "gui/MainMenu"
local SetupMenu = require "gui/SetupMenu"
local ScenarioMenu = require "gui/ScenarioMenu"

local MenuScrollSpeed = 500

return function( scen )
    local menuState = {}
    local scenarios = {}
    
    for _,v in pairs( require "scenario_list" ) do
        local new_scen = {}
        local s  = dofile( "scripts/scenarios/" .. v .. ".lua" )

        new_scen.filename = v
        new_scen.name = s.name or ""
        new_scen.description = s.description or ""

        scenarios[#scenarios+1] = new_scen
    end

    menuState.selectedScenario = nil 

    function menuState.goMain()
        scen.gamemode.camera:setGoal( scen.cameras.main.view, MenuScrollSpeed )

        if scen.gui ~= nil then
            scen.gui:destroy()
            scen.gui = nil
        end
        scen.gui = MainMenu:new({},menuState)
    end

    function menuState.goScenario()
        scen.gamemode.camera:setGoal( scen.cameras.scen.view, MenuScrollSpeed )

        if scen.gui ~= nil then
            scen.gui:destroy()
            scen.gui = nil
        end
        scen.gui = ScenarioMenu:new({scenarios = scenarios},menuState)
    end

    function menuState.goSetup()
        scen.gamemode.camera:setGoal( scen.cameras.setup.view, MenuScrollSpeed )

        if scen.gui ~= nil then
            scen.gui:destroy()
            scen.gui = nil
        end
        scen.gui = SetupMenu:new({},menuState)
    end

    function menuState.goCredits()
        scen.gamemode.camera:setGoal( scen.cameras.credits.view, MenuScrollSpeed )

        if scen.gui ~= nil then
            scen.gui:destroy()
            scen.gui = nil
        end
        --scen.gui = SetupMenu:new({},menuState)
    end

    function menuState.goSettings()
        scen.gamemode.camera:setGoal( scen.cameras.settings.view, MenuScrollSpeed )

        if scen.gui ~= nil then
            scen.gui:destroy()
            scen.gui = nil
        end
        --scen.gui = SetupMenu:new({},menuState)
    end

    local function onKey( key, scancode, action )
        if key == core.input.keyboard.key.A and action == core.input.action.Release then
            menuState.goMain()
        end  

        if key == core.input.keyboard.key.B and action == core.input.action.Release then
            menuState.goScenario()
        end  

        if key == core.input.keyboard.key.C and action == core.input.action.Release then
            menuState.goSetup()
        end  

        if key == core.input.keyboard.key.D and action == core.input.action.Release then
            menuState.goCredits()
        end  

        if key == core.input.keyboard.key.E and action == core.input.action.Release then
            menuState.goSettings()
        end  
    end

    local function init()
        scen.gamemode.camera:setView( scen.cameras.main.view ) 
        input.registerOnKey( onKey )
        menuState.goMain()
    end

    local function destroy()
        input.deregisterOnKey( onKey )
        
        if scen.gui ~= nil then
            scen.gui:destroy()
            scen.gui = nil
        end
    end

    scen:registerInitCallback( init ) 
    scen:registerDestroyCallback( destroy )

    scen.gamemode = require "gamemodes/menu":new()
    scen:registerUpdateCallback( function(delta) scen.gamemode:update(delta) end )
    scen:registerDestroyCallback( function() scen.gamemode:destroy() end )

    local T = {}
    
    return T
end
