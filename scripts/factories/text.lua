local Text = {}

function Text:destroy()
      
end
--
--function Text:setText( string )
--
--end
--
--function Text:setPosition( x,y )
--
--end

Text.__index = Text

return function( x,y, string )
    local self = {}
    
    setmetatable( self, Text )
    
    self.ent = core.entity.create( core.componentType.HoverTextComponent,
                              core.componentType.WorldPositionComponent )

    self.textId = core.system.hoverText.string.new( string )

    self.ent:set( core.componentType.HoverTextComponent, {hoverTextId = self.textId} )
    self.ent:set( core.componentType.WorldPositionComponent, { position = {x,-y,0 }})

    return self
end
