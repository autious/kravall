local Button = {}

local GUIBase = require "GUIBase"


function Button.new()
    local self = {}
    setmetatable( self, { __index = GUIBase } )

    
end

return Button
