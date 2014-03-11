local entity = require "entities"
local group = entity.get "group"
local vec4 = require( "utility" ).expandMixxedHexToVec4

return function( scen, T )
    
    local deserterGroup = nil
    local waypoints = {}
    local waypoint_positions = {}
    local fists = core.weaponData.pushWeapon( 1.0, 0.75, 10, 0.05, 0.01, 3.2, 2.9, 0.05, 0.5, "punch" )
    local currentWaypoint = 1
    local initialSize = {10,5}

    function T.getInitialDeserterCount()
        return initialSize[1] * initialSize[2]
    end

    local function getCurrentWaypoint()
        return waypoints["waypoint"..currentWaypoint]
    end

    function T.registerWaypoint(ent)
        local nc = ent:get(core.componentType.NameComponent) 
        local wpc = ent:get(core.componentType.WorldPositionComponent)
        
        waypoints[nc.name] = ent 
        waypoint_positions[nc.name] = wpc.position
        print( "Registering waypoint: " .. nc.name )
    end

    -- Create rioter on area:
    function T.createDeserter( ent, xsize, ysize )
        local wpc = ent:get( core.componentType.WorldPositionComponent )
        local ac = ent:get( core.componentType.AreaComponent )
        verts = ac.vertices

        -- Make vertex positions from local space to world space
        for i = 1, 8, 2 do
            verts[i] = verts[i] + wpc.position[1]
            verts[i + 1] = verts[i + 1] + wpc.position[3]
        end
        local grp = core.system.groups.createGroup(1)
        group( scen, ac.vertices, grp, {xsize, ysize}, fists, {0.9,0.3,0,1}, 1, 1,core.RioterAlignment.Pacifist )
        return grp
    end

    function T.spawnDesertersOnMain(ent)
        scen.gamemode:registerOnStateChange( function( stateName )
            if stateName == "Main" then
                assert( waypoint_positions["waypoint1"], "Missing waypoint1, there is something wrong in blenders export, maybe you're not registering it?" )
                deserterGroup = T.createDeserter( ent,unpack(initialSize) ) 
                return false -- return false to indicate that we have served our purpose and wish no longer to be called.
            end
            return true
        end)
    end

    function T.getDeserterGroup()
        return deserterGroup
    end

    scen:registerTickCallback( function( )
        if deserterGroup then
            core.system.squad.enableOutline({deserterGroup}, (vec4{"#09FF00FF",2.0}):get())
            if getCurrentWaypoint()  then 
                local memberCount = core.system.groups.getGroupMemberCount( deserterGroup )
                if core.system.area.getAreaRioterCount( getCurrentWaypoint(), deserterGroup ) > math.max(memberCount/2 - 5,0) then
                    currentWaypoint = currentWaypoint + 1
                    if getCurrentWaypoint() then
                        print( "Set new goal to waypoint " .. currentWaypoint .. " at pos: ", core.glm.vec3.new( unpack( waypoint_positions["waypoint"..currentWaypoint] ) ))
                        core.system.groups.setGroupGoal( deserterGroup, unpack( waypoint_positions["waypoint"..currentWaypoint] ) )
                    end   
                end 
            end
        end
    end)
end
