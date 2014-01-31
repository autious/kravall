return function( scen )
    scen:registerInitCallback( function()
        print( "Starting main menu" )       
    end) 

    scen.cameras = scen.cameras or {}
    scen.cameras.first = { {}, {} }
    
    scen.gamemode = require "gamemodes/menu":new()
    scen:registerUpdateCallback( function(delta) scen.gamemode:update(delta) end )
    scen:registerDestroyCallback( function() scen.gamemode:destroy() end )

    local T = {}
    
    return T
end
