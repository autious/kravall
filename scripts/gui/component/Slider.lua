local GUIComponent = require "gui/component/GUIComponent"
local image = require"factories/image"

local Slider = 
                {
                    x=0,
                    y=0,
                    a=0,
                    matReleased = "assets/material/ui/slider_knob_release.material",
                    matPressed = "assets/material/ui/slider_knob_pressed.material",
                    matHover = "assets/material/ui/slider_knob_hover.material",
                    matBackground = "assets/material/ui/slider_background.material"
                
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

    o.onChange = o.onChange or function( value ) print ("knob: " .. value) end

    o.GUIComponent = GUIComponent:new
                                        {
                                            x=o.x,
                                            y=o.y,
                                            width=o.knobWidth,
                                            height=o.knobHeight,
                                            onDrag = function(x,y) print( "hej" ); o:onDrag(x,y) end,
                                            onPress = function()  o:onPress() end,
                                            onRelease = function() o:onRelease() end,
                                            onEnter = function() o:onEnter() end,
                                            onExit = function() o:onExit() end
                                        }

    o.pressedKnob.ent:set( core.componentType.GraphicsComponent, { render = false }, true )
    o.releasedKnob.ent:set( core.componentType.GraphicsComponent, { render = true }, true )
    o.hoverKnob.ent:set( core.componentType.GraphicsComponent, { render = false }, true )
    o.bg.ent:set( core.componentType.GraphicsComponent, { render = true }, true )

    return o
end

function Slider:onDrag( x, y )
    local fact = (x - self.x) / self.bgWidth
        
    if fact > 1 then
        fact = 1
    end

    if fact < 0 then
        fact = 0
    end

    self.onChange( fact ) 
    
    local newXPos = fact * self.bgWidth + self.x - self.knobWidth/2
    print( newXPos )

    self.GUIComponent.x = newXPos
    self.pressedKnob.ent:set( core.componentType.WorldPositionComponent, { position = { newXPos , -self.y, 0 } }, true )
    self.releasedKnob.ent:set( core.componentType.WorldPositionComponent, { position = { newXPos , -self.y, 0 } }, true )
    self.hoverKnob.ent:set( core.componentType.WorldPositionComponent, { position = { newXPos , -self.y, 0 } }, true )

end

function Slider:onPress() 
    self.pressedKnob.ent:set( core.componentType.GraphicsComponent, { render = true }, true )
    self.releasedKnob.ent:set( core.componentType.GraphicsComponent, { render = false }, true )
    self.hoverKnob.ent:set( core.componentType.GraphicsComponent, { render = false }, true )

end

function Slider:onRelease()
    self.pressedKnob.ent:set( core.componentType.GraphicsComponent, { render = false }, true )
    self.releasedKnob.ent:set( core.componentType.GraphicsComponent, { render = false }, true )
    self.hoverKnob.ent:set( core.componentType.GraphicsComponent, { render = true }, true )
end

function Slider:onEnter()
    self.pressedKnob.ent:set( core.componentType.GraphicsComponent, { render = false }, true )
    self.releasedKnob.ent:set( core.componentType.GraphicsComponent, { render = false }, true )
    self.hoverKnob.ent:set( core.componentType.GraphicsComponent, { render = true }, true )
end

function Slider:onExit()
    self.pressedKnob.ent:set( core.componentType.GraphicsComponent, { render = false }, true )
    self.releasedKnob.ent:set( core.componentType.GraphicsComponent, { render = true }, true )
    self.hoverKnob.ent:set( core.componentType.GraphicsComponent, { render = false }, true )
end

return Slider
