local PoliceSquadHandler =  {
                                particleDefinitions = nil,
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
                                onUsableAbilitiesChange = function(abilities)
                                    core.log.error( "No function is set  for onSelectedAbilitiesChange" )
                                end,
                                onSelectedUnitInformationChange = function(data)
                                    core.log.error( "No function set for onSelectedUnitInformationChange")
                                end,
                                onMoveToPosition = function( squads, position, success )
                                    core.log.error( "No function set for onMoveToPosition" )
                                end
                                 
                            }
local U = require "utility"

local keyboard = core.input.keyboard
local mouse = core.input.mouse
local s_squad = core.system.squad

local standardPolice = (require "game_constants").standardPolice
local guiBehaviour = (require "game_constants").guiBehaviour
local tearGas = (require "game_constants").tearGas
local sprinting = (require "game_constants").sprinting

local Reticule = require "gamemodes/kravall/reticule"

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
	o.outlinedRioterGroups = nil

    -- List for units recently damaged, with ttl.
    o.squadDamageQueue = {}

    -- List of total health in previous frame
    o.prevFrameSquadHealth = {}

    -- List of entity, abilities
    o.usableAbilities = {}

    -- List of timed entities created for abilities
    o.abilityEntities = {}

    -- Reticule that is displayed
    o.reticule = Reticule:new()

    o.createdSquads = {}

    -- Function to call when aiming
    o.aimingFunction = nil
    
    registerCallbacks(o)

    return o
end

function PoliceSquadHandler:destroy()
    for _,v in pairs(self.abilityEntities) do
        v.entity:destroy()
    end 
end

function PoliceSquadHandler:DeselectAllSquads()
    s_squad.disableOutline(self.selectedSquads)    
    self:setFormation( core.system.squad.formations.NoFormation )
    self:setSquadSelection( {} )
    self:setUsableAbilities( {} )
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
    self:setUsableAbilities( self.selectedSquads )
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
                    stamina = sq.squadStamina, 
                    stance = sq.squadStance,
                    formation = sq.squadFormation,
                    name = "Police Squad" 
                 }

    return data
end

