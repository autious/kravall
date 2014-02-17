local GUIComponent = require "gui/component/GUIComponent"
local image = require"factories/image"

local Button = 
                {
                    x=0,
                    y=0,
                    xoffset=0,
                    yoffset=0,
                    matReleased = "assets/texture/ui/button-release_00.material",
                    matPressed = "assets/texture/ui/button-press_00.material",
                    matHover = "assets/texture/ui/button-hover_00.material",
                    onClick = function( self ) end
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
end

function Button:onRelease()
    self.pressedImg:show(false)
    self.releasedImg:show(false)
    self.hoverImg:show(true)

    self:onClick()
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
