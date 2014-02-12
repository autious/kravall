#include "PoliceGoalSystem.hpp"
#include "World.hpp"
#include <gfx/GFXInterface.hpp>
#include <logger/Logger.hpp>

#include <GameUtility/PathfindingUtility.hpp>

#include <DebugMacros.hpp>
#include <Systems/AI/MovementSystem.hpp>

#include <GameUtility/GameData.hpp>

#define POLICE_GOAL_ARRIVAL_THRESHOLD 0.2f


Core::PoliceGoalSystem::PoliceGoalSystem()
	: BaseSystem( EntityHandler::GenerateAspect< WorldPositionComponent, MovementComponent, 
		UnitTypeComponent, AttributeComponent, FlowfieldComponent >(), 0ULL )
{
}





void Core::PoliceGoalSystem::Update( float delta )
{

	Core::NavigationMesh* instance = Core::GetNavigationMesh();
	if( !instance )
		return;

	if( !instance->AllocateFrameMemoryForAstar() )
		return;

	for( std::vector<Entity>::iterator it = m_entities.begin(); it != m_entities.end(); it++ )
	{
		UnitTypeComponent* utc = WGETC<UnitTypeComponent>(*it);
		if( utc->type != Core::UnitType::Police )
			continue;
		
		Core::AttributeComponent* attribc = WGETC<Core::AttributeComponent>(*it);
		int groupId = attribc->police.squadID;
		if( groupId < 0 )
			continue;
		
		Core::MovementComponent* mvmc = WGETC<Core::MovementComponent>(*it);
		if( mvmc->NavMeshGoalNodeIndex < 0 )
			continue;

		Core::WorldPositionComponent* wpc = WGETC<Core::WorldPositionComponent>(*it);
		Core::FlowfieldComponent* ffc = WGETC<Core::FlowfieldComponent>(*it);

		glm::vec3 position = glm::vec3( wpc->position[0], 0.0f, wpc->position[2] );
		glm::vec3 target = glm::vec3( mvmc->goal[0], 0.0f, mvmc->goal[2] );	

		bool move = true;

		if( !PathFinder::CheckLineVsNavMesh( position, target, 3.0f, ffc->node ) ) 
		{
			glm::vec2 deltaVector = glm::vec2(target.x, target.z ) - glm::vec2( wpc->position[0], wpc->position[2] );
			float dot = glm::dot( deltaVector, deltaVector );
			if( dot > POLICE_GOAL_ARRIVAL_THRESHOLD )
			{
				glm::vec3 direction = glm::normalize( target - position );
				MovementComponent::SetDirection( mvmc, direction.x, 0.0f, direction.z );
			}
			else
			{
				move = false;
			}
		}
		else if( ffc->node != mvmc->NavMeshGoalNodeIndex )
		{
			Core::PathData path = instance->CalculateShortPath( ffc->node, position,  mvmc->NavMeshGoalNodeIndex, target );
				
			// project position onto target line...
			int targetEdge = path.entryEdge;
			int targetNode = ffc->node;

			int ii = targetEdge * 2;
			int oo = (ii + 2) % 8;
			glm::vec3 lineStart = glm::vec3( instance->nodes[ targetNode ].points[ ii ], 0.0f, instance->nodes[ targetNode ].points[ ii + 1 ] );
			glm::vec3 lineEnd	= glm::vec3( instance->nodes[ targetNode ].points[ oo ], 0.0f, instance->nodes[ targetNode ].points[ oo + 1 ] );
			glm::vec3 lineMid = lineStart + (( lineEnd - lineStart ) * 0.5f );
			glm::vec3 fromStartToObject = position - lineStart;
			float distanceAlongLine = glm::dot( (lineEnd - lineStart) * instance->nodes[targetNode].corners[targetEdge].inverseLength, fromStartToObject );

			glm::vec3 targetPosition;
			if( instance->nodes[ targetNode ].corners[ targetEdge ].length < distanceAlongLine || distanceAlongLine < 0 )
			{
				// is outside edges...
				if( distanceAlongLine < 0 )
					targetPosition = lineStart + glm::normalize( lineEnd - lineStart ) * 1.25f;
				else 
					targetPosition = lineEnd + glm::normalize( lineStart - lineEnd ) * 1.25f;
			}
			else 
			{
				// is inside edges...
				targetPosition = path.point;
			}				

			glm::vec3 flowfieldDirection = glm::normalize( targetPosition - position );
			MovementComponent::SetDirection( mvmc, flowfieldDirection.x, 0, flowfieldDirection.z );
		}

		if( move )
		{
			// set speed according to state...
			mvmc->SetDesiredSpeed( Core::GameData::GetMovementDataWithState( mvmc->state ).speedToDesire, Core::DesiredSpeedSetPriority::PoliceGoalSytemDesiredSpeedPriority ); 
		}
		else
		{
			mvmc->SetDesiredSpeed( 0.0f, Core::DesiredSpeedSetPriority::PoliceGoalSytemDesiredSpeedPriority ); 
		}
	}
}
