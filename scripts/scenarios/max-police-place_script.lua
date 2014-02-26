local entity = require "entities"

return function( scen )
    local T = {}
    
    -- Set game to start in prepmode
    scen.gamemode =  require "gamemodes/kravall":new({initGamestate="Prep"})

    -- Setup callbacks for gamemode
    scen:registerInitCallback( function() scen.gamemode:init() 
									local plane = entity.get "plane"
									plane(scen, 0, -1, 0, 900)
        end )

    scen:registerUpdateCallback( function(delta) scen.gamemode:update(delta) end )
    scen:registerDestroyCallback( function() scen.gamemode:destroy() end )
    
    -- Script goes here

    --Function for adding more spawnpoints.
    function T.registerSpawn(ent)
        print( "Registering spawn point" )
        
        scen.gamemode:addAreaSpawnZone(ent)
    end
    
    return T
end
