local MoveMarker = require "visual_objects/MoveMarker"
local PoliceSquadHandler = require "gamemodes/kravall/PoliceSquadHandler"
local OverviewHandler = require "gamemodes/kravall/OverviewHandler"
local KravallControl = require "gui/kravall/main/KravallControl"
local PDC = require "particle_definition"
local ASM = require "assembly_loader"

local input = require "input"
local ent = require "entities"
local squadInstance = ent.get "squadInstance"

local Main = 
            { 
                name = "Main",
                onRequestPause = function() core.log.error("missing function onRequestPause in Main") end,
            }

local function registerCallbacks(o)
    input.registerOnButton( o.onButton, "GAME" )
end


function Main:new(o)
    o = o or {}
    setmetatable( o, self )
    self.__index = self

    assert( o.asm, "you need to give Main an asm instance" )


    o.particleDefinitions =
    {
        TearGas = PDC:createParticleDefinition(o.asm, 5000, "assets/texture/particle/smoke.material"),
        GrenadeTrail = PDC:createParticleDefinition(o.asm, 2000, "assets/texture/particle/smoke_trail.material")
    }

    o.onButton = function(button, action, mods, consumed)
        consumed = o.overviewHandler:onButton(button, action, mods, consumed)        

        o.policeHandler:onButton(button, action, mods, consumed)
        return consumed
    end

    --Init the things that are required.
    o.moveMarker = MoveMarker:new()

    o.gui = KravallControl:new( 
    {
        onRequestPause = o.onRequestPause,
        -- Called when the user is changing the formation from the gui.
        onFormationSelect = function( formation )
            o.policeHandler:setFormation( formation )
        end,
        -- Called when the user is changing stance from the gui.
        onStanceSelect = function( stance )
            o.policeHandler:setStance( stance )
        end,
        -- Called when the user is changing the current active ability in the gui.
        onAbilitySelect = function( ability )
            o.policeHandler:setAbility( ability )          
        end,

        onUseOverview = function()
            if o.overviewHandler.inOverview then
                local pos = {o.camera.position:get()}
                o.overviewHandler:ExitOverview(core.glm.vec3.new(pos[1], 0, pos[3]))
            else
                o.overviewHandler:EnterOverview()
            end
        end,

        onCycleSquads = function()
            local x,y,z = o.policeHandler:CycleSquad()
            
            local cameraPosition = {o.camera.position:get()}
            local cameraBackward = core.glm.vec3.normalize(core.camera.gameCamera:getForward() * (-1))
            local alpha = math.acos(core.glm.vec3.dot(cameraBackward, core.glm.vec3.new(0, 1, 0)))
            local angle = (math.pi/2) - alpha
            local distance = cameraPosition[2]
           
            if angle < math.pi/2 then
                distance = distance / math.sin(angle)
            end

            local pos = core.glm.vec3.new(x, y, z) + (cameraBackward * distance)
            o:AddCameraPoint(pos, o.camera.quatRotation) 
        end
    })

    o.policeHandler = PoliceSquadHandler:new( 
    {
        -- Called when the currently active formation is changed logically.
        onFormationChange = function( formation )
            o.gui:setFormation( formation ) 
        end,
        -- Called when the currently active stance is changed logically.
        onStanceChange = function( stance )
            o.gui:setStance( stance )
        end,

        onAbilityChange = function( ability )
            o.gui:setAbility( ability )
        end,

        -- Called whenever the selection changes
        -- might contain previously sent units.
        -- Could be empty
        onSelectedSquadsChange = function( squads )
            if squads == nil or #squads == 0 then
                o.gui.squadMenuGUI:setShow( false ) 
            else
                o.gui.squadMenuGUI:setShow( true ) 
            end
            o.gui:setSelectedSquads( squads )
        end,

        -- Called when the currently active unit changes
        -- or when the state of the unit might have changes (like health)
        onSelectedUnitInformationChange = function( data )
            o.gui:setUnitInformation( data ) 
        end,

        onMoveToPosition = function( squads, position, accept )
            if accept then
                o.moveMarker:playAccept(  position )
            else
                o.moveMarker:playDeny( position )
            end
        end,

        onUsableAbilitiesChange = function( abilities )
            o.gui:setUsableAbilities( abilities )
        end,

        onEventMessage = function( component )
            o.gui:addEvent(component)
        end,

        particleDefinitions = o.particleDefinitions,
    })	

    o.overviewHandler = OverviewHandler:new(
    {
        onEnterOverview = function()
            o:enterOverview()
            o.gui:setOverview(true)
        end,
        onExitOverview = function(pos)
            o:exitOverview(pos)
            o.gui:setOverview(false)
        end,       
    })
	
	local policeTeam = 1
    if o.unitInstances then
        for _,v in  pairs( o.unitInstances ) do
            o.policeHandler:addSquad( squadInstance( o.asm, v, o.activeWeaponList, policeTeam ) )
        end
    end

    registerCallbacks(o)

    return o
end

function Main:update(delta)

    self.gui:update(delta)

    self.policeHandler:update(delta)
   
    self.overviewHandler:update(delta)
    self.moveMarker:update( delta )

end

function Main:enterOverview()
    self.camera.inOverview = true
    self.policeHandler.takeInput = false
    self.cameraPosition = self.camera.position
    self.cameraRotation = self.camera.quatRotation
    self.cameraBackward = core.camera.gameCamera:getForward() * (-1)
    local pos = {self.camera.position:get()}
    self:AddCameraPoint(core.glm.vec3.new(pos[1], 250, pos[3]), core.glm.quat.new(math.sin(math.pi/4), 0, 0, math.cos(math.pi/4)))
end

function Main:exitOverview(target)
    self.camera.inOverview = false
    self.policeHandler.takeInput = true
    local camPos = {self.cameraPosition:get()}
    local alpha = math.acos(core.glm.vec3.dot(self.cameraBackward, core.glm.vec3.new(0, 1, 0)))
    local angle = (math.pi/2) - alpha
    local distance = camPos[2]
           
    if angle < math.pi/2 then
        distance = distance / math.sin(angle)
    end

    local pos = target + (self.cameraBackward * distance)

    self:AddCameraPoint(pos, self.cameraRotation)
end

function Main:AddCameraPoint(position, rotation)
    self.camera:addInterpolationPoint(position, rotation)
end

function Main:destroy()
    input.deregisterOnButton(self.onButton)
    self.moveMarker:destroy()
    self.policeHandler:destroy()
    self.gui:destroy()
end

return Main
