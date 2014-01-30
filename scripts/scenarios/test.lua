local ent = require "entities"
local scenario = require "scenario"
local scen = scenario.new()

-- ents
local ambient = ent.get "ambientLight"
local directional = ent.get "directionalLight"
local street_light = ent.get "streetLight"
local area = ent.get "area"
local whitebox = ent.get "whitebox"

---
local street_light_intensity = 2.0

ambient(scen, 1.0, 1.0, 1.0, 0.1)
directional(scen, -1, -1, 0.5)

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

local building = ent.get "building"
		
local plane = ent.get "plane"
plane(scen, 0, -1, 0, 150)

--building(scen, 64, 12)
--building(scen, 64, 2)
--building(scen, 64, -8)
--building(scen, 55, -12)
--building(scen, 45, -11)
--building(scen, 30, -10)
--building(scen, 20, -8)
--building(scen, 10, -9)
--building(scen, 13, -23)
--building(scen, 23, -24)
--building(scen, 33, -23)
--building(scen, 36, -40)
--building(scen, 33, -53)
--building(scen, 21, -52)
--building(scen, 11, -52)
--building(scen, 1, -49)
--building(scen, -10, -50)
--building(scen, -15, -40)
--building(scen, -18, -27)
--building(scen, -17, -10)
--building(scen, -29, -8)
--building(scen, -39, -9)
--building(scen, -49, -9)
--building(scen, -57, -4)
--building(scen, -58, 6)
--building(scen, -56, 14)
--building(scen, -44, 16)
--building(scen, -33, 15)
--building(scen, -18, 15)
--building(scen, -24, 21)
--building(scen, -30, 27)
--building(scen, -33, 40)
--building(scen, -21, 45)
--building(scen, -12, 43)
--building(scen, -1, 38)
--building(scen, 6, 30)
--building(scen, 9, 15)
--building(scen, 18, 18)
--building(scen, 28, 17)
--building(scen, 36, 20)
--building(scen, 45, 19)
--building(scen, 56, 19)

for x=-10,10 do
    for y=-10,10 do
        whitebox(scen,x*200,0,y*200)
    end
end

--------------
local function area_init( entity )
--    print ("Wow, such init")
end

local function area_update( entity )
--    print ("Wow, such update")
end

area( scen, {0,0,0}, { 0,0, 1,0, 1,1, 0,1 }, "test_area", area_init, area_update )

scen:registerTickCallback( function()  end)
scen:registerInitCallback( function() print "init" end )

return scen;
