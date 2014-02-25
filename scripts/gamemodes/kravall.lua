local fac_image = require "factories/image"
local window = require "window"
local Camera = require "rts_camera"
local ObjectiveHandler = require "gui/component/ObjectiveHandler"

local keyboard = core.input.keyboard
local mouse = core.input.mouse
local s_squad = core.system.squad

local T = { initGamestate = "Main" }

--T.gamestate can be either Prep, Main or End

function T:new(o)
    o = o or {}

    setmetatable( o, self )
    self.__index = self

    o.camera = Camera.new()
	
	-- set default movementData
	core.movementData.setMovementMetaData( core.movementData.Walking, 5.8, 17, 17, 0.0 )
	core.movementData.setMovementMetaData( core.movementData.Sprinting, 8.8, 17, 14, 0.2 )

    return o
end

function T:setState( state )
    if type( self.gamestate ) == "table" then
        self.gamestate:destroy()
    end

    if state == "Main" then
        print( "State set to \"Main\"" )
        self.gamestate = require( "gamemodes/kravall/state/Main" ):new()
    elseif state == "Prep" then
        print( "State set to \"Prep\"" )
        self.gamestate =  require( "gamemodes/kravall/state/Prep" ):new()
    elseif state == "End" then
        print( "State set to \"End\"" )
        self.gamestate =  require( "gamemodes/kravall/state/End" ):new()
    end
end

function T:init()
    self.objectiveHandler = ObjectiveHandler:new{ anchor="NorthWest" }

    self:setState( self.initGamestate )
end

function T:createObjective( )
    return self.objectiveHandler:createObjective( )
end

function T:update( delta )
    self.camera:update( delta )
    self.gamestate:update( delta )
end

function T:destroy()
    if type( self.gamestate ) == "table" then
        self.gamestate:destroy()
    end

	core.gameMetaData.clearGameData()
end

function T:name()
    return "Kravall"
end

return T
