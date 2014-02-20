#ifndef CORE_PARTICLE_PARTICLE_CONTAINER_HPP
#define CORE_PARTICLE_PARTICLE_CONTAINER_HPP

namespace Core
{
    class ParticleData;
}

namespace GFX
{
    class Particle;
}

namespace Core
{
    class ParticleContainer
    {
    public:
        ~ParticleContainer();;

        int m_numberOfParticles;
        int m_particlePointer;
        unsigned int m_particleBufferId;
        unsigned long long int m_materialId;

        GFX::Particle* m_particles;
        Core::ParticleData* m_particleData;
        Core::ParticleContainer* m_prev;
    };
}


#endif
