
local ASML = require "assembly_loader" 
local asm = ASML.loadPack( {} )

asm:specific_content( core.contentmanager.load( 
		core.loaders.NavigationMeshLoader, "prototypeLevel.txt", function( value ) end, false ) )

local ambient = require "entities/ambientLight"
local directional = require "entities/directionalLight"
local street_light = require "entities/streetLight"
local street_light_intensity = 2.0
ambient.create(asm, 1.0, 1.0, 1.0, 0.1)
directional.create(asm, -1, -1, 0.5)

-- Group 0 start to end, top row (left side)
street_light.create(asm, -50, -0.5, street_light_intensity)
street_light.create(asm, -40, -0.5, street_light_intensity)
street_light.create(asm, -30, -0.5, street_light_intensity)
street_light.create(asm, -20, -0.5, street_light_intensity)
street_light.create(asm, -10.5, -0.5, street_light_intensity)
street_light.create(asm, 1, -0.5, street_light_intensity)
street_light.create(asm, 11, -0.5, street_light_intensity)
street_light.create(asm, 21, -0.5, street_light_intensity)
street_light.create(asm, 30.5, -2, street_light_intensity)
street_light.create(asm, 40, -4, street_light_intensity)
street_light.create(asm, 49.5, -4, street_light_intensity)
street_light.create(asm, 59, -4, street_light_intensity)

-- Group 0 start to end, bottom row (right side)
street_light.create(asm, -50, 10, street_light_intensity)
street_light.create(asm, -40, 10, street_light_intensity)
street_light.create(asm, -30, 10, street_light_intensity)
street_light.create(asm, -20, 10, street_light_intensity)
street_light.create(asm, -10.5, 10, street_light_intensity)
street_light.create(asm, 1, 10, street_light_intensity)
street_light.create(asm, 11, 10, street_light_intensity)
street_light.create(asm, 21, 10, street_light_intensity)
street_light.create(asm, 30.5, 11.5, street_light_intensity)
street_light.create(asm, 40, 13.5, street_light_intensity)
street_light.create(asm, 49.5, 13.5, street_light_intensity)
street_light.create(asm, 59, 13.5, street_light_intensity)

-- Group 0 end light
street_light.create(asm, 59, 4.75, street_light_intensity)

-- Group 1 start to end, bottom part, left row
street_light.create(asm, -25, 32, street_light_intensity)
street_light.create(asm, -17, 32, street_light_intensity)
street_light.create(asm, -10.5, 28, street_light_intensity)
street_light.create(asm, -10.5, 19, street_light_intensity)

-- Group 1 start to end, bottom part, right row
street_light.create(asm, -25, 40.5, street_light_intensity)
street_light.create(asm, -17, 40.5, street_light_intensity)
street_light.create(asm, -8.5, 34.25, street_light_intensity)
street_light.create(asm, 0, 28, street_light_intensity)
street_light.create(asm, 1, 19, street_light_intensity)

-- Group 1 start to end, top part, left row
street_light.create(asm, -10.5, -12, street_light_intensity)
street_light.create(asm, -10.5, -25, street_light_intensity)
street_light.create(asm, -5.75, -34, street_light_intensity)
street_light.create(asm, -1, -43, street_light_intensity)
street_light.create(asm, 14, -46, street_light_intensity)
street_light.create(asm, 28, -46, street_light_intensity)

-- Group 1 start to end, top part, right row
street_light.create(asm, 1, -12, street_light_intensity)
street_light.create(asm, 1, -25, street_light_intensity)
street_light.create(asm, 6, -34, street_light_intensity)
street_light.create(asm, 14, -35, street_light_intensity)
street_light.create(asm, 28, -35, street_light_intensity)

local rioter = require "entities/rioter"
local police = require "entities/police"
local building = require "entities/building"
	
-- Release	
local centerPoint = { 49, 0, 5 }		
for i = -7, 6 do
	for p = -6, 6 do
		rioter.create( asm, p * 1.5 + centerPoint[1], 0  + centerPoint[2], i * 1.5  + centerPoint[3], 0)
	end
end
core.nav_mesh.set_group_goal(0, -43, 0, 4)

local centerPoint = { 20.5, 0, -40 }		
for i = -4, 4 do
	for p = -5, 5 do
		rioter.create( asm, p * 1.5 + centerPoint[1], 0  + centerPoint[2], i * 1.5  + centerPoint[3], 1)
	end
end
core.nav_mesh.set_group_goal(1, -21, 0, 36)

-- Debug
--local centerPoint = { 49, 0, 5 }		
--for i = -2, 2 do
--	for p = -2, 2 do
--		rioter.create( asm, p * 1.5 + centerPoint[1], 0  + centerPoint[2], i * 1.5  + centerPoint[3], 0)
--	end
--end
--core.nav_mesh.set_group_goal(0, -43, 0, 4)
--
--local centerPoint = { 20.5, 0, -40 }		
--for i = -2, 2 do
--	for p = -2, 2 do
--		rioter.create( asm, p * 1.5 + centerPoint[1], 0  + centerPoint[2], i * 1.5  + centerPoint[3], 1)
--	end
--end
--core.nav_mesh.set_group_goal(1, -21, 0, 36)

local navmesh = require "entities/navMesh"
navmesh.create(asm, 0, -0.1, 0)
local plane = require "entities/plane"
plane.create(asm, 0, -1, 0)

local navmesh = require "entities/navMesh"
navmesh.create(asm, 0, -0.1, 0)
local plane = require "entities/plane"
plane.create(asm, 0, -1, 0)

building.create(asm, 64, 12)
building.create(asm, 64, 2)
building.create(asm, 64, -8)
building.create(asm, 55, -12)
building.create(asm, 45, -11)
building.create(asm, 30, -10)
building.create(asm, 20, -8)
building.create(asm, 10, -9)
building.create(asm, 13, -23)
building.create(asm, 23, -24)
building.create(asm, 33, -23)
building.create(asm, 36, -40)
building.create(asm, 33, -53)
building.create(asm, 21, -52)
building.create(asm, 11, -52)
building.create(asm, 1, -49)
building.create(asm, -10, -50)
building.create(asm, -15, -40)
building.create(asm, -18, -27)
building.create(asm, -17, -10)
building.create(asm, -29, -8)
building.create(asm, -39, -9)
building.create(asm, -49, -9)
building.create(asm, -57, -4)
building.create(asm, -58, 6)
building.create(asm, -56, 14)
building.create(asm, -44, 16)
building.create(asm, -33, 15)
building.create(asm, -18, 15)
building.create(asm, -24, 21)
building.create(asm, -30, 27)
building.create(asm, -33, 40)
building.create(asm, -21, 45)
building.create(asm, -12, 43)
building.create(asm, -1, 38)
building.create(asm, 6, 30)
building.create(asm, 9, 15)
building.create(asm, 18, 18)
building.create(asm, 28, 17)
building.create(asm, 36, 20)
building.create(asm, 45, 19)
building.create(asm, 56, 19)
return asm;
