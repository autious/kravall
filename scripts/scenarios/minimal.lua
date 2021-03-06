local ent = require "entities"
local ASML = require "assembly_loader" 
local asm = ASML.loadPack( {} )

-- ents
local ambient = ent.get "ambientLight"
local directional = ent.get "directionalLight"
local street_light = ent.get "streetLight"
local area = ent.get "area"

---

local street_light_intensity = 2.0

ambient(asm, 1.0, 1.0, 1.0, 0.1)
directional(asm, -1, -1, 0.5)

-- Group 0 start to end, top row (left side)
street_light(asm, -50, -0.5, street_light_intensity)
street_light(asm, -40, -0.5, street_light_intensity)
street_light(asm, -30, -0.5, street_light_intensity)
street_light(asm, -20, -0.5, street_light_intensity)
street_light(asm, -10.5, -0.5, street_light_intensity)
street_light(asm, 1, -0.5, street_light_intensity)
street_light(asm, 11, -0.5, street_light_intensity)
street_light(asm, 21, -0.5, street_light_intensity)
street_light(asm, 30.5, -2, street_light_intensity)
street_light(asm, 40, -4, street_light_intensity)
street_light(asm, 49.5, -4, street_light_intensity)
street_light(asm, 59, -4, street_light_intensity)

-- Group 0 start to end, bottom row (right side)
street_light(asm, -50, 10, street_light_intensity)
street_light(asm, -40, 10, street_light_intensity)
street_light(asm, -30, 10, street_light_intensity)
street_light(asm, -20, 10, street_light_intensity)
street_light(asm, -10.5, 10, street_light_intensity)
street_light(asm, 1, 10, street_light_intensity)
street_light(asm, 11, 10, street_light_intensity)
street_light(asm, 21, 10, street_light_intensity)
street_light(asm, 30.5, 11.5, street_light_intensity)
street_light(asm, 40, 13.5, street_light_intensity)
street_light(asm, 49.5, 13.5, street_light_intensity)
street_light(asm, 59, 13.5, street_light_intensity)

-- Group 0 end light
street_light(asm, 59, 4.75, street_light_intensity)

-- Group 1 start to end, bottom part, left row
street_light(asm, -25, 32, street_light_intensity)
street_light(asm, -17, 32, street_light_intensity)
street_light(asm, -10.5, 28, street_light_intensity)
street_light(asm, -10.5, 19, street_light_intensity)

-- Group 1 start to end, bottom part, right row
street_light(asm, -25, 40.5, street_light_intensity)
street_light(asm, -17, 40.5, street_light_intensity)
street_light(asm, -8.5, 34.25, street_light_intensity)
street_light(asm, 0, 28, street_light_intensity)
street_light(asm, 1, 19, street_light_intensity)

-- Group 1 start to end, top part, left row
street_light(asm, -10.5, -12, street_light_intensity)
street_light(asm, -10.5, -25, street_light_intensity)
street_light(asm, -5.75, -34, street_light_intensity)
street_light(asm, -1, -43, street_light_intensity)
street_light(asm, 14, -46, street_light_intensity)
street_light(asm, 28, -46, street_light_intensity)

-- Group 1 start to end, top part, right row
street_light(asm, 1, -12, street_light_intensity)
street_light(asm, 1, -25, street_light_intensity)
street_light(asm, 6, -34, street_light_intensity)
street_light(asm, 14, -35, street_light_intensity)
street_light(asm, 28, -35, street_light_intensity)

local building = ent.get "building"
		
local plane = ent.get "plane"
plane(asm, 0, -1, 0, 150)

building(asm, 64, 12)
building(asm, 64, 2)
building(asm, 64, -8)
building(asm, 55, -12)
building(asm, 45, -11)
building(asm, 30, -10)
building(asm, 20, -8)
building(asm, 10, -9)
building(asm, 13, -23)
building(asm, 23, -24)
building(asm, 33, -23)
building(asm, 36, -40)
building(asm, 33, -53)
building(asm, 21, -52)
building(asm, 11, -52)
building(asm, 1, -49)
building(asm, -10, -50)
building(asm, -15, -40)
building(asm, -18, -27)
building(asm, -17, -10)
building(asm, -29, -8)
building(asm, -39, -9)
building(asm, -49, -9)
building(asm, -57, -4)
building(asm, -58, 6)
building(asm, -56, 14)
building(asm, -44, 16)
building(asm, -33, 15)
building(asm, -18, 15)
building(asm, -24, 21)
building(asm, -30, 27)
building(asm, -33, 40)
building(asm, -21, 45)
building(asm, -12, 43)
building(asm, -1, 38)
building(asm, 6, 30)
building(asm, 9, 15)
building(asm, 18, 18)
building(asm, 28, 17)
building(asm, 36, 20)
building(asm, 45, 19)
building(asm, 56, 19)

--------------

area( asm, {0,0,0}, { 0,0, 1,0, 1,1, 0,1 }, "test_area" )

return asm;
