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
        /*!
            Sets the formation of the given squad.
        */
        void SetSquadFormation(int squadID, Core::SquadFormation formation, const glm::vec3& startPoint, const glm::vec3& endPoint);
        /*!
            Prints decals of where a squad would be placed if given a formation with the current paramters
        */
        void PreviewSquadFormation(int squadID, Core::SquadFormation formation, const glm::vec3& startPoint, const glm::vec3& endPoint);

    };
}

#endif
