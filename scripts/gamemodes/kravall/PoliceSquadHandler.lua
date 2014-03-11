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
                                end,
                                onEventMessage = function( component )
                                    core.log.error( "No function set for onEventMessage" )
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
local policeStamina = (require "game_constants").policeStamina

local Reticule = require "gamemodes/kravall/reticule"


local TextLabel = require "gui/component/TextLabel"

local input = require "input"

local QuickGroupHandler = require "gamemodes/kravall/QuickGroupHandler"

function PoliceSquadHandler:onButton(button, action, mods, consumed)
    if action == core.input.action.Press then
        --Only allow press if UI element hasn't been pressed
        if not consumed then
            if button == mouse.button.Left then
                self.leftPressed = true   
                self.leftClicked = true
            elseif button == mouse.button.Right then
                self.rightPressed = true
                self.rightClicked = true
            end
        end
    elseif action == core.input.action.Release then
        if button == mouse.button.Left then
            self.leftPressed = false
        elseif button == mouse.button.Right then
            self.rightPressed = false
        end
    end        
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

    -- Used for double click selection
    o.lastClickTime = os.clock()   
    o.lastClickType = ""

    -- Used for cycling squads
    o.cycleSquad = 1

    o.quickGroupHandler = QuickGroupHandler:new{
        onSelectGroup = function( groupList )
            o:setSquadSelection( groupList )
        end,
        onAddGroup = function( groupList )
            o:addSquadsToSelection( groupList )
        end,
    }

    return o
end

function PoliceSquadHandler:destroy()
    self.quickGroupHandler:destroy()

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
    local newSquadList = {}
    for _,sel in pairs( self.selectedSquads ) do
        table.insert( newSquadList, sel )
    end

    for _,v in pairs( squads ) do
        local exists = false
        for _,sel in pairs( self.selectedSquads ) do
            if sel == v then
                exists = true
            end
        end
        
        if exists == false then
            table.insert( newSquadList, v )
        end
    end    

    self:setSquadSelection( newSquadList )
end

function PoliceSquadHandler:setSquadSelection( squads )
    self.selectedSquads = squads     
    self.onSelectedSquadsChange( squads )

    self:setUsableAbilities( self.selectedSquads )

    if #(self.selectedSquads) > 0 then
        self.onSelectedUnitInformationChange( self:evaluateSquadInformation( self.selectedSquads[1] ))
    end

    self.quickGroupHandler:setSquadSelection( self.selectedSquads )
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
    for i=1, #(self.selectedSquads) do
        local squad = self:getSquad(self.selectedSquads[i])
        for _,member in pairs(squad.members) do
            abilities[member] = member.getAbilities()
            for i=1, #(member.getAbilities()) do

                local has = false

                for _,ag_bill in ipairs( aggregatedAbilities ) do
                    has = has or ag_bill == abilities[member][i]
                end

                if has == false then
                    table.insert( aggregatedAbilities, abilities[member][i] )
                end
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
                self.onEventMessage(TextLabel:new({label="Unit under attack"}))
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
        if self:CanUseAbility(ability) then
            self.isAiming = false
            self:SetReticuleRender(false)
            self.AimingFunction = nil
            self:UseSprint() 
        end
    elseif ability == core.system.squad.abilities.Flee then
        if self:CanUseAbility(ability) then
            self.isAiming = false
            self:SetReticuleRender(false)
            self.AimingFunction = nil
            self:UseFlee() 
        end
	elseif ability == core.system.squad.abilities.Attack then
        if self:CanUseAbility(ability) then
		    self.isAiming = true
		    self:SetReticuleRender(false)
		    self.AimingFunction = self.AttackGroup
        end
    else        
		self.isAiming = false
		self:SetReticuleRender(false)
		self.AimingFunction = nil
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
        self:setAbility(nil)
        return
    end

    for member, abilities in pairs(self.usableAbilities) do    
        for i=1, #abilities do
            if abilities[i] == core.system.squad.abilities.TearGas then
                local wpc = member.entity:get(core.componentType.WorldPositionComponent)
                local rangeToTarget = math.sqrt(((wpc.position[1]-x) * (wpc.position[1]-x)) + ((wpc.position[2]-y) * (wpc.position[2]-y)) + ((wpc.position[3]-z) * (wpc.position[3]-z))) 
                if rangeToTarget < tearGas.tearGasRange then

                    if inRange == false then 
                        inRange = true 
                    end

                    if self.leftClicked then
                        local attributeComponent = member.entity:get(core.componentType.AttributeComponent)
                        if attributeComponent.stamina >= tearGas.tearGasStaminaCost then
                            --Consume click to avoid deselecting squads
                            self.leftClicked = false
                            self.leftPressed = false
                            self:UseTearGas(member.entity, x, y, z, rangeToTarget) 

                            if not keyboard.isKeyDown(keyboard.key.Left_shift) then
                                self:setAbility(nil)
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
        self.onEventMessage(TextLabel:new({label=errorMessage}))
    end
