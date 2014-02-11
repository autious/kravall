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

        void BindParticle(unsigned int bufferId);
        void UnbindParticle();
            
    private:
        std::vector<GFX::ParticleData> m_particles;
        unsigned long long int m_idCounter;
        
        inline const unsigned int FindParticle(unsigned long long int id)
        {
            for(unsigned int i=0;i < m_particles.size(); ++i)
            {
                if(m_particles[i].id == static_cast<unsigned int>(id))
                {
                    return i;
                }
            }
            return std::numeric_limits<std::result_of<decltype(&ParticleManager::FindParticle)(ParticleManager, unsigned long long)>::type>::max();
        }

    };
}

#endif
