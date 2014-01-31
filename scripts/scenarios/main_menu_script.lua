local input = require "input" 

return function( scen )

    local function onKey( key, scancode, action )
        if key == core.input.keyboard.key.A and action == core.input.action.Release then
            scen.gamemode.camera:setGoal( scen.cameras.main.view )
        end  

        if key == core.input.keyboard.key.B and action == core.input.action.Release then
            scen.gamemode.camera:setGoal( scen.cameras.setup.view )
        end  
    end

    

    local function init()
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
