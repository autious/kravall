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
		CIRCLE_FORMATION,
        HALF_CIRCLE_FORMATION
	};

    enum SquadAbility
    {
        SPRINT,
        FLEE,
        ROUT,
        ATTACK,
        ARREST_INDIVIDUAL,
        ARREST_GROUP,
        TEAR_GAS,
        TAZE,
        BLITZ
    };

	/*!
		Component used for controlling a group of police. The Entity asigned to the squadLeader varaible must match an aspect of Core::WorldPositionComponent.
	*/
	struct SquadComponent
	{
		Core::PoliceStance squadStance = Core::PoliceStance::Passive;
		Core::PoliceStance prevSquadStance = Core::PoliceStance::Passive;

		Core::SquadFormation squadFormation = Core::SquadFormation::NO_FORMATION;

		int squadID = -1;
		float squadGoal[3];
		float squadForward[2];
		float squadTargetForward[2];
		Core::Entity squadLeader;
		bool squadMoveInFormation = false;
		bool waitForStraggler = false;
		int squadHealth = 0;
		int squadStamina = 0;
		float squadMorale = 0.0f;
		int targetGroup = std::numeric_limits<int>::max();


		inline static const char* GetName()
		{
			return "SquadComponent";
		}

		SquadComponent();
	};
}

#endif
