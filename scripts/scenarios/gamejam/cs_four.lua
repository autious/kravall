local GUIComponent = require "gui/component/GUIComponent"
local image = require"factories/image"

local Button = 
                {
                    x=0,
                    y=0,
                    matReleased = "assets/material/gamejam/cs_four.material",
                    matPressed = "assets/material/gamejam/cs_four.material",
                    matHover = "assets/material/gamejam/cs_four.material"
                }

function Button:new(o)
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

function Button:setPosition(x,y)
    self.GUIComponent:setPosition(x,y)
    self.pressedImg:setPosition(x,y)
    self.releasedImg:setPosition(x,y)
    self.hoverImg:setPosition(x,y)
end

function Button:onPress() 
    self.pressedImg:show(true)
    self.releasedImg:show(false)
    self.hoverImg:show(false)

    self.onClick() 
end

function Button:onRelease()
    self.pressedImg:show(false)
    self.releasedImg:show(false)
    self.hoverImg:show(true)
end

function Button:onEnter()
    self.pressedImg:show( false )
    self.releasedImg:show( false )
    self.hoverImg:show( true )
end

function Button:onExit()
    self.pressedImg:show(false)
    self.releasedImg:show(true)
    self.hoverImg:show(false)
end

function Button:destroy()
    self.pressedImg:destroy()
    self.releasedImg:destroy()
    self.hoverImg:destroy()
    self.GUIComponent:destroy()
end

return Button
