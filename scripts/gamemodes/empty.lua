local window = require "window"
local Camera = require "rts_camera"
local T = {}

local keyboard = core.input.keyboard
local mouse = core.input.mouse


function T:new(o)
    o = o or {}

    setmetatable( o, self )
    self.__index = self

    o.camera = Camera.new()

    return o
end

function T:init()
end

function T:update( delta )
    self.camera:update( delta )
end

function T:destroy()
end

function T:name()
    return "Empty"
end

return T
