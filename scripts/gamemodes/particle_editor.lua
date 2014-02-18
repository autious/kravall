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

    local WestPlacer = require "gui/placement/WestPlacer"

    local westGui = GUI:new()
    local westGuiContainer = GUI:new()

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
    self.coneEmitterLabel = Label:new({label = "Cone Emitter"})
    self.surfaceEmitterLabel = Label:new({label = "Surface Emitter"})

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

    local coneEmitterCheckbox = Checkbox:new({checked=false})
    local surfaceEmitterCheckbox = Checkbox:new({checked=false})

    function rateSlider.onChange(rateSlider, value )
        local newRate = (value > 0) and value*1000 or 1
        self.emitterEntity:set(core.componentType.EmitterComponent, {rate = newRate}, true)        
        self.rateLabel:setLabel( "Emission Rate " .. string.format("%i /sec", newRate))
    end

    function xOffsetSlider.onChange(xOffsetSlider, value)
        local xOffset = ((value > 0) and value*100 or 0) - 50
        local emitterComponent = self.emitterEntity:get(core.componentType.EmitterComponent)      
        local currentOffset = emitterComponent.offset
        self.emitterEntity:set(core.componentType.EmitterComponent, {offset={xOffset, currentOffset[2], currentOffset[3]}}, true)
        self.offsetLabel:setLabel( "Offset " .. string.format("%+6.2f %+6.2f %+6.2f", xOffset, currentOffset[2], currentOffset[3]))
    end

    function yOffsetSlider.onChange(yOffsetSlider, value)
        local yOffset = ((value > 0) and value*100 or 0) - 50
        local emitterComponent = self.emitterEntity:get(core.componentType.EmitterComponent)      
        local currentOffset = emitterComponent.offset
        self.emitterEntity:set(core.componentType.EmitterComponent, {offset={currentOffset[1], yOffset, currentOffset[3]}}, true)
        self.offsetLabel:setLabel( "Offset " .. string.format("%+6.2f %+6.2f %+6.2f", currentOffset[1], yOffset, currentOffset[3]))
    end

    function zOffsetSlider.onChange(zOffsetSlider, value)
        local zOffset = ((value > 0) and value*100 or 0) - 50
        local emitterComponent = self.emitterEntity:get(core.componentType.EmitterComponent)      
        local currentOffset = emitterComponent.offset
        self.emitterEntity:set(core.componentType.EmitterComponent, {offset={currentOffset[1], currentOffset[2], zOffset}}, true)
        self.offsetLabel:setLabel( "Offset " .. string.format("%+6.2f %+6.2f %+6.2f", currentOffset[1], currentOffset[2], zOffset))
    end

    function lifeSlider.onChange(lifeSlider, value)
        local newLife = (value > 0) and value*5 or 0
        self.emitterEntity:set(core.componentType.EmitterComponent, {life=newLife}, true)
        self.lifeLabel:setLabel( "Life " .. string.format("%.2f sec", newLife))
        lifeVarianceSlider:onChange(lifeVarianceSlider.a)
    end

    function lifeVarianceSlider.onChange(lifeVarianceSlider, value)
        local emitterComponent = self.emitterEntity:get(core.componentType.EmitterComponent)      
        local newLifeVariance = (value > 0) and value * emitterComponent.life or 0
        self.emitterEntity:set(core.componentType.EmitterComponent, {lifeVariance=newLifeVariance}, true)
        self.lifeVarianceLabel:setLabel("Life Variance " .. string.format("%.2f sec", newLifeVariance));
    end

    function lifeReductionSlider.onChange(lifeReductionSlider, value)
        local newLifeReduction = (value > 0) and value + 1 or 1
        self.emitterEntity:set(core.componentType.EmitterComponent, {lifeReduction=newLifeReduction}, true)
        self.lifeReductionLabel:setLabel( "Life Reduction " .. string.format("%.2f /sec", newLifeReduction))
        lifeReductionVarianceSlider:onChange(lifeVarianceSlider.a)
    end

    function lifeReductionVarianceSlider.onChange(lifeReductionVarianceSlider, value)
        local emitterComponent = self.emitterEntity:get(core.componentType.EmitterComponent)      
        local newLifeReductionVariance = (value > 0) and value * emitterComponent.lifeReduction or 0
        self.emitterEntity:set(core.componentType.EmitterComponent, {lifeReductionVariance=newLifeReductionVariance}, true)
        self.lifeReductionVarianceLabel:setLabel("Life Reduction Variance " .. string.format("%.2f sec", newLifeReductionVariance));
    end

    function xVelocitySlider.onChange(xVelocitySlider, value)
        local xVelocity = ((value > 0) and value*50 or 0) 
        local emitterComponent = self.emitterEntity:get(core.componentType.EmitterComponent)      
        local currentVelocity = emitterComponent.velocity
        self.emitterEntity:set(core.componentType.EmitterComponent, {velocity={xVelocity, currentVelocity[2], currentVelocity[3]}}, true)
        self.velocityLabel:setLabel( "Velocity " .. string.format("%+6.2f %+6.2f %+6.2f", xVelocity, currentVelocity[2], currentVelocity[3]))
    end

    function yVelocitySlider.onChange(yVelocitySlider, value)
        local yVelocity = ((value > 0) and value*50 or 0)
        local emitterComponent = self.emitterEntity:get(core.componentType.EmitterComponent)      
        local currentVelocity = emitterComponent.velocity
        self.emitterEntity:set(core.componentType.EmitterComponent, {velocity={currentVelocity[1], yVelocity, currentVelocity[3]}}, true)
        self.velocityLabel:setLabel( "Velocity " .. string.format("%+6.2f %+6.2f %+6.2f", currentVelocity[1], yVelocity, currentVelocity[3]))
    end

    function zVelocitySlider.onChange(zVelocitySlider, value)
        local zVelocity = ((value > 0) and value*50 or 0)
        local emitterComponent = self.emitterEntity:get(core.componentType.EmitterComponent)      
        local currentVelocity = emitterComponent.velocity
        self.emitterEntity:set(core.componentType.EmitterComponent, {velocity={currentVelocity[1], currentVelocity[2], zVelocity}}, true)
        self.velocityLabel:setLabel( "Velocity " .. string.format("%+6.2f %+6.2f %+6.2f", currentVelocity[1], currentVelocity[2], zVelocity))
    end

    function xVelocityVarianceSlider.onChange(xVelocityVarianceSlider, value)
        local xVelocityVariance = ((value > 0) and value*50 or 0)
        local emitterComponent = self.emitterEntity:get(core.componentType.EmitterComponent)      
        local currentVelocityVariance = emitterComponent.velocityVariance
        self.emitterEntity:set(core.componentType.EmitterComponent, {velocityVariance={xVelocityVariance, currentVelocityVariance[2], currentVelocityVariance[3]}}, true)
        self.velocityVarianceLabel:setLabel( "Velocity Variance " .. string.format("%+6.2f %+6.2f %+6.2f", xVelocityVariance, currentVelocityVariance[2], currentVelocityVariance[3] ))

    end

    function yVelocityVarianceSlider.onChange(yVelocityVarianceSlider, value)
        local yVelocityVariance = ((value > 0) and value*50 or 0)
        local emitterComponent = self.emitterEntity:get(core.componentType.EmitterComponent)      
        local currentVelocityVariance = emitterComponent.velocityVariance
        self.emitterEntity:set(core.componentType.EmitterComponent, {velocityVariance={currentVelocityVariance[1], yVelocityVariance, currentVelocityVariance[3]}}, true)
        self.velocityVarianceLabel:setLabel( "Velocity Variance " .. string.format("%+6.2f %+6.2f %+6.2f", currentVelocityVariance[1], yVelocityVariance, currentVelocityVariance[3]))
    end

    function zVelocityVarianceSlider.onChange(zVelocityVarianceSlider, value)
        local zVelocityVariance = ((value > 0) and value*50 or 0)
        local emitterComponent = self.emitterEntity:get(core.componentType.EmitterComponent)      
        local currentVelocityVariance = emitterComponent.velocityVariance
        self.emitterEntity:set(core.componentType.EmitterComponent, {velocityVariance={currentVelocityVariance[1], currentVelocityVariance[2], zVelocityVariance}}, true)
        self.velocityVarianceLabel:setLabel( "Velocity Variance " .. string.format("%+6.2f %+6.2f %+6.2f", currentVelocityVariance[1], currentVelocityVariance[2], zVelocityVariance))
    end
    local function onClick(self, value )
        coneEmitterCheckbox:setChecked(false)
        surfaceEmitterCheckbox:setChecked(false)
        self:setChecked(true)
    end

    function xAccelerationSlider.onChange(xAccelerationSlider, value)
        local xAcceleration = ((value > 0) and value*100 or 0) - 50
        local emitterComponent = self.emitterEntity:get(core.componentType.EmitterComponent)      
        local currentAcceleration = emitterComponent.acceleration
        self.emitterEntity:set(core.componentType.EmitterComponent, {acceleration={xAcceleration, currentAcceleration[2], currentAcceleration[3]}}, true)
        self.accelerationLabel:setLabel( "Acceleration " .. string.format("%+6.2f %+6.2f %+6.2f", xAcceleration, currentAcceleration[2], currentAcceleration[3]))
    end

    function yAccelerationSlider.onChange(yAccelerationSlider, value)
        local yAcceleration = ((value > 0) and value*100 or 0) - 50
        local emitterComponent = self.emitterEntity:get(core.componentType.EmitterComponent)      
        local currentAcceleration = emitterComponent.acceleration
        self.emitterEntity:set(core.componentType.EmitterComponent, {acceleration={currentAcceleration[1], yAcceleration, currentAcceleration[3]}}, true)
        self.accelerationLabel:setLabel( "Acceleration " .. string.format("%+6.2f %+6.2f %+6.2f", currentAcceleration[1], yAcceleration, currentAcceleration[3]))
    end

    function zAccelerationSlider.onChange(zAccelerationSlider, value)
        local zAcceleration = ((value > 0) and value*100 or 0) - 50
        local emitterComponent = self.emitterEntity:get(core.componentType.EmitterComponent)      
        local currentAcceleration = emitterComponent.acceleration
        self.emitterEntity:set(core.componentType.EmitterComponent, {acceleration={currentAcceleration[1], currentAcceleration[2], zAcceleration}}, true)
        self.accelerationLabel:setLabel( "Acceleration " .. string.format("%+6.2f %+6.2f %+6.2f", currentAcceleration[1], currentAcceleration[2], zAcceleration))
    end
    rateSlider:setToMiddle()
    xOffsetSlider:setToMiddle()
    yOffsetSlider:setToMiddle()
    zOffsetSlider:setToMiddle()
    lifeSlider:setToMiddle()
    lifeVarianceSlider:setToMinimum()
    lifeReductionSlider:setToMinimum()
    lifeReductionVarianceSlider:setToMinimum()
    xVelocitySlider:setToMinimum()
    yVelocitySlider:setToMinimum()
    zVelocitySlider:setToMinimum()
    xVelocityVarianceSlider:setToMinimum()
    yVelocityVarianceSlider:setToMinimum()
    zVelocityVarianceSlider:setToMinimum()
    xAccelerationSlider:setToMiddle()
    yAccelerationSlider:setToMiddle()
    zAccelerationSlider:setToMiddle()

    coneEmitterCheckbox.onClick = onClick
    surfaceEmitterCheckbox.onClick = onClick

    westGui:addComponent(self.rateLabel)
    westGui:addComponent(rateSlider)
    westGui:addComponent(self.offsetLabel)
    westGui:addComponent(xOffsetSlider)
    westGui:addComponent(yOffsetSlider)
    westGui:addComponent(zOffsetSlider)

    westGui:addComponent(self.lifeLabel)
    westGui:addComponent(lifeSlider)
    westGui:addComponent(self.lifeVarianceLabel)
    westGui:addComponent(lifeVarianceSlider)

    westGui:addComponent(self.lifeReductionLabel)
    westGui:addComponent(lifeReductionSlider)
    westGui:addComponent(self.lifeReductionVarianceLabel)
    westGui:addComponent(lifeReductionVarianceSlider)
    westGui:addComponent(self.velocityLabel)
    westGui:addComponent(xVelocitySlider)
    westGui:addComponent(yVelocitySlider)
    westGui:addComponent(zVelocitySlider)
    westGui:addComponent(self.velocityVarianceLabel)
    westGui:addComponent(xVelocityVarianceSlider)
    westGui:addComponent(yVelocityVarianceSlider)
    westGui:addComponent(zVelocityVarianceSlider)
    westGui:addComponent(self.accelerationLabel)
    westGui:addComponent(xAccelerationSlider)
    westGui:addComponent(yAccelerationSlider)
    westGui:addComponent(zAccelerationSlider)

    westGui:addComponent(self.coneEmitterLabel)
    westGui:addComponent(coneEmitterCheckbox)
    westGui:addComponent(self.surfaceEmitterLabel)
    westGui:addComponent(surfaceEmitterCheckbox)
    westGui:addPlacementHandler(WestPlacer)
   
    westGui.xoffset = 12
    westGui.width = 1
    westGui.height = 1
    westGui.height = 1

    self.westGuiContainer = westGuiContainer
    self.westGuiContainer:addPlacementHandler(WestPlacer)
    self.westGuiContainer:addComponent(westGui)
    
    self:CreateConeEmitterControlls()
