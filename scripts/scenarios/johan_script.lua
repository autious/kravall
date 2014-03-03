return function( scen )
    local T = {}
    
	scen.gamemode = require "gamemodes/kravall":new()

	
    scen:registerInitCallback( function() scen.gamemode:init(); scen.gamemode.camera:lookAt( core.glm.vec3.new( -40, 30, 180 ), core.glm.vec3.new( -40, 0, 155 ) ) end )
    scen:registerUpdateCallback( function(delta) scen.gamemode:update(delta) end )
    scen:registerDestroyCallback( function() scen.gamemode:destroy() end )
    
    -- Script goes here
    
    return T
end
