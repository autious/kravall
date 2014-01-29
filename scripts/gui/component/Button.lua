local GUIComponent = require "gui/component/GUIComponent"
local image = require"factories/image"

local Button = 
                {
                    x=0,
                    y=0,
                    matReleased = "assets/material/ui/button.material",
                    matPressed = "assets/material/ui/button_pressed.material",
                    matHover = "assets/material/ui/button_hover.material"
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

    o.pressedImg.ent:set( core.componentType.GraphicsComponent, { render = false }, true )
    o.releasedImg.ent:set( core.componentType.GraphicsComponent, { render = true }, true )
    o.hoverImg.ent:set( core.componentType.GraphicsComponent, { render = false }, true )

    return o
end

function Button:onPress() 
    self.pressedImg.ent:set( core.componentType.GraphicsComponent, { render = true }, true )
    self.releasedImg.ent:set( core.componentType.GraphicsComponent, { render = false }, true )
    self.hoverImg.ent:set( core.componentType.GraphicsComponent, { render = false }, true )
end

function Button:onRelease()
    self.pressedImg.ent:set( core.componentType.GraphicsComponent, { render = false }, true )
    self.releasedImg.ent:set( core.componentType.GraphicsComponent, { render = false }, true )
    self.hoverImg.ent:set( core.componentType.GraphicsComponent, { render = true }, true )
end

function Button:onEnter()
    self.pressedImg.ent:set( core.componentType.GraphicsComponent, { render = false }, true )
    self.releasedImg.ent:set( core.componentType.GraphicsComponent, { render = false }, true )
    self.hoverImg.ent:set( core.componentType.GraphicsComponent, { render = true }, true )
end

function Button:onExit()
    self.pressedImg.ent:set( core.componentType.GraphicsComponent, { render = false }, true )
    self.releasedImg.ent:set( core.componentType.GraphicsComponent, { render = true }, true )
    self.hoverImg.ent:set( core.componentType.GraphicsComponent, { render = false }, true )
end

return Button
