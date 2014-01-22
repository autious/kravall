local objective_handler = require "objective_handler"
local objective = require "objective"
local fac_image = require "factories/image"
local T = {}

function T.new()
    local self = {}

    self.objectiveHandler = objective_handler.new()
    setmetatable( self, { __index = T } )
    return self
end

function T:update( delta )
    self.objectiveHandler:update( delta )

--    if self.objectiveHandler.isWin() then
--        self.popup = fac_image( x, y, "assets/material/ui/test.material" )
--    elseif self.objectiveHandler.isLoss() then
--        self.popup = fac_image( 0,0, "assets/material/ui/test.material" ) 
--    end
end

function T:destroy()
    self.objectiveHandler:destroy() 
end

function T:name()
    return "Normal"
end

return T
