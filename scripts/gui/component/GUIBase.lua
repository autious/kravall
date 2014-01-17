local GUIBase = {}

function GUIBase:destroy()
    self.mesh:free()
    self.material:free()
    self.ent:destroy()
end

function GUIBase:newindex( index, value )
    error( "Nope" )
end

local vComponentTypes = { core.componentType.GraphicsComponent,
                          core.componentType.WorldPositionComponent,
                          core.componentType.ScaleComponent,
                          core.componentType.RotationComponent}

function GUIBase.new()
    local self = {}

    GUIBase.createEntity( self )
    setmetatable( self, { __gc = GUIBase.destroy, __index = GUIBase} )

    return self
end

function GUIBase:createEntity()
    self.ent = core.entity.create( unpack( vComponentTypes ) )

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
        "assets/material/ui/test.material",
        function ( material_handle )
            -- Check if the value exist and if the entity is not destroyed.
            if self.ent and self.ent:isValid() then 
                self.ent:set( core.componentType.GraphicsComponent, { material = material_handle } )
            else
                error( "Unable to set VAO handle on Gui Component, invalid entity" )
            end
        end,
        false )

    self.ent:set( core.componentType.GraphicsComponent, { type = core.gfx.objectTypes.OverlayGeometry} )
    self.ent:set( core.componentType.WorldPositionComponent, { position = {10,-10, 0} } )
    self.ent:set( core.componentType.ScaleComponent, {scale = {100,100,1}} )
    self.ent:set( core.componentType.RotationComponent, { rotation = {0,0,0,1}} )
end

return GUIBase
