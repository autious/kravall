#include "SquadComponent.hpp"

Core::SquadComponent::SquadComponent()
{
	squadID = -1;

	squadGoal[0] = 0.0f;
	squadGoal[1] = 0.0f;
	squadGoal[2] = 0.0f;

	squadForward[0] = 1.0f;
	squadForward[0] = 0.0f;

	squadTargetForward[0] = 1.0f;
	squadTargetForward[1] = 0.0f;

	squadLeader = INVALID_ENTITY;
	squadMoveInFormation = false;
	waitForStraggler = false;
	squadHealth = 0;
	squadStamina = 0;
	squadMorale = 0.0f;
}