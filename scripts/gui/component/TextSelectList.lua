local GUIComponent = require "gui/component/GUIComponent"
local image = require"factories/image"
local text = require"factories/text"
local AABB = require"collision/aabb"

local TextSelectList = 
                {
                    x=0,
                    y=0,
                    xoffset=0,
                    yoffset=0,
                    width=0,
                    height=0,
                    padding=4,
                    show=true,
                    elements = {{name = "default"}, {name = "default2"}},
                    onSelect = function() end
                }

function TextSelectList:new(o)
    o = o or {}
    setmetatable( o,self )
    self.__index = self

    o.onClick = o.onClick or function()  end

    o.textElements = {}

    o.GUIComponent = GUIComponent:new
                                        {
                                            x=o.x,
                                            y=o.y,
                                            width=o.width,
                                            height=o.height,
                                            onPress = function(x,y)  o:onPress(x,y) end,
                                            onRelease = function() o:onRelease() end,
                                            onEnter = function() o:onEnter() end,
                                            onExit = function() o:onExit() end
                                        }

    o:setShow( o.show )
    o:updateList()

    return o
end

function TextSelectList:setShow( flag )
    self.show = flag

    self.GUIComponent:setShow(flag)

    for _,v in pairs( self.textElements ) do
        v:show(flag)
    end
end

function TextSelectList:updateList()
    for _,v in pairs( self.textElements ) do
        v:destroy()        
        v = nil
    end

    self.textElements = {}
     
    local heightOffset = 0
    local widest = 0

    for k,v in pairs( self.elements ) do
        local text1 = text(self.x,self.y,v.name)
        text1:show(self.show)
        local width, height = text1:getDim()
        
        text1:setPosition( self.x, self.y + heightOffset )
        heightOffset = heightOffset + height + self.padding

        text1.object = v
        self.textElements[#(self.textElements)+1] = text1

        if width > widest then
            widest = width
        end
    end

    self.width = widest
    self.height = heightOffset
    self.GUIComponent.width = self.width
    self.GUIComponent.height = self.height

    self:colorActive()
end

function TextSelectList:render()

end

function TextSelectList:setPosition(x,y)
    self.x = x;
    self.y = y 
    self.GUIComponent.x = self.x
    self.GUIComponent.y = self.y
    self:updateList()
end

function TextSelectList:onPress(x,y) 
    if self.show then
        local newActive = nil

        for k,v in pairs( self.textElements ) do
            local width,height = v:getDim()
            if AABB:new({v.x,v.y,width,height}):collides( x, y ) then
                newActive = v.object
            end
        end

        if newActive ~= nil and newActive ~= self.activeObject then
            self:setActive( newActive )
        end
    end
end

function TextSelectList:setActive( object )
    self.activeObject = object
    self.onSelect( object )
    self:colorActive()
end

function TextSelectList:colorActive()
    for k,v in pairs( self.textElements ) do
        if v.object == self.activeObject then
            v:setColor( "#f3ff2c" )
        else
            v:setColor( "#ffffff" )
        end
    end
end

function TextSelectList:onRelease()
end

function TextSelectList:onEnter()
end

function TextSelectList:onExit()
end

function TextSelectList:destroy()
    for _,v in pairs( self.textElements ) do
        v:destroy()        
        v = nil
    end

    self.textElements = nil
    self.GUIComponent:destroy()
end

return TextSelectList
