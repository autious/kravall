local PoliceSquadHandler = {}

local keyboard = core.input.keyboard
local mouse = core.input.mouse
local s_squad = core.system.squad

function PoliceSquadHandler:new(o)
    o = o or {}
    setmetatable( o, self )
    self.__index = self

    o.groupsSelectedByBox = {}
    o.previousGroupSelectedByBox = {}
    o.selectedSquads = {}

    return o
end

function PoliceSquadHandler:setFormation( formation )

end

function PoliceSquadHandler:DeselectAllSquads()
    s_squad.disableOutline(self.selectedSquads)    
    selectedSquads = {}
end

function PoliceSquadHandler:update( delta )
    --Formations
    if mouse.isButtonDownOnce(mouse.button.Left) then        
		self.boxStartX, self.boxStartY = mouse.getPosition()
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

                        for i=1, #(self.selectedSquads) do
                            if self.selectedSquads[i] == attributeComponent.squadID then
                                found = true
                            end
                        end

                        if not found then
                            self.selectedSquads[#(self.selectedSquads)+1] = attributeComponent.squadID
                            if #(self.selectedSquads) == 1 then                                
                                self.selectedFormation = squadComponent.squadFormation
                            end
                        end
                    else
                        self:DeselectAllSquads()
                        self.selectedSquads[#(self.selectedSquads)+1] = attributeComponent.squadID
                        self.selectedFormation = squadComponent.squadFormation
                    end

                    if squadComponent.squadFormation ~= self.selectedFormation then
                        self.selectedFormation = s_squad.formations.NoFormation
                    end
                end
            end
		elseif not keyboard.isKeyDown(keyboard.key.Left_shift) and not core.config.stickySelection and not self.isClick then
			self:DeselectAllSquads()
        end

        if self.isClick == true then
            self.isClick = false
        end

	elseif mouse.isButtonDown(mouse.button.Left) then
		self.boxEndX, self.boxEndY = mouse.getPosition()
		self.groupsSelectedByBox = core.system.picking.getPoliceGroupsInsideBox( self.boxStartX, self.boxStartY, self.boxEndX, self.boxEndY, core.config.boxSelectionGraceDistance )
        
        if self.previousGroupsSelectedByBox and #(self.previousGroupsSelectedByBox) then
            s_squad.disableOutline(self.previousGroupsSelectedByBox)
        end

        if self.groupsSelectedByBox and #(self.groupsSelectedByBox) >= 1 then
            s_squad.enableOutline(self.groupsSelectedByBox, 1, 0.9, 0.1, 1)
        end

        self.previousGroupsSelectedByBox = self.groupsSelectedByBox

    elseif mouse.isButtonDownOnce(mouse.button.Right) then
        if #(self.selectedSquads) > 0 then
            self.isClick = true
            local mouseX, mouseY = mouse.getPosition()
            self.clickStartX, self.clickStartY, self.clickStartZ = core.system.picking.getGroundHit(mouseX, mouseY);
        end   
    elseif mouse.isButtonDown(mouse.button.Right) then        
        if #(self.selectedSquads) > 0 and self.clickStartX and self.clickStartY and self.clickStartZ and self.isClick then
            local mouseX, mouseY = mouse.getPosition()
            local dragX, dragY, dragZ = core.system.picking.getGroundHit(mouseX, mouseY)    

            s_squad.previewSquadFormation(self.selectedSquads, self.selectedFormation, self.clickStartX, self.clickStartY, self.clickStartZ, dragX, dragY, dragZ)
        end
    elseif mouse.isButtonUp(mouse.button.Right) then
        if #(self.selectedSquads) > 0 and self.clickStartX and self.clickStartY and self.clickStartZ and self.isClick then
            local mouseX, mouseY = mouse.getPosition()
            self.clickEndX, self.clickEndY, self.clickEndZ = core.system.picking.getGroundHit(mouseX, mouseY)    
            s_squad.setSquadFormation(self.selectedSquads, self.selectedFormation, self.clickStartX, self.clickStartY, self.clickStartZ, self.clickEndX, self.clickEndY, self.clickEndZ)
            if self.clickEndX and self.clickEndY and self.clickEndZ and self.selectedFormation ~= s_squad.formations.CircleFormation then
	            s_squad.setSquadGoal(self.selectedSquads, (self.clickStartX + self.clickEndX) / 2, (self.clickStartY + self.clickEndY) / 2, (self.clickStartZ + self.clickEndZ) / 2)
            else                
	            s_squad.setSquadGoal(self.selectedSquads, self.clickStartX, self.clickStartY, self.clickStartZ)
            end


            self.clickStartX, self.clickStartY, self.clickStartZ = nil, nil, nil
            self.isClick = false
        end
    end

    --Formation selectiong
    if keyboard.isKeyDown(keyboard.key.H) then
        self.selectedFormation = s_squad.formations.HalfCircleFormation
    elseif keyboard.isKeyDown(keyboard.key.C) then
        self.selectedFormation = s_squad.formations.CircleFormation
    elseif keyboard.isKeyDown(keyboard.key.L) then
        self.selectedFormation = s_squad.formations.LineFormation
    end

    --Stances
    if keyboard.isKeyDown(keyboard.key.I)  then        
        if #(self.selectedSquads) > 0 then
            s_squad.setSquadStance(self.selectedSquads, core.PoliceStance.Aggressive)
        end   
    elseif keyboard.isKeyDown(keyboard.key.O) then
        if #(self.selectedSquads) > 0 then
            s_squad.setSquadStance(self.selectedSquads, core.PoliceStance.Defensive)
        end   
    elseif keyboard.isKeyDown(keyboard.key.P) then
        if #(self.selectedSquads) > 0 then
            s_squad.setSquadStance(self.selectedSquads, core.PoliceStance.Passive)
        end
    end
	
	-- box select
	if self.boxStartX and self.boxStartY and self.boxEndX and self.boxEndY and mouse.isButtonUp(mouse.button.Left) then
		if self.boxStartX ~= self.boxEndX and self.boxStartY ~= self.boxEndY then
			if not keyboard.isKeyDown(keyboard.key.Left_shift) then
				self:DeselectAllSquads()
			end
			if self.groupsSelectedByBox then

				for p = 1, #(self.groupsSelectedByBox) do
					local found = false
					for i=1, #(self.selectedSquads) do
						if self.selectedSquads[i] == self.groupsSelectedByBox[p] then
							found = true
						end
					end
					
					if not found then                            
                        local squadEntity = s_squad.getSquadEntity(self.groupsSelectedByBox[p])
                        local squadComponent = squadEntity:get(core.componentType.SquadComponent)

						self.selectedSquads[#(self.selectedSquads)+1] = self.groupsSelectedByBox[p]
                        if #(self.selectedSquads) == 1 then
                            self.selectedFormation = squadComponent.squadFormation
                        end

                        if self.selectedFormation ~= squadComponent.squadFormation then
                            self.selectedFormation = s_squad.formations.NoFormation
                        end
					end			
				end
				self.groupsSelectedByBox = {}
			elseif not core.config.stickySelection and not keyboard.isKeyDown(keyboard.key.Left_shift) then
				self:DeselectAllSquads()
			end
		end
		self.boxStartX, self.boxStartY, self.boxEndX, self.boxEndY = nil, nil, nil, nil
	end
    
    if self.selectedSquads and #(self.selectedSquads) >= 1 then
        s_squad.enableOutline(self.selectedSquads, 0.1, 0.9, 0.3, 1.0)
    end
end

return PoliceSquadHandler
