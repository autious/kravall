return function( scen, T )
    --Hides objects when we enter main game state
    function T.hideOnMain(ent)
        scen.gamemode:registerOnStateChange( function( stateName )
            if stateName == "Main" then
                ent:set( core.componentType.GraphicsComponent, {render=false}, true )
                return false -- return false to indicate that we have served our purpose and wish no longer to be called.
            end
            return true
        end)
    end

    function T.destroyOnMain(ent)
        scen.gamemode:registerOnStateChange( function( stateName )
            if stateName == "Main" then
                ent:destroy()
                return false -- return false to indicate that we have served our purpose and wish no longer to be called.
            end
            return true
        end)
    end

    function T.showOnMain(ent)
        scen.gamemode:registerOnStateChange( function( stateName )
            if stateName == "Main" then
                ent:set( core.componentType.GraphicsComponent, {render=true}, true )
                return false -- return false to indicate that we have served our purpose and wish no longer to be called.
            end
            return true
        end)
    end
end
