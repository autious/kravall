#ifndef GFX_PARTICLES_PARTICLE_MANAGER_HPP
#define GFX_PARTICLES_PARTICLE_MANAGER_HPP

#include <vector>
#include <limits>

#include <Particle/ParticleData.hpp>


namespace GFX
{
    class Particle;
}

namespace GFX
{
    class ParticleManager
    {
    public:
        ParticleManager();
        ~ParticleManager();

        void CreateParticleBuffer(unsigned int& bufferId, unsigned int particleCount);
        void DeleteParticleBuffer(unsigned int bufferId);

        void BufferParticleData(unsigned int bufferId, GFX::Particle* data);

        const GFX::ParticleData* GetParticleData(unsigned int bufferId);            

    private:
        std::vector<GFX::ParticleData> m_particles;
        
        inline const unsigned int FindParticle(unsigned int id)
        {
            for(unsigned int i=0;i < m_particles.size(); ++i)
            {
                if(m_particles[i].VAO == static_cast<unsigned int>(id))
                {
                    return i;
                }
            }
            return std::numeric_limits<std::result_of<decltype(&ParticleManager::FindParticle)(ParticleManager, unsigned long long)>::type>::max();
        }

    };
}

#endif
