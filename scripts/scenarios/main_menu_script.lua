local input = require "input" 
local MainMenu = require "gui/MainMenu"
local SetupMenu = require "gui/SetupMenu"
local ScenarioMenu = require "gui/ScenarioMenu"

local MenuScrollSpeed = 500

return function( scen )
    local menuState = {}

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
        scen.gui = ScenarioMenu:new({},menuState)
    end

    function menuState.goSetup()
        scen.gamemode.camera:setGoal( scen.cameras.setup.view, MenuScrollSpeed )

        if scen.gui ~= nil then
            scen.gui:destroy()
            scen.gui = nil
        end
        scen.gui = SetupMenu:new({},menuState)
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
