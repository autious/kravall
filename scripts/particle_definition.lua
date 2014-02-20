local T = {}

function T:createParticleDefinition(ASM, numberOfParticles, materialFileName)
    local particleDefinition
    ASM:specific_content(core.contentmanager.load(core.loaders.MaterialLoader, materialFileName, 
        function(value)
            particleDefinition = core.system.particle.createParticleDefinition(numberOfParticles, value)
        end, false))
    return particleDefinition
end

return T
