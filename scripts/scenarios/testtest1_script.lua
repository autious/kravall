
local entity = require "entities"

return function( scen )
    local T = {}
	
    scen.gamemode = require "gamemodes/kravall":new()
	 scen:registerInitCallback( function() 
		scen.gamemode:init() 
		scen.gamemode.camera:addInterpolationPoint(scen.cameras.devcam.translation, scen.cameras.devcam.quaternion)

		-- range, graceDistance, damageToHealth, damageToMorale, damageToMoraleOnMiss, enemyRageIncrease, enemyPressureIncrease, staminaCost, timeWhenAnimationDealsDamage, animationName
		fists = core.weaponData.pushWeapon( 1.0, 0.75, 10, 0.05, 0.01, 3.2, 2.9, 0.05, 0.5, "punch" )
    end )
    scen:registerUpdateCallback( function(delta) scen.gamemode:update(delta) end )
    scen:registerDestroyCallback( function() scen.gamemode:destroy() end )
    
	scen.gamemode.camera:addInterpolationPoint(scen.cameras.devcam.translation, scen.cameras.devcam.quaternion)
	
    -- Script goes here
    function T:CreateGroupA()
	
		--local rGroup2 = core.system.groups.createGroup( 5 );
		--local centerPoint2 = { -30, 0, 0 }
		--local side = math.sqrt( 300 )
		--for i = -side/2, side/2 do
		--	for p = -side/2, side/2 do
		--		rioter( scen, p * 1.5 + centerPoint2[1], 0  + centerPoint2[2], i * 1.5  + centerPoint2[3], 
		--		rGroup2, nil, nil, fists, nil, 100, 100, core.RioterAlignment.Anarchist, core.movementData.Jogging )
		--	end
		--end
		--core.system.groups.setGroupGoal(rGroup2, 120, 0, 0)	
	
	end
	
	function T:CreateGroupB()
	
		--local rGroup3 = core.system.groups.createGroup( 1 );
		--local centerPoint3 = { 30, 0, 0 }
		--local side = math.sqrt( 10 )
		--for i = -side/2, side/2 do
		--	for p = -side/2, side/2 do
		--		rioter( scen, p * 1.5 + centerPoint3[1], 0  + centerPoint3[2], i * 1.5  + centerPoint3[3], 
		--		rGroup3, nil, nil, fists, nil, 100, 100, core.RioterAlignment.Anarchist, core.movementData.Jogging )
		--	end
		--end
		--core.system.groups.setGroupGoal(rGroup3, -30, 0, 0)	
	end
	
	
    return T
end
