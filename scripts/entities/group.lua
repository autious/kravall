local entities = require "entities"
local rioter = entities.get "rioter"

return function(scen, verts, group, amount, weaponType, outlineColor,rage,pressure )
    rage = rage or 1
    pressure = pressure or 1
	--local center = {(verts[1] + verts[3] + verts[5] + verts[7]) / 4,
	--	(verts[2] + verts[4] + verts[6] + verts[8]) / 4}
	
	lowestX = math.min(verts[1], verts[3], verts[5], verts[7])
	highestX = math.max(verts[1], verts[3], verts[5], verts[7])
	lowestY = math.min(verts[2], verts[4], verts[6], verts[8])
	highestY = math.max(verts[2], verts[4], verts[6], verts[8])

	for i = 1, amount[1] do
		for k = 1, amount[2] do
			rioter( scen,
				lowestX - ( ( (highestX - lowestX) / amount[1] ) / 2 ) + ((highestX - lowestX) * ((1 / amount[1]) * i)),
				0,
				lowestY - ( ( (highestY - lowestY) / amount[2] ) / 2 ) + ((highestY - lowestY) * ((1 / amount[2]) * k)),
				group, nil, nil, weaponType, outlineColor,rage,pressure)
		end
	end
end
