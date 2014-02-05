local objective_handler = require "objective_handler"
local objective = require "objective"
local fac_image = require "factories/image"
local window = require "window"
local Camera = require "rts_camera"
local T = {}

local selectedSquad = nil
local clickStartX, clickStartY, clickStartZ
local clickEndX, clickEndY, clickEndZ
local isClick;

core.movementData.setMovementMetaData( core.movementData.Idle, 0, 17, 17 )
core.movementData.setMovementMetaData( core.movementData.Walking, 5.8, 17, 17 )
core.movementData.setMovementMetaData( core.movementData.Sprinting, 8.8, 17, 14 )

function squadHandling()
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
    elseif core.input.mouse.isButtonDownOnce(core.input.mouse.button.Right) then
        if selectedSquad then
            local mouseX, mouseY = core.input.mouse.getPosition()
            clickStartX, clickStartY, clickStartZ = core.system.picking.getGroundHit(mouseX, mouseY);
            --core.system.groups.setGroupGoal(selectedSquad, groundX, groundY, groundZ);		
        end   
    elseif core.input.mouse.isButtonDown(core.input.mouse.button.Right) then        
        if selectedSquad and clickStartX and clickStartY and clickStartZ then
            local mouseX, mouseY = core.input.mouse.getPosition()
            local dragX, dragY, dragZ = core.system.picking.getGroundHit(mouseX, mouseY)    

            core.system.squad.previewSquadFormation(selectedSquad, core.system.squad.formations.LineFormation, clickStartX, clickStartY, clickStartZ, dragX, dragY, dragZ)

        end
    elseif core.input.mouse.isButtonUp(core.input.mouse.button.Right) then
        if selectedSquad and clickStartX and clickStartY and clickStartZ then
            local mouseX, mouseY = core.input.mouse.getPosition()
            clickEndX, clickEndY, clickEndZ = core.system.picking.getGroundHit(mouseX, mouseY)    

            core.system.squad.setSquadFormation(selectedSquad, core.system.squad.formations.LineFormation, clickStartX, clickStartY, clickStartZ, clickEndX, clickEndY, clickEndZ)
	        core.system.squad.setSquadGoal(selectedSquad, clickStartX, clickStartY, clickStartZ)

            clickStartX, clickStartY, clickStartZ = nil, nil, nil
        end
    end
end


function T:new(o)
    o = o or {}

    o.objectiveHandler = objective_handler:new()
    o.camera = Camera.new()

    setmetatable( o, self )
    self.__index = self

    return o
end

function T:init()
end

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

    squadHandling()

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
