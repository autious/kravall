local GC = {}

local vComponentTypes = { core.componentType.GraphicsComponent,
                          core.componentType.WorldPositionComponent,
                          core.componentType.ScaleComponent,
                          core.componentType.RotationComponent }

require "gui/viewport"

function GC:destroy()
    self.mesh:free()
    self.material:free()
    self.ent:destroy()
end

function GC:newindex( index, value )
    error( "Nope" )
end

function GC.new()
    local self = {}
    self.ent = core.entity.create( unpack( vComponentTypes ) )

    self.mesh = core.contentmanager.load( 
        core.loaders.GnomeLoader, 
        "assets/cube.bgnome", 
        function( vao_handle )
            -- Check if the value exist and if the entity is not destroyed.
            if self.ent and self.ent:isValid() then 
                self.ent:set( core.componentType.GraphicsComponent, { mesh = vao_handle } )
            else
                print( "Unable to set VAO handle on Gui Component, invalid entity" )
            end
        end, 
        false )

    self.material = core.contentmanager.load( 
        core.loaders.MaterialLoader, 
        "assets/material/cop.material",
        function ( material_handle )
            -- Check if the value exist and if the entity is not destroyed.
            if self.ent and self.ent:isValid() then 
                self.ent:set( core.componentType.GraphicsComponent, { material = material_handle } )
            else
                print( "Unable to set VAO handle on Gui Component, invalid entity" )
            end
        end,
        false )



    self.ent:set( core.componentType.GraphicsComponent, { type = core.gfx.objectTypes.OverlayGeometry} )
    self.ent:set( core.componentType.WorldPositionComponent, { position = {0,0, -1} } )
    self.ent:set( core.componentType.ScaleComponent, {scale = {1000,1000,0.1}} )
    self.ent:set( core.componentType.RotationComponent, { rotation = {0,0,0,1}} )

    setmetatable( self, { __gc = GC.destroy, __index = GC} )

    return self
end

return GC
