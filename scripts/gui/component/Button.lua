local GUIComponent = require "gui/component/GUIComponent"
local image = require"factories/image"

local Button = 
                {
                    x=0,
                    y=0,
                    matReleased = "assets/material/ui/button.material",
                    matPressed = "assets/material/ui/button_pressed.material" 
                }

function Button:new(o)
    o = o or {}
    setmetatable( o,self )
    self.__index = self
    
    o.pressedImg = image( o.x, o.y, o.matPressed )
    o.releasedImg = image( o.x, o.y, o.matReleased )
    
    o.width = o.pressedImg.width
    o.height = o.pressedImg.height

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
    return o
end

function Button:onPress()
     
end

return Button
