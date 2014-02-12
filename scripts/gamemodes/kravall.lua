local objective_handler = require "objective_handler"
local objective = require "objective"
local fac_image = require "factories/image"
local window = require "window"
local Camera = require "rts_camera"
local T = {}

local selectedSquads = {}
local clickStartX, clickStartY, clickStartZ
local clickEndX, clickEndY, clickEndZ
local selectedFormation
local isClick;

local boxStartX, boxStartY, boxEndX, boxEndY
local groupsSelectedByBox = {}
local previousGroupsSelectedByBox = {}

local keyboard = core.input.keyboard
local mouse = core.input.mouse
local s_squad = core.system.squad

core.movementData.setMovementMetaData( core.movementData.Idle, 0, 17, 17 )
core.movementData.setMovementMetaData( core.movementData.Walking, 5.8, 17, 17 )
core.movementData.setMovementMetaData( core.movementData.Sprinting, 8.8, 17, 14 )

-- usual weapons...
fists = core.weaponData.pushWeapon( 1.5, 20, 0.2, 0.5, 1 )


function DeselectAllSquads()
    s_squad.disableOutline(selectedSquads)    
    selectedSquads = {}
end

function squadHandling()
    --Formations
    if mouse.isButtonDownOnce(mouse.button.Left) then        
		boxStartX, boxStartY = mouse.getPosition()
        selectedEntity = core.system.picking.getLastHitEntity()
        if selectedEntity then
            local unitTypeComponent = selectedEntity:get(core.componentType.UnitTypeComponent);
            local attributeComponent = selectedEntity:get(core.componentType.AttributeComponent);
            if attributeComponent and unitTypeComponent then                 
                if unitTypeComponent.unitType == core.UnitType.Police then

                    local squadEntity = s_squad.getSquadEntity(attributeComponent.squadID)
                    local squadComponent = squadEntity:get(core.componentType.SquadComponent)

                    if keyboard.isKeyDown(keyboard.key.Left_shift) then
                        local found = false

                        for i=1, #selectedSquads do
                            if selectedSquads[i] == attributeComponent.squadID then
                                found = true
                            end
                        end

                        if not found then
                            selectedSquads[#selectedSquads+1] = attributeComponent.squadID
                            if #selectedSquads == 1 then                                
                                selectedFormation = squadComponent.squadFormation
                            end
                        end
                    else
                        DeselectAllSquads()
                        selectedSquads[#selectedSquads+1] = attributeComponent.squadID
                        selectedFormation = squadComponent.squadFormation
                    end

                    if squadComponent.squadFormation ~= selectedFormation then
                        selectedFormation = s_squad.formations.NoFormation
                    end
                end
            end
		elseif not keyboard.isKeyDown(keyboard.key.Left_shift) and not core.config.stickySelection and not isClick then
			DeselectAllSquads()
        end

        if isClick == true then
            isClick = false
        end

	elseif mouse.isButtonDown(mouse.button.Left) then
		boxEndX, boxEndY = mouse.getPosition()
		groupsSelectedByBox = core.system.picking.getPoliceGroupsInsideBox( boxStartX, boxStartY, boxEndX, boxEndY, core.config.boxSelectionGraceDistance )
        
        if previousGroupsSelectedByBox and #previousGroupsSelectedByBox then
            s_squad.disableOutline(previousGroupsSelectedByBox)
        end

        if groupsSelectedByBox and #groupsSelectedByBox >= 1 then
            s_squad.enableOutline(groupsSelectedByBox, 1, 0.9, 0.1, 1)
        end

        previousGroupsSelectedByBox = groupsSelectedByBox

    elseif mouse.isButtonDownOnce(mouse.button.Right) then
        if #selectedSquads > 0 then
            isClick = true
            local mouseX, mouseY = mouse.getPosition()
            clickStartX, clickStartY, clickStartZ = core.system.picking.getGroundHit(mouseX, mouseY);
        end   
    elseif mouse.isButtonDown(mouse.button.Right) then        
        if #selectedSquads > 0 and clickStartX and clickStartY and clickStartZ and isClick then
            local mouseX, mouseY = mouse.getPosition()
            local dragX, dragY, dragZ = core.system.picking.getGroundHit(mouseX, mouseY)    

            s_squad.previewSquadFormation(selectedSquads, selectedFormation, clickStartX, clickStartY, clickStartZ, dragX, dragY, dragZ)
        end
    elseif mouse.isButtonUp(mouse.button.Right) then
        if #selectedSquads > 0 and clickStartX and clickStartY and clickStartZ and isClick then
            local mouseX, mouseY = mouse.getPosition()
            clickEndX, clickEndY, clickEndZ = core.system.picking.getGroundHit(mouseX, mouseY)    
            s_squad.setSquadFormation(selectedSquads, selectedFormation, clickStartX, clickStartY, clickStartZ, clickEndX, clickEndY, clickEndZ)
            if clickEndX and clickEndY and clickEndZ and selectedFormation ~= s_squad.formations.CircleFormation then
	            s_squad.setSquadGoal(selectedSquads, (clickStartX + clickEndX) / 2, (clickStartY + clickEndY) / 2, (clickStartZ + clickEndZ) / 2)
            else                
	            s_squad.setSquadGoal(selectedSquads, clickStartX, clickStartY, clickStartZ)
            end


            clickStartX, clickStartY, clickStartZ = nil, nil, nil
            isClick = false
        end
    end

    --Formation selectiong
    if keyboard.isKeyDown(keyboard.key.H) then
        selectedFormation = s_squad.formations.HalfCircleFormation
    elseif keyboard.isKeyDown(keyboard.key.C) then
        selectedFormation = s_squad.formations.CircleFormation
    elseif keyboard.isKeyDown(keyboard.key.L) then
        selectedFormation = s_squad.formations.LineFormation
    end

    --Stances
    if keyboard.isKeyDown(keyboard.key.I)  then        
        if #selectedSquads > 0 then
            s_squad.setSquadStance(selectedSquads, core.PoliceStance.Aggressive)
        end   
    elseif keyboard.isKeyDown(keyboard.key.O) then
        if #selectedSquads > 0 then
            s_squad.setSquadStance(selectedSquads, core.PoliceStance.Defensive)
        end   
    elseif keyboard.isKeyDown(keyboard.key.P) then
        if #selectedSquads > 0 then
            s_squad.setSquadStance(selectedSquads, core.PoliceStance.Passive)
        end
    end
	
	-- box select
	if boxStartX and boxStartY and boxEndX and boxEndY and mouse.isButtonUp(mouse.button.Left) then
		if boxStartX ~= boxEndX and boxStartY ~= boxEndY then
			if not keyboard.isKeyDown(keyboard.key.Left_shift) then
				DeselectAllSquads()
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
                        local squadEntity = s_squad.getSquadEntity(groupsSelectedByBox[p])
                        local squadComponent = squadEntity:get(core.componentType.SquadComponent)

						selectedSquads[#selectedSquads+1] = groupsSelectedByBox[p]
                        if #selectedSquads == 1 then
                            selectedFormation = squadComponent.squadFormation
                        end

                        if selectedFormation ~= squadComponent.squadFormation then
                            selectedFormation = s_squad.formations.NoFormation
                        end
					end			
				end
				groupsSelectedByBox = {}
			elseif not core.config.stickySelection and not keyboard.isKeyDown(keyboard.key.Left_shift) then
				DeselectAllSquads()
			end
		end			
		boxStartX, boxStartY, boxEndX, boxEndY = nil, nil, nil, nil
	end
    
    if selectedSquads and #selectedSquads >= 1 then
        s_squad.enableOutline(selectedSquads, 0.1, 0.9, 0.3, 1.0)
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
