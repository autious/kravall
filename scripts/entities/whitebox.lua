return function(asm, x,y,z )

    asm:loadAssembly( 
    {
        {
            type = core.componentType.WorldPositionComponent,
            data = { position = { x,y,z } }
        },
        {
            type = core.componentType.GraphicsComponent,
            data = { render = true, mesh = 2000, material = 2000, type = core.gfx.objectTypes.OpaqueGeometry },
            load = { 
                        mesh = { core.loaders.GnomeLoader, "assets/model/test_whitebox.bgnome" },
                        material = { core.loaders.MaterialLoader, "assets/material/whitePixel.material" }
                   }
        
        },
        {
            type = core.componentType.ScaleComponent,
            data = { scale = 100.0 }
        },
        {
            type = core.componentType.RotationComponent,
            data = { rotation = { 0,0,0,1 } }
        }
    } 
    )

end
