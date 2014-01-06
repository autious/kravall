local ASML = require "assembly_loader" 

local asm = ASML.loadPack( {} )

for i = -10, 9 do
    for j = -10, 9 do
        asm:loadAssembly( 
        {
            {
                type = core.componentType.WorldPositionComponent,
                data = { position = { i*20, j*20,0 } }
            },
            {
                type = core.componentType.GraphicsComponent,
                data = { mesh = 0, material = 0, type = core.gfx.objectTypes.OpaqueGeometry },
                load = { 
                            mesh = { core.loaders.GnomeLoader, "assets/teapot.bgnome" },
                            material = { core.loaders.MaterialLoader, "assets/material/test-material.material" }
                       }
            },
            {
                type = core.componentType.ScaleComponent,
                data = { scale = 0.1 }
            },
            {
                type = core.componentType.RotationComponent,
                data = { rotation = { 0,0, math.sin( 3.14 ), math.cos(3.14/2.0) } }
            }
        } 
        )
    end
end

for i = 0, 127 do
    asm:loadAssembly( 
    {
        {
            type = core.componentType.LightComponent,
            data =  { 
                        color = { math.random(), math.random(), math.random() },
                        intensity = 1.3,
                        type = core.gfx.objectTypes.Light,
                        lighttype = core.gfx.lightTypes.Point
                    }
        },
        {
            type = core.componentType.WorldPositionComponent,
            data = { position = { -200.0 + 300.0 * math.random(), -400.0 + 600.0 * math.random(), 17.0 } }
        },
        {
            type = core.componentType.ScaleComponent,
            data = { scale = 15.0 + 5.0 * math.random() }
        },
        {
            type = core.componentType.RotationComponent,
            data = { rotation = { 0,0,0,0 } }
        }
    } 
    )
end

return asm;
