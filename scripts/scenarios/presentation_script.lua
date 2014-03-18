return function( scen )
    local T = {}
    
    scen.gamemode =  require "gamemodes/kravall":new()
    scen:registerInitCallback( function() scen.gamemode:init() end )
    scen:registerUpdateCallback( function(delta) scen.gamemode:update(delta) end )
    scen:registerDestroyCallback( function() scen.gamemode:destroy() end )
    
    -- Script goes here
    
    return T
end
