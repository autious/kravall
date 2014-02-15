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
end

function T:update( delta )

    self.camera:update( delta )
end

function T:destroy()
    self.asm:destroy()
end

function T:name()
    return "ParticleEditor"
end

return T

