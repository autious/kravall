local fac_image = require "factories/image"
local window = require "window"
local Camera = require "rts_camera" 
local T = {}

function T:new(o)
    o = o or {}

    o.camera = Camera.new()

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
    return "Normal"
end

return T
