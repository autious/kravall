local GUIComponent = require "gui/component/GUIComponent"
local image = require"factories/image"

local Button = 
                {
                    x=0,
                    y=0,
                    xoffset=0,
                    yoffset=0,
                    show = true,
                    pressed = false,
                    enabled = true,
                    matReleased = "assets/texture/ui/button-release_00.material",
                    matPressed = "assets/texture/ui/button-press_00.material",
                    matHover = "assets/texture/ui/button-hover_00.material",
                    matDisable = "assets/texture/ui/button-disable_00.material",
                    onClick = function( self ) end
                }

function Button:new(o)
    o = o or {}
    setmetatable( o,self )
    self.__index = self
    
    o.pressedImg = image( o.x, o.y, o.matPressed )
    o.releasedImg = image( o.x, o.y, o.matReleased )
    o.hoverImg = image( o.x, o.y, o.matHover )
    o.disableImg = image( o.x, o.y, o.matDisable )
    
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
                                            onExit = function() o:onExit() end,
											toolTip = o.toolTip
                                        }

    o:updateVisual()

    o:setShow( o.show )

    return o
end

function Button:updateVisual()
    self.pressedImg:show( false )
    self.releasedImg:show( false )
    self.hoverImg:show( false )
    self.disableImg:show( false )

    if self.show then
        if self.enabled then
            if self.pressed then
               self.pressedImg:show( true )
            elseif self.hover then
               self.hoverImg:show( true ) 
            else
               self.releasedImg:show( true )
            end
        else
            self.disableImg:show( true )
        end
    end
end

function Button:setShow( flag )
    self.show = flag
    self.GUIComponent:setShow(flag)
    self.releasedImg:show(flag)
    
    self:updateVisual()
end

function Button:setEnabled( flag )
    self.enabled = flag
    self:updateVisual()
end

function Button:setPosition(x,y)
    self.GUIComponent:setPosition(x,y)
    self.pressedImg:setPosition(x,y)
    self.releasedImg:setPosition(x,y)
    self.hoverImg:setPosition(x,y)
    self.disableImg:setPosition(x,y)
    self:updateVisual()
end

function Button:onPress() 
    self.pressed = true
    self:updateVisual()
end

function Button:onRelease()
    self.pressed = false
    self:updateVisual()
    if self.show and self.enabled then
        self:onClick()
    end
end

function Button:onEnter()
    self.hover = true
    self:updateVisual()
end

function Button:onExit()
    self.hover = false
    self.pressed = false
    self:updateVisual()
end

function Button:destroy()
    self.pressedImg:destroy()
    self.pressedImg = nil
    self.releasedImg:destroy()
    self.releasedImg = nil
    self.hoverImg:destroy()
    self.hoverImg = nil
    self.disableImg:destroy()
    self.disableImg = nil
    self.GUIComponent:destroy()
end

return Button
