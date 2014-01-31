local input = require "input" 

local MenuScrollSpeed = 500

return function( scen )
    local function onKey( key, scancode, action )
        if key == core.input.keyboard.key.A and action == core.input.action.Release then
            scen.gamemode.camera:setGoal( scen.cameras.main.view, MenuScrollSpeed )
        end  

        if key == core.input.keyboard.key.B and action == core.input.action.Release then
            scen.gamemode.camera:setGoal( scen.cameras.scen.view, MenuScrollSpeed )
        end  

        if key == core.input.keyboard.key.C and action == core.input.action.Release then
            scen.gamemode.camera:setGoal( scen.cameras.setup.view, MenuScrollSpeed )
        end  
    end

    local function init()
        scen.gamemode.camera:setView( scen.cameras.main.view ) 
        input.registerOnKey( onKey )
    end

    local function destroy()
        input.deregisterOnKey( onKey )
    end

    scen:registerInitCallback( init ) 
    scen:registerDestroyCallback( destroy )

    scen.gamemode = require "gamemodes/menu":new()
    scen:registerUpdateCallback( function(delta) scen.gamemode:update(delta) end )
    scen:registerDestroyCallback( function() scen.gamemode:destroy() end )

    local T = {}
    
    return T
end
