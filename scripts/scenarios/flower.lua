local ASML = require "assembly_loader" 
local scen = require "scenario":new()
local keyboard = core.input.keyboard
local key = keyboard.key

scen.gamemode = require "gamemodes/particle_editor":new()

scen:registerUpdateCallback( function(delta) scen.gamemode:update(delta) end )
scen:registerDestroyCallback( function() scen.gamemode:destroy() end )
scen:registerInitCallback( function() scen.gamemode:init() end )

scen.gamemode.camera:lookAt( core.glm.vec3.new( 0, 60, 120 ), core.glm.vec3.new( 0, 20, 50 ) )
--core.gfx.setWhitepoint(1.0, 1.0, 1.0)

scen.name = "Particle editor"
scen.description = "Edit particles by using the sliders (2particle4me)"
--Plane
function scen:load()

    scen.asm:loadAssembly( 
    {
        {
            type = core.componentType.WorldPositionComponent,
            data = { position = { 0, 0, 0 } }
        },
        {
            type = core.componentType.GraphicsComponent,
            data = { mesh = 0, material = 0, type = core.gfx.objectTypes.OpaqueGeometry, layer = core.gfx.layerTypes.MeshLayer, outlineColor = {0, 0, 1, 1}, render = true },
            load = { 
                        mesh = { core.loaders.GnomeLoader, "assets/plane.bgnome", false },
                        material = { core.loaders.MaterialLoader, "assets/material/light_test.material", false }
                   }
        },
        {
            type = core.componentType.ScaleComponent,
            data = { scale = 150.0 }
        },
        {
            type = core.componentType.RotationComponent,
            data = { rotation = { -math.sin(math.pi/4.0), 0, 0, math.cos(math.pi/4.0) } }
        }
    })

    -- Spotlight
    light = scen.asm:loadAssembly( 
    {
        {
            type = core.componentType.LightComponent,
            data =  { 
                        color = { 1, 1, 1 },
                        speccolor = { 1, 1, 1},
                        intensity = 0.8,
                        spotangle = 3.14/4.0,
                        spotpenumbra = 0.03,
                        type = core.gfx.objectTypes.Light,
                        lighttype = core.gfx.lightTypes.Dir
                    }
        },
        {
            type = core.componentType.WorldPositionComponent,
            data = { position = { 0, 50, 0 } }
        },
        {
            type = core.componentType.ScaleComponent,
            data = { scale = 150 }
        },
        {
            type = core.componentType.RotationComponent,
            data = { rotation = {1,-0.7,-1,0 } }
        }
    })
    
end

local function Update(delta)
end

scen:registerUpdateCallback( Update )

return scen;
