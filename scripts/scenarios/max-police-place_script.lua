local entity = require "entities"
local group = entity.get "group"

--For the kravall defintions
local abilities = core.system.squad.abilities
local standardPolice = (require "game_constants").standardPolice

return function( scen )
    local T = {}

    scen.name = "Max's police prep test"
    scen.description = "This is simple scenario demonstrating how to create a kravall gamemode with a prep mode."
    
    -- Set game to start in prepmode
    scen.gamemode =  require "gamemodes/kravall":new(
    {
        initGamestate="Prep",
        weapons = 
        {
            punch = {1.0, 0.75, 20, 0.2, 0.05, 3.2, 2.9, 0.05, 0.5, "punch"}
        },
        policeTypes =
        {
            {
                name = "Common Squad",
                description = "Can beat the fuck out of you",
                cost = 100,
                setup = 
                {
                    {
                        positionOffset = {0,0,0},
                        weapon = "punch",
                        mesh = "assets/model/animated/rioter/rioter-male_00.bgnome",
                        material = "assets/texture/animated/police/cop/cop-light_00.material",
                        abilities = {
                            abilities.Attack, 
                            abilities.ArrestGroup, 
                            abilities.Sprint, 
                            abilities.Rout,
                        },
                        health = standardPolice.maxHealth, 
                        stamina = standardPolice.maxStamina, 
                        morale = standardPolice.maxMorale, 
                    },
                    {
                        positionOffset = {1,0,0},
                        weapon = "punch",
                        mesh = "assets/model/animated/rioter/rioter-male_00.bgnome",
                        material = "assets/texture/animated/police/cop/cop-light_00.material",
                        abilities = {
                            abilities.Attack, 
                            abilities.ArrestGroup, 
                            abilities.Sprint, 
                            abilities.Rout,
                        },
                        health = standardPolice.maxHealth, 
                        stamina = standardPolice.maxStamina, 
                        morale = standardPolice.maxMorale, 
                    },
                    {
                        positionOffset = {0,0,1},
                        weapon = "punch",
                        mesh = "assets/model/animated/rioter/rioter-male_00.bgnome",
                        material = "assets/texture/animated/police/cop/cop-light_00.material",
                        abilities = {
                            abilities.Attack, 
                            abilities.ArrestGroup, 
                            abilities.Sprint, 
                            abilities.Rout,
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
                            abilities.ArrestGroup, 
                            abilities.Sprint, 
                            abilities.Rout,
                        },
                        health = standardPolice.maxHealth, 
                        stamina = standardPolice.maxStamina, 
                        morale = standardPolice.maxMorale, 
                    },
                    {
                        positionOffset = {-1,0,0},
                        weapon = "punch",
                        mesh = "assets/model/animated/rioter/rioter-male_00.bgnome",
                        material = "assets/texture/animated/police/cop/cop-light_00.material",
                        abilities = {
                            abilities.Attack, 
                            abilities.ArrestGroup, 
                            abilities.Sprint, 
                            abilities.Rout,
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
                cost = 100,
                setup = 
                {
                    {
                        positionOffset = {-1,0,0},
                        weapon = "punch",
                        mesh = "assets/model/animated/police/cop/cop-teargas_00.bgnome",
                        material = "assets/texture/animated/police/cop/cop-teargas_00.material",
                        abilities = {
                            abilities.Attack, 
                            abilities.ArrestGroup, 
                            abilities.Sprint, 
                            abilities.TearGas, 
                            abilities.Rout,
                        },
                        health = standardPolice.maxHealth, 
                        stamina = standardPolice.maxStamina, 
                        morale = standardPolice.maxMorale, 
                    },
                    {
                        positionOffset = {0,0,1},
                        weapon = "punch",
                        mesh = "assets/model/animated/police/cop/cop-teargas_00.bgnome",
                        material = "assets/texture/animated/police/cop/cop-teargas_00.material",
                        abilities = {
                            abilities.Attack, 
                            abilities.ArrestGroup, 
                            abilities.Sprint, 
                            abilities.Rout,
                        },
                        health = standardPolice.maxHealth, 
                        stamina = standardPolice.maxStamina, 
                        morale = standardPolice.maxMorale, 
                    },
                    {
                        positionOffset = {1,0,0},
                        weapon = "punch",
                        mesh = "assets/model/animated/rioter/rioter-male_00.bgnome",
                        material = "assets/texture/animated/police/cop/cop-light_00.material",
                        abilities = {
                            abilities.Attack, 
                            abilities.ArrestGroup, 
                            abilities.Sprint, 
                            abilities.Rout,
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
                            abilities.ArrestGroup, 
                            abilities.Sprint, 
                            abilities.Rout,
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
    local group1 = nil

    -- Setup callbacks for gamemode
    scen:registerInitCallback( function() scen.gamemode:init() 
            local plane = entity.get "plane"
            plane(scen, 0, -1, 0, 900)

            obj1 = scen.gamemode:createObjective()
            obj1.title = "Prevent rioters from reaching objective (by killing them)"
        end )

    scen:registerUpdateCallback( 
    function(delta) 
            scen.gamemode:update(delta) 
            if group1 then
                local count = core.system.groups.getGroupMemberCount( group1 )
                obj1.title = "Prevent rioters from reaching objective (by killing them). " .. count .. " remain."
                if  count < 5 then
                    obj1.state = "success"
                end
            end
    end )
    scen:registerDestroyCallback( function() scen.gamemode:destroy() end )
    
    -- Script goes here

    --Function for adding more spawnpoints.
    function T.registerSpawn(ent)
        print( "Registering spawn point" )
        
        scen.gamemode:addAreaSpawnZone(ent)
    end

    function T.registerRioterSpawn(ent)

    end

    local groupsgoal
    function T.allGroupGoal(ent)
        local wpc = ent:get( core.componentType.WorldPositionComponent )
        groupsgoal = wpc.position
    end

    function T.spawnRiotersOnMain(ent)
        scen.gamemode:registerOnStateChange( function( stateName )
            if stateName == "Main" then
                T.createRioter( ent,10,5) 
                return false -- return false to indicate that we have served our purpose and wish no longer to be called.

            end
            return true
        end)

    end

    function T.lossOnRioterInside( ent )
        if core.system.area.getAreaRioterCount(ent) > 5 then
            obj1.state = "fail" 
        end
    end
    
    -- Create rioter on area:
	function T.createRioter( ent, xsize,ysize )
		local wpc = ent:get( core.componentType.WorldPositionComponent )
		local ac = ent:get( core.componentType.AreaComponent )
		verts = ac.vertices

		-- Make vertex positions from local space to world space
		for i = 1, 8, 2 do
			verts[i] = verts[i] + wpc.position[1]
			verts[i + 1] = verts[i + 1] + wpc.position[3]
		end
	    group1 = group1 or core.system.groups.createGroup(1)
		group( scen, ac.vertices, group1, {xsize, ysize}, fists )
        core.system.groups.setGroupGoal( group1, unpack(groupsgoal) )
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

    function T.showOnMain(ent)
        scen.gamemode:registerOnStateChange( function( stateName )
            if stateName == "Main" then
                ent:set( core.componentType.GraphicsComponent, {render=true}, true )
                return false -- return false to indicate that we have served our purpose and wish no longer to be called.
            end
            return true
        end)
    end

    return T
end
