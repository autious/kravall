local objective_handler = require "objective_handler"
local objective = require "objective"
local fac_image = require "factories/image"
local window = require "window"
local Camera = require "rts_camera"
local KravallControl = require "gui/KravallControl"
local PoliceSquadHandler = require "gamemodes/kravall/PoliceSquadHandler"
local PDC = require "particle_definition"
local ASM = require "assembly_loader"
local MoveMarker = require "visual_objects/MoveMarker"

local T = {}

local keyboard = core.input.keyboard
local mouse = core.input.mouse
local s_squad = core.system.squad

function T:new(o)
    o = o or {}

    setmetatable( o, self )
    self.__index = self

    o.objectiveHandler = objective_handler:new()
    o.camera = Camera.new()
	
	-- set default movementData
	core.movementData.setMovementMetaData( core.movementData.Walking, 5.8, 17, 17, 0.0 )
	core.movementData.setMovementMetaData( core.movementData.Sprinting, 8.8, 17, 14, 0.2 )

    return o
end

--This function should not be used once squad creation is moved to inside Kravall game mode
function T:addSquad(squad)
    self.policeHandler:addSquad(squad)
end

function T:init()

    self.asm = ASM.loadPack({})
    self.particleDefinitions =
    {
        TearGas = PDC:createParticleDefinition(self.asm, 5000, "assets/texture/particle/smoke.material")
    }

    self.moveMarker = MoveMarker:new()
    self.gui = KravallControl:new( 
    {
        -- Called when the user is changing the formation from the gui.
        onFormationSelect = function( formation )
            self.policeHandler:setFormation( formation )
        end,
        -- Called when the user is changing stance from the gui.
        onStanceSelect = function( stance )
            self.policeHandler:setStance( stance )
        end,

        -- Called when the user is changing the current active ability in the gui.
        onAbilitySelect = function( ability )
            self.policeHandler:setAbility( ability )
        end
    })

    self.policeHandler = PoliceSquadHandler:new( 
    {
        particleDefinitions = self.particleDefinitions,
        -- Called when the currently active formation is changed logically.
        onFormationChange = function( formation )
            self.gui:setFormation( formation ) 
        end,
        -- Called when the currently active stance is changed logically.
        onStanceChange = function( stance )
            self.gui:setStance( stance )
        end,

        onAbilityChange = function( ability )
            self.gui:setAbility( ability )
        end,

        -- Called whenever the selection changes
        -- might contain previously sent units.
        -- Could be empty
        onSelectedSquadsChange = function( squads )
            self.gui:setSelectedSquads( squads )
        end,

        onUsableAbilitiesChange = function( abilities )
            self.gui:setUsableAbilities( abilities )
        end,

        -- Called when the currently active unit changes
        -- or when the state of the unit might have changes (like health)
        onSelectedUnitInformationChange = function( data )
            self.gui:setUnitInformation( data ) 
        end,
        onMoveToPosition = function( squads, position, accept )
            if accept then
                self.moveMarker:playAccept(  position )
            else
                self.moveMarker:playDeny( position )
            end
        end,
        
    })	
end

function T:update( delta )
    self.objectiveHandler:update( delta )
    self.gui:update(delta)
    self.policeHandler:update(delta)

    if self.popup == nil then
        if self.objectiveHandler:isWin() then
            self.popup = fac_image( window.width/2, window.height/2, "assets/texture/ui/win.material",true )
        elseif self.objectiveHandler:isLoss() then
            self.popup = fac_image( window.width/2, window.height/2, "assets/texture/ui/loss.material",true ) 
        end
    end
    self.camera:update( delta )
    self.moveMarker:update( delta )
end

function T:destroy()
    self.gui:destroy()
    self.objectiveHandler:destroy() 
    if self.popup ~= nil then
        self.popup:destroy()
    end
    self.moveMarker:destroy()
	core.gameMetaData.clearGameData()
end

function T:name()
    return "Kravall"
end

return T
