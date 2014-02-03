local objective_handler = require "objective_handler"
local objective = require "objective"
local fac_image = require "factories/image"
local window = require "window"
local Camera = require "rts_camera"
local T = {}

local selectedSquad = nil

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

    if core.input.mouse.isButtonDown(core.input.mouse.button.Left) then        
        selectedEntity = core.system.picking.getLastHitEntity()
        if selectedEntity then
            local unitTypeComponent = selectedEntity:get(core.componentType.UnitTypeComponent);
            local attributeComponent = selectedEntity:get(core.componentType.AttributeComponent);
            if attributeComponent and unitTypeComponent then                   
                if unitTypeComponent.unitType == core.UnitType.Police then
                    selectedSquad = attributeComponent.squadID
                else
                    selectedSquad = nil
                end
            end
        end
    elseif core.input.mouse.isButtonDown(core.input.mouse.button.Right) then
        if selectedSquad then
            local mouseX, mouseY = core.input.mouse.getPosition()
            local groundX, groundY, groundZ = core.system.picking.getGroundHit(mouseX, mouseY);
            core.system.groups.setGroupGoal(selectedSquad, groundX, groundY, groundZ);            
        end        
    end

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
    return "Kravall"
end

return T
