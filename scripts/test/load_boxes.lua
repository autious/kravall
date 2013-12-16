
entities = {}

boxMeshId = -1

function callback( meshid )
    boxMeshId = meshid
end 

core.contentmanager.load( core.loaders.GnomeLoader, "assets/cube.gnome", callback, false )

if boxMeshId ~= -1 then
    for i = -100, 100 do
        for j = -10, 10 do
        local ent = core.entity.create( core.componentType.WorldPositionComponent,
                                                            core.componentType.GraphicsComponent,
                                                            core.componentType.ScaleComponent,
                                                            core.componentType.RotationComponent )

        ent:set( core.componentType.WorldPositionComponent, { position = { i*10,j*10,10,0 } } )
        ent:set( core.componentType.ScaleComponent, { scale = 0.1 } )
        ent:set( core.componentType.RotationComponent, { rotation = { 0,0,math.sin( 3.14 ), math.cos(3.14/2.0) } } )
        ent:set( core.componentType.GraphicsComponent, { mesh = boxMeshId } )
        
        entities[#entities+1] = ent
        end
    end
end
     