end

local moodOutline = 0
local rioterGroupIds = {}
function PoliceSquadHandler:HighlightMood()

	if not  keyboard.isKeyDown(core.config.playerBindings.moodHighlight) then

		if moodOutline == 1 then
			moodOutline = 0
			core.system.squad.disableOutline(rioterGroupIds)
			for k,v in pairs(rioterGroupIds) do rioterGroupIds[k]=nil end

		end
		return;
	end

	if moodOutline == 0 then
		local groupCount = core.system.groups.getNumberOfGroups()
	
		for i=0, groupCount-1 do 
			local members = core.system.groups.getMembersInGroup(i)
       
			if #members > 0 then
				local attrbComponent = members[1]:get(core.componentType.AttributeComponent)
				local utc = members[1]:get(core.componentType.UnitTypeComponent)
				if utc.unitType == core.UnitType.Rioter then
					 core.system.squad.enableMoodOutline({attrbComponent.groupID})
					 table.insert(rioterGroupIds, attrbComponent.groupID)
				end
			end
		end
		moodOutline = 1
	end
	
	
end

function PoliceSquadHandler:AttackGroup()
	if #(self.selectedSquads) == 0 or self.rightClicked then
	    self:setAbility(nil)	
		self.rightClicked = false
		self.rightPressed = false
		
		return
	end
	
    local mouseX, mouseY = mouse.getPosition()
    local aspct = core.entity.generateAspect( core.componentType.AttributeComponent, core.componentType.UnitTypeComponent, core.componentType.BoundingVolumeComponent )
    local selectedEntity = core.system.picking.getHitEntity(mouseX, mouseY, aspct )

	if selectedEntity then
		local unitTypeComponent = selectedEntity:get(core.componentType.UnitTypeComponent);
		local attributeComponent = selectedEntity:get(core.componentType.AttributeComponent);

		if attributeComponent and unitTypeComponent then                 
			
			--cursor is hovering over rioters
			if unitTypeComponent.unitType == core.UnitType.Rioter then
				s_squad.enableOutline( { attributeComponent.groupID }, standardPolice.selectionPrimaryOutline:get() )
				self.outlinedRioterGroups = attributeComponent.groupID;
				
				if self.leftClicked then
					self.isAiming = false
					self:SetReticuleRender(false)
					self.AimingFunction = nil
					
					self.leftClicked = false
					self.leftPressed = false
					
					core.orders.attackGroup( self.selectedSquads, attributeComponent.groupID )
				end				
			end			
		end		
	elseif self.leftClicked then
	
		self.leftClicked = false
		self.leftPressed = false
	end	
end

function PoliceSquadHandler:RevertAttackingStateOfSelected()
	if #(self.selectedSquads) ~= 0 then
		core.orders.attackGroup( self.selectedSquads, -1 )
	end
end

