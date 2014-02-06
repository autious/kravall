local Text = {}

function Text:destroy()
    self.ent:destroy()
    self.textId:free()
end

function Text:setText( string )
    self.string = string
    self.textId = core.system.hoverText.string.reallocate( self.textId, string )
end

function Text:setPosition( x,y )
    local width,height = self:getDim()
    self.x = x
    self.y = y
    self.ent:set( core.componentType.WorldPositionComponent, { position = {self.x,-(self.y + height),0 }})
end

function Text:getPosition( x,y )
    return self.x, self.y
end 

function Text:getDim( )
    return string.len( self.string ) * 10, 14
end

Text.__index = Text

return function( x,y, string )
    local self = {}
    
    setmetatable( self, Text )
    
    self.ent = core.entity.create( core.componentType.HoverTextComponent,
                              core.componentType.WorldPositionComponent )

    self.string = string
    self.textId = core.system.hoverText.string.new( string )

    self.ent:set( core.componentType.HoverTextComponent, {hoverTextId = self.textId} )

    self:setPosition( x,y )

    return self
end
