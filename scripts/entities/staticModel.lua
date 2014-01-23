return function(asm,x,y,z,qx,qy,qz,qw,scale,model,material)

    return asm:loadAssembly( 
    {
        {
            type = core.componentType.WorldPositionComponent,
            data = { position = { x,y,z } }
        },
        {
            type = core.componentType.GraphicsComponent,
            data = { mesh = 2000, material = 2000, type = core.gfx.objectTypes.OpaqueGeometry },
            load = { 
                        mesh = { core.loaders.GnomeLoader, model },
                        material = { core.loaders.MaterialLoader, material }
                   }
        
        },
        {
            type = core.componentType.ScaleComponent,
            data = { scale = scale }
        },
        {
            type = core.componentType.RotationComponent,
            data = { rotation = { qx,qy,qz,qw } }
        }
    } 
    )

end
