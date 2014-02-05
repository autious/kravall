local objective_handler = require "objective_handler"
local objective = require "objective"
local fac_image = require "factories/image"
local window = require "window"
local Camera = require "rts_camera" 
local T = {}

function T:new(o)
    o = o or {}

    o.objectiveHandler = objective_handler:new()
    o.camera = Camera.new()

    setmetatable( o, self )
    self.__index = self

    return o
end

function T:update( delta )
    self.objectiveHandler:update( delta )

    if self.popup == nil then
        if self.objectiveHandler:isWin() then
            self.popup = fac_image( window.width/2,window.height/2, "assets/material/ui/win.material",true )
        elseif self.objectiveHandler:isLoss() then
            self.popup = fac_image( window.width/2,window.height/2, "assets/material/ui/loss.material",true ) 
        end
    end
    self.camera:update( delta )
end

function T:destroy()
    self.objectiveHandler:destroy() 
    if self.popup ~= nil then
        self.popup:destroy()
    end
end

function T:name()
    return "Normal"
end

return T
