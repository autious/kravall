-- Create an internal table for simulating an object in Lua. Return it at the end to get a pointer to this object
local t = {}

-- Must be prefixed by the internal table name. Must have the name create (so that it is always the same) and the parameter asm
function t.create(asm, x,y,z )

    asm:loadAssembly( 
    {
        {
            type = core.componentType.WorldPositionComponent,
            data = { position = { x,y,z } }
        },
        {
            type = core.componentType.GraphicsComponent,
            data = { mesh = 2000, material = 2000, type = core.gfx.objectTypes.OpaqueGeometry },
            load = { 
                        mesh = { core.loaders.GnomeLoader, "assets/tower.bgnome" },
                        material = { core.loaders.MaterialLoader, "assets/material/simple.material" }
                   }
        
        },
        {
            type = core.componentType.ScaleComponent,
            data = { scale = 1.0 }
        },
        {
            type = core.componentType.RotationComponent,
            data = { rotation = { 0,0,0,1 } }
        }
    } 
    )

end

return t

-- This entity is loaded by adding the rows:
-- 		local ENTITY_NAME = require "entities/FILENAME" (does not need the .lua filetype)
--		ENTITY_NAME.create()