function PoliceSquadHandler:UseTearGas(usingEntity, x, y, z, rangeToTarget)
    local attributeComponent = usingEntity:get(core.componentType.AttributeComponent)
    usingEntity:set(core.componentType.AttributeComponent, {stamina = (attributeComponent.stamina - tearGas.tearGasStaminaCost)}, true)

    local wpc = usingEntity:get(core.componentType.WorldPositionComponent)        
    local projectileSpeed = 40
    local velocities = {(core.glm.vec3.normalize(core.glm.vec3.new(x - wpc.position[1], 0, z - wpc.position[3])) * projectileSpeed):get()}

    local t = (rangeToTarget * 0.5 / projectileSpeed)

    local smokeTrailEntity = {
        targetDistance = rangeToTarget,
        traveledDistance = 0,
        speed = projectileSpeed,
        v_x = velocities[1],
        v_y = 9.82 * t, 
        v_z = velocities[3],
    }    

    smokeTrailEntity.entity = core.entity.create(core.componentType.EmitterComponent
                                        , core.componentType.WorldPositionComponent)

    smokeTrailEntity.entity:set(core.componentType.WorldPositionComponent, {position = {wpc.position[1], wpc.position[2] + 1.5, wpc.position[3]}})
    smokeTrailEntity.entity:set(core.componentType.EmitterComponent, {
            rate = 400,
            offset = {0, 0, 0},
            life = 0.7,
            lifeVariance = 0.25,
            lifeReduction = 0.8,
            lifeReductionVariance = 0,
            velocity = {0, 0, 1.5},
            velocityVariance = {0, 0, 0.5},
            acceleration = {0, 2, 0},
            coneDirection = {0, 1, 0},
            coneAngle = 180,
            coneAngleVariance = 0,
            type = core.system.particle.emitters.Cone,
            handle = self.particleDefinitions["GrenadeTrail"]
            }, true)

    smokeTrailEntity.update = function(o, delta)        
        if o.traveledDistance >= o.targetDistance then
            local abilityEntity = {life = 10}
            local wpc = o.entity:get(core.componentType.WorldPositionComponent)

            abilityEntity.entity = core.entity.create(core.componentType.EmitterComponent
                                                , core.componentType.WorldPositionComponent
                                                , core.componentType.MovementComponent
                                                , core.componentType.MovementDataComponent
                                                , core.componentType.RotationComponent
                                                , core.componentType.UnitTypeComponent
                                                , core.componentType.AttributeComponent
                                                , core.componentType.FlowfieldComponent)

            abilityEntity.entity:set(core.componentType.AttributeComponent, {pfObjectType = core.PFObjectType.TearGas}, true)
            abilityEntity.entity:set(core.componentType.UnitTypeComponent, {unitType = core.UnitType.Object}, true)
            abilityEntity.entity:set(core.componentType.WorldPositionComponent, {position = {wpc.position[1], wpc.position[2], wpc.position[3]}})
            abilityEntity.entity:set(core.componentType.EmitterComponent, {
                    rate = 100,
                    offset = {0, -2, 0},
                    life = 3,
                    lifeVariance = 0.5,
                    lifeReduction = 1.5,
                    lifeReductionVariance = 0,
                    velocity = {0, 0, 2.5},
                    velocityVariance = {0, 0, 4},
                    acceleration = {0, 2, 0},
                    coneDirection = {0, 1, 0},
                    coneAngle = 60,
                    coneAngleVariance = 30,
                    type = core.system.particle.emitters.Cone,
                    handle = self.particleDefinitions["TearGas"]
                    }, true)

            abilityEntity.update = function(o, delta)
                if o.life <= 0 then
                    print "Removing Smoke"
                    o.entity:destroy()
                    local newAbilityEntities = {}
                    for i=1, #(self.abilityEntities) do
                        if self.abilityEntities[i] ~= o then
                            table.insert(newAbilityEntities, self.abilityEntities[i])
                        end    
                    end
                    self.abilityEntities = newAbilityEntities
                else
                    o.life = o.life - delta
                end

            end

            self.abilityEntities[#(self.abilityEntities) + 1] = abilityEntity
            
            o.entity:destroy()
            local newAbilityEntities = {}
            for i=1, #(self.abilityEntities) do
                if self.abilityEntities[i] ~= o then
                    table.insert(newAbilityEntities, self.abilityEntities[i])
                end    
            end
            self.abilityEntities = newAbilityEntities
        else
            o.traveledDistance = o.traveledDistance + o.speed * delta
            o.v_y = o.v_y - 9.82 * delta

            local wpc = o.entity:get(core.componentType.WorldPositionComponent)
            o.entity:set(core.componentType.WorldPositionComponent, {position = {wpc.position[1] + o.v_x * delta, wpc.position[2] + o.v_y * delta, wpc.position[3] + o.v_z * delta}})           
        end
    end
    

    self.abilityEntities[#(self.abilityEntities) + 1] = smokeTrailEntity
end

function PoliceSquadHandler:UseSprint()

    for member, abilities in pairs(self.usableAbilities) do    
        for i=1, #abilities do
            if abilities[i] == core.system.squad.abilities.Sprint then
                member.isSprinting = true                
            end
        end
    end
    
	self:RevertAttackingStateOfSelected()
    self:setAbility(nil)
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
	
	self:RevertAttackingStateOfSelected()
    self:setAbility(nil)
end

function PoliceSquadHandler:CycleSquad()
    if self.cycleSquad > #(self.createdSquads) then
        self.cycleSquad = 1
    end

    if #(self.createdSquads) > 0 then
        if #(self.selectedSquads) > 0 then

            for i=1, #(self.createdSquads) do
                local found = false

                for _,v in pairs(self.selectedSquads) do
                    if v == self.createdSquads[self.cycleSquad].groupId or #(self.createdSquads[self.cycleSquad].members) > 0 then
                        found = true
                    end 
                end       

                if found then
                    self.cycleSquad = self.cycleSquad + 1

                    if self.cycleSquad > #(self.createdSquads) then
                        self.cycleSquad = 1
                    end
                else
                    break
                end
            end
        else
            for i=1, #(self.createdSquads) do
                if #(self.createdSquads[self.cycleSquad].members) > 0 then
                    break
                end                

                self.cycleSquad = self.cycleSquad + 1
                if self.cycleSquad > #(self.createdSquads) then
                    self.cycleSquad = 1
                end
            end
        end
    end

    self:DeselectAllSquads()
    self:addSquadsToSelection({self.createdSquads[self.cycleSquad].groupId})
    local x,y,z = core.system.groups.getGroupMedianPosition(self.createdSquads[self.cycleSquad].groupId)
    self.cycleSquad = self.cycleSquad + 1
    return x,y,z 
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
        self:setAbility(nil)
        --Consume event to prevent setting goals
        self.rightClicked = false
        self.rightPressed = false
    end

    local i = 1
    while i <= #(self.abilityEntities) do
        self.abilityEntities[i]:update(delta)
        i=i+1
    end

	if keyboard.isKeyDownOnce(keyboard.key.Kp_1) or keyboard.isKeyDownOnce(core.config.playerBindings.attackAbility) then
        self:setAbility(core.system.squad.abilities.Attack)
	end

    if keyboard.isKeyDownOnce(keyboard.key.Kp_2) or keyboard.isKeyDownOnce(core.config.playerBindings.tearGasAbility) then
        self:setAbility(core.system.squad.abilities.TearGas)
    end	

    if keyboard.isKeyDownOnce(keyboard.key.Kp_3) or keyboard.isKeyDownOnce(core.config.playerBindings.sprintAbility) then
        self:setAbility(core.system.squad.abilities.Sprint)
    end

    if keyboard.isKeyDownOnce(keyboard.key.Kp_4) or keyboard.isKeyDownOnce(core.config.playerBindings.fleeAbility) then
        self:setAbility(core.system.squad.abilities.Flee)
    end

	if keyboard.isKeyDownOnce(core.config.playerBindings.haltOrder) then
		core.orders.haltGroup( self.selectedSquads )
	end
	
	
    --Ability: Sprint
    -- This block is expensive (~2ms) should be put in a c-system
    for _,squad in pairs(self.createdSquads) do
        local squadEntity = core.system.squad.getSquadEntity(squad.groupId)
        assert( squadEntity, "no squad entity bound to " .. squad.groupId )
        local sqdc = squadEntity:get(core.componentType.SquadComponent)
        for _,member in pairs(squad.members) do
            local attrbc = member.entity:get(core.componentType.AttributeComponent)
            if member.isSprinting == true then
                local remainingStamina = attrbc.stamina - sprinting.sprintingStaminaCost * delta
                if remainingStamina > 0 then
                    local wpc = member.entity:get(core.componentType.WorldPositionComponent)
                    local frmtnc = member.entity:get(core.componentType.FormationComponent)
                    local goal = {sqdc.squadGoal[1] + frmtnc.relativePosition[1], 
                                    sqdc.squadGoal[2],
                                    sqdc.squadGoal[3] + frmtnc.relativePosition[2]}
                    local pos = wpc.position

                    local distToTarget = math.sqrt((pos[1] - goal[1]) * (pos[1] - goal[1]) + (pos[2] - goal[2]) * (pos[2] - goal[2]) + (pos[3] - goal[3]) * (pos[3] - goal[3]))

                    if distToTarget > sprinting.sprintingReachThreshold then                
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
                local remainingStamina = attrbc.stamina + policeStamina.staminaRegeneration * delta

                if remainingStamina > member.maximumStamina then
                    remainingStamina = member.maximumStamina
                end
                --These functions are expensive, should be optimized
                member.entity:set(core.componentType.MovementComponent, {state = core.movementData.Jogging}, true)
                member.entity:set(core.componentType.AttributeComponent, {stamina = remainingStamina}, true)
            end         
        end
    end

    if self.isAiming == true then
        self:AimingFunction()
	else
		self:HighlightMood()
    end   

    if keyboard.isKeyDownOnce(core.config.playerBindings.rotateSquadSelection) and #(self.selectedSquads) > 0 then        
        local firstSquad = self.selectedSquads[1]
        table.remove(self.selectedSquads, 1)
        table.insert(self.selectedSquads, firstSquad)
    end

    --Formations
    --Click Selection
    if self.leftClicked then               

        local clickTime = os.clock()
		self.boxStartX, self.boxStartY = mouse.getPosition()

        local aspct = core.entity.generateAspect( core.componentType.AttributeComponent, core.componentType.UnitTypeComponent, core.componentType.BoundingVolumeComponent )
        local selectedEntity = core.system.picking.getHitEntity(self.boxStartX, self.boxStartY, aspct )

        if selectedEntity then
            local unitTypeComponent = selectedEntity:get(core.componentType.UnitTypeComponent);
            local attributeComponent = selectedEntity:get(core.componentType.AttributeComponent);            

            --Selected normal police unit
            if unitTypeComponent.unitType == core.UnitType.Police then
                local squad = self:getSquad(attributeComponent.squadID)

                local deltaTime = (clickTime - self.lastClickTime)
                print( deltaTime)
                if deltaTime < core.config.doubleClickDelay and self.lastClickedType == squad.type then
                    --Double click same unit type, select all units of same type
                    local selectedSquads = {}
                    for _,v in pairs(self.createdSquads) do
                        if v.type == squad.type then
                            table.insert(selectedSquads, v.groupId)
                        end
                    end

                    self:DeselectAllSquads()
                    self:addSquadsToSelection(selectedSquads)                    
                else
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
                self.lastClickedType = squad.type
            end
		elseif not keyboard.isKeyDown(keyboard.key.Left_shift) and not core.config.stickySelection and not self.isClick then
			self:DeselectAllSquads()
        end

        self.lastClickTime = clickTime
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
				self:RevertAttackingStateOfSelected()
            else                
                --Tell other systems that we are telling things to move.
                s_squad.setSquadGoal(self.selectedSquads, self.clickStartX, self.clickStartY, self.clickStartZ)
				self:RevertAttackingStateOfSelected()
            end

            local clickPos = core.glm.vec3.new(core.system.picking.getGroundHit(  mouseX, mouseY ))

            local clickStart = core.glm.vec3.new( self.clickStartX, self.clickStartY, self.clickStartZ )
            local clickEnd = core.glm.vec3.new( self.clickEndX, self.clickEndY, self.clickEndZ )
            --Event that create move marker
            local clickDiff = clickEnd - clickStart 
            if clickDiff:dot(clickDiff) < 10.0 then
                self.onMoveToPosition( self.selectedSquads, clickPos, core.system.picking.getIsInsideNavigationMesh(clickPos) )
            end


            self.clickStartX, self.clickStartY, self.clickStartZ = nil, nil, nil
            self.isClick = false
        end
    end

    --Formation selection
    if keyboard.isKeyDown(core.config.playerBindings.halfCircleFormation) then
        self:setFormation( s_squad.formations.HalfCircleFormation )
    elseif keyboard.isKeyDown(core.config.playerBindings.circleFormation) then
        self:setFormation( s_squad.formations.CircleFormation )
    elseif keyboard.isKeyDown(core.config.playerBindings.lineFormation) then
        self:setFormation( s_squad.formations.LineFormation)
    end

    --Stances
    if keyboard.isKeyDown(core.config.playerBindings.aggressiveStance)  then        
        if #(self.selectedSquads) > 0 then
            self:setStance( core.PoliceStance.Aggressive )
        end   
    elseif keyboard.isKeyDown(core.config.playerBindings.defensiveStance) then
        if #(self.selectedSquads) > 0 then
            self:setStance( core.PoliceStance.Defensive )
        end   
    elseif keyboard.isKeyDown(core.config.playerBindings.passiveStance) then
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
