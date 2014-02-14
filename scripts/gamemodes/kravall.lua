local objective_handler = require "objective_handler"
local objective = require "objective"
local fac_image = require "factories/image"
local window = require "window"
local Camera = require "rts_camera"
local KravallControl = require "gui/KravallControl"
local PoliceSquadHandler = require "gamemodes/kravall/PoliceSquadHandler"
local T = {}

local keyboard = core.input.keyboard
local mouse = core.input.mouse
local s_squad = core.system.squad

core.movementData.setMovementMetaData( core.movementData.Walking, 5.8, 17, 17, 0.0 )
core.movementData.setMovementMetaData( core.movementData.Sprinting, 8.8, 17, 14, 0.2 )

-- usual weapons...
fists = core.weaponData.pushWeapon( 1.0, 20, 0.2, 0.05, 0.05, 0.5, 3 )


function T:new(o)
    o = o or {}

    setmetatable( o, self )
    self.__index = self

    o.objectiveHandler = objective_handler:new()
    o.camera = Camera.new()

    return o
end

function T:init()
    print"NOT HERE"
    local function onFormationSelect(formation )
        self.policeHandler:setFormation( formation )
    end

    self.gui = KravallControl:new( {onFormationSelect = onFormationSelect} )

    local function onFormationChange(formation)
        self.gui:setFormation( formation ) 
    end 
    self.policeHandler = PoliceSquadHandler:new( {onFormationChange = onFormationChange} )
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
end

function T:destroy()
    self.gui:destroy()
    self.objectiveHandler:destroy() 
    if self.popup ~= nil then
        self.popup:destroy()
    end
end

function T:name()
    return "Kravall"
end

return T
