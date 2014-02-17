local GUIComponent = require "gui/component/GUIComponent"
local image = require"factories/image"

local Slider = 
                {
                    x=0,
                    y=0,
                    a=0,
                    xoffset=0,
                    yoffset=0,
                    width =0,
                    height = 0,
                    matReleased = "assets/texture/ui/slider-knob-release_00.material",
                    matPressed = "assets/texture/ui/slider-knob-press_00.material",
                    matHover = "assets/texture/ui/slider-knob-hover_00.material",
                    matBackground = "assets/texture/ui/slider-background_00.material",
                    onChange = function ( self, value ) end
                
                }

function Slider:new(o)
    o = o or {}
    setmetatable( o,self )
    self.__index = self
    
    o.pressedKnob = image( o.x, o.y, o.matPressed )
    o.releasedKnob = image( o.x, o.y, o.matReleased )
    o.hoverKnob = image( o.x, o.y, o.matHover )
    o.bg = image( o.x, o.y, o.matBackground )
    
    o.knobWidth = o.releasedKnob.width
    o.knobHeight = o.releasedKnob.height
    o.bgWidth = o.bg.width
    o.bgHeight = o.bg.height
    o.width = o.bg.width
    o.height = o.bg.height

    o.GUIComponent = GUIComponent:new
                                        {
                                            x=o.x,
                                            y=o.y,
                                            width=o.knobWidth,
                                            height=o.knobHeight,
                                            onDrag = function(x,y) o:onDrag(x,y) end,
                                            onPress = function()  o:onPress() end,
                                            onRelease = function() o:onRelease() end,
                                            onEnter = function() o:onEnter() end,
                                            onExit = function() o:onExit() end
                                        }

    o.pressedKnob:show( false )
    o.releasedKnob:show( true )
    o.hoverKnob:show( false )
    o.bg:show( true )

    return o
end

function Slider:setPosition(x,y)
    self.x = x
    self.y = y

    local newXPos = self.a * (self.bgWidth - self.knobWidth) + self.x 
    self.GUIComponent:setPosition(newXPos,y)
    self.pressedKnob:setPosition( newXPos, self.y + self.bgHeight / 2 - self.knobHeight / 2)
    self.releasedKnob:setPosition(newXPos , self.y + self.bgHeight / 2 - self.knobHeight / 2)
    self.hoverKnob:setPosition( newXPos , self.y + self.bgHeight / 2 - self.knobHeight / 2)
    self.bg:setPosition( self.x, self.y )
end

function Slider:onDrag( x, y )
    local fact = (x - (self.x + self.knobWidth / 2) ) / (self.bgWidth - self.knobWidth)
        
    if fact > 1 then
        fact = 1
    end

    if fact < 0 then
        fact = 0
    end

    self.a = fact

    self:onChange( fact ) 

    self:setPosition(self.x,self.y)
end

function Slider:onPress() 
    self.pressedKnob:show(true)
    self.releasedKnob:show(false)
    self.hoverKnob:show(false)

end

function Slider:onRelease()
    self.pressedKnob:show(false)
    self.releasedKnob:show(false)
    self.hoverKnob:show(true)
end

function Slider:onEnter()
    self.pressedKnob:show(false)
    self.releasedKnob:show(false)
    self.hoverKnob:show(true)
end

function Slider:onExit()
    self.pressedKnob:show(false)
    self.releasedKnob:show(true)
    self.hoverKnob:show(false)
end

function Slider:destroy()
    self.pressedKnob:destroy()
    self.releasedKnob:destroy()
    self.hoverKnob:destroy()
    self.bg:destroy()
    self.GUIComponent:destroy()
end

return Slider
