#ifndef CORE_SYSTEMS_PARTICLE_SYSTEM_HPP
#define CORE_SYSTEMS_PARTICLE_SYSTEM_HPP

#include <ComponentFramework/BaseSystem.hpp>
#include <Components/EmitterComponent.hpp>

#include <random>

namespace Core
{
    class ParticleContainer;
}

namespace Core
{
    /*!
        Particle system that updates particles and calls GFX to render them. Uses the level heap in Core::world to store particle data. 
    */
    class ParticleSystem : public Core::BaseSystem
    {
    public:
        ParticleSystem();
        ~ParticleSystem();
        
        virtual void Update(float delta) override;
        virtual const char* GetHumanName() { return "ParticleSystem"; }

        /*!
            Resets the particles, when called no more particles will be rendered until new particles are added.
        */
        void Reset();

        /*!
            Adds a particle container that stores particle data for up dating and rendering. Multiple calls with the same parameters returns unique handles.
            \param numberOfParticles The maximum number of particles that can be rendered for the created type of particle at any given time.
            \param materialId The material that is used when rendering the particles.
            \returns A handle to the newly created particle container.
        */
        Core::ParticleHandle CreateParticle(int numberOfParticles, unsigned long long int materialId);
        
    private:       
        std::random_device m_randomDevice;
        int m_numberOfContainers;
        Core::ParticleContainer* m_particleContainer;
        bool m_reseterCreated;
    };
}

#endif
