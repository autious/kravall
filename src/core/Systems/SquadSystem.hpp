#ifndef CORE_SYSTEMS_SQUAD_SYSTEM_HPP
#define CORE_SYSTEMS_SQUAD_SYSTEM_HPP

#include <ComponentFramework/BaseSystem.hpp>
#include <Components/SquadComponent.hpp>
#include <glm/glm.hpp>

namespace Core
{
    class SquadSystem : public Core::BaseSystem
    {
    public:
        SquadSystem();

        virtual void Update(float delta) override;
        virtual const char* GetHumanName() { return "SquadSystem"; }
        
        void SetSquadGoal(int squadID, glm::vec3 target);
        void SetSquadFormation(int squadID, Core::SquadFormation formation, const glm::vec3& startPoint, const glm::vec3& endPoint);
    };
}

#endif
