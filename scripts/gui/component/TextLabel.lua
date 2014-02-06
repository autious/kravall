local GUIComponent = require "gui/component/GUIComponent"
local image = require"factories/image"
local text = require"factories/text"

local TextLabel = 
                {
                    x=0,
                    y=0,
                    xoffset=0,
                    yoffset=0,
                    width=0,
                    height=0,
                    padding=4,
                    label = "default" 
                }

function TextLabel:new(o)
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
    o.text = text( o.x, o.y, o.label)
    o.width, o.height = o.text:getDim()
    o:setPosition( o.x, o.y )

    return o
end

function TextLabel:updateList()
    for _,v in pairs( self.textElements ) do
        v:destroy()        
    end

    self.textElements = {}

    local heightOffset = 0
    local widest = 0

    for k,v in pairs( self.elements ) do
        heightOffset = heightOffset + height + self.padding

        self.textElements[#(self.textElements)+1] = text

        if width > widest then
            widest = width
        end
    end

    self.width = widest
    self.height = heightOffset
end

function TextLabel:render()

end

function TextLabel:setPosition( x, y )
    self.x = x;
    self.y = y 
    
    self.text:setPosition( self.x, self.y )
end

function TextLabel:onPress() 
end

function TextLabel:onRelease()
end

function TextLabel:onEnter()
end

function TextLabel:onExit()
end

function TextLabel:destroy()
    self.text:destroy()
    self.GUIComponent:destroy()
end

return TextLabel
