return function( scen )
    local T = {}
    	
    -- Script goes here
    scen.gamemode = require "gamemodes/normal":new()
    scen:registerUpdateCallback( function( delta ) scen.gamemode:update(delta) end )
    scen:registerDestroyCallback( function() scen.gamemode:destroy() end )
    return T
end
