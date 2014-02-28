


return function( )
{
    {
        type = core.componentType.WorldPositionComponent,
        data = { position = { 0, 1, 5 } }
    },
    {
        type = core.componentType.GraphicsComponent,
        data = { mesh = 0, material = 0, type = core.gfx.objectTypes.OpaqueGeometry,layer = core.gfx.layerTypes.MeshLayer, outlineColor = {0, 0, 1, 1}, render = true },
        load = { 
                    mesh = { core.loaders.GnomeLoader, "assets/model/animated/marker/dest-accept_00.bgnome", false },
                    material = { core.loaders.MaterialLoader, "assets/texture/animated/marker/dest-accept_00.material", false }
               }
    },
    {
        type = core.componentType.AnimationComponent,
        data = { 
            animationID = 0,
            queuedAnimationID = 0,
            currentTime = 0.0,
            loop = false,
            playing = false,
            speed = 1.0,
            currentFrame = 0
        }
    },
    {
        type = core.componentType.ScaleComponent,
        data = { scale = 1 }
    },
    {
        type = core.componentType.RotationComponent,
        data = { rotation = { 0, 0, 0, 1 }}
    }
}
)
