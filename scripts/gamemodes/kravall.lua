local fac_image = require "factories/image"
local window = require "window"
local Camera = require "rts_camera"
local ObjectiveHandler = require "gui/component/ObjectiveHandler"
local input = require "input"
local PauseMenuGUI = require "gui/kravall/PauseMenuGUI"

local keyboard = core.input.keyboard
local mouse = core.input.mouse
local s_squad = core.system.squad

local Prep = require( "gamemodes/kravall/state/Prep" ) 
local Main = require( "gamemodes/kravall/state/Main" )
local End = require( "gamemodes/kravall/state/End" )

local abilities = core.system.squad.abilities

local standardPolice = (require "game_constants").standardPolice

local ASM = require "assembly_loader"

local standardPolice = (require "game_constants").standardPolice
local tearGasPolice = (require "game_constants").tearGasPolice

local T = 
    { 
        cashLimit = 1000,
        initGamestate = "Main",
        weapons = 
        {
            -- range, graceDistance, damageToHealth, damageToMorale, damageToMoraleOnMiss, enemyRageIncrease, enemyPressureIncrease, staminaCost, timeWhenAnimationDealsDamage, animationName
            punch = {1.0, 0.75, 20, 0.2, 0.05, 3.2, 2.9, 0.05, 0.5, "punch"},
            shield = {
                1.0,    -- range
                0.75,   -- graceDiscance
                40,     -- damageToHealth
                0.0,    -- damageToMorale
                0.0,    -- damageToMoraleOnMiss
                4,      -- enemyRageIncrease
                0.0,    -- enemyPressureIncrease
                0.02,   -- staminaCost
                0.3,    -- timeWhenAnimationDealsDamage
                "punch" -- animationName
            }
        },
        policeTypes =
        {
            {
                name = "Shield Squad",
                description = "Can beat the fuck out of you",
                cost = standardPolice.cost,
                setup = 
                {

                    {
                        positionOffset = {2,0,0},
                        weapon = "shield",
                        mesh = "assets/model/animated/police/cop/cop-shield_00.bgnome",
                        material = "assets/texture/animated/police/cop/cop-shield_00.material",
                        abilities = {
                            abilities.Attack, 
                            abilities.Sprint, 
                            abilities.Flee,
                            abilities.Halt,
                        },
                        health = standardPolice.maxHealth, 
                        stamina = standardPolice.maxStamina, 
                        morale = standardPolice.maxMorale, 
                    },
                    {
                        positionOffset = {0,0,0},
                        weapon = "shield",
                        mesh = "assets/model/animated/police/cop/cop-shield_00.bgnome",
                        material = "assets/texture/animated/police/cop/cop-shield_00.material",
                        abilities = {
                            abilities.Attack, 
                            abilities.Sprint, 
                            abilities.Flee,
                            abilities.Halt,
                        },
                        health = standardPolice.maxHealth, 
                        stamina = standardPolice.maxStamina, 
                        morale = standardPolice.maxMorale, 
                    },
                    {
                        positionOffset = {1,0,0},
                        weapon = "shield",
                        mesh = "assets/model/animated/police/cop/cop-shield_00.bgnome",
                        material = "assets/texture/animated/police/cop/cop-shield_00.material",
                        abilities = {
                            abilities.Attack, 
                            abilities.Sprint, 
                            abilities.Flee,
                            abilities.Halt,
                        },
                        health = standardPolice.maxHealth, 
                        stamina = standardPolice.maxStamina, 
                        morale = standardPolice.maxMorale, 
                    },
                    {
                        positionOffset = {0,0,1},
                        weapon = "shield",
                        mesh = "assets/model/animated/police/cop/cop-shield_00.bgnome",
                        material = "assets/texture/animated/police/cop/cop-shield_00.material",
                        abilities = {
                            abilities.Attack, 
                            abilities.Sprint, 
                            abilities.Flee,
                            abilities.Halt,
                        },
                        health = standardPolice.maxHealth, 
                        stamina = standardPolice.maxStamina, 
                        morale = standardPolice.maxMorale, 
                    },
                    {
                        positionOffset = {0,0,-1},
                        weapon = "shield",
                        mesh = "assets/model/animated/police/cop/cop-shield_00.bgnome",
                        material = "assets/texture/animated/police/cop/cop-shield_00.material",
                        abilities = {
                            abilities.Attack, 
                            abilities.Sprint, 
                            abilities.Flee,
                            abilities.Halt,
                        },
                        health = standardPolice.maxHealth, 
                        stamina = standardPolice.maxStamina, 
                        morale = standardPolice.maxMorale, 
                    },
                    {
                        positionOffset = {-1,0,0},
                        weapon = "shield",
                        mesh = "assets/model/animated/police/cop/cop-shield_00.bgnome",
                        material = "assets/texture/animated/police/cop/cop-shield_00.material",
                        abilities = {
                            abilities.Attack, 
                            abilities.Sprint, 
                            abilities.Flee,
                            abilities.Halt,
                        },
                        health = standardPolice.maxHealth, 
                        stamina = standardPolice.maxStamina, 
                        morale = standardPolice.maxMorale, 
                    },
                },
            },
            {
                name = "Teargas Squad",
                description = "Can make people cry ='D",
                cost = tearGasPolice.cost,
                setup = 
                {
                    {
                        positionOffset = {-1,0,0},
                        weapon = "punch",
                        mesh = "assets/model/animated/police/cop/cop-teargas_00.bgnome",
                        material = "assets/texture/animated/police/cop/cop-teargas_00.material",
                        abilities = {
                            abilities.Attack, 
                            abilities.Sprint, 
                            abilities.TearGas, 
                            abilities.Flee,
                            abilities.Halt,
                        },
                        health = tearGasPolice.maxHealth, 
                        stamina = tearGasPolice.maxStamina, 
                        morale = tearGasPolice.maxMorale, 
                    },
                    {
                        positionOffset = {0,0,1},
                        weapon = "punch",
                        mesh = "assets/model/animated/police/cop/cop-teargas_00.bgnome",
                        material = "assets/texture/animated/police/cop/cop-teargas_00.material",
                        abilities = {
                            abilities.Attack, 
                            abilities.Sprint, 
                            abilities.Flee,
                            abilities.Halt,
                        },
                        health = tearGasPolice.maxHealth, 
                        stamina = tearGasPolice.maxStamina, 
                        morale = tearGasPolice.maxMorale, 
                    },
                    {
                        positionOffset = {1,0,0},
                        weapon = "punch",
                        mesh = "assets/model/animated/rioter/rioter-male_00.bgnome",
                        material = "assets/texture/animated/police/cop/cop-light_00.material",
                        abilities = {
                            abilities.Attack, 
                            abilities.Sprint, 
                            abilities.Flee,
                            abilities.Halt,
                        },
                        health = standardPolice.maxHealth, 
                        stamina = standardPolice.maxStamina, 
                        morale = standardPolice.maxMorale, 
                    },
                    {
                        positionOffset = {0,0,-1},
                        weapon = "punch",
                        mesh = "assets/model/animated/rioter/rioter-male_00.bgnome",
                        material = "assets/texture/animated/police/cop/cop-light_00.material",
                        abilities = {
                            abilities.Attack, 
                            abilities.Sprint, 
                            abilities.Flee,
                            abilities.Halt,
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
    o.beforeStateChangeFunctions = {}
    o.asm = ASM.loadPack({})

    return o
end

function T:setState( state )

    self:triggerBeforeStateChange( state )

    if type( self.gamestate ) == "table" then
        self.gamestate:destroy()
    end

    if state == "Main" then
        print( "State set to \"Main\"" )
        self.gamestate = Main:new( 
        { 
            onRequestPause = function() self:togglePause() end,
            unitInstances = self.unitInstances, --definitions of the units placed
            activeWeaponList = self.activeWeaponList,
            camera = self.camera,
            asm = self.asm,
        } )
		
		self.objectiveHandler:setShow( true )
    elseif state == "Prep" then
        print( "State set to \"Prep\"" )
        self.gamestate = Prep:new
        {
            cashLimit = self.cashLimit,
            policeTypes = self.policeTypes,
            onFinished = function( unitInstances ) 
                self.unitInstances = unitInstances
                self:setState( "Main" ) 
            end,
        }
		self.objectiveHandler:setShow( false )

    elseif state == "End" then
        print( "State set to \"End\"" )
		
        self.gamestate = End:new( { won=self.objectiveHandler:isWin(), objectiveList=self.objectiveHandler.objectiveComponents } )
		
		self.objectiveHandler:setShow( false )
    else
        error( "Invalid state set" )
    end

    self:triggerOnStateChange( state )
end

function T:init()
    self.objectiveHandler = ObjectiveHandler:new{ anchor="NorthWest" }

    self.activeWeaponList = {}

    self.onKeyCallback = function( key, scancode, action )
        if key == keyboard.key.Escape and action == core.input.action.Press then
            self:togglePause()      
        end
    end

    input.registerOnKey( self.onKeyCallback )

    for i,v in pairs( self.weapons ) do
        self.activeWeaponList[i] = core.weaponData.pushWeapon(unpack(v))
    end 

    self:setState( self.initGamestate )
end

function T:togglePause()
    if self.pauseGUI then
        self.pauseGUI:destroy()
        self.pauseGUI = nil
    else
        self.pauseGUI = PauseMenuGUI:new( { onClickContinue = function() self:togglePause() end } )
    end  
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
    
    input.deregisterOnKey( self.onKeyCallback )

    if self.pauseGUI then
        self.pauseGUI:destroy()
        self.pauseGUI = nil
    end

    if type( self.gamestate ) == "table" then
        self.gamestate:destroy()
    end

    if self.objectiveHandler then
        self.objectiveHandler:destroy()
        self.objectiveHandler = nil
    end

    if self.asm then
        self.asm:destroy()
        self.asm = nil
    end

    -- Remove all weapons that was created.
	core.gameMetaData.clearGameData()
    
end

function T:registerOnStateChange(f)
    self.onStateChangeFunctions[f] = true   
end

function T:registerBeforeStateChange(f)
    self.beforeStateChangeFunctions[f] = true
end

function T:deregisterOnStateChange(f)
    self.onStateChangeFunctions[f] = nil
end

function T:deregisterBeforeStateChange(f)
    self.beforeStateChangeFunctions[f] = nil
end

function T:triggerBeforeStateChange( stateName )
    local deleteList = {}
    for i,_ in pairs( self.beforeStateChangeFunctions ) do
        deleteList[i] = i(stateName)
    end

    for i,v in pairs( deleteList ) do
        --Keep or destroy based on return value
        self.beforeStateChangeFunctions[i] = v or nil
    end
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
