#ifndef CORE_COMPONENTS_SQUAD_COMPONENT_HPP
#define CORE_COMPONENTS_SQUAD_COMPONENT_HPP

#include <ComponentFramework/SystemTypes.hpp>
#include <Components/AttributeComponent.hpp>

namespace Core
{
    /*!
    Enumeration for the various formations a squad can take.
    */
    enum SquadFormation
    {
        NO_FORMATION,
        LINE_FORMATION,
        CIRCLE_FORMATION
    };

    /*!
    Component used for controlling a group of police. The Entity asigned to the squadLeader varaible must match an aspect of Core::WorldPositionComponent.
    */
    struct SquadComponent
    {
        Core::PoliceStance squadStance = Core::PoliceStance::Passive;
        Core::SquadFormation squadFormation = Core::SquadFormation::NO_FORMATION;

        int squadID = -1;
        float squadGoal[3] = {0.0f, 0.0f, 0.0f};
        float squadForward[2] = {0.0f, -1.0f};
        float squadTargetForward[2] = {0.0f, -1.0f};
        Core::Entity squadLeader = INVALID_ENTITY;
        bool squadMoveInFormation = false;
        bool waitForStraggler = false;
        int squadHealth = 0;
        int squadStamina = 0;
        float squadMorale = 0.0f;

        inline static const char* GetName()
        {
            return "SquadComponent";
        }

        SquadComponent()
        {

        }                           
        
        SquadComponent(Core::PoliceStance stance, Core::SquadFormation formation, int id, Core::Entity leader, bool moveInFormation )
        {
            squadStance = stance;
            squadFormation = formation;
            squadID = id;
            squadLeader = leader;
            squadMoveInFormation = moveInFormation;
        }
    };
}

#endif
