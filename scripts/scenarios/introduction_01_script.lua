local entity = require "entities"
local Statistics = require "statistics/Statistics"
local vec4 = require( "utility" ).expandMixxedHexToVec4
local module = require( "module" )

--For the kravall defintions
local abilities = core.system.squad.abilities
local standardPolice = (require "game_constants").standardPolice
local tearGasPolice = (require "game_constants").tearGasPolice

return function( scen )
    local T = {}
	
    scen.name = "Introduction, Escort, Tear Gas"
    scen.description =  [[Escort the group of deserts to the end goal, prevent agitators from andvancing to deserters using the tear gas ability, bonus for not letting any people getting hurt.]]
    
    -- Set game to start in prepmode
    scen.gamemode =  require "gamemodes/kravall":new(
    {
        cashLimit = 500,
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
                name = "Teargas Squad",
                description = "Can make people cry ='D",
                cost = tearGasPolice.cost,
                setup = 
                {
                    {
                        positionOffset = {-2,0,0},
                        weapon = "punch",
                        mesh = "assets/model/animated/police/cop/cop-teargas_00.bgnome",
                        material = "assets/texture/animated/police/cop/cop-teargas_00.material",
                        abilities = {
                            abilities.TearGas, 
                        },
                        health = tearGasPolice.maxHealth, 
                        stamina = tearGasPolice.maxStamina, 
                        morale = tearGasPolice.maxMorale, 
                    },
                    {
                        positionOffset = {2,0,0},
                        weapon = "punch",
                        mesh = "assets/model/animated/police/cop/cop-teargas_00.bgnome",
                        material = "assets/texture/animated/police/cop/cop-teargas_00.material",
                        abilities = {
                            abilities.TearGas, 
                        },
                        health = tearGasPolice.maxHealth, 
                        stamina = tearGasPolice.maxStamina, 
                        morale = tearGasPolice.maxMorale, 
                    },
                    {
                        positionOffset = {0,0,2},
                        weapon = "punch",
                        mesh = "assets/model/animated/police/cop/cop-teargas_00.bgnome",
                        material = "assets/texture/animated/police/cop/cop-teargas_00.material",
                        abilities = {
                            abilities.TearGas, 
                        },
                        health = tearGasPolice.maxHealth, 
                        stamina = tearGasPolice.maxStamina, 
                        morale = tearGasPolice.maxMorale, 
                    },
                    {
                        positionOffset = {0,0,-2},
                        weapon = "punch",
                        mesh = "assets/model/animated/police/cop/cop-teargas_00.bgnome",
                        material = "assets/texture/animated/police/cop/cop-teargas_00.material",
                        abilities = {
                            abilities.TearGas, 
                        },
                        health = tearGasPolice.maxHealth, 
                        stamina = tearGasPolice.maxStamina, 
                        morale = tearGasPolice.maxMorale, 
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
        obj2.title = "(Bonus) Don't let anyone die"
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
        end
    end )
    
    scen:registerTickCallback( function()
        if  core.system.groupDead.getDeadUnitCount() > 0 then
            obj2.state = 'fail'
        end
    end)

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

    return T
end