end

function T:update( delta )
    local emitterComponent = self.emitterEntity:get(core.componentType.EmitterComponent)
    self.camera:update( delta )
end

function T:destroy()
    self.asm:destroy()
    self.westGuiContainer:destroy()
    core.entity.destroy(self.emitterEntity)
end

function T:CreateConeEmitterControlls()
    local GUI = require "gui/GUI"
    local Slider = require "gui/component/Slider"
    local Label = require "gui/component/TextLabel"

    local EastPlacer = require "gui/placement/EastPlacer"

    if self.eastGuiContainer ~= nil then
        self.eastGuiContainer:destroy()
    end

    self.eastGuiContainer = GUI:new()
    local eastGui = GUI:new()

    self.coneDirectionLabel = Label:new({label = "Direction"})
    self.coneAngleLabel = Label:new({label = "Angle"})
    self.coneAngleVarianceLabel = Label:new({label = "Angle Variance"})

    local folder = "assets/texture/ui/"
    local function genNewSliderTable() return {matReleased=folder .. "slider-knob-release_01.material", matPressed=folder .. "slider-knob-press_01.material", matHover=folder .. "slider-knob-hover_01.material", matBackground=folder .. "slider-background_01.material", x=0, y=0, xoffset=0, yoffset=0, a=0, onChange = function(self, value) end } end

    local pitchConeDirectionSlider = Slider:new(genNewSliderTable())
    local yawConeDirectionSlider = Slider:new(genNewSliderTable())
    local coneAngleSlider = Slider:new(genNewSliderTable())
    local coneAngleVarianceSlider = Slider:new(genNewSliderTable())

    function pitchConeDirectionSlider.onChange(pitchConeDirectionSlider, value)
        local pitchRotation =  value*2*3.14 
        local yawRotation = yawConeDirectionSlider.a*2*3.14
        local emitterComponent = self.emitterEntity:get(core.componentType.EmitterComponent)      
        
        local pitchMat = core.glm.mat4.new()
        local yawMat = core.glm.mat4.new()
        local yAxis = core.glm.vec3.new(0, 1, 0)
        local xAxis = core.glm.vec3.new(1, 0, 0)
        local forward = core.glm.vec4.new(0, 0, -1, 0)

        pitchMat = core.glm.mat4.rotate(pitchMat, pitchRotation, yAxis)
        yawMat = core.glm.mat4.rotate(yawMat, yawRotation, xAxis)

        local finalRotation = core.glm.mat4.multiply(yawMat, pitchMat)

        local direction = finalRotation:multiply(forward) 
        local direction = {direction:get()}
        self.emitterEntity:set(core.componentType.EmitterComponent, {coneDirection={direction[1], direction[2], direction[3] }}, true)
        self.coneDirectionLabel:setLabel( "Direction " .. string.format("%+6.2f %+6.2f %+6.2f", direction[1], direction[2], direction[3]))
    end

    function yawConeDirectionSlider.onChange(yawConeDirectionSlider, value)
        local yawRotation =  value*2*3.14 
        local pitchRotation = pitchConeDirectionSlider.a*2*3.14
        local emitterComponent = self.emitterEntity:get(core.componentType.EmitterComponent)      
        
        local pitchMat = core.glm.mat4.new()
        local yawMat = core.glm.mat4.new()
        local yAxis = core.glm.vec3.new(0, 1, 0)
        local xAxis = core.glm.vec3.new(1, 0, 0)
        local forward = core.glm.vec4.new(0, 0, -1, 0)

        pitchMat = core.glm.mat4.rotate(pitchMat, pitchRotation, yAxis)
        yawMat = core.glm.mat4.rotate(yawMat, yawRotation, xAxis)

        local finalRotation = core.glm.mat4.multiply(yawMat, pitchMat)

        local direction = finalRotation:multiply(forward) 
        local direction = {direction:get()}
        self.emitterEntity:set(core.componentType.EmitterComponent, {coneDirection={direction[1], direction[2], direction[3] }}, true)
        self.coneDirectionLabel:setLabel( "Direction " .. string.format("%+6.2f %+6.2f %+6.2f", direction[1], direction[2], direction[3]))
    end

    function coneAngleSlider.onChange(coneAngleSlider, value )
        local newConeAngle = value*180 
        self.emitterEntity:set(core.componentType.EmitterComponent, {coneAngle = newConeAngle}, true)        
        self.coneAngleLabel:setLabel( "Cone Angle " .. string.format("%i degrees", newConeAngle))
    end

    function coneAngleVarianceSlider.onChange(coneAngleVarianceSlider, value )
        local newConeAngleVariance = value*180
        self.emitterEntity:set(core.componentType.EmitterComponent, {coneAngleVariance = newConeAngleVariance}, true)        
        self.coneAngleVarianceLabel:setLabel( "Cone Angle Variance " .. string.format("%i degrees", newConeAngleVariance))
    end

    pitchConeDirectionSlider:setToMinimum()
    yawConeDirectionSlider:setToMinimum()
    coneAngleSlider:setToMinimum()
    coneAngleVarianceSlider:setToMinimum()

    eastGui:addComponent(self.coneDirectionLabel)
    eastGui:addComponent(pitchConeDirectionSlider)
    eastGui:addComponent(yawConeDirectionSlider)
    eastGui:addComponent(self.coneAngleLabel)
    eastGui:addComponent(coneAngleSlider)
    eastGui:addComponent(self.coneAngleVarianceLabel)
    eastGui:addComponent(coneAngleVarianceSlider)
    eastGui:addPlacementHandler(EastPlacer)

    eastGui.xoffset = -12
    eastGui.height = 1
    eastGui.width = 1
    self.eastGuiContainer:addPlacementHandler(EastPlacer)
    self.eastGuiContainer:addComponent(eastGui)
end

function T:name()
    return "ParticleEditor"
end

return T

