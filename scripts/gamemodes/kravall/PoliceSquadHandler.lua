local PoliceSquadHandler =  { 

                                onFormationChange = function() 
                                    core.log.error( "No function set for onFormationChange in PoliceSquadHandler") 
                                end,
                                onStanceChange = function(stance) 
                                    core.log.error( "No functionset for StanceChange in PoliceSquadHandler")   
                                end,
                                onAbilityChange= function( ability )
                                    core.log.error( "No functionset for AbilityChange in PoliceSquadHandler")   
                                end,
                                onSelectedSquadsChange = function(squads)
                                    core.log.error( "No function set for onSelectedSquadsChange" )
                                end,
                                onSelectedUnitInformationChange = function(data)
                                    core.log.error( "No function set for onSelectedUnitInformationChange")
                                end
                            }
local U = require "utility"

local keyboard = core.input.keyboard
local mouse = core.input.mouse
local s_squad = core.system.squad

local standardPolice = U.convertToUsefulConstants( (require "game_constants").standardPolice )
local guiBehaviour = U.convertToUsefulConstants( (require "game_constants").guiBehaviour )

local input = require "input"

local function registerCallbacks(o)
    input.registerOnButton( function( button, action, mods, consumed )
        if action == core.input.action.Press then
            --Only allow press if UI element hasn't been pressed
            if not consumed then
                if button == mouse.button.Left then
                    o.leftPressed = true   
                    o.leftClicked = true
                elseif button == mouse.button.Right then
                    o.rightPressed = true
                    o.rightClicked = true
                end
            end
        elseif action == core.input.action.Release then
            if button == mouse.button.Left then
                o.leftPressed = false
            elseif button == mouse.button.Right then
                o.rightPressed = false
            end
        end
    end, "GAME")
end

function PoliceSquadHandler:new(o)
    o = o or {}
    setmetatable( o, self )
    self.__index = self

    o.groupsSelectedByBox = {}
    o.previousGroupSelectedByBox = {}
    o.selectedSquads = {}

    -- List for units recently damaged, with ttl.
    o.squadDamageQueue = {}

    -- List of total health in previous frame
    o.prevFrameSquadHealth = {}
    
    registerCallbacks(o)

    return o
end

function PoliceSquadHandler:DeselectAllSquads()
    s_squad.disableOutline(self.selectedSquads)    
    self:setFormation( core.system.squad.formations.NoFormation )
    self:setSquadSelection( {} )
end

function PoliceSquadHandler:setFormation( formation )
    assert( type(formation) == "userdata" )
    local doEvent = (self.selectedFormation ~= formation)

    self.selectedFormation = formation

    if doEvent and self.onFormationChange then
        self.onFormationChange( formation ) 
    end
end

