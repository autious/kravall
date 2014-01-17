
local ent = require "entities"
local ASML = require "assembly_loader" 
local asm = ASML.loadPack( {} )

asm:specific_content( core.contentmanager.load( 
		core.loaders.NavigationMeshLoader, "prototypeLevel.txt", function( value ) end, false ) )

local rioter = ent.get "rioter"
local building = ent.get "building"
		
local centerPoint = { -43, 0, 4 }		
for i = -3, 3 do
	for p = -3, 3 do
		rioter( asm, p * 2 + centerPoint[1], 0  + centerPoint[2], i * 2  + centerPoint[3], 0)
	end
end
core.nav_mesh.set_group_goal(0, 43, 0, 15)

local centerPoint = { -21, 0, 36 }		
for i = -3, 3 do
	for p = -3, 3 do
		rioter( asm, p * 1.5 + centerPoint[1], 0  + centerPoint[2], i * 1.5  + centerPoint[3], 1)
	end
end
core.nav_mesh.set_group_goal(1, 22, 0, -40)

local plane = ent.get "plane"
plane(asm, 0, -0.1, 0)

building(asm, 64, 12)
building(asm, 64, 2)
building(asm, 64, -8)
building(asm, 55, -15)
building(asm, 45, -16)
building(asm, 30, -13)
building(asm, 20, -11)
building(asm, 10, -10)
building(asm, 13, -23)
building(asm, 23, -24)
building(asm, 33, -23)
building(asm, 37, -40)
building(asm, 33, -53)
building(asm, 21, -58)
building(asm, 11, -58)
building(asm, 1, -54)
building(asm, -10, -50)
building(asm, -15, -40)
building(asm, -19, -30)
building(asm, -17, -12)
building(asm, -29, -14)
building(asm, -39, -12)
building(asm, -49, -12)
building(asm, -57, -4)
building(asm, -58, 6)
building(asm, -56, 14)
building(asm, -44, 12)
building(asm, -33, 13)
building(asm, -18, 14)
building(asm, -24, 21)
building(asm, -30, 27)
building(asm, -33, 40)
building(asm, -21, 45)
building(asm, -12, 43)
building(asm, -1, 38)
building(asm, 6, 30)
building(asm, 9, 15)
building(asm, 18, 18)
building(asm, 28, 16)
building(asm, 36, 20)
building(asm, 45, 18)
building(asm, 56, 18)
return asm;




