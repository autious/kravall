return function( scen )
    local T = {}
	local entity = require "entities"
	local rioter = entity.get "rioter"
	local group = entity.get "group"
	local pathFlag = 1
	local activeGoal = ""

	scen.gamemode = require "gamemodes/kravall":new()
	scen:registerUpdateCallback( function(delta) scen.gamemode:update(delta) end )
	scen:registerDestroyCallback( function() scen.gamemode:destroy() end )
	
	-- Just random helpful stuffzzz --
	--core.system.name.getEntitiesByName( "area1" )[1]
	-- local myName = GetEntityName( ent )
	
	
	--====================== MAIN FUNCTIONS ======================--
	
		
	---- Returns amount of rioters in an area
	function T.checkObjCount( ent )
		return core.system.area.getAreaRioterCount( ent )
    end
	
	-- Return name of an area
	local function GetEntityName(ent)
		return ent:get( core.componentType.NameComponent )["name"]
	end
	
	---- Create a group of rioters
	function T.createRioter( ent, grp, size )
		wpc = ent:get( core.componentType.WorldPositionComponent )
		ac = ent:get( core.componentType.AreaComponent )
		local verts = ac.vertices

		-- Make vertex positions from local space to world space
		for i = 1, 8, 2 do
			verts[i] = verts[i] + wpc.position[1]
			verts[i + 1] = verts[i + 1] + wpc.position[3]
		end
			
		group( scen, ac.vertices, grp, {size, size} )

	end
	
	-- Set destination for rioters based on an area's name
	function setGoal( grp )
		
		-- Set group destination to the next destination
		activeGoal = "goal" .. pathFlag
		destArea = core.system.name.getEntitiesByName( activeGoal )[1]
		destination = destArea:get( core.componentType.WorldPositionComponent )
		core.system.groups.setGroupGoal( 0, destination.position[1], 0, destination.position[3])

		-- Check if the group has reached their destination
		if T.checkObjCount( destArea ) > 10 then
			pathFlag = pathFlag + 1;
	end
	
	---- Return position of current goal
	--function getGoalPosition( )
	--	position = core.system.name.getEntitiesByName( "area" .. currGoal )[1]:get( core.componentType.WorldPositionComponent )
	--	return position
	--end
	
	end

	--===================== SHORTCUTS ======================--
	function T.createRioter_0( ent )
		local groupid = core.system.groups.createGroup()
		T.createRioter( ent, groupid, 10 )
	end
	
	function T.setDestination( ent )
			setGoal( ent )
	end
	
	return T;
end
