return function( scen )
    local T = {}
	local entity = require "entities"
	local rioter = entity.get "rioter"
	local group = entity.get "group"

	local group0 = core.system.groups.createGroup()
	local group1 = core.system.groups.createGroup()
	
	function T.printCount( ent )
	--    print( core.system.area.getAreaRioterCount( ent ) ) 
	--    print( #(core.system.area.getAreaRioters( ent )) ) 
	end

	function T.testPrint( ent )
		print( "swigg" )
	end
	
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

	function T.goal( ent, grp )
		wpc = ent:get( core.componentType.WorldPositionComponent )
		core.system.groups.setGroupGoal( grp, wpc.position[1], 0, wpc.position[3] )
	end

	function T.createRioter_0( ent )
		T.createRioter( ent, group0, 10 )
	end

	function T.createRioter_1( ent )
		T.createRioter( ent, group1, 5 )
	end

	function T.goal_0( ent )
		T.goal( ent, group0 )
	end

	function T.goal_1( ent )
		T.goal( ent, group1 )
	end
	
	return T;
end