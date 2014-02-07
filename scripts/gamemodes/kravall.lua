local objective_handler = require "objective_handler"
local objective = require "objective"
local fac_image = require "factories/image"
local window = require "window"
local Camera = require "rts_camera"
local T = {}

local selectedSquads = {}
local clickStartX, clickStartY, clickStartZ
local clickEndX, clickEndY, clickEndZ
local isClick;

local boxStartX, boxStartY, boxEndX, boxEndY
local groupsSelectedByBox = {}

core.movementData.setMovementMetaData( core.movementData.Idle, 0, 17, 17 )
core.movementData.setMovementMetaData( core.movementData.Walking, 5.8, 17, 17 )
core.movementData.setMovementMetaData( core.movementData.Sprinting, 8.8, 17, 14 )

function squadHandling()
    if core.input.mouse.isButtonDownOnce(core.input.mouse.button.Left) then
		boxStartX, boxStartY = core.input.mouse.getPosition()
        selectedEntity = core.system.picking.getLastHitEntity()
        if selectedEntity then
            local unitTypeComponent = selectedEntity:get(core.componentType.UnitTypeComponent);
            local attributeComponent = selectedEntity:get(core.componentType.AttributeComponent);
            if attributeComponent and unitTypeComponent then                   
                if unitTypeComponent.unitType == core.UnitType.Police then
                    if core.input.keyboard.isKeyDown(core.input.keyboard.key.Left_shift) then
                        local found = false

                        for i=1, #selectedSquads do
                            if selectedSquads[i] == attributeComponent.squadID then
                                found = true
                            end
                        end

                        if not found then                            
                            selectedSquads[#selectedSquads+1] = attributeComponent.squadID
                            print(selectedSquads[#selectedSquads])
                        end
                    else
                        selectedSquads = {};
                        selectedSquads[#selectedSquads+1] = attributeComponent.squadID
                        --print(selectedSquads[#selectedSquads])
                    end
                end
            end
		elseif not core.input.keyboard.isKeyDown(core.input.keyboard.key.Left_shift) and not core.config.stickySelection then
			selectedSquads = {}
        end   
	elseif core.input.mouse.isButtonDown(core.input.mouse.button.Left) then
		boxEndX, boxEndY = core.input.mouse.getPosition()
		groupsSelectedByBox = core.system.picking.getPoliceGroupsInsideBox( boxStartX, boxStartY, boxEndX, boxEndY, core.config.boxSelectionGraceDistance )
    elseif core.input.mouse.isButtonDownOnce(core.input.mouse.button.Right) then
        if #selectedSquads > 0 then
            local mouseX, mouseY = core.input.mouse.getPosition()
            clickStartX, clickStartY, clickStartZ = core.system.picking.getGroundHit(mouseX, mouseY);
        end   
    elseif core.input.mouse.isButtonDown(core.input.mouse.button.Right) then        
        if #selectedSquads > 0 and clickStartX and clickStartY and clickStartZ then
            local mouseX, mouseY = core.input.mouse.getPosition()
            local dragX, dragY, dragZ = core.system.picking.getGroundHit(mouseX, mouseY)    

            core.system.squad.previewSquadFormation(selectedSquads, core.system.squad.formations.LineFormation, clickStartX, clickStartY, clickStartZ, dragX, dragY, dragZ)
        end
    elseif core.input.mouse.isButtonUp(core.input.mouse.button.Right) then
        if #selectedSquads > 0 and clickStartX and clickStartY and clickStartZ then
            local mouseX, mouseY = core.input.mouse.getPosition()
            clickEndX, clickEndY, clickEndZ = core.system.picking.getGroundHit(mouseX, mouseY)    
            core.system.squad.setSquadFormation(selectedSquads, core.system.squad.formations.LineFormation, clickStartX, clickStartY, clickStartZ, clickEndX, clickEndY, clickEndZ)
	        core.system.squad.setSquadGoal(selectedSquads, clickStartX, clickStartY, clickStartZ)

            clickStartX, clickStartY, clickStartZ = nil, nil, nil
        end
    end

    if core.input.keyboard.isKeyDown(core.input.keyboard.key.I)  then        
        if #selectedSquads > 0 then
            core.system.squad.setSquadStance(selectedSquads, core.PoliceStance.Aggressive)
        end   
    else
        core.system.squad.setSquadStance(selectedSquads, core.PoliceStance.Passive)
    end
	
	-- box select
	if boxStartX and boxStartY and boxEndX and boxEndY and core.input.mouse.isButtonUp(core.input.mouse.button.Left) then
		if boxStartX ~= boxEndX and boxStartY ~= boxEndY then
			if not core.input.keyboard.isKeyDown(core.input.keyboard.key.Left_shift) then
				selectedSquads = {}
			end
			if groupsSelectedByBox then
				for p = 1, #groupsSelectedByBox do
					local found = false
					for i=1, #selectedSquads do
						if selectedSquads[i] == groupsSelectedByBox[p] then
							found = true
						end
					end
					
					if not found then                            
						selectedSquads[#selectedSquads+1] = groupsSelectedByBox[p]
					end			
				end
				groupsSelectedByBox = {}
			elseif not core.config.stickySelection then
				selectedSquads = {}
			end
		end			
		boxStartX, boxStartY, boxEndX, boxEndY = nil, nil, nil, nil
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
