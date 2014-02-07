local Text = {}
local utility = require "utility"

function Text:destroy()
    self.ent:destroy()
    self.textId:free()
    self.textId = nil
    self.ent = nil
end

function Text:setText( string )
    self.string = string
    self.textId:reallocate( string )
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
    return self.textId:getDimensions()
    --return string.len( self.string ) * 10, 14
end

function Text:setColor( r,g,b,a )
    if type( r ) == "string" then
        r,g,b,a = utility.fromHtmlHex( r )
    end

    self.ent:set( core.componentType.HoverTextComponent, {color = {r,g,b,a} }, true )
end

Text.__index = Text

return function( x,y, string, color )
    local self = {}

    color = color or {1,1,1,1}
    
    setmetatable( self, Text )
    
    self.ent = core.entity.create( core.componentType.HoverTextComponent,
                              core.componentType.WorldPositionComponent )

    self.string = string
    self.textId = core.system.hoverText.string.new( string )

    self.ent:set( core.componentType.HoverTextComponent, {hoverTextId = self.textId, color = color} )

    self:setPosition( x,y )

    return self
end
