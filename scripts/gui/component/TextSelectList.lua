local GUIComponent = require "gui/component/GUIComponent"
local image = require"factories/image"

local TextSelectList = 
                {
                    x=0,
                    y=0,
                    xoffset=0,
                    yoffset=0,
                    elements = { { name = "default" } }
                }

function TextSelectList:new(o)
    o = o or {}
    setmetatable( o,self )
    self.__index = self
    
    o.pressedImg = image( o.x, o.y, o.matPressed )
    o.releasedImg = image( o.x, o.y, o.matReleased )
    o.hoverImg = image( o.x, o.y, o.matHover )
    
    o.width = o.pressedImg.width
    o.height = o.pressedImg.height

    o.onClick = o.onClick or function()  end

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

    o.pressedImg:show(false)
    o.releasedImg:show(true)
    o.hoverImg:show(false)

    return o
end

function TextSelectList:updateList()
    for k,v in pairs( self.elements ) do
          
    end
end

function TextSelectList:render()

end

function TextSelectList:setPosition(x,y)
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
    self.GUIComponent:destroy()
end

return TextSelectList
