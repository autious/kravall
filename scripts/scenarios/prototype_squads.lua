local ent = require "entities"
local scen = require "scenario":new()

scen.asm:specific_content( core.contentmanager.load(
		core.loaders.NavigationMeshLoader, "prototypeLevel.txt", function( value ) end, false ) )

scen.gamemode = require "gamemodes/kravall":new()

scen:registerUpdateCallback( function(delta) scen.gamemode:update(delta) end )
scen:registerDestroyCallback( function() scen.gamemode:destroy() end )

local ambient = ent.get "ambientLight"
local directional = ent.get "directionalLight"
local street_light = ent.get "streetLight"
local street_light_intensity = 2.0
ambient(scen, 1.0, 1.0, 1.0, 0.01)
directional(scen, -1, -1, 0.5)

scen:registerUpdateCallback( function( delta ) scen.gamemode:update(delta) end )
scen:registerDestroyCallback( function() scen.gamemode:destroy() end )

--camera:lookAt( core.glm.vec3.new( -20, 35, 20 ), core.glm.vec3.new( 0, 0, 20 ) )
scen.gamemode.camera:lookAt( core.glm.vec3.new( -20, 35, 30 ), core.glm.vec3.new( 0, 0, 0 ) )

-- Group 0 start to end, top row (left side)
street_light(scen, -50, -0.5, street_light_intensity)
street_light(scen, -40, -0.5, street_light_intensity)
street_light(scen, -30, -0.5, street_light_intensity)
street_light(scen, -20, -0.5, street_light_intensity)
street_light(scen, -10.5, -0.5, street_light_intensity)
street_light(scen, 1, -0.5, street_light_intensity)
street_light(scen, 11, -0.5, street_light_intensity)
street_light(scen, 21, -0.5, street_light_intensity)
street_light(scen, 30.5, -2, street_light_intensity)
street_light(scen, 40, -4, street_light_intensity)
street_light(scen, 49.5, -4, street_light_intensity)
street_light(scen, 59, -4, street_light_intensity)

-- Group 0 start to end, bottom row (right side)
street_light(scen, -50, 10, street_light_intensity)
street_light(scen, -40, 10, street_light_intensity)
street_light(scen, -30, 10, street_light_intensity)
street_light(scen, -20, 10, street_light_intensity)
street_light(scen, -10.5, 10, street_light_intensity)
street_light(scen, 1, 10, street_light_intensity)
street_light(scen, 11, 10, street_light_intensity)
street_light(scen, 21, 10, street_light_intensity)
street_light(scen, 30.5, 11.5, street_light_intensity)
street_light(scen, 40, 13.5, street_light_intensity)
street_light(scen, 49.5, 13.5, street_light_intensity)
street_light(scen, 59, 13.5, street_light_intensity)

-- Group 0 end light
street_light(scen, 59, 4.75, street_light_intensity)

-- Group 1 start to end, bottom part, left row
street_light(scen, -25, 32, street_light_intensity)
street_light(scen, -17, 32, street_light_intensity)
street_light(scen, -10.5, 28, street_light_intensity)
street_light(scen, -10.5, 19, street_light_intensity)

-- Group 1 start to end, bottom part, right row
street_light(scen, -25, 40.5, street_light_intensity)
street_light(scen, -17, 40.5, street_light_intensity)
street_light(scen, -8.5, 34.25, street_light_intensity)
street_light(scen, 0, 28, street_light_intensity)
street_light(scen, 1, 19, street_light_intensity)

-- Group 1 start to end, top part, left row
street_light(scen, -10.5, -12, street_light_intensity)
street_light(scen, -10.5, -25, street_light_intensity)
street_light(scen, -5.75, -34, street_light_intensity)
street_light(scen, -1, -43, street_light_intensity)
street_light(scen, 14, -46, street_light_intensity)
street_light(scen, 28, -46, street_light_intensity)

