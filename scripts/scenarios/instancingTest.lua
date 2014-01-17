local ent = require "entities"

local ASML = require "assembly_loader" 
local asm = ASML.loadPack( {} )

-- Entity factories
local ambient = ent.get "ambientLight"
local directional = ent.get "directionalLight"
local street_light = ent.get "streetLight"
local rioter = ent.get "rioter"
local police = ent.get "police"
local building = ent.get "building"
--

local street_light_intensity = 2.0
ambient(asm, 1.0, 1.0, 1.0, 0.1)
directional(asm, -1, -1, 0.5)

		
local centerPoint = { 0, 0, 0 }		
for i = -25, 25 do
	for p = -25, 25 do
		building( asm, p * 5 + centerPoint[1], 0  + centerPoint[2], i * 5 + centerPoint[3], 0)
	end
end

return asm;




