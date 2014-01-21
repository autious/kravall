local objective_handler = require "objective_handler"
local objective = require "objective"
local T = {}

function T.new()
    local self = {}
    self.objectiveHandler = objective_handler.new()
    self.objectiveHandler:addObjective(objective.new("Kill your people"))

    setmetatable( self, { __index = T } )

    return self
end

function T:update( delta )
    self.objectiveHandler:update( delta )
end

function T:destroy()
    
end

function T:name()
    return "Normal"
end

return T