-- Group 1 start to end, top part, right row
street_light(scen, 1, -12, street_light_intensity)
street_light(scen, 1, -25, street_light_intensity)
street_light(scen, 6, -34, street_light_intensity)
street_light(scen, 14, -35, street_light_intensity)
street_light(scen, 28, -35, street_light_intensity)

local rioter = ent.get "rioter"
local squad = ent.get "policeSquad"
local building = ent.get "building"


local squadOne = squad(scen, -5, 0, 0, -math.pi/4);
--local squadTwo = squad(scen, -5, 0, -10, math.pi/2);
--core.system.groups.setGroupGoal(policeGroup, -43, 0, 4)
   

--police(scen, 1, 0, -22);
--police(scen, 1, 0, -21);
--police(scen, 1, 0, -20);


-- Release	
local rGroup = core.system.groups.createGroup();
local centerPoint = { 49, 0, 5 }		
for i = -7, 6 do
	for p = -6, 6 do
		rioter( scen, p * 1.5 + centerPoint[1], 0  + centerPoint[2], i * 1.5  + centerPoint[3], rGroup)
	end
end
core.system.groups.setGroupGoal(rGroup, -43, 0, 4)
--rioter( scen, 6 * 1.5 + centerPoint[1], 0  + centerPoint[2], 6 * 1.5  + centerPoint[3], 0)
--rioter( scen, 6 * 1.5 + centerPoint[1], 0  + centerPoint[2], -7 * 1.5  + centerPoint[3], 0)

local rioterGroup = core.system.groups.createGroup()
local centerPoint = { 20.5, 0, -40 }		
for i = -4, 4 do
	for p = -5, 5 do
		rioter( scen, p * 1.5 + centerPoint[1], 0  + centerPoint[2], i * 1.5  + centerPoint[3], rioterGroup)
	end
end
core.system.groups.setGroupGoal(rioterGroup, -21, 0, 36)


-- Debug
--local centerPoint = { 49, 0, 5 }		
--for i = -2, 2 do
--	for p = -2, 2 do
--		rioter( scen, p * 1.5 + centerPoint[1], 0  + centerPoint[2], i * 1.5  + centerPoint[3], 0)
--	end
--end
--core.nav_mesh.set_group_goal(0, -43, 0, 4)
--
--local centerPoint = { 20.5, 0, -40 }		
--for i = -2, 2 do
--	for p = -2, 2 do
--		rioter( scen, p * 1.5 + centerPoint[1], 0  + centerPoint[2], i * 1.5  + centerPoint[3], 1)
--	end
--end
--core.nav_mesh.set_group_goal(1, -21, 0, 36)

local navmesh = ent.get "navMesh"
--navmesh(scen, 0, -0.1, 0)
local plane = ent.get "plane"
plane(scen, 0, -1, 0, 150)

building(scen, 64, 12)
building(scen, 64, 2)
building(scen, 64, -8)
building(scen, 55, -12)
building(scen, 45, -11)
building(scen, 30, -10)
building(scen, 20, -8)
building(scen, 10, -9)
building(scen, 13, -23)
building(scen, 23, -24)
building(scen, 33, -23)
building(scen, 36, -40)
building(scen, 33, -53)
building(scen, 21, -52)
building(scen, 11, -52)
building(scen, 1, -49)
building(scen, -10, -50)
building(scen, -15, -40)
building(scen, -18, -27)
building(scen, -17, -10)
building(scen, -29, -8)
building(scen, -39, -9)
building(scen, -49, -9)
building(scen, -57, -4)
building(scen, -58, 6)
building(scen, -56, 14)
building(scen, -44, 16)
building(scen, -33, 15)
building(scen, -18, 15)
building(scen, -24, 21)
building(scen, -30, 27)
building(scen, -33, 40)
building(scen, -21, 45)
building(scen, -12, 43)
building(scen, -1, 38)
building(scen, 6, 30)
building(scen, 9, 15)
building(scen, 18, 18)
building(scen, 28, 17)
building(scen, 36, 20)
building(scen, 45, 19)
building(scen, 56, 19)

return scen;
