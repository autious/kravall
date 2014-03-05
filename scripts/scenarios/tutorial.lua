local window = require "window"
local MenuCamera = require "menu_camera"
local T = {}

function T:new(o)
    o = o or {}


	o.gui:addComponent(Button:new({
                                    matReleased = "assets/texture/ui/back-button-release.material",
                                    matPressed = "assets/texture/ui/back-button-press.material",
                                    matHover = "assets/texture/ui/back-button-hover.material",
                                    anchor="SouthWest",xoffset=0,yoffset=0,onClick = menuState.goMain }))

    o.camera = MenuCamera:new()
    setmetatable( o, self )
    self.__index = self
    

    return o
end

function T:update( delta )
    self.camera:update( delta ) 
end

function T:destroy()
end

function T:name()
    return "Menu"
end

return T
