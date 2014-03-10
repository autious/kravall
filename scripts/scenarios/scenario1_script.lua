local entity = require "entities"
local group = entity.get "group"
local Statistics = require "factories/Statistics"
local vec4 = require( "utility" ).expandMixxedHexToVec4

--For the kravall defintions
local abilities = core.system.squad.abilities
local standardPolice = (require "game_constants").standardPolice
local tearGasPolice = (require "game_constants").tearGasPolice

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
        weapons = 
        {
            -- range, graceDistance, damageToHealth, damageToMorale, damageToMoraleOnMiss, enemyRageIncrease, enemyPressureIncrease, staminaCost, timeWhenAnimationDealsDamage, animationName
            punch = {1.0, 0.75, 20, 0.2, 0.05, 3.2, 2.9, 0.05, 0.5, "punch"},
            --shield = {1.0, 0.75, 20, 0.2, 0.05, 3.2, 2.9, 0.05, 0.5, "shield"},
            shield = {1.0, 0.75, 40, 0.4, 0.05, 4, 2.0, 0.02, 0.3, "punch"}
        },
        policeTypes =
        {
            {
                name = "Shield Squad",
                description = "Can beat the fuck out of you",
                cost = standardPolice.cost,
                setup = 
                {

                    {
                        positionOffset = {2,0,0},
                        weapon = "shield",
                        mesh = "assets/model/animated/police/cop/cop-shield_00.bgnome",
                        material = "assets/texture/animated/police/cop/cop-shield_00.material",
                        abilities = {
                            abilities.Attack, 
                            abilities.Sprint, 
                            abilities.Flee,
                        },
                        health = standardPolice.maxHealth, 
                        stamina = standardPolice.maxStamina, 
                        morale = standardPolice.maxMorale, 
                    },
                    {
                        positionOffset = {0,0,0},
                        weapon = "shield",
                        mesh = "assets/model/animated/police/cop/cop-shield_00.bgnome",
                        material = "assets/texture/animated/police/cop/cop-shield_00.material",
                        abilities = {
                            abilities.Attack, 
                            abilities.Sprint, 
                            abilities.Flee,
                        },
                        health = standardPolice.maxHealth, 
                        stamina = standardPolice.maxStamina, 
                        morale = standardPolice.maxMorale, 
                    },
                    {
                        positionOffset = {1,0,0},
                        weapon = "shield",
                        mesh = "assets/model/animated/police/cop/cop-shield_00.bgnome",
                        material = "assets/texture/animated/police/cop/cop-shield_00.material",
                        abilities = {
                            abilities.Attack, 
                            abilities.Sprint, 
                            abilities.Flee,
                        },
                        health = standardPolice.maxHealth, 
                        stamina = standardPolice.maxStamina, 
                        morale = standardPolice.maxMorale, 
                    },
                    {
                        positionOffset = {0,0,1},
                        weapon = "shield",
                        mesh = "assets/model/animated/police/cop/cop-shield_00.bgnome",
                        material = "assets/texture/animated/police/cop/cop-shield_00.material",
                        abilities = {
                            abilities.Attack, 
                            abilities.Sprint, 
                            abilities.Flee,
                        },
                        health = standardPolice.maxHealth, 
                        stamina = standardPolice.maxStamina, 
                        morale = standardPolice.maxMorale, 
                    },
                    {
                        positionOffset = {0,0,-1},
                        weapon = "shield",
                        mesh = "assets/model/animated/police/cop/cop-shield_00.bgnome",
                        material = "assets/texture/animated/police/cop/cop-shield_00.material",
                        abilities = {
                            abilities.Attack, 
                            abilities.Sprint, 
                            abilities.Flee,
                        },
                        health = standardPolice.maxHealth, 
                        stamina = standardPolice.maxStamina, 
                        morale = standardPolice.maxMorale, 
                    },
                    {
                        positionOffset = {-1,0,0},
                        weapon = "shield",
                        mesh = "assets/model/animated/police/cop/cop-shield_00.bgnome",
                        material = "assets/texture/animated/police/cop/cop-shield_00.material",
                        abilities = {
                            abilities.Attack, 
                            abilities.Sprint, 
                            abilities.Flee,
                        },
                        health = standardPolice.maxHealth, 
                        stamina = standardPolice.maxStamina, 
                        morale = standardPolice.maxMorale, 
                    },
                },
            },
            {
                name = "Teargas Squad",
                description = "Can make people cry ='D",
                cost = tearGasPolice.cost,
                setup = 
                {
                    {
                        positionOffset = {-1,0,0},
                        weapon = "punch",
                        mesh = "assets/model/animated/police/cop/cop-teargas_00.bgnome",
                        material = "assets/texture/animated/police/cop/cop-teargas_00.material",
                        abilities = {
                            abilities.Attack, 
                            abilities.Sprint, 
                            abilities.TearGas, 
                            abilities.Flee,
                        },
                        health = tearGasPolice.maxHealth, 
                        stamina = tearGasPolice.maxStamina, 
                        morale = tearGasPolice.maxMorale, 
                    },
                    {
                        positionOffset = {0,0,1},
                        weapon = "punch",
                        mesh = "assets/model/animated/police/cop/cop-teargas_00.bgnome",
                        material = "assets/texture/animated/police/cop/cop-teargas_00.material",
                        abilities = {
                            abilities.Attack, 
                            abilities.Sprint, 
                            abilities.Flee,
                        },
                        health = tearGasPolice.maxHealth, 
                        stamina = tearGasPolice.maxStamina, 
                        morale = tearGasPolice.maxMorale, 
                    },
                    {
                        positionOffset = {1,0,0},
                        weapon = "punch",
                        mesh = "assets/model/animated/rioter/rioter-male_00.bgnome",
                        material = "assets/texture/animated/police/cop/cop-light_00.material",
                        abilities = {
                            abilities.Attack, 
                            abilities.Sprint, 
                            abilities.Flee,
                        },
                        health = standardPolice.maxHealth, 
                        stamina = standardPolice.maxStamina, 
                        morale = standardPolice.maxMorale, 
                    },
                    {
                        positionOffset = {0,0,-1},
                        weapon = "punch",
                        mesh = "assets/model/animated/rioter/rioter-male_00.bgnome",
                        material = "assets/texture/animated/police/cop/cop-light_00.material",
                        abilities = {
                            abilities.Attack, 
                            abilities.Sprint, 
                            abilities.Flee,
                        },
                        health = standardPolice.maxHealth, 
                        stamina = standardPolice.maxStamina, 
                        morale = standardPolice.maxMorale, 
                    },
                },
            },
        }
    })
    local obj1
    local deserterGroup = nil
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
        plane(scen, 0, -1, 0, 900)

        --scen.gamemode.camera:lookAt( core.glm.vec3.new( 50, 100, 10), core.glm.vec3.new( -60, 0, -50 ) )
        scen.gamemode.camera:addInterpolationPoint(scen.cameras.startcam.translation, scen.cameras.startcam.quaternion)
        --scen.gamemode.camera:addInterpolationPoint(scen.cameras.devcam.translation, scen.cameras.devcam.quaternion)
        obj1 = scen.gamemode:createObjective()
        obj1.title = "At least one deserter must survive and reach the goal."
		
		Statistics.addObjectives( { obj1 } )
		
        -- range, graceDistance, damageToHealth, damageToMorale, damageToMoraleOnMiss, enemyRageIncrease, enemyPressureIncrease, staminaCost, timeWhenAnimationDealsDamage, animationName
		--fists = core.weaponData.pushWeapon( 1.0, 0.75, 20, 0.2, 0.05, 3.2, 2.9, 0.05, 0.5, "punch" )
		fists = core.weaponData.pushWeapon( 1.0, 0.75, 10, 0.05, 0.01, 3.2, 2.9, 0.05, 0.5, "punch" )
    end )

    scen:registerUpdateCallback( 
    function(delta) 
        scen.gamemode:update(delta) 
        if deserterGroup then
			core.system.squad.enableOutline({deserterGroup}, (vec4{"#09FF00FF",2.0}):get())

            local count = core.system.groups.getGroupMemberCount( deserterGroup)
            obj1.title = "At least one deserter must survive and reach the goal. " .. count .. " remain."
            if  count == 0 then
                obj1.state = "fail"
            end
        end
    end )

    scen:registerTickCallback( function( )
        if getCurrentWaypoint() and deserterGroup then 
            local memberCount = core.system.groups.getGroupMemberCount( deserterGroup )
            if core.system.area.getAreaRioterCount( getCurrentWaypoint(), deserterGroup ) > math.max(memberCount/2 - 5,0) then
                currentWaypoint = currentWaypoint + 1
                if getCurrentWaypoint() then
                    print( "Set new goal!" )
                    core.system.groups.setGroupGoal( deserterGroup, unpack( waypoint_positions["waypoint"..currentWaypoint] ) )
                end   
            end 
        end
    end)

    scen:registerTickCallback( function()
        for i,v in  pairs( agitatorGroups ) do
            core.system.groups.setGroupGoal( v, core.system.groups.getGroupMedianPosition(deserterGroup))
        end     
    end)

    scen:registerDestroyCallback( function() scen.gamemode:destroy() end )
    
    -- Script goes here

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

    for i = 1,4 do
        T["registerAg"..i.."Spawn"] = function(ent) 
		    local wpc = ent:get(core.componentType.WorldPositionComponent)
            print( "Registering Ag"..i )
		    core.gameMetaData.registerEscapePoint( wpc.position[1], wpc.position[2], wpc.position[3] )

            if T["ag"..i.."spawn"] == nil then
                T["ag"..i.."spawn"] = {}
            end
    
            table.insert( T["ag"..i.."spawn"], ent )
        end

        T["checkAg"..i.. "Enter"] = function( ent )
            local spawns = T["ag"..i.."spawn"]
    
            if #spawns > 0 and core.system.area.getAreaRioterCount( ent, deserterGroup ) > 0 then
                table.insert( agitatorGroups, T.createAgitators( spawns, 10,10 ) ) 
                T["ag"..i.."spawn"] = {}
            end
        end
    end

    function T.checkAg2And4Enter( ent )
        T.checkAg2Enter( ent )
        T.checkAg4Enter( ent ) 
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
