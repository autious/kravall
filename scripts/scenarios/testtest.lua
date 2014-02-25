return function( scen )
    local T = {}
	local groupIds = {}
	local entity = require "entities"
	local rioter = entity.get "rioter"
	local policeSquad = entity.get "policeSquad"
    local tearGasSquad = entity.get "policeTearGasSquad"
	local group = entity.get "group"
	local atkRange = 90
	local pathFlag = 1
	local activeGoal = ""
	local friendlyGroup, angryGroup1, angryGroup2, angryGroup3
	
	local DONT_DIE_MSG = "one can not simply" 
	local ESCORT_MSG = "derp into querp"
	local objDontDie
	local objLeadThrough
	local track = true

    scen.name = "The Adventure"
    scen.description = "test scenario pls ignore"


	-- weapons
	local fists
	
	scen.gamemode = require "gamemodes/kravall":new()


	scen:registerUpdateCallback( function(delta) scen.gamemode:update(delta) end )
	scen:registerDestroyCallback( function() scen.gamemode:destroy() end )
	scen:registerInitCallback( function() 
                                    scen.gamemode:init()
									scen.gamemode.camera:lookAt( core.glm.vec3.new( -40, 30, 180 ), core.glm.vec3.new( -40, 0, 155 ) ) 
									--scen.gamemode.camera.yaw = scen.gamemode.camera.yaw + math.pi
									local plane = entity.get "plane"
									plane(scen, 0, -1, 0, 900)
									
									-- load weapons...
									fists = core.weaponData.pushWeapon( 1.0, 0.75, 20, 0.2, 0.05, 3.2, 2.9, 0.05, 0.5, "punch" )
							   end)

    scen:registerInitCallback( function()
        print( "Creating objectives.." )
        objDontDie      = scen.gamemode:createObjective()
        objLeadThrough  = scen.gamemode:createObjective()

        objDontDie.title      = DONT_DIE_MSG 
        objLeadThrough.title  = ESCORT_MSG
    end)

	
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
		local wpc = ent:get( core.componentType.WorldPositionComponent )
		local ac = ent:get( core.componentType.AreaComponent )
		verts = ac.vertices

		-- Make vertex positions from local space to world space
		for i = 1, 8, 2 do
			verts[i] = verts[i] + wpc.position[1]
			verts[i + 1] = verts[i + 1] + wpc.position[3]
		end
			
		group( scen, ac.vertices, grp, {size, size}, fists )
	end
	
	---- Create a police squad
	function T.createSquad( ent )
		local wpc = ent:get( core.componentType.WorldPositionComponent )
		local ac = ent:get( core.componentType.AreaComponent )
		
		scen.gamemode:addSquad( policeSquad( scen, wpc.position[1], 0, wpc.position[3], 0, fists))
		scen.gamemode:addSquad( tearGasSquad( scen, wpc.position[1], 0, wpc.position[3], 0, fists))
	end
	
	-- Set destination for rioters based on an area's name
	local function setGoal( grp )
		-- Set group destination to the next destination
		local activeGoal = "goal" .. pathFlag
		local destArea = core.system.name.getEntitiesByName( activeGoal )[1]
		local destination = destArea:get( core.componentType.WorldPositionComponent )
		core.system.groups.setGroupGoal( friendlyGroup, destination.position[1], 0, destination.position[3])

		-- Check if the group has reached their destination
		local count = core.system.area.getAreaRioterCount( grp )
		local alive_count = core.system.groups.getGroupMemberCount( friendlyGroup )
		
		if pathFlag < 4 then
			if T.checkObjCount( destArea ) > 10 then
				pathFlag = pathFlag + 1;
			end
			
			-- WIN CONDITION
			else if T.checkObjCount( destArea ) > 50 then
				objLeadThrough.state = "success" 
				objDontDie.state = "success" 
				track = false
			end
		end
		
		objDontDie.title = DONT_DIE_MSG .. " " .. alive_count .. " still alive."
	end
	
	local function trackGroup( grp )
		-- Calculating distance between hunting / target groups
		local x1,y1,z1 = core.system.groups.getGroupMedianPosition(grp)
		local x2,y2,z2 = core.system.groups.getGroupMedianPosition(friendlyGroup)
		local tmpx = x1-x2
		local tmpx = tmpx * tmpx
		local tmpz = z1-z2
		local tmpz = tmpz * tmpz
		local tmpxz = tmpx + tmpz
		local result = math.sqrt(tmpxz)

		if result < atkRange then
			core.system.groups.setGroupGoal( grp, x2, 0, z2)
		end
		
		-- TEMPORARY LOSE CONDITION
		local kill = 10
		if result < kill then
			objLeadThrough.state = "fail"
			objDontDie.state = "fail"
		end
	end


	--===================== SHORTCUTS ======================--
	function T.createRioter_0( ent )
		friendlyGroup = core.system.groups.createGroup()
		groupIds[ent] = friendlyGroup
		T.createRioter( ent, friendlyGroup, 15 )
	end
	
	function T.createPoliceSquad( ent )
		T.createSquad( ent )
	end
	
	function T.createRioter_1( ent )
		angryGroup1 = core.system.groups.createGroup()
		groupIds[ent] = angryGroup1
		T.createRioter( ent, angryGroup1, 10 )
	end
	
	function T.createRioter_2( ent )
		angryGroup2 = core.system.groups.createGroup()
		groupIds[ent] = angryGroup2
		T.createRioter( ent, angryGroup2, 10 )
	end
	
	function T.createRioter_3( ent )
		angryGroup3 = core.system.groups.createGroup()
		groupIds[ent] = angryGroup3
		T.createRioter( ent, angryGroup3, 10 )
	end
	
	function T.setDestination( ent )
			setGoal( ent )
	end
	
	function T.track( ent )
			trackGroup( groupIds[ent] )
	end
	
	return T;
end
