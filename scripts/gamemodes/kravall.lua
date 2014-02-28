local fac_image = require "factories/image"
local window = require "window"
local Camera = require "rts_camera"
local ObjectiveHandler = require "gui/component/ObjectiveHandler"

local keyboard = core.input.keyboard
local mouse = core.input.mouse
local s_squad = core.system.squad

local Prep = require( "gamemodes/kravall/state/Prep" ) 
local Main = require( "gamemodes/kravall/state/Main" )
local End = require( "gamemodes/kravall/state/End" )

local abilities = core.system.squad.abilities

local standardPolice = (require "game_constants").standardPolice

local ASM = require "assembly_loader"

local T = 
    { 
        initGamestate = "Main",
        weapons = 
        {
            punch = {1.0, 0.75, 20, 0.2, 0.05, 3.2, 2.9, 0.05, 0.5, "punch"}
        },
        policeTypes =
        {
            {
                name = "Common Squad",
                cost = 100,
                description = "Default",
                setup = 
                {
                    {
                        positionOffset = {0,0,0},
                        weapon = "punch",
                        mesh = "assets/model/animated/police/cop/cop-teargas_00.bgnome",
                        material = "assets/texture/animated/police/cop/cop-teargas_00.material",
                        abilities = {
                            abilities.Attack, 
                            abilities.ArrestGroup, 
                            abilities.Sprint, 
                            abilities.Rout,
                        },
                        health = standardPolice.maxHealth, 
                        stamina = standardPolice.maxStamina, 
                        morale = standardPolice.maxMorale, 
                    },
                },
            },
        }
    }

--T.gamestate can be either Prep, Main or End

function T:new(o)
    o = o or {}

    setmetatable( o, self )
    self.__index = self

    o.camera = Camera.new()
    o.onStateChangeFunctions = {}
    o.asm = ASM.loadPack({})

	
	-- set default movementData
	core.movementData.setMovementMetaData( core.movementData.Walking, 1.5, 17, 17, 0.0 )
	core.movementData.setMovementMetaData( core.movementData.Jogging, 5.8, 17, 14, 0.0 )
	core.movementData.setMovementMetaData( core.movementData.Sprinting, 8.8, 17, 14, 0.0 )

    return o
end

function T:setState( state )
    if type( self.gamestate ) == "table" then
        self.gamestate:destroy()
    end

    if state == "Main" then
        print( "State set to \"Main\"" )
        self.gamestate = Main:new( 
        { 
            unitInstances = self.unitInstances, --definitions of the units placed
            activeWeaponList = self.activeWeaponList,
            asm = self.asm,
        } )
    elseif state == "Prep" then
        print( "State set to \"Prep\"" )
        self.gamestate = Prep:new
        {
            policeTypes = self.policeTypes,
            onFinished = function( unitInstances ) 
                self.unitInstances = unitInstances
                self:setState( "Main" ) 
            end,
        }

    elseif state == "End" then
        print( "State set to \"End\"" )
        self.gamestate = End:new( {won = self.objectiveHandler:isWin() })
    else
        error( "Invalid state set" )
    end

    self:triggerOnStateChange( state )
end

function T:init()
    self.objectiveHandler = ObjectiveHandler:new{ anchor="NorthWest" }

    self.activeWeaponList = {}

    for i,v in pairs( self.weapons ) do
        self.activeWeaponList[i] = core.weaponData.pushWeapon(unpack(v))
    end 

    self:setState( self.initGamestate )
end

--This function should not be used once squad creation is moved to inside Kravall game mode
function T:addSquad(squad)
    self.gamestate.policeHandler:addSquad(squad)
end

function T:createObjective( )
    return self.objectiveHandler:createObjective( )
end

function T:addAreaSpawnZone(ent)
    assert( self.gamestate.name == "Prep", "Can't add spawn zones when not in Prep mode" )
    self.gamestate:addAreaSpawnZone( ent )
end

function T:update( delta )
    self.camera:update( delta )
    self.gamestate:update( delta )

    if self.objectiveHandler:isEnd() and self.gamestate.name ~= "End" then
        self:setState( "End" )
    end
end

function T:destroy()
    
    if type( self.gamestate ) == "table" then
        self.gamestate:destroy()
    end

    self.objectiveHandler:destroy()
    self.asm:destroy()
    -- Remove all weapons that was created.
	core.gameMetaData.clearGameData()
    
end

function T:registerOnStateChange(f)
    self.onStateChangeFunctions[f] = true   
end

function T:deregisterOnStateChange(f)
    self.onStateChangeFunctions[f] = nil
end

function T:triggerOnStateChange( stateName )
    local deleteList = {}
    for i,_ in pairs( self.onStateChangeFunctions ) do
        deleteList[i] = i(stateName)
    end

    for i,v in pairs( deleteList ) do
        --Keep or destroy based on return value
        self.onStateChangeFunctions[i] = v or nil
    end
end

function T:name()
    return "Kravall"
end

return T