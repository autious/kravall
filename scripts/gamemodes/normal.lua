local objective_handler = require "objective_handler"
local objective = require "objective"
local T = {}

function T.new()
    local self = {}

    self.objectiveHandler = objective_handler.new()
    setmetatable( self, { __index = T } )
    return self
end

function T:update( delta )
    self.objectiveHandler:update( delta )
end

function T:destroy()
    self.objectiveHandler:destroy() 
end

function T:name()
    return "Normal"
end

return T
