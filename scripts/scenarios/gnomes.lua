-- Loads 4221 gnomes onto the screen.
local ASML = require "assembly_loader" 

local asm = ASML.loadPack( {} )

local haus = require "entities/haus"

haus.create( asm,-6,0,-5 )
haus.create( asm,-3,0,5 )
haus.create( asm,3,0,-5 )
haus.create( asm,3,0,5)

asm:loadAssembly( 
{
    {
        type = core.componentType.WorldPositionComponent,
        data = { position = { 0, -0.1,0} }
    },
    {
        type = core.componentType.GraphicsComponent,
        data = { mesh = 2000, material = 2000, type = core.gfx.objectTypes.OpaqueGeometry },
        load = { 
                    mesh = { core.loaders.GnomeLoader, "assets/plane.bgnome" },
                    material = { core.loaders.MaterialLoader, "assets/material/test-material.material" }
               }
    
    },
    {
        type = core.componentType.ScaleComponent,
        data = { scale = 1.0 }
    },
    {
        type = core.componentType.RotationComponent,
        data = { rotation = { 0,0, 0, 1 } }
    }
} )

asm:loadAssembly( 
{
    {
        type = core.componentType.WorldPositionComponent,
        data = { position = { 0, 0,0} }
    },
    {
        type = core.componentType.GraphicsComponent,
        data = { mesh = 2000, material = 2000, type = core.gfx.objectTypes.OpaqueGeometry },
        load = { 
                    mesh = { core.loaders.GnomeLoader, "assets/demo_path.bgnome" },
                    material = { core.loaders.MaterialLoader, "assets/material/asphalt.material" }
               }
    
    },
    {
        type = core.componentType.ScaleComponent,
        data = { scale = 1.0 }
    },
    {
        type = core.componentType.RotationComponent,
        data = { rotation = { 0,0, 0, 1 } }
    }
} )

collectgarbage() --For niceness, always good to do right after loading a scenario as the
                 --assembly files are quite large.
return asm;
