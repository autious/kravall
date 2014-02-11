#include "ParticleSystem.hpp"

#include <World.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <cstring>

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

    struct ParticleContainer
    {
        int numberOfParticles;
        int particlePointer;
        unsigned long long int materialId;
        GFX::Particle* particles;
        Core::ParticleData* particleData;

        Core::ParticleContainer* prev;
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
                int k = prc->particlePointer;
                std::uniform_real_distribution<float> distributor(0.0f, 1.0f);
                switch (emc->type)
                {
                    case Core::EmitterType::SURFACE_EMITTER:
                    {
                        glm::vec3 position = Core::WorldPositionComponent::GetVec3(*wpc) + glm::vec3(emc->offset[0], emc->offset[1], emc->offset[2]) 
                            + glm::vec3(emc->surface.directionOne[0], emc->surface.directionOne[1], emc->surface.directionOne[2]) * distributor(generator) 
                            + glm::vec3(emc->surface.directionTwo[0], emc->surface.directionTwo[1],emc->surface.directionTwo[2]) * distributor(generator);

                        prc->particles[k].position[0] = position.x; 
                        prc->particles[k].position[1] = position.y; 
                        prc->particles[k].position[2] = position.z; 

                        glm::vec3 velocity = glm::vec3(emc->velocity[0], emc->velocity[1], emc->velocity[2]) 
                            + glm::vec3(emc->velocityVariance[0] * distributor(generator), emc->velocityVariance[1] * distributor(generator), emc->velocityVariance[2] * distributor(generator)); 

                        prc->particleData[k].velocity[0] = velocity.x;
                        prc->particleData[k].velocity[1] = velocity.y;
                        prc->particleData[k].velocity[2] = velocity.z;
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

                        prc->particleData[k].velocity[0] = velocity.x;
                        prc->particleData[k].velocity[1] = velocity.y;
                        prc->particleData[k].velocity[2] = velocity.z;

                        glm::vec3 position = Core::WorldPositionComponent::GetVec3(*wpc) + glm::vec3(emc->offset[0], emc->offset[1], emc->offset[2]);
                        prc->particles[k].position[0] = position.x;
                        prc->particles[k].position[1] = position.y;
                        prc->particles[k].position[2] = position.z;
                        break;
                    }
                }

                prc->particleData[k].acceleration[0] = emc->acceleration[0];
                prc->particleData[k].acceleration[1] = emc->acceleration[1];
                prc->particleData[k].acceleration[2] = emc->acceleration[2];
                
                prc->particles[k].life = emc->life + distributor(generator) * emc->lifeVariance;
                prc->particleData[k].lifeReduction = emc->lifeReduction + distributor(generator) * emc->lifeReductionVariance;                          

                prc->particlePointer += 1;
                if(prc->particlePointer >= prc->numberOfParticles)
                {
                    prc->particlePointer = 0;
                }
            }
        }

        for(Core::ParticleContainer* prc = m_particleContainer; prc != nullptr; prc = prc->prev)
        {
            for(int i=0; i < prc->numberOfParticles; ++i)
            {
                prc->particles[i].position[0] += ((prc->particleData[i].velocity[0] += (prc->particleData[i].acceleration[0] * delta)) * delta);
                prc->particles[i].position[1] += ((prc->particleData[i].velocity[1] += (prc->particleData[i].acceleration[1] * delta)) * delta);
                prc->particles[i].position[2] += ((prc->particleData[i].velocity[2] += (prc->particleData[i].acceleration[2] * delta)) * delta);

                prc->particles[i].life -= (prc->particleData[i].lifeReduction * delta);

                if(prc->particles[i].life < 0.0f)
                {
                    prc->particles[i].life = 0.0f;
                }
            }
        }
    }

    void ParticleSystem::Reset()
    {
        m_numberOfContainers = 0;
        m_particleContainer = nullptr;
        m_reseterCreated = false;
    }

    ParticleHandle ParticleSystem::CreateParticle(int numberOfParticles, unsigned long long int materialId)
    {
        ParticleContainer* next = Core::world.m_levelHeap.NewPOD<Core::ParticleContainer>();
        next->numberOfParticles = numberOfParticles;
        next->particlePointer = 0;
        next->materialId = materialId;
        next->particles = Core::world.m_levelHeap.NewPODArray<GFX::Particle>(numberOfParticles);
        memset(next->particles, 0, numberOfParticles * sizeof(GFX::Particle));
        next->particleData = Core::world.m_levelHeap.NewPODArray<Core::ParticleData>(numberOfParticles);
        memset(next->particleData, 0, numberOfParticles * sizeof(Core::ParticleData));
        next->prev = m_particleContainer;

        m_particleContainer = next;

        return static_cast<ParticleHandle>(m_particleContainer);
    }
}
