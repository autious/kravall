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

    self.emitterEntity = core.entity.create(core.componentType.EmitterComponent, core.componentType.WorldPositionComponent)
    
    self.emitterEntity:set(core.componentType.WorldPositionComponent, { position = {0, 0, 0} })
    self.emitterEntity:set(core.componentType.EmitterComponent
            , {
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
            }, true)

    local GUI = require "gui/GUI"
    local Slider = require "gui/component/Slider"
    local Checkbox = require "gui/component/Checkbox"
    local Label = require "gui/component/TextLabel"

    local CenterPlacer = require "gui/placement/CenterPlacer"
    local WestPlacer = require "gui/placement/WestPlacer"

    local gui = GUI:new()
    local guiContainer = GUI:new();

    local folder = "assets/texture/ui/"
    local function genNewSliderTable() return {matReleased=folder .. "slider-knob-release_01.material", matPressed=folder .. "slider-knob-press_01.material", matHover=folder .. "slider-knob-hover_01.material", matBackground=folder .. "slider-background_01.material", x=0, y=0, xoffset=0, yoffset=0, a=0, onChange = function(self, value) end } end


    self.rateLabel = Label:new({label = "Emission Rate" })
    self.offsetLabel = Label:new({label = "Emitter Offset"})
    self.lifeLabel = Label:new({label = "Life"})
    self.lifeVarianceLabel = Label:new({label = "Life Variance"})
    self.lifeReductionLabel = Label:new({label = "Life Reduction" })
    self.lifeReductionVarianceLabel = Label:new({label = "Life Reduction Variance"})
    self.velocityLabel = Label:new({label = "Velocity"})
    self.velocityVarianceLabel = Label:new({label = "Velocity Variance" })
    self.accelerationLabel = Label:new({label = "Acceleration"})

    local rateSlider = Slider:new(genNewSliderTable())
    local xOffsetSlider = Slider:new(genNewSliderTable())
    local yOffsetSlider = Slider:new(genNewSliderTable())
    local zOffsetSlider = Slider:new(genNewSliderTable())
    local lifeSlider = Slider:new(genNewSliderTable())
    local lifeVarianceSlider = Slider:new(genNewSliderTable())
    local lifeReductionSlider = Slider:new(genNewSliderTable())
    local lifeReductionVarianceSlider = Slider:new(genNewSliderTable())
    local xVelocitySlider = Slider:new(genNewSliderTable())
    local yVelocitySlider = Slider:new(genNewSliderTable())
    local zVelocitySlider = Slider:new(genNewSliderTable())
    local xVelocityVarianceSlider = Slider:new(genNewSliderTable())
    local yVelocityVarianceSlider = Slider:new(genNewSliderTable())
    local zVelocityVarianceSlider = Slider:new(genNewSliderTable())
    local xAccelerationSlider = Slider:new(genNewSliderTable())
    local yAccelerationSlider = Slider:new(genNewSliderTable())
    local zAccelerationSlider = Slider:new(genNewSliderTable())

    local coneEmitterCheckbox = Checkbox:new({x=100,y=400,checked=true})
    local surfaceEmitterCheckbox = Checkbox:new({x=100,y=400,checked=false})

    function rateSlider.onChange(rateSlider, value )
        local newRate = (value > 0) and value*1000 or 1
        self.emitterEntity:set(core.componentType.EmitterComponent, {rate = newRate}, true)        
    end

    function xOffsetSlider.onChange(xOffsetSlider, value)
        local xOffset = (value > 0) and 
    end

    local function onClick(self, value )
        coneEmitterCheckbox:setChecked(false)
        surfaceEmitterCheckbox:setChecked(false)
        self:setChecked(true)
    end

    rateSlider:setToMiddle()
    xOffsetSlider:setToMiddle()
    yOffsetSlider:setToMiddle()
    zOffsetSlider:setToMiddle()

    coneEmitterCheckbox.onClick = onClick
    surfaceEmitterCheckbox.onClick = onClick

    gui:addComponent(self.rateLabel)
    gui:addComponent(rateSlider)
    gui:addComponent(self.offsetLabel)
    gui:addComponent(xOffsetSlider)
    gui:addComponent(yOffsetSlider)
    gui:addComponent(zOffsetSlider)

    gui:addComponent(self.lifeLabel)
    gui:addComponent(lifeSlider)
    gui:addComponent(self.lifeVarianceLabel)
    gui:addComponent(lifeVarianceSlider)

    gui:addComponent(self.lifeReductionLabel)
    gui:addComponent(lifeReductionSlider)
    gui:addComponent(self.lifeReductionVarianceLabel)
    gui:addComponent(lifeReductionVarianceSlider)
    gui:addComponent(self.velocityLabel)
    gui:addComponent(xVelocitySlider)
    gui:addComponent(yVelocitySlider)
    gui:addComponent(zVelocitySlider)
    gui:addComponent(self.velocityVarianceLabel)
    gui:addComponent(xVelocityVarianceSlider)
    gui:addComponent(yVelocityVarianceSlider)
    gui:addComponent(zVelocityVarianceSlider)
    gui:addComponent(self.accelerationLabel)
    gui:addComponent(xAccelerationSlider)
    gui:addComponent(yAccelerationSlider)
    gui:addComponent(zAccelerationSlider)

    gui:addComponent(coneEmitterCheckbox)
    gui:addComponent(surfaceEmitterCheckbox)
    
    gui:addPlacementHandler( WestPlacer )
   
    gui.xoffset = 12;
    gui.width = 1;
    gui.height = 1;
    self.guiContainer = guiContainer
    self.guiContainer:addPlacementHandler(WestPlacer)
    self.guiContainer:addComponent(gui)

end

function T:update( delta )
    local emitterComponent = self.emitterEntity:get(core.componentType.EmitterComponent)
    self.rateLabel:setLabel( "Emission Rate " .. emitterComponent.rate )
    self.camera:update( delta )
end

function T:destroy()
    self.asm:destroy()
    self.guiContainer:destroy()
    core.entity.destroy(self.emitterEntity)
end

function T:name()
    return "ParticleEditor"
end

return T

