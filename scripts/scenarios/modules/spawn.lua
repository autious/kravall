return function( scen,T )
    --Function for adding more spawnpoints.
    function T.registerSpawn(ent)
        print( "Registering spawn point" )
        
        scen.gamemode:addAreaSpawnZone(ent)
    end
end
