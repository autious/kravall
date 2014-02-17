local window = require "window"
local Camera = require "rts_camera"
local PDC = require "particle_definition"
local ASM = require "assembly_loader"

local T = {}

local keyboard = core.input.keyboard
local mouse = core.input.mouse
local particleDefinition

function T:new(o)
    o = o or {}

    setmetatable( o, self )
    self.__index = self

    o.camera = Camera.new()
    return o
end

function T:init()
    self.asm = ASM.loadPack({})
    particleDefinition = PDC:createParticleDefinition(self.asm, 2000, "assets/texture/particle/smoke.material")

    self.asm:loadAssembly(
    {
        {
            type = core.componentType.WorldPositionComponent,
            data = { position = { 0, 0, 0} }
        },
        {
            type = core.componentType.EmitterComponent,
            data = { 
                rate = 100,
                offset = {0, 0, 0},
                life = 2.5,
                lifeVariance = 0,
                lifeReduction = 1.0,
                lifeReductionVariance = 0,
                velocity = {0, 0, 10},
                velocityVariance = {0, 0, 0},
                acceleration = {0, 0, 0},
                coneDirection = {0, 1, 0},
                coneAngle = 30.0,
                coneAngleVariance = 0.0,
                type = core.system.particle.emitters.Cone,
                handle = particleDefinition                
            },
            ignoreHard = true

        }
        
    })

    local GUI = require "gui/GUI"
    local Button = require "gui/component/Button"
    local Slider = require "gui/component/Slider"
    local Checkbox = require "gui/component/Checkbox"

    local CenterPlacer = require "gui/placement/CenterPlacer"
    local WestPlacer = require "gui/placement/WestPlacer"

    local gui = GUI:new()

    local button = Button:new({x=1000,y=100})
    function button:onClick()
            scen.lastCreated = rioter( scen, 25, 0, 0, groupOneGroupId)
    end
    local folder = "assets/texture/ui/"

    local rateSlider = Slider:new({x=1000,y=300})
    local xOffsetSlider = Slider:new({matReleased=folder .. "slider-knob-release_01.material", matPressed=folder .. "slider-knob-press_01.material", matHover=folder .. "slider-knob-hover_01.material", matBackground=folder .. "slider-background_01.material" })
    local yOffsetSlider = Slider:new()
    local zOffsetSlider = Slider:new()
    local lifeSlider = Slider:new()
    local lifeVarianceSlider = Slider:new()
    local lifeReductionSlider = Slider:new()
    local lifeReductionVarianceSlider = Slider:new()
    local xVelocitySlider = Slider:new()
    local yVelocitySlider = Slider:new()
    local zVelocitySlider = Slider:new()
    local xVelocityVarianceSlider = Slider:new()
    local yVelocityVarianceSlider = Slider:new()
    local zVelocityVarianceSlider = Slider:new()
    local xAccelerationSlider = Slider:new()
    local yAccelerationSlider = Slider:new()
    local zAccelerationSlider = Slider:new()

    local coneEmitterCheckbox = Checkbox:new({x=100,y=400,checked=true})
    local surfaceEmitterCheckbox = Checkbox:new({x=100,y=400,checked=false})

    function rateSlider:onChange(  value )
        local rate = (value > 0) and value*200 or 1
    end
    function surfaceEmitterCheckbox:onChange( value )
        coneEmitterCheckbox.checked = not value
    end

    function coneEmitterCheckbox:onChange( value )
        surfaceEmitterCheckbox.checked = not value
    end

    gui:addComponent(rateSlider)
    gui:addComponent(xOffsetSlider)
    gui:addComponent(yOffsetSlider)
    gui:addComponent(zOffsetSlider)

    gui:addComponent(lifeSlider)
    gui:addComponent(lifeVarianceSlider)

    gui:addComponent(lifeReductionSlider)
    gui:addComponent(lifeReductionVarianceSlider)
    gui:addComponent(xVelocitySlider)
    gui:addComponent(yVelocitySlider)
    gui:addComponent(zVelocitySlider)
    gui:addComponent(xVelocityVarianceSlider)
    gui:addComponent(yVelocityVarianceSlider)
    gui:addComponent(zVelocityVarianceSlider)
    gui:addComponent(xAccelerationSlider)
    gui:addComponent(yAccelerationSlider)
    gui:addComponent(zAccelerationSlider)

    gui:addComponent(coneEmitterCheckbox)
    gui:addComponent(surfaceEmitterCheckbox)
    
    gui:addPlacementHandler( CenterPlacer )

    self.gui = gui
end

function T:update( delta )

    self.camera:update( delta )
end

function T:destroy()
    self.asm:destroy()
    self.gui:destroy()
end

function T:name()
    return "ParticleEditor"
end

return T

