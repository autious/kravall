#include "ParticleSystem.hpp"

#include <World.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <cstring>

#include <gfx/Particle.hpp>
#include <gfx/GFXInterface.hpp>
#include <gfx/BitmaskDefinitions.hpp>

namespace 
{
    class ParticleSystemReseter
    {
    public:
        ParticleSystemReseter(Core::ParticleSystem* particleSystem)
        {
            m_particleSystem = particleSystem;
        }

        ~ParticleSystemReseter()
        {
            m_particleSystem->Reset();
        }

    private:
        Core::ParticleSystem* m_particleSystem;
    };
}

namespace Core
{
    struct ParticleData
    {
        float velocity[3];       
        float acceleration[3];

        float lifeReduction;
    };

    class ParticleContainer
    {
    public:
        ~ParticleContainer()
        {
            GFX::Content::DeleteParticleBuffer(m_particleBufferId);        
        }

        int m_numberOfParticles;
        int m_particlePointer;
        unsigned int m_particleBufferId;
        unsigned long long int m_materialId;

        GFX::Particle* m_particles;
        Core::ParticleData* m_particleData;
        Core::ParticleContainer* m_prev;
    };
}

namespace Core
{
    ParticleSystem::ParticleSystem() : BaseSystem(EntityHandler::GenerateAspect<Core::WorldPositionComponent, Core::EmitterComponent>(), 0ULL)
    {
        m_numberOfContainers = 0;
        m_particleContainer = nullptr;
        m_reseterCreated = false;
    }

    ParticleSystem::~ParticleSystem()
    {

    }

