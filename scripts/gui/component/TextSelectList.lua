local GUIComponent = require "gui/component/GUIComponent"
local image = require"factories/image"
local text = require"factories/text"

local TextSelectList = 
                {
                    x=0,
                    y=0,
                    xoffset=0,
                    yoffset=0,
                    width=0,
                    height=0,
                    padding=4,
                    elements = {{name = "default"}, {name = "default2"}}
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
                                            onPress = function()  o:onPress() end,
                                            onRelease = function() o:onRelease() end,
                                            onEnter = function() o:onEnter() end,
                                            onExit = function() o:onExit() end
                                        }

    o:updateList()

    return o
end

function TextSelectList:updateList()
    for _,v in pairs( self.textElements ) do
        v:destroy()        
    end

    self.textElements = {}

    local heightOffset = 0
    local widest = 0

    for k,v in pairs( self.elements ) do
        local text = text(self.x,self.y,v.name)
        local width, height = text:getDim()
        
        text:setPosition( self.x, self.y + heightOffset )
        heightOffset = heightOffset + height + self.padding

        self.textElements[#(self.textElements)+1] = text

        if width > widest then
            widest = width
        end
    end

    self.width = widest
    self.height = heightOffset
end

function TextSelectList:render()

end

function TextSelectList:setPosition(x,y)
    self.x = x;
    self.y = y 
    self:updateList()
end

function TextSelectList:onPress() 
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
    end

    self.textElements = nil
    self.GUIComponent:destroy()
end

return TextSelectList
