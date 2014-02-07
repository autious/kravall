return function(asm, x,y,z )

    asm:loadAssembly( 
    {
        {
            type = core.componentType.WorldPositionComponent,
            data = { position = { x,y,z } }
        },
        {
            type = core.componentType.GraphicsComponent,
            data = { render = true, mesh = 2000, material = 2000, type = core.gfx.objectTypes.OpaqueGeometry, layer = core.gfx.layerTypes.MeshLayer, outlineColor = {0, 0, 0, 0} },
            load = { 
                        mesh = { core.loaders.GnomeLoader, "assets/haus.bgnome" },
                        material = { core.loaders.MaterialLoader, "assets/material/brick.material" }
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
