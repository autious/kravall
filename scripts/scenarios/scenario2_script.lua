local entity = require "entities"
local group = entity.get "group"
local Statistics = require "factories/Statistics"
local vec4 = require( "utility" ).expandMixxedHexToVec4

--For the kravall defintions
local abilities = core.system.squad.abilities

return function( scen )
    local T = {}
    
    scen.name = "Save the deserters!"
    scen.description =  [[Deserters and transhumans are equal, or so they say. The rebellion of the lower districts have sparked a revolution filled with violence, blood and tears. All in the name of freedom.

    Not everyone can handle it. That is why some people abandon their cause to seek refuge at the transhuman embassy where they will be assimilated into transhuman society in exchange for valuable information regarding the rebels' hideouts.

    A group of deserters are on their way to the embassy right now. Bands of rebels are certain to lie in wait for them and it is your job to protect the deserters until they reach the embassy. Keep them safe at any cost.]]
    
    -- Set game to start in prepmode
    scen.gamemode =  require "gamemodes/kravall":new(
    {
        initGamestate="Prep",
    })
    local obj1
    local deserterGroup = nil
    local rebelGroup = nil
    local waypoints = {}
    local waypoint_positions = {}
    local currentWaypoint = 0
    local fists 

    local agitatorGroups = {}

    local function getCurrentWaypoint()
        return waypoints["waypoint"..currentWaypoint]
    end
    
    -- Setup callbacks for gamemode
    scen:registerInitCallback( function() 
        scen.gamemode:init() 
        local plane = entity.get "plane"
        --plane(scen, 0, -1, 0, 900)

        scen.gamemode.camera:addInterpolationPoint(scen.cameras.startcam.translation, scen.cameras.startcam.quaternion)
        scen.gamemode.camera:addInterpolationPoint(scen.cameras.devcam.translation, scen.cameras.devcam.quaternion)
        obj1 = scen.gamemode:createObjective()
        obj1.title = "At least one deserter must survive and reach the goal"

        obj2 = scen.gamemode:createObjective()
        obj2.title = "Optional: Beat down the rebel hideout"
        obj2.bonus = true

        obj3 = scen.gamemode:createObjective()
        obj3.title = "Optional: Don't let more than 5 deserters die"
        obj3.bonus = true
        
        Statistics.addObjectives( { obj1, obj2, obj3 } )

        -- range, graceDistance, damageToHealth, damageToMorale, damageToMoraleOnMiss, enemyRageIncrease, enemyPressureIncrease, staminaCost, timeWhenAnimationDealsDamage, animationName
        fists = core.weaponData.pushWeapon( 1.0, 0.75, 10, 0.05, 0.01, 3.2, 2.9, 0.05, 0.5, "punch" )
    end )

    scen:registerUpdateCallback( 
    function(delta) 
        scen.gamemode:update(delta) 
        if deserterGroup then
            core.system.squad.enableOutline({deserterGroup}, (vec4{"#09FF00FF",2.0}):get())

            local count = core.system.groups.getGroupMemberCount( deserterGroup )
            obj1.title = "At least one deserter must reach the goal, " .. count .. " remain"
            if  count == 0 then
                obj1.state = "fail"
            end
        end

        if T.objBeatdown() then
            obj2.state = "success"
        end

        if T.objCasualties() then
            obj3.state = "fail"
        end
    end )

    scen:registerTickCallback( function( )
        if getCurrentWaypoint() and deserterGroup then
            local memberCount = core.system.groups.getGroupMemberCount( deserterGroup )
            if core.system.area.getAreaRioterCount( getCurrentWaypoint(), deserterGroup ) > math.max(memberCount/2 - 5,0) then
                currentWaypoint = currentWaypoint + 1
                if getCurrentWaypoint() then
                    print( "Set new goal to waypoint " .. currentWaypoint .. " at pos: ", core.glm.vec3.new( unpack( waypoint_positions["waypoint"..currentWaypoint] ) ))
                    core.system.groups.setGroupGoal( deserterGroup, unpack( waypoint_positions["waypoint"..currentWaypoint] ) )
                end
            end
        end
    end )

    scen:registerTickCallback( function()
        for i,v in  pairs( agitatorGroups ) do
            core.system.groups.setGroupGoal( v, core.system.groups.getGroupMedianPosition(deserterGroup))
        end     
    end)

    scen:registerDestroyCallback( function() scen.gamemode:destroy() end )

    -- Rebel beatdown objective check
    function T.objBeatdown()
        if rebelGroup then
            rebelCount = core.system.groups.getGroupMemberCount( rebelGroup )
            obj2.title = "Optional: Beat down the rebel hideout, " .. rebelCount .. " rebels remain"
            if rebelCount < 5 then
                return true
            else
                return false
            end
        else
            return false
        end
    end

    -- Low casualties objective check
    function T.objCasualties()
        if deserterGroup then
            deserterCount = core.system.groups.getGroupMemberCount( deserterGroup )
            obj3.title = "Optional: Don't let more than 5 deserters die, current deaths: " .. 50 - deserterCount
            if deserterCount < 45 then
                return false
            else
                return true
            end
        else
            return false
        end
    end

    --Function for adding more spawnpoints.
    function T.registerSpawn(ent)
        print( "Registering spawn point" )
        
        scen.gamemode:addAreaSpawnZone(ent)
    end

    function T.spawnDesertersOnMain(ent)
        scen.gamemode:registerOnStateChange( function( stateName )
            if stateName == "Main" then
                assert( waypoint_positions["waypoint1"], "Missing waypoint1, there is something wrong in blenders export, maybe you're not registering it?" )
                deserterGroup = T.createDeserter( ent,10,5 ) 
                return false -- return false to indicate that we have served our purpose and wish no longer to be called.
            end
            return true
        end)
    end

    scen.gamemode:registerBeforeStateChange( function( stateName )
        if stateName == "End" then
            print("We have reached before creation of end game state")

            if T.objBeatdown() then
                obj2.state = "success"
            else
                obj2.state = "fail"
            end

            if T.objCasualties() then
                obj3.state = "success"
            else
                obj3.state = "fail"
            end

            return false -- return false to indicate that we have served our purpose and wish no longer to be called.
        end
        return true
    end)

    function T.winOnRioterInside( ent )
        if core.system.area.getAreaRioterCount(ent,deserterGroup) > 0 then
            obj1.state = "success"
        end
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

    -- Create rioter on area:
    function T.createAgitators( ents, xsize, ysize )

        local grp = core.system.groups.createGroup(2)

        for i,ent in pairs( ents ) do
            local wpc = ent:get( core.componentType.WorldPositionComponent )
            local ac = ent:get( core.componentType.AreaComponent )
            verts = ac.vertices

            -- Make vertex positions from local space to world space
            for i = 1, 8, 2 do
                verts[i] = verts[i] + wpc.position[1]
                verts[i + 1] = verts[i + 1] + wpc.position[3]
            end
            group( scen, ac.vertices, grp, {xsize, ysize}, fists, nil, 1, 1, core.RioterAlignment.Anarchist, core.movementData.Jogging )
        end

        return grp
    end

    -- Create rebel on area:
    function T.createRebels( ent, xsize, ysize )

        local grp = core.system.groups.createGroup(2)

        local wpc = ent:get( core.componentType.WorldPositionComponent )
        local ac = ent:get( core.componentType.AreaComponent )
        verts = ac.vertices

        -- Make vertex positions from local space to world space
        for i = 1, 8, 2 do
            verts[i] = verts[i] + wpc.position[1]
            verts[i + 1] = verts[i + 1] + wpc.position[3]
        end
        group( scen, ac.vertices, grp, {xsize, ysize}, fists, nil, 1, 1, core.RioterAlignment.Anarchist, core.movementData.Jogging )
        
        return grp
    end

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

    function T.registerWaypoint(ent)
        local nc = ent:get(core.componentType.NameComponent) 
        local wpc = ent:get(core.componentType.WorldPositionComponent)
        
        waypoints[nc.name] = ent 
        waypoint_positions[nc.name] = wpc.position
        print( "Registering waypoint: " .. nc.name )
    end
    local spawnCounts = {
        {10,10},
        {17,18},
    }
    for i = 1,3 do
        T["registerAg"..i.."Spawn"] = function(ent) 
            local wpc = ent:get(core.componentType.WorldPositionComponent)
            print( "Registering Ag"..i )

            local groupId = core.gameMetaData.registerEscapePoint( wpc.position[1], wpc.position[2], wpc.position[3] )

            scen:registerTickCallback(function()
                if ent:isValid() then
                    local rioters = core.system.area.getAreaRioters(ent)

                    for _,v in pairs( rioters ) do
                        local alignmentComponent = v:get( core.componentType.AttributeComponent )
                        --print( alignmentComponent.groupID )
                        if alignmentComponent.groupID == groupId then
                            scen.asm:destroyEntity( v )
                        end  
                    end
                end    
            end)

            if T["ag"..i.."spawn"] == nil then
                T["ag"..i.."spawn"] = {}
            end
    
            table.insert( T["ag"..i.."spawn"], ent )
        end

        T["checkAg"..i.. "Enter"] = function( ent )
            local spawns = T["ag"..i.."spawn"]
    
            if #spawns > 0 and core.system.area.getAreaRioterCount( ent, deserterGroup ) > 0 then
                table.insert( agitatorGroups, T.createAgitators( spawns, unpack( spawnCounts[i] )  ) ) 
                T["ag"..i.."spawn"] = {}
            end
        end
    end

    function T.spawnRebel( ent )
        scen.gamemode:registerOnStateChange( function( stateName )
            if stateName == "Main" then
                rebelGroup = T.createRebels( ent, 5, 5 )
                return false -- return false to indicate that we have served our purpose and wish no longer to be called.
            end
            return true
        end)
    end

    function T.rotateY( ent, delta )
        local rotation = ent:get( core.componentType.RotationComponent ).rotation

        local qrot = core.glm.quat.new( unpack(rotation) )
        
        qrot = qrot:rotate( delta*10, core.glm.vec3.new(0,1,0) )

        ent:set( core.componentType.RotationComponent, { rotation = {qrot:get()}})
    end

    function T.rotateX( ent, delta )
        local rotation = ent:get( core.componentType.RotationComponent ).rotation

        local qrot = core.glm.quat.new( unpack(rotation) )
        
        qrot = qrot:rotate( delta*10, core.glm.vec3.new(1,0,0) )

        ent:set( core.componentType.RotationComponent, { rotation = {qrot:get()}})
    end

    function T.rotateZ( ent, delta )
        local rotation = ent:get( core.componentType.RotationComponent ).rotation

        local qrot = core.glm.quat.new( unpack(rotation) )
        
        qrot = qrot:rotate( delta*10, core.glm.vec3.new(0,0,1) )

        ent:set( core.componentType.RotationComponent, { rotation = {qrot:get()}})
    end

    function T.rotateYLight( ent, delta )

        local r = ent:get(core.componentType.RotationComponent).rotation
        r[4] = 0
        local vec = core.glm.vec4.new(unpack(r))

        local qrot = core.glm.quat.new( )
        
        qrot = qrot:rotate( delta*10, core.glm.vec3.new(0,1,0) )
        local d = qrot:mat4Cast() 
        local newD = d * vec     
        
        ent:set( core.componentType.RotationComponent, { rotation = {newD:get()}})
    end

    return T
end
