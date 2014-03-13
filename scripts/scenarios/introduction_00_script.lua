local entity = require "entities"
local Statistics = require "factories/Statistics"
local vec4 = require( "utility" ).expandMixxedHexToVec4
local module = require( "module" )

--For the kravall defintions
local abilities = core.system.squad.abilities
local standardPolice = (require "game_constants").standardPolice
local tearGasPolice = (require "game_constants").tearGasPolice

return function( scen )
    local T = {}
	
    scen.name = "Introduction, Escort"
    scen.description =  [[Escort the group of deserts to the end goal using the shield squad, bonus points of all deserts survive to the end.]]
    
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
                },
            },
        }
    })
    local obj1
    local obj2
    local rioterGoalCount = 0 
	
    -- Setup callbacks for gamemode
    scen:registerInitCallback( function() 
        scen.gamemode:init() 
        local plane = entity.get "plane"

        obj1 = scen.gamemode:createObjective()
        obj1.title = "At least one deserter must survive and reach the goal."

        obj2 = scen.gamemode:createObjective()
        obj2.title = "(Bonus) Let all deserters reach the goal unharmed."
        obj2.bonus = true
		
		Statistics.addObjectives( { obj1,obj2 } )

        scen.gamemode.camera:addInterpolationPoint(scen.cameras.startcam.translation, scen.cameras.startcam.quaternion)
		
        -- range, graceDistance, damageToHealth, damageToMorale, damageToMoraleOnMiss, enemyRageIncrease, enemyPressureIncrease, staminaCost, timeWhenAnimationDealsDamage, animationName
		-- fists = core.weaponData.pushWeapon( 1.0, 0.75, 20, 0.2, 0.05, 3.2, 2.9, 0.05, 0.5, "punch" )
    end )

    scen:registerUpdateCallback( 
    function(delta) 
        scen.gamemode:update(delta) 
        if T.getDeserterGroup() then
            local count = core.system.groups.getGroupMemberCount( T.getDeserterGroup() )
            obj1.title = "At least one deserter must survive and reach the goal. " .. count + rioterGoalCount .. " remain."
            if count + rioterGoalCount < T.getInitialDeserterCount() then
                obj2.state = "fail" 
            end
        end
    end )

    scen:registerDestroyCallback( function() scen.gamemode:destroy() end )

    module( "agitator", scen, T ) 
    module( "deserter", scen, T ) 
    module( "onstate" , scen, T ) 
    module( "rotation", scen, T ) 
    module( "spawn"   , scen, T ) 

    function T.winOnRioterInside( ent )
        if T.getDeserterGroup() then
            local count = core.system.groups.getGroupMemberCount( T.getDeserterGroup() )
            if count == 0  then
                if rioterGoalCount > 0 then
                    obj1.state = "success"
                else
                    obj1.state = "fail"
                end
            end

            local rioters = core.system.area.getAreaRioters(ent)
            for _,v in pairs( rioters ) do
                local alignmentComponent = v:get( core.componentType.AttributeComponent )
                if alignmentComponent.groupID == T.getDeserterGroup() then
                    scen.asm:destroyEntity( v )
                    rioterGoalCount = rioterGoalCount + 1
                end  
            end
        end
    end
	
    scen.gamemode:registerOnStateChange( 
		function( stateName )
			if stateName == "End" then
				-- TODO: write wrap up code for the scenario.
				
				print("We have reached end game state")
				return false -- return false to indicate that we have served our purpose and wish no longer to be called.
			end
			return true
		end)

    return T
end