-- Adds a created police squad to the internal list of squads
function PoliceSquadHandler:addSquad(squad)
    self.createdSquads[#(self.createdSquads) +1] = squad
end

-- Retrieves the squad with the given squadId
function PoliceSquadHandler:getSquad(squadId)
    for i=1, #(self.createdSquads) do
        if self.createdSquads[i].groupId == squadId then
            return self.createdSquads[i]    
        end
    end
end

-- Sets the current usable abilities to the abilities 
-- available to the members of the given squadIds
function PoliceSquadHandler:setUsableAbilities(squads)
    local abilities = {}
    local aggregatedAbilities = {}
    for i=1, #squads do
        local squad = self:getSquad(squads[i])
        for _,member in pairs(squad.members) do
            abilities[member] = member.getAbilities()
            for i=1, #(member.getAbilities()) do
                aggregatedAbilities[#aggregatedAbilities+1] = abilities[member][i]
            end
        end                 
    end
    self.usableAbilities = abilities;
    self.onUsableAbilitiesChange(aggregatedAbilities)
end

function PoliceSquadHandler:evaluateUsableAbilities()
    local abilities = {}
    local aggregatedAbilities = {}
    for i=1, #self.selectedSquads do
        local squad = self:getSquad(self.selectedSquads[i])
        for _,member in pairs(squad.members) do
            abilities[member] = member.getAbilities()
            for i=1, #(member.getAbilities()) do
                aggregatedAbilities[#aggregatedAbilities+1] = abilities[member][i]
            end
        end                 
    end
    self.usableAbilities = abilities;
    self.onUsableAbilitiesChange(aggregatedAbilities)
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
    if ability == core.system.squad.abilities.TearGas then
        self.isAiming = true
        self:SetReticuleRender(true)
        self.AimingFunction = self.AimTearGas
    elseif ability == core.system.squad.abilities.Sprint then
        self.isAiming = true
        self:SetReticuleRender(false)
        self.AimingFunction = self.AimSprint
    elseif ability == core.system.squad.abilities.Flee then
        if self:CanUseAbility(ability) then
            self:UseFlee() 
        end
    end
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


function PoliceSquadHandler:CanUseAbility(ability)
    for _,abilityList in pairs(self.usableAbilities) do
        for _,a in pairs(abilityList) do
            if a == ability then
                return true
            end
        end
    end
    return false
end

function PoliceSquadHandler:SetReticuleRender(value)
    self.reticule.entity:set(core.componentType.GraphicsComponent, {render = value}, true) 
end

function PoliceSquadHandler:AimTearGas()
    local mouseX, mouseY = mouse.getPosition()
    local x,y,z = core.system.picking.getGroundHit(mouseX, mouseY);
    local errorMessage = nil
    local inRange = false
    self.reticule:SetPosition(x, y, z)

    if not self:CanUseAbility(core.system.squad.abilities.TearGas) then
        self.isAiming = false
        self.AimingFunction = nil
        self:SetReticuleRender(false)
        return
    end

    for member, abilities in pairs(self.usableAbilities) do    
        for i=1, #abilities do
            if abilities[i] == core.system.squad.abilities.TearGas then
                local wpc = member.entity:get(core.componentType.WorldPositionComponent)
                if math.sqrt(((wpc.position[1]-x) * (wpc.position[1]-x)) + ((wpc.position[2]-y) * (wpc.position[2]-y)) + ((wpc.position[3]-z) * (wpc.position[3]-z))) < tearGas.tearGasRange then

                    if inRange == false then 
                        inRange = true 
                    end

                    if self.leftClicked then
                        local attributeComponent = member.entity:get(core.componentType.AttributeComponent)
                        if attributeComponent.stamina >= tearGas.tearGasStaminaCost then
                            --Consume click to avoid deselecting squads
                            self.leftClicked = false
                            self.leftPressed = false
                            self:UseTearGas(member.entity, x, y, z) 

                            if not keyboard.isKeyDown(keyboard.key.Left_shift) then
                                self.isAiming = false
                                self:SetReticuleRender(false)
                                self.AimingFunction = nil
                            end

                            return
                        else
                            errorMessage = "Not enough stamina"    
                        end
                    end
                end
            end
        end
    end

    if inRange then
        self.reticule:SetAccept()
    else
        self.reticule:SetDeny()
    end

    if self.leftClicked then
        --Consume click to avoid deselecting squads
        self.leftClicked = false
        self.leftPressed = false
        if errorMessage == nil then
            errorMessage = "Too far away"
        end
    end

    if errorMessage then
        print( errorMessage)
    end
end

-----------------------------------------------------------------------------------------------------------------------------
function PoliceSquadHandler:AttackGroup()
	if #self.selectedSquads == 0 then
		self.isAiming = false
		self:SetReticuleRender(false)
		self.AimingFunction = nil
	end
	
	local selectedEntity = core.system.picking.getLastHitEntity()
	if selectedEntity then
		local unitTypeComponent = selectedEntity:get(core.componentType.UnitTypeComponent);
		local attributeComponent = selectedEntity:get(core.componentType.AttributeComponent);

		if attributeComponent and unitTypeComponent then                 
			
			--cursor is hovering over rioters
			if unitTypeComponent.unitType == core.UnitType.Rioter then
				s_squad.enableOutline( { attributeComponent.groupID }, standardPolice.selectionPrimaryOutline:get() )
				self.outlinedRioterGroups = attributeComponent.groupID;
				
				
				if mouse.isButtonDown(mouse.button.Left) then
					self.isAiming = false
					self:SetReticuleRender(false)
					self.AimingFunction = nil
					
					core.orders.attackGroup( attributeComponent.groupID, self.selectedSquads )
				end				
			end			
		end		
	end	
end

function PoliceSquadHandler:AimSprint()
    local mouseX, mouseY = mouse.getPosition()
    local x,y,z = core.system.picking.getGroundHit(mouseX, mouseY);
    if not self:CanUseAbility(core.system.squad.abilities.Sprint) then
        self.isAiming = false
        self:SetReticuleRender(false)
        self.AimingFunction = nil
        return
    end

    if self.leftClicked then
        --Consume click to avoid deselecting squads
        self.leftClicked = false
        self.leftPressed = false
        

        self:UseSprint(x, y, z)
        if not keyboard.isKeyDown(keyboard.key.Left_shift) then
            self.isAiming = false
            self:SetReticuleRender(false)
            self.AimingFunction = nil
        end


    end
end

function PoliceSquadHandler:UseTearGas(entity, x, y, z)
    local attributeComponent = entity:get(core.componentType.AttributeComponent)
    entity:set(core.componentType.AttributeComponent, {stamina = (attributeComponent.stamina - tearGas.tearGasStaminaCost)}, true)
    local pairTable = {}                
    local entity = core.entity.create(core.componentType.EmitterComponent
                                        , core.componentType.WorldPositionComponent
                                        , core.componentType.MovementComponent
										, core.componentType.MovementDataComponent
                                        , core.componentType.RotationComponent
                                        , core.componentType.UnitTypeComponent
                                        , core.componentType.AttributeComponent
                                        , core.componentType.FlowfieldComponent)

    entity:set(core.componentType.AttributeComponent, {pfObjectType = core.PFObjectType.TearGas}, true)
    entity:set(core.componentType.UnitTypeComponent, {unitType = core.UnitType.Object}, true)
    entity:set(core.componentType.WorldPositionComponent, {position = {x, y, z}})
    entity:set(core.componentType.EmitterComponent, {
            rate = 10,
            offset = {0, 0, 0},
            life = 5,
            lifeVariance = 0,
            lifeReduction = 1,
            lifeReductionVariance = 0,
            velocity = {0, 0, 6},
            velocityVariance = {3, 3, 3},
            acceleration = {0, -2, 0},
            coneDirection = {0, 1, 0},
            coneAngle = 30,
            coneAngleVariance = 60,
            type = core.system.particle.emitters.Cone,
            handle = self.particleDefinitions["TearGas"]
            }, true)

    pairTable.entity = entity
    pairTable.timer = tearGas.tearGasDuration 

    self.abilityEntities[#(self.abilityEntities) + 1] = pairTable
end

function PoliceSquadHandler:UseSprint(x, y, z)

    for member, abilities in pairs(self.usableAbilities) do    
        for i=1, #abilities do
            if abilities[i] == core.system.squad.abilities.Sprint then
                member.isSprinting = true                
            end
        end
    end
    
    core.system.squad.setSquadGoal(self.selectedSquads, x, y, z)
end

function PoliceSquadHandler:UseFlee()
    local squadIDs = {}
    for member, abilities in pairs(self.usableAbilities) do    
        for i=1, #abilities do
            if abilities[i] == core.system.squad.abilities.Flee then
                local attrbc = member.entity:get(core.componentType.AttributeComponent)
                squadIDs[attrbc.squadID] = self:getSquad(attrbc.squadID).startPosition
                member.entity:set(core.componentType.FormationComponent, {relativePosition = member.startOffset}, true)
                member.isSprinting = true
            end
        end
    end

    for k,v in pairs(squadIDs) do
        core.system.squad.setSquadStance({k}, core.PoliceStance.Passive)
        core.system.squad.setSquadGoal({k}, v[1], v[2], v[3])
    end
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
		
		if self.outlinedRioterGroups then
			core.system.squad.disableOutline( { self.outlinedRioterGroups } )		
		end		
		self.outlinedRioterGroups = nil
    end

    clearOutlines()

    local function updateSquads()
        for k,v in pairs(self.createdSquads) do
            local groupId = v.groupId  
            local squadMembers = core.system.groups.getMembersInGroup(groupId)

            local i=1
            while i <= #(v.members) do
                local found = false
                for _, entity in pairs(squadMembers) do
                    if core.entity.isSameEntity(v.members[i].entity, entity) then
                        --Squad member is still in the squad, e.g not killed
                        found = true
                    end
                end

                if not found then
                    table.remove(v.members, i)
                else
                    i=i+1
                end
            end            
        end
        self:evaluateUsableAbilities()
    end

    updateSquads()


    --Abilities
    if self.isAiming and self.rightClicked then
        self:SetReticuleRender(false)
        self.isAiming = false
        --Consume event to prevent setting goals
        self.rightClicked = false
        self.rightPressed = false
    end

    local i = 1
    while i <= #(self.abilityEntities) do
        local e = self.abilityEntities[i]
        self.abilityEntities[i].timer = self.abilityEntities[i].timer - delta
        if self.abilityEntities[i].timer <= 0 then
            self.abilityEntities[i].entity:destroy()
            table.remove(self.abilityEntities, i)
        else
            i=i+1
        end
    end

    if keyboard.isKeyDownOnce(keyboard.key.Kp_8) then
        if self:CanUseAbility(core.system.squad.abilities.TearGas) then            
            if self.isAiming and self.AimingFunction == self.AimTearGas then
                self.isAiming = false
                self:SetReticuleRender(false)
                self.AimingFunction = nil
            else
                self.isAiming = true
                self:SetReticuleRender(true)
                self.AimingFunction = self.AimTearGas
            end            
       end
    end
	
	-- attack group
	if keyboard.isKeyDownOnce(keyboard.key.Kp_5) then
		if self.isAiming and self.AimingFunction == self.AttackGroup then
			self.isAiming = false
			self:SetReticuleRender(false)
			self.AimingFunction = nil
		else
			self.isAiming = true
			self:SetReticuleRender(false)
			self.AimingFunction = self.AttackGroup
		end            	
	end

    if keyboard.isKeyDownOnce(keyboard.key.Kp_9) then
        if self:CanUseAbility(core.system.squad.abilities.Flee) then            
            self:UseFlee()
       end
    end

    if keyboard.isKeyDownOnce(keyboard.key.Kp_3) then
        if self:CanUseAbility(core.system.squad.abilities.Sprint) then            
            if self.isAiming and self.AimingFunction == self.AimSprint then
                self.isAiming = false
                self.AimingFunction = nil
            else
                self.isAiming = true
                self:SetReticuleRender(false)
                self.AimingFunction = self.AimSprint
            end            
       end
    end

    --Ability: Sprint
    for _,squad in pairs(self.createdSquads) do
        local squadEntity = core.system.squad.getSquadEntity(squad.groupId)
        assert( squadEntity, "no squad entity bound to " .. squad.groupId )
        local sqdc = squadEntity:get(core.componentType.SquadComponent)
        for _,member in pairs(squad.members) do
            if member.isSprinting == true then
                local attrbc = member.entity:get(core.componentType.AttributeComponent)
                local remainingStamina = attrbc.stamina - sprinting.sprintingStaminaCost * delta

                if remainingStamina > 0 then
                    local wpc = member.entity:get(core.componentType.WorldPositionComponent)
                    local frmtnc = member.entity:get(core.componentType.FormationComponent)
                    local goal = {sqdc.squadGoal[1] + frmtnc.relativePosition[1], 
                                    sqdc.squadGoal[2],
                                    sqdc.squadGoal[3] + frmtnc.relativePosition[2]}
                    local pos = wpc.position

                    local distToTarget = math.sqrt((pos[1] - goal[1]) * (pos[1] - goal[1]) + (pos[2] - goal[2]) * (pos[2] - goal[2]) + (pos[3] - goal[3]) * (pos[3] - goal[3]))

                    if distToTarget  > sprinting.sprintingReachThreshold then
                
                        member.entity:set(core.componentType.MovementComponent, {state = core.movementData.Sprinting}, true)
                    else
                        member.entity:set(core.componentType.MovementComponent, {state = core.movementData.Jogging}, true)
                        member.isSprinting = nil
                    end
                else
                    remainingStamina = 0
                    member.isSprinting = nil
                    member.entity:set(core.componentType.MovementComponent, {state = core.movementData.Jogging}, true)
                end
                member.entity:set(core.componentType.AttributeComponent, {stamina = remainingStamina}, true)
            else              
                member.entity:set(core.componentType.MovementComponent, {state = core.movementData.Jogging}, true)
            end         
        end
    end

    if self.isAiming == true then
        self:AimingFunction()
    end   

    --Formations
    --Click Selection
    if self.leftClicked then        
		self.boxStartX, self.boxStartY = mouse.getPosition()
        local selectedEntity = core.system.picking.getLastHitEntity()
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

    elseif self.rightClicked then
        if #(self.selectedSquads) > 0 then
            self.isClick = true
            local mouseX, mouseY = mouse.getPosition()
            self.clickStartX, self.clickStartY, self.clickStartZ = core.system.picking.getGroundHit(mouseX, mouseY);
        end   
    elseif self.rightPressed then        
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
                --Tell other systems that we are telling things to move.
                s_squad.setSquadGoal(self.selectedSquads, self.clickStartX, self.clickStartY, self.clickStartZ)
            end

            local clickPos = core.glm.vec3.new(core.system.picking.getGroundHit(  mouseX, mouseY ))
            self.onMoveToPosition( self.selectedSquads, clickPos, core.system.picking.getIsInsideNavigationMesh(clickPos) )


            self.clickStartX, self.clickStartY, self.clickStartZ = nil, nil, nil
            self.isClick = false
        end
    end

    --Formation selection
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
