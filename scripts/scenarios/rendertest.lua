local ent = require "entities"
local ASML = require "assembly_loader" 
local asm = ASML.loadPack( {} )

asm:specific_content( core.contentmanager.load( 
		core.loaders.NavigationMeshLoader, "prototypeLevel.txt", function( value ) end, false ) )

local ambient = ent.get "ambientLight"
local directional = ent.get "directionalLight"
local street_light = ent.get "streetLight"
local street_light_intensity = 2.0
ambient(asm, 1.0, 1.0, 1.0, 0.1)
directional(asm, -1, -1, 0.5)

local rioter = ent.get "rioter"
local police = ent.get "police"
local building = ent.get "building"
		
local centerPoint = {0, 0, 0 }		
for i = -250, 250 do
	for p = -25, 25 do
		building( asm, p * 1.5 + centerPoint[1], 0  + centerPoint[2], i * 1.5  + centerPoint[3], 0)
	end
end
return asm;




