return function( x,y, material )  
    local self = {}
    setmetatable( self,{ __index = { destroy = function(self)
                self.ent:destroy()
                self.mesh:free()
                self.material:free()
            end
        }
    })

    self.ent = core.entity.create( core.componentType.GraphicsComponent,
                              core.componentType.WorldPositionComponent,
                              core.componentType.ScaleComponent,
                              core.componentType.RotationComponent )

    self.mesh = core.contentmanager.load( 
        core.loaders.GnomeLoader, 
        "assets/model/ui/overlay.bgnome", 
        function( vao_handle )
            -- Check if the value exist and if the entity is not destroyed.
            if self.ent and self.ent:isValid() then 
                self.ent:set( core.componentType.GraphicsComponent, { mesh = vao_handle } )
            else
                error( "Unable to set VAO handle on Gui Component, invalid entity" )
            end
        end, 
        false )

    self.material = core.contentmanager.load( 
        core.loaders.MaterialLoader, 
        material,
        function ( material_handle )
            -- Check if the value exist and if the entity is not destroyed.
            if self.ent and self.ent:isValid() then 
                self.ent:set( core.componentType.GraphicsComponent, { material = material_handle } )
                self.ent:set( core.componentType.ScaleComponent, {scale = {150,122,1}} )
            else
                error( "Unable to set VAO handle on Gui Component, invalid entity" )
            end
        end,
        false )

    self.ent:set( core.componentType.GraphicsComponent, { type = core.gfx.objectTypes.OverlayGeometry} )
    self.ent:set( core.componentType.WorldPositionComponent, { position = {x,-y, 0} } )
    self.ent:set( core.componentType.RotationComponent, { rotation = {0,0,0,1}} )

    return self
end 
