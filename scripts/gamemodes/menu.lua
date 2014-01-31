local window = require "window"
local T = {}

function T:new(o)
    o = o or {}

    setmetatable( o, self )
    self.__index = self

    return o
end

function T:update( delta )
end

function T:destroy()
end

function T:name()
    return "Menu"
end

return T
