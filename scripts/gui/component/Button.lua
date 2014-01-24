local Button = {x=0,y=0}
local GUIBase = require "GUIBase"

function Button:new(o)
    o = o or {}
    setmetatable( o, self )
    self.__index = self
    return o
end


return{"model",{"material","material2"}}

return Button
