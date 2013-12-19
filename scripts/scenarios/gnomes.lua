-- Loads 4221 gnomes onto the screen.
local ASML = require "assembly_loader" 

local asm = ASML.loadPack( {} )

for i = -10, 10 do
    for j = -10, 10 do
        asm:loadAssembly( 
        {
            {
                type = core.componentType.WorldPositionComponent,
                data = { position = { i*10, j*10,10 } }
            },
            {
                type = core.componentType.GraphicsComponent,
                data = { mesh = 0 },
                load = { mesh = { core.loaders.GnomeLoader, "assets/teapot.bgnome" } }
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

collectgarbage() --For niceness, always good to do right after loading a scenario as the
                 --assembly files are quite large.
return asm;
