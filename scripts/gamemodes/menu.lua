local window = require "window"
local MenuCamera = require "menu_camera"
local T = {}

function T:new(o)
    o = o or {}

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
