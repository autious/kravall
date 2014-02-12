local ent = require "entities"
local scen = require "scenario":new()

scen.asm:specific_content( core.contentmanager.load(
		core.loaders.NavigationMeshLoader, "extremeScenario.txt", function( value ) end, false ) )


scen.gamemode = require "gamemodes/kravall":new()

local mouse = core.input.mouse
local squad = ent.get "policeSquad"

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


scen.gamemode.camera:lookAt( core.glm.vec3.new( 50, 25, 50 ), core.glm.vec3.new( 0, 0, 0 ) )




local rioter = ent.get "rioter"
local police = ent.get "police"
local building = ent.get "building"


local rGroup = core.system.groups.createGroup();
local centerPoint = { 110, 0, 0 }
local side = math.sqrt( 50 )
for i = -side/2, side/2 do
	for p = -side/2, side/2 do
		--rioter( scen, p * 1.5 + centerPoint[1], 0  + centerPoint[2], i * 1.5  + centerPoint[3], rGroup, fists)
	end
end
rioter( scen, 0, 0, 0, rGroup, fists)
core.system.groups.setGroupGoal(rGroup, 0, 0, 0)


local squadOne = squad(scen, 20, 0, 0, math.pi/2, fists);
--local squadTwo = squad(scen, 77, 0, 43, math.pi/2, fists);



local clicked = false
local prevClicked = true
local firstX, firstY

function Update(dt)
	
end






scen:registerUpdateCallback( Update )

return scen;
