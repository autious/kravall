-- Create an internal table for simulating an object in Lua. Return it at the end to get a pointer to this object
local t = {}

-- Must be prefixed by the internal table name. Must have the name create (so that it is always the same) and the parameter asm
function t.create(asm, dirX, dirY, dirZ)
	asm:loadAssembly( 
		{
			{
				type = core.componentType.LightComponent,
				data =  { 
							color = { 0.5, 0.5, 0.5},
							intensity = 0.5,
							type = core.gfx.objectTypes.Light,
							lighttype = core.gfx.lightTypes.Dir,
                            speccolor = { r,g,b }
						}
			},
			{
				type = core.componentType.WorldPositionComponent,
				data = { position = { 0, 0, 0 } }
			},
			{
				type = core.componentType.ScaleComponent,
				data = { scale = 1.0 } 
			},
			{
				type = core.componentType.RotationComponent,
				data = { rotation = { dirX, dirY, dirZ, 0 } } -- Lights uses rotation component as a direction vector, not a quaternion
			}
		} 
	)
end

return t

-- This entity is loaded by adding the rows:
-- 		local ENTITY_NAME = require "entities/FILENAME" (does not need the .lua filetype)
--		ENTITY_NAME.create()
