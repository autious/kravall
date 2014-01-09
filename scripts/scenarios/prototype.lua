
local ASML = require "assembly_loader" 
local asm = ASML.loadPack( {} )

navMesh = core.contentmanager.load( 
		core.loaders.NavigationMeshLoader, "prototypeLevel.txt", function( value ) end, false )

local rioter = require "entities/rioter"
local building = require "entities/building"
		
local centerPoint = { -43, 0, 4 }		
for i = -3, 3 do
	for p = -3, 3 do
		rioter.create( asm, p * 2 + centerPoint[1], 0  + centerPoint[2], i * 2  + centerPoint[3], 0)
	end
end
core.nav_mesh.set_group_goal(0, 43, 0, 15)

local centerPoint = { -21, 0, 36 }		
for i = -3, 3 do
	for p = -3, 3 do
		rioter.create( asm, p * 1.5 + centerPoint[1], 0  + centerPoint[2], i * 1.5  + centerPoint[3], 1)
	end
end
core.nav_mesh.set_group_goal(1, 22, 0, -40)

building.create(asm, 64, 12)
building.create(asm, 64, 2)
building.create(asm, 64, -8)
building.create(asm, 55, -15)
building.create(asm, 45, -16)
building.create(asm, 30, -13)
building.create(asm, 20, -11)
building.create(asm, 10, -10)
building.create(asm, 13, -23)
building.create(asm, 23, -24)
building.create(asm, 33, -23)
building.create(asm, 37, -40)
building.create(asm, 33, -53)
building.create(asm, 21, -58)
building.create(asm, 11, -58)
building.create(asm, 1, -54)
building.create(asm, -10, -50)
building.create(asm, -15, -40)
building.create(asm, -19, -30)
building.create(asm, -17, -12)
building.create(asm, -29, -14)
building.create(asm, -39, -12)
building.create(asm, -49, -12)
building.create(asm, -57, -4)
building.create(asm, -58, 6)
building.create(asm, -56, 14)
building.create(asm, -44, 12)
building.create(asm, -33, 13)
building.create(asm, -18, 14)
building.create(asm, -24, 21)
building.create(asm, -30, 27)
building.create(asm, -33, 40)
building.create(asm, -21, 45)
building.create(asm, -12, 43)
building.create(asm, -1, 38)
building.create(asm, 6, 30)
building.create(asm, 9, 15)
building.create(asm, 18, 18)
building.create(asm, 28, 16)
building.create(asm, 36, 20)
building.create(asm, 45, 18)
building.create(asm, 56, 18)
return asm;




