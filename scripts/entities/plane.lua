-- Create an internal table for simulating an object in Lua. Return it at the end to get a pointer to this object
local t = {}

local planeScale = 17

-- Must be prefixed by the internal table name. Must have the name create (so that it is always the same) and the parameter asm
function t.create(asm, posX, posY, posZ, scale)
	asm:loadAssembly( 
		{
			{
				type = core.componentType.GraphicsComponent,
				data = { mesh = 2000, material = 2000, type = core.gfx.objectTypes.OpaqueGeometry },
				load = { 
							mesh = { core.loaders.GnomeLoader, "assets/plane.bgnome" },
							material = { core.loaders.MaterialLoader, "assets/material/whitePixel.material" }
					   }
			},
			{
				type = core.componentType.WorldPositionComponent,
				data = { position = { posX, posY, posZ} }
			},
			{
				type = core.componentType.RotationComponent,
				data = { rotation = { -math.sin(math.pi/4.0), 0, 0, math.cos(math.pi/4.0) } }
			},
			{
				type = core.componentType.ScaleComponent,
				data = { scale = scale }
			},
			{
				type = core.componentType.UnitTypeComponent,
				data = { unitType = core.UnitType.Object }
			},
		}
	)

end

return t

-- This entity is loaded by adding the rows:
-- 		local ENTITY_NAME = require "entities/FILENAME" (does not need the .lua filetype)
--		ENTITY_NAME.create()