#include "PoliceGoalSystem.hpp"
#include "World.hpp"
#include <gfx/GFXInterface.hpp>
#include <logger/Logger.hpp>

#include <GameUtility/PathfindingUtility.hpp>

#include <DebugMacros.hpp>
#include <Systems/AI/MovementSystem.hpp>

#include <GameUtility/GameData.hpp>
#include <CoreUtility/Threadpool.hpp>
#include <functional>

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

	if( !instance->AllocateFrameMemoryForAstar( Core::world.m_threadHandler.GetNrThreads() ) )
		return;

	int head = 0;
	Core::Entity* policeList = Core::world.m_frameHeap.NewPODArray<Core::Entity>( m_entities.size() );
	if( policeList == nullptr )
	{
		LOG_FATAL << "not enough memory for police goal system!" << std::endl;
		return;
	}
	
	for( std::vector<Entity>::iterator it = m_entities.begin(); it != m_entities.end(); it++ )
	{
		UnitTypeComponent* utc = WGETC<UnitTypeComponent>(*it);
		if( utc->type != Core::UnitType::Police )
			continue;
		policeList[ head++ ] = *it;
	}

	int nrCores = Core::world.m_threadHandler.GetNrThreads();
	int nrPerCore = std::ceil((float)head / (float)nrCores);

	for( int i = 0; i < nrCores; i++ )
	{
		int startIndex = nrPerCore * i;
		int endIndex = startIndex + nrPerCore;
		if( endIndex > head )
			endIndex = head;
		if( startIndex > head )
			startIndex = head;

		int memoryIndex = i;

		Core::world.m_threadHandler.Enqueue( [ policeList, instance, startIndex, endIndex, memoryIndex ]()
		{

			//for( std::vector<Entity>::iterator it = m_entities.begin() + startIndex; it != m_entities.begin() + endIndex; it++ )
			for( int i = startIndex; i < endIndex; i++ )
			{		
				Core::Entity ent = policeList[ i ];

				Core::AttributeComponent* attribc = WGETC<Core::AttributeComponent>(ent);
				int groupId = attribc->police.squadID;
				if( groupId < 0 )
					continue;
		
				Core::MovementComponent* mvmc = WGETC<Core::MovementComponent>(ent);
				if( mvmc->NavMeshGoalNodeIndex < 0 )
					continue;

				Core::WorldPositionComponent* wpc = WGETC<Core::WorldPositionComponent>(ent);
				Core::FlowfieldComponent* ffc = WGETC<Core::FlowfieldComponent>(ent);

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
					Core::PathData path = instance->CalculateShortPath( ffc->node, position,  mvmc->NavMeshGoalNodeIndex, target, memoryIndex );
				
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

					//GFX::Debug::DrawLine( position, targetPosition, GFXColor( 1, 1, 0, 1 ), false );

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
		});
	}
	

	Core::world.m_threadHandler.Wait();
}
