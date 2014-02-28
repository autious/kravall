#ifndef CORE_PARTICLE_PARTICLE_DATA_HPP
#define CORE_PARTICLE_PARTICLE_DATA_HPP

namespace Core
{
    struct ParticleData
    {
        float velocity[3];       
        float acceleration[3];

        float lifeReduction;
    };
}

#endif
