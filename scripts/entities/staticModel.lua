return function(asm,x,y,z,qx,qy,qz,qw,scale,model,material,radius,name)
    local components = {
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
        },
        {
            type = core.componentType.NameComponent,
            data = { name = name }
        }
    }
    radius = 1
    print(#components)
    if radius then
        components[#components + 1] = {
            type = core.componentType.BoundingVolumeComponent,
            data = { sphereOffset = { 0, 0, 0 }, sphereRadius = radius, 
                    collisionModel = core.BoundingVolumeCollisionModel.StaticResolution, 
                    type = core.BoundingVolumeType.SphereBoundingType }
        }
    end
    print(#components)

    return asm:loadAssembly( components )

end
