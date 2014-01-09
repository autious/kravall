-- Create an internal table for simulating an object in Lua. Return it at the end to get a pointer to this object
local t = {}

local r = 0.94
local g = 0.89
local b = 0.69
local posY = 5
local size = 15

-- Must be prefixed by the internal table name. Must have the name create (so that it is always the same) and the parameter asm
function t.create(asm, posX, posZ, strength)
	asm:loadAssembly( 
		{
			{
				type = core.componentType.LightComponent,
				data =  { 
							color = { r, g, b },
							intensity = strength,
							type = core.gfx.objectTypes.Light,
							lighttype = core.gfx.lightTypes.Point
						}
			},
			{
				type = core.componentType.WorldPositionComponent,
				data = { position = { posX, posY, posZ } }
			},
			{
				type = core.componentType.ScaleComponent,
				data = { scale = 10.0 }
			},
			{
				type = core.componentType.RotationComponent,
				data = { rotation = { 0, 0, 0, 0 } }
			}
		} 
    )
end

return t

-- This entity is loaded by adding the rows:
-- 		local ENTITY_NAME = require "entities/FILENAME" (does not need the .lua filetype)
--		ENTITY_NAME.create()