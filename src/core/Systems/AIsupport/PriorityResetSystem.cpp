#include "PriorityResetSystem.hpp"

#include <Input/InputManager.hpp>
#include <GameUtility/GameData.hpp>
#include "World.hpp"
#include <SystemDef.hpp>


Core::PriorityResetSystem::PriorityResetSystem() : 
BaseSystem( EntityHandler::GenerateAspect<MovementComponent>(), 0ULL )
{
}

void Core::PriorityResetSystem::Update(float delta)
{
	// reset goal-priority values...
	for (std::vector<Entity>::iterator it = m_entities.begin(); it != m_entities.end(); it++)
	{
		Core::MovementComponent* mvmc = WGETC<Core::MovementComponent>(*it);
		mvmc->currentGoalPriority = Core::MovementGoalPriority::NoGoalPriority;
		mvmc->currentMovementStatePriority = Core::MovementStatePriority::MovementState_NoPriority;
	}

}
