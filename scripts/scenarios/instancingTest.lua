
local ASML = require "assembly_loader" 
local asm = ASML.loadPack( {} )

local ambient = require "entities/ambientLight"
local directional = require "entities/directionalLight"
local street_light = require "entities/streetLight"
local street_light_intensity = 2.0
ambient.create(asm, 1.0, 1.0, 1.0, 0.1)
directional.create(asm, -1, -1, 0.5)

local rioter = require "entities/rioter"
local police = require "entities/police"
local building = require "entities/building"
		
local centerPoint = { 0, 0, 0 }		
for i = -25, 25 do
	for p = -25, 25 do
		building.create( asm, p * 5 + centerPoint[1], 0  + centerPoint[2], i * 5 + centerPoint[3], 0)
	end
end

return asm;




