#include "ParticleManager.hpp"

#include <Particle.hpp>

namespace GFX
{
    ParticleManager::ParticleManager()
    {
    }

    ParticleManager::~ParticleManager()
    {
        for(decltype(m_particles)::size_type i=0; i < m_particles.size(); ++i)
        {
            glDeleteBuffers(1, &m_particles[i].VBO);
            glDeleteVertexArrays(1, &m_particles[i].VAO);
        }
    }

    void ParticleManager::CreateParticleBuffer(unsigned int& bufferId, unsigned int particleCount)
    {
        GFX::ParticleData particleData;
        particleData.particleCount = particleCount;

        glGenBuffers(1, &particleData.VBO);
        glBindBuffer(GL_ARRAY_BUFFER, particleData.VBO);
        glBufferData(GL_ARRAY_BUFFER, particleCount * sizeof(GFX::Particle), 0, GL_DYNAMIC_DRAW);

        glGenVertexArrays(1, &particleData.VAO);
        glBindVertexArray(particleData.VAO);
        
        glBindBuffer(GL_ARRAY_BUFFER, particleData.VBO);

        //Position
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GFX::Particle), (void*)0);
        
        //Life
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, sizeof(GFX::Particle), (void*)(3 * sizeof(float)));
        
        glBindVertexArray(0);

        bufferId = particleData.VAO;
        m_particles.push_back(particleData);
    }

    void ParticleManager::DeleteParticleBuffer(unsigned int bufferId)
    {
        unsigned int index = FindParticle(bufferId);

        if(index != std::numeric_limits<decltype(index)>::max())
        {
            glDeleteBuffers(1, &m_particles[index].VBO);
            glDeleteVertexArrays(1, &m_particles[index].VAO);
            m_particles.erase(m_particles.begin() + index);
        }
    }

    void ParticleManager::BufferParticleData(unsigned int bufferId, GFX::Particle* data)
    {
        unsigned int index = FindParticle(bufferId);

        if(index != std::numeric_limits<decltype(index)>::max())
        {
            glBindBuffer(GL_ARRAY_BUFFER, m_particles[index].VBO);
            glBufferData(GL_ARRAY_BUFFER, m_particles[index].particleCount * sizeof(GFX::Particle), data, GL_DYNAMIC_DRAW);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
        }
    }

    const GFX::ParticleData* ParticleManager::GetParticleData(unsigned int bufferId)
    {
        unsigned int index = FindParticle(bufferId);
        if(index != std::numeric_limits<decltype(index)>::max())
        {
            return &m_particles[index];
        }
        return nullptr;
    }
}
