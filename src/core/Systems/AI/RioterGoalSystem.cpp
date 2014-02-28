#include "RioterGoalSystem.hpp"
#include "World.hpp"
#include <gfx/GFXInterface.hpp>
#include <logger/Logger.hpp>

#include <GameUtility/GameData.hpp>

#define RIOTER_GOAL_ARRIVAL_THRESHOLD 0.2f


Core::RioterGoalSystem::RioterGoalSystem()
	: BaseSystem( EntityHandler::GenerateAspect< WorldPositionComponent, MovementComponent, 
		UnitTypeComponent, AttributeComponent, FlowfieldComponent >(), 0ULL )
{
}





void Core::RioterGoalSystem::Update( float delta )
{
	Core::NavigationMesh* instance = Core::GetNavigationMesh();

	for( std::vector<Entity>::iterator it = m_entities.begin(); it != m_entities.end(); it++ )
	{
		UnitTypeComponent* utc = WGETC<UnitTypeComponent>(*it);
		if( utc->type != Core::UnitType::Rioter )
			continue;

		Core::MovementComponent* mvmc = WGETC<Core::MovementComponent>( *it );
		mvmc->state = Core::MovementState::Movement_Walking;

		glm::vec3 goal = glm::vec3( mvmc->goal[0], mvmc->goal[1], mvmc->goal[2] );

		// if rioters are following nav-mesh, check if they can go directly to the target...
		if( goal.x != std::numeric_limits<float>::max() )
		{

			Core::WorldPositionComponent* wpc = WGETC<Core::WorldPositionComponent>( *it );
			glm::vec3 position = glm::vec3( wpc->position[0], wpc->position[1], wpc->position[2] );
			glm::vec3 delta = goal - position;

			if( glm::dot( delta, delta ) > RIOTER_GOAL_ARRIVAL_THRESHOLD )
			{
				Core::AttributeComponent* owmAttribc = WGETC<Core::AttributeComponent>( *it );
				if( owmAttribc->stamina > 30.0f )
					mvmc->state = Core::MovementState::Movement_Sprinting;

				delta = glm::normalize( delta );
				MovementComponent::SetDirection( mvmc, delta.x, 0.0f, delta.z );
			}
		}

		mvmc->SetDesiredSpeed( Core::GameData::GetMovementDataWithState( mvmc->state ).speedToDesire, Core::DesiredSpeedSetPriority::RioterGoalSystemDesiredSpeedPriority );
	}
}