--The two following functions are used to manipulate
-- the current squad selection, and are responsible for calling
-- and callbacks relevant to such a modification
-- Both functions take tables contianing a continous
-- list of squads, nil is not allowed, instead to
-- deselect use an empty list
function PoliceSquadHandler:addSquadsToSelection( squads )
    for _,v in pairs( squads ) do
        self.selectedSquads[#(self.selectedSquads)+1] = v
    end    

    self:setSquadSelection( self.selectedSquads )
end

function PoliceSquadHandler:setSquadSelection( squads )
    self.selectedSquads = squads     
    self.onSelectedSquadsChange( squads )

    if #(self.selectedSquads) > 0 then
        self.onSelectedUnitInformationChange( self:evaluateSquadInformation( self.selectedSquads[1] ))
    end
end

function PoliceSquadHandler:setSquadPrimary( squad )
    local newselection = {squad}
    
    for i,v in pairs( self.selectedSquads ) do
        if v ~= squad then
            newselection[#newselection+1] = v
        end 
    end

    self.selectedSquads = newselection
end

-- Generates a table containing information about the given
-- squad, to be sent to the gui components.
function PoliceSquadHandler:evaluateSquadInformation( squad )
    local squadEntity = core.system.squad.getSquadEntity( squad )

    local sq = squadEntity:get( core.componentType.SquadComponent )

    local data = { 
                    health = sq.squadHealth,
                    morale = sq.squadMorale, 
                    stamina = sq.squadHealth, 
                    stance = sq.squadStance,
                    formation = sq.squadFormation,
                    name = "Police Squad" 
                 }

    return data
end

function PoliceSquadHandler:updateSquadDamageStatus( delta )
    local allSquads = core.system.squad.getAllSquadEntities()

    for i,v in pairs( allSquads ) do 
        local squadComponent = v:get( core.componentType.SquadComponent )

        if self.prevFrameSquadHealth[squadComponent.squadID] ~= nil
            and self.prevFrameSquadHealth[squadComponent.squadID] > squadComponent.squadHealth then
            if self.squadDamageQueue[squadComponent.squadID] then
                self.squadDamageQueue[squadComponent.squadID].ttl = guiBehaviour.damageBlinkingLinger
            else
                self.squadDamageQueue[squadComponent.squadID] = {ttl=guiBehaviour.damageBlinkingLinger}
            end
        end 

        self.prevFrameSquadHealth[squadComponent.squadID] = squadComponent.squadHealth
    end

    if not self.hasLived then
        self.hasLived = 0
    end

    local savedSquadDamageQueue = {}
    for i,v in pairs( self.squadDamageQueue ) do
        if v.ttl > 0 then
            savedSquadDamageQueue[i] = v
        end
        v.ttl = v.ttl - delta

        if self.hasLived % 1.0 > 0.5 then
            core.system.squad.enableOutline( {i}, standardPolice.damageOutline:get() )
        end
    end

    self.hasLived = self.hasLived + delta 

    --Reset counter to counteract the noticable rounding error that will occur
    --after ca 20 billion years of running the program
    if self.hasLived > 1.0 then
        self.hasLived = self.hasLived - 1.0
    end

    self.squadDamageQueue = savedSquadDamageQueue
end

-- If given non-nil, sets the stance to given value on selection
-- and calls callback informing gui and other systems about change.
-- If given nil, ignores value for squad set, but
-- does a callback with nil to inform other systems and gui that
-- there is no current valid selection (because theres a mixxed selection or no selection)
function PoliceSquadHandler:setStance( stance )
    if #(self.selectedSquads) > 0 then
        core.log.debug( "PoliceSquadHandler: Settings stance of squad to ", stance )

        if stance then
            s_squad.setSquadStance(self.selectedSquads, stance)
        end

        self.onStanceChange( stance )  
    end
end

function PoliceSquadHandler:setAbility( ability )
    self.onAbilityChange( ability )
end

-- Check if given group has any differing stances, if so, return nil
-- otherwise, return the shared stance
function PoliceSquadHandler:evaluateStanceForGroups( policeGroup )
    local firstStance = nil
    for _,v in pairs(policeGroup) do
        local s_ent = core.system.squad.getSquadEntity( v )
        local s_utc = s_ent:get(core.componentType.SquadComponent)

        if firstStance then
            if s_utc.squadStance ~= firstStance then
                return nil 
            end 
        else
            firstStance = s_utc.squadStance
        end 
    end
    
    return firstStance
end

function PoliceSquadHandler:update( delta )
   
    -- Sets the box selection outline to the given squads
    -- If any of the given squads match the primary selection, it gets a different tint
    local function applyBoxOutline( squads )
        local notPrimary = {}
        local primaryGroup = nil
        
        if #(self.selectedSquads) > 0 then
            primaryGroup = self.selectedSquads[1]
        end
            

        for _,v in pairs( squads ) do 
            if primaryGroup == v then
                s_squad.enableOutline( {v}, standardPolice.selectionBoxHoverPrimaryOutline:get() )
            else
                notPrimary[#notPrimary+1] = v    
            end
        end
        s_squad.enableOutline(notPrimary, standardPolice.selectionBoxHoverOutline:get())
    end

    local function applySelectionOutline( squads )
        local notPrimary = {}
            
        for i,v in ipairs( squads ) do 
            if i == 1 then
                s_squad.enableOutline( {v}, standardPolice.selectionPrimaryOutline:get() )
            else
                notPrimary[#notPrimary+1] = v    
            end
        end
        s_squad.enableOutline(notPrimary, standardPolice.selectionOutline:get())
        
    end

    local function clearOutlines()
        local allSquads = core.system.squad.getAllSquadEntities()

        for i,v in pairs( allSquads ) do 
            local squadComponent = v:get( core.componentType.SquadComponent )
            core.system.squad.disableOutline( {squadComponent.squadID} )
        end
    end

    clearOutlines()

    --Formations
    --Click Selection
    if self.leftClicked then        
		self.boxStartX, self.boxStartY = mouse.getPosition()
        selectedEntity = core.system.picking.getLastHitEntity()
        if selectedEntity then
            local unitTypeComponent = selectedEntity:get(core.componentType.UnitTypeComponent);
            local attributeComponent = selectedEntity:get(core.componentType.AttributeComponent);

            if attributeComponent and unitTypeComponent then                 
                --Selected normal police unit
                if unitTypeComponent.unitType == core.UnitType.Police then

                    local squadEntity = s_squad.getSquadEntity(attributeComponent.squadID)
                    local squadComponent = squadEntity:get(core.componentType.SquadComponent)

                    --Append units
                    if keyboard.isKeyDown(keyboard.key.Left_shift) then
                        local found = false

                        for i=1, #(self.selectedSquads) do
                            if self.selectedSquads[i] == attributeComponent.squadID then
                                found = true
                            end
                        end

                        if not found then
                            self:addSquadsToSelection( {attributeComponent.squadID} )
                            -- New selection, take formation and put in gui
                            if #(self.selectedSquads) == 1 then
                                self:setFormation( squadComponent.squadFormation )
                            end
                        else
                            self:setSquadPrimary( attributeComponent.squadID ) 
                        end


                    else --Select new group of units
                        self:DeselectAllSquads()
                        self:addSquadsToSelection( {attributeComponent.squadID} )
                        self:setFormation( squadComponent.squadFormation )
                    end

                    if squadComponent.squadFormation ~= self.selectedFormation then
                        self:setFormation( s_squad.formations.NoFormation )
                    end

                    -- Called so that we set the gui squad button to current selection.
                    -- (Or to nothing if current selection has mixxed stances)
                    self:setStance( self:evaluateStanceForGroups( self.selectedSquads ) )
                    applySelectionOutline( self.selectedSquads )
                end
            end
		elseif not keyboard.isKeyDown(keyboard.key.Left_shift) and not core.config.stickySelection and not self.isClick then
			self:DeselectAllSquads()
        end

        if self.isClick == true then
            self.isClick = false
        end

	elseif self.leftPressed then
		self.boxEndX, self.boxEndY = mouse.getPosition()
		self.groupsSelectedByBox = core.system.picking.getPoliceGroupsInsideBox( self.boxStartX, self.boxStartY, self.boxEndX, self.boxEndY, core.config.boxSelectionGraceDistance )
        
        if self.previousGroupsSelectedByBox and #(self.previousGroupsSelectedByBox) > 0 then
            s_squad.disableOutline(self.previousGroupsSelectedByBox)
        end

        if self.groupsSelectedByBox and #(self.groupsSelectedByBox) >= 1 then
            applyBoxOutline( self.groupsSelectedByBox )
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
        self:setFormation( s_squad.formations.HalfCircleFormation )
    elseif keyboard.isKeyDown(keyboard.key.C) then
        self:setFormation( s_squad.formations.CircleFormation )
    elseif keyboard.isKeyDown(keyboard.key.L) then
        self:setFormation( s_squad.formations.LineFormation)
    end

    --Stances
    if keyboard.isKeyDown(keyboard.key.I)  then        
        if #(self.selectedSquads) > 0 then
            self:setStance( core.PoliceStance.Aggressive )
        end   
    elseif keyboard.isKeyDown(keyboard.key.O) then
        if #(self.selectedSquads) > 0 then
            self:setStance( core.PoliceStance.Defensive )
        end   
    elseif keyboard.isKeyDown(keyboard.key.P) then
        if #(self.selectedSquads) > 0 then
            self:setStance( core.PoliceStance.Passive )
        end
    end
	
	-- box select
	if self.boxStartX and self.boxStartY and self.boxEndX and self.boxEndY and not self.leftPressed then
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

						self:addSquadsToSelection( {self.groupsSelectedByBox[p]} )
                        if #(self.selectedSquads) == 1 then
                            self:setFormation( squadComponent.squadFormation )
                        end

                        if self.selectedFormation ~= squadComponent.squadFormation then
                            self:setFormation(s_squad.formations.NoFormation)
                        end
					end			

                    -- Called so that we set the gui squad button to current selection.
                    -- (Or to nothing if current selection has mixxed stances)
                    self:setStance( self:evaluateStanceForGroups( self.selectedSquads ) )
				end
				self.groupsSelectedByBox = {}
			elseif not core.config.stickySelection and not keyboard.isKeyDown(keyboard.key.Left_shift) then
				self:DeselectAllSquads()
			end
		end
		self.boxStartX, self.boxStartY, self.boxEndX, self.boxEndY = nil, nil, nil, nil
	end
    
    if self.selectedSquads and #(self.selectedSquads) >= 1 then
        applySelectionOutline(self.selectedSquads)
    end
    self:updateSquadDamageStatus( delta )

    self.leftClicked = false
    self.rightClicked = false

    -- Update the data each frame, should probably be changed to only update
    -- when relevant. TODO: Fix aforementioned thing
    if #(self.selectedSquads) > 0 then
        self.onSelectedUnitInformationChange( self:evaluateSquadInformation( self.selectedSquads[1] ))
    else
        -- This can be seen as relatively temp or redundant, 
        -- as the gui elements should hide when there is no selection.
        self.onSelectedUnitInformationChange( {name = "", health=0, morale=0, stamina=0} )
    end
end

return PoliceSquadHandler
