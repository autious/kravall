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
    
    scen:registerInitCallback( function() 
        scen.gamemode:init()

        obj1 = scen.gamemode:createObjective()
        obj1.title = "Use the attack ability to kill all agitators"

		Statistics.addObjectives( { obj1 } )

        scen.gamemode.camera:addInterpolationPoint(scen.cameras.startcam.translation, scen.cameras.startcam.quaternion)
    end )

    scen:registerUpdateCallback( function(delta) 
        scen.gamemode:update(delta) 
    end )

    scen:registerTickCallback( function()
        if #(T.getAgitatorGroups()) > 0 then
            local count = 0
            for _,v in pairs( T.getAgitatorGroups() ) do
                count = count + core.system.groups.getGroupMemberCount( v )
            end
            obj1.title = "Use the attack ability to kill all agitators: " .. count

            if count <= 0 then
                obj1.state = 'success'
            end
        end
    end )

    scen:registerDestroyCallback( function() 
        scen.gamemode:destroy() 
    end )
    -- Script goes here

    module( "deserter", scen, T ) 
    module( "agitator", scen, T ) 
    module( "onstate" , scen, T ) 
    module( "spawn"   , scen, T ) 
    
    return T
end