    void ParticleSystem::Update(float delta)
    {
        if(!m_reseterCreated)
        {
            Core::world.m_levelHeap.NewObject<ParticleSystemReseter>(this);
            m_reseterCreated = true;
        }

        std::mt19937 generator(m_randomDevice());
        for(std::vector<Core::Entity>::iterator it = m_entities.begin(); it != m_entities.end(); ++it)
        {
            Core::WorldPositionComponent* wpc = WGETC<Core::WorldPositionComponent>(*it);
            Core::EmitterComponent* emc = WGETC<Core::EmitterComponent>(*it);
            Core::ParticleContainer* prc = static_cast<Core::ParticleContainer*>(emc->handle);

            for(int i=0; i < emc->rate; ++i)
            {
                int k = prc->m_particlePointer;
                std::uniform_real_distribution<float> distributor(0.0f, 1.0f);
                switch (emc->type)
                {
                    case Core::EmitterType::SURFACE_EMITTER:
                    {
                        glm::vec3 position = Core::WorldPositionComponent::GetVec3(*wpc) + glm::vec3(emc->offset[0], emc->offset[1], emc->offset[2]) 
                            + glm::vec3(emc->surface.directionOne[0], emc->surface.directionOne[1], emc->surface.directionOne[2]) * distributor(generator) 
                            + glm::vec3(emc->surface.directionTwo[0], emc->surface.directionTwo[1],emc->surface.directionTwo[2]) * distributor(generator);

                        prc->m_particles[k].position[0] = position.x; 
                        prc->m_particles[k].position[1] = position.y; 
                        prc->m_particles[k].position[2] = position.z; 

                        glm::vec3 velocity = glm::vec3(emc->velocity[0], emc->velocity[1], emc->velocity[2]) 
                            + glm::vec3(emc->velocityVariance[0] * distributor(generator), emc->velocityVariance[1] * distributor(generator), emc->velocityVariance[2] * distributor(generator)); 

                        prc->m_particleData[k].velocity[0] = velocity.x;
                        prc->m_particleData[k].velocity[1] = velocity.y;
                        prc->m_particleData[k].velocity[2] = velocity.z;
                        break;
                    }

                    case Core::EmitterType::CONE_EMITTER:
                    {
                        float rotation = 3.14f * distributor(generator);
                        float cosVal = glm::cos(rotation);
                        float sinVal = glm::sin(rotation);
                        glm::quat q_1 = glm::quat(cosVal, 0.0f, 0.0f, -sinVal); 

                        rotation = glm::radians(emc->cone.angle + emc->cone.angleVariance * distributor(generator)) / 2.0f;
                        cosVal = glm::cos(rotation);
                        sinVal = glm::sin(rotation);
                        glm::quat q_2 = glm::quat(cosVal, sinVal, 0.0f, 0.0f);

                        glm::vec3 target = glm::normalize(glm::vec3(emc->cone.direction[0], emc->cone.direction[1], emc->cone.direction[2]));
                        glm::vec3 axis = glm::cross(target, glm::vec3(0.0f, 0.0f, -1.0f));
                        rotation = glm::acos(glm::dot(target, axis)) / 2.0f;
                        cosVal = glm::cos(rotation);
                        sinVal = glm::sin(rotation);
                        glm::quat q_3 = glm::quat(cosVal, axis.x * sinVal, axis.y * sinVal, axis.z * sinVal);

                        glm::quat quatRotation = glm::cross(q_3, glm::cross(q_2, q_1));
                       
                        glm::vec3 velocity = glm::vec3(emc->velocity[0], emc->velocity[1], emc->velocity[2]) 
                            + glm::vec3(emc->velocityVariance[0] * distributor(generator), emc->velocityVariance[1] * distributor(generator), emc->velocityVariance[2] * distributor(generator)); 
                        velocity = quatRotation * velocity * glm::conjugate(quatRotation);

                        prc->m_particleData[k].velocity[0] = velocity.x;
                        prc->m_particleData[k].velocity[1] = velocity.y;
                        prc->m_particleData[k].velocity[2] = velocity.z;

                        glm::vec3 position = Core::WorldPositionComponent::GetVec3(*wpc) + glm::vec3(emc->offset[0], emc->offset[1], emc->offset[2]);
                        prc->m_particles[k].position[0] = position.x;
                        prc->m_particles[k].position[1] = position.y;
                        prc->m_particles[k].position[2] = position.z;
                        break;
                    }
                }

                prc->m_particleData[k].acceleration[0] = emc->acceleration[0];
                prc->m_particleData[k].acceleration[1] = emc->acceleration[1];
                prc->m_particleData[k].acceleration[2] = emc->acceleration[2];
                
                prc->m_particles[k].life = emc->life + distributor(generator) * emc->lifeVariance;
                prc->m_particleData[k].lifeReduction = emc->lifeReduction + distributor(generator) * emc->lifeReductionVariance;                          

                prc->m_particlePointer += 1;
                if(prc->m_particlePointer >= prc->m_numberOfParticles)
                {
                    prc->m_particlePointer = 0;
                }
            }
        }

        for(Core::ParticleContainer* prc = m_particleContainer; prc != nullptr; prc = prc->m_prev)
        {
            for(int i=0; i < prc->m_numberOfParticles; ++i)
            {
                prc->m_particles[i].position[0] += ((prc->m_particleData[i].velocity[0] += (prc->m_particleData[i].acceleration[0] * delta)) * delta);
                prc->m_particles[i].position[1] += ((prc->m_particleData[i].velocity[1] += (prc->m_particleData[i].acceleration[1] * delta)) * delta);
                prc->m_particles[i].position[2] += ((prc->m_particleData[i].velocity[2] += (prc->m_particleData[i].acceleration[2] * delta)) * delta);

                prc->m_particles[i].life -= (prc->m_particleData[i].lifeReduction * delta);

                if(prc->m_particles[i].life < 0.0f)
                {
                    prc->m_particles[i].life = 0.0f;
                }
            }
        }
        
        for(Core::ParticleContainer* prc = m_particleContainer; prc != nullptr; prc = prc->m_prev)
        {
            /*
            GFX::Content::BufferParticleData(prc->m_particleBufferId, prc->m_particles);
            GFX::GFXBitmask bitmask;
            GFX::SetBitmaskValue(bitmask, GFX::BITMASK::TYPE, GFX::OBJECT_TYPES::PARTICLE_GEOMETRY);
            GFX::SetBitmaskValue(bitmask, GFX::BITMASK::MESH_ID, prc->m_particleBufferId);
            GFX::SetBitmaskValue(bitmask, GFX::BITMASK::MATERIAL_ID, prc->m_materialId);
            GFX::SetBitmaskValue(bitmask, GFX::BITMASK::LAYER, GFX::LAYER_TYPES::MESH_LAYER);

            GFX::Draw(bitmask, nullptr);
            */
        }
    }

    void ParticleSystem::Reset()
    {
        m_numberOfContainers = 0;
        m_particleContainer = nullptr;
        m_reseterCreated = false;
    }

    ParticleHandle ParticleSystem::CreateParticle(int numberOfParticles, unsigned long long int m_materialId)
    {
        ParticleContainer* next = Core::world.m_levelHeap.NewObject<Core::ParticleContainer>();
        next->m_numberOfParticles = numberOfParticles;
        next->m_particlePointer = 0;
        next->m_materialId = m_materialId;
        next->m_particles = Core::world.m_levelHeap.NewPODArray<GFX::Particle>(numberOfParticles);
        GFX::Content::CreateParticleBuffer(next->m_particleBufferId, next->m_numberOfParticles);
        memset(next->m_particles, 0, numberOfParticles * sizeof(GFX::Particle));
        next->m_particleData = Core::world.m_levelHeap.NewPODArray<Core::ParticleData>(numberOfParticles);
        memset(next->m_particleData, 0, numberOfParticles * sizeof(Core::ParticleData));
        next->m_prev = m_particleContainer;

        m_particleContainer = next;

        return static_cast<ParticleHandle>(m_particleContainer);
    }
}
