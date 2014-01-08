
local ASML = require "assembly_loader" 
local asm = ASML.loadPack( {} )

navMesh = core.contentmanager.load( 
		core.loaders.NavigationMeshLoader, "prototypeLevel.txt", function( value ) end, false )

local rioter = require "entities/rioter"
		
local centerPoint = { -43, 0, 4 }		
for i = -3, 3 do
	for p = -3, 3 do
		rioter.create( asm, p * 2 + centerPoint[1], 0  + centerPoint[2], i * 2  + centerPoint[3], 0)
	end
end
--core.nav_mesh.set_group_goal(0, 43, 0, 15)

local centerPoint = { -21, 0, 35 }		
for i = -2, 2 do
	for p = -2, 2 do
		rioter.create( asm, p * 2 + centerPoint[1], 0  + centerPoint[2], i * 2  + centerPoint[3], 1)
	end
end
--core.nav_mesh.set_group_goal(0, 43, 0, 15)
	
return asm;




