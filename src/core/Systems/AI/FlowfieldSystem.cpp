#include "FlowfieldSystem.hpp"
#include "World.hpp"
#include <logger/Logger.hpp>

#include <gfx/GFXInterface.hpp>
#include <GameUtility/PathfindingUtility.hpp>



Core::FlowfieldSystem::FlowfieldSystem()
	: BaseSystem( EntityHandler::GenerateAspect<
		WorldPositionComponent, UnitTypeComponent, AttributeComponent, FlowfieldComponent >(), 0ULL )
{
}

void Core::FlowfieldSystem::Update( float delta )
{
	Core::NavigationMesh* instance = Core::GetNavigationMesh();
	if( instance )
	{
		// for all entities
		for( std::vector<Entity>::iterator it = m_entities.begin(); it != m_entities.end(); it++ )
		{
			UnitTypeComponent* utc = WGETC<UnitTypeComponent>(*it);
			if( utc->type == Core::UnitType::Police || utc->type == Core::UnitType::Object )
				continue;

			// entity is not assigned to a node...
			Core::FlowfieldComponent* ffc = WGETC<Core::FlowfieldComponent>(*it);
			if( ffc->node < 0 )
				continue;

			// entity has separate goal...
			Core::MovementComponent* mvmc = WGETC<Core::MovementComponent>(*it);
			if( mvmc->goal[0] != std::numeric_limits<float>::max() )
				continue;


			Core::AttributeComponent* attribc = WGETC<Core::AttributeComponent>(*it);
			int groupID = attribc->rioter.groupID;

			if( instance->flowfields[ groupID ].goal[0] == std::numeric_limits<float>::max() )
			{
				MovementComponent::SetDirection( mvmc, 0.0f, 0.0f, 0.0f );
				continue;
			}

			Core::WorldPositionComponent* wpc = WGETC<Core::WorldPositionComponent>(*it);
			glm::vec3 position = wpc->GetVec3( *wpc );

			float* navMeshGoal = instance->flowfields[ attribc->rioter.groupID ].goal;
			glm::vec3 navGoal = glm::vec3( navMeshGoal[0], 0.0f, navMeshGoal[1] );

			if( instance->flowfields[ groupID ].deadNodes[ ffc->node ] )
			{
				// calculate path out of police occupied node here
				CalculatePathInDeadNode( *it, groupID );
			}
			else if( !Core::PathFinder::CheckLineVsNavMesh( position, navGoal, 3.0f, ffc->node ) )
			{
				if( glm::dot( navGoal - position, navGoal - position ) == 0.0f )
					MovementComponent::SetDirection( mvmc, 0.0f, 0.0f, 0.0f );
				else
				{
					glm::vec3 newDir = glm::normalize( navGoal - position );
					glm::vec3 currentDir = glm::vec3( mvmc->newDirection[0], mvmc->newDirection[1], mvmc->newDirection[2] ); 
					Core::MovementComponent::SetDirection( mvmc, newDir.x, 0.0f, newDir.z );
				}
			}
			else
			{
				// project position onto target line...
				int targetEdge = instance->flowfields[groupID].edges[ffc->node];
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
					targetPosition = instance->flowfields[groupID].list[ ffc->node ];
				}
				
				glm::vec3 flowfieldDirection = glm::normalize( targetPosition - position );

				MovementComponent::SetDirection( mvmc, flowfieldDirection.x, 0, flowfieldDirection.z );
			}
		}
	}
}


#define DRAW_LINE_TO_POLICE
#ifdef DRAW_LINE_TO_POLICE
#define DRAW_POLICE_LINE( x ) x
#else
#define DRAW_POLICE_LINE( x ) ;
#endif

#define SQR_SEARCH_DISTANCE 250

void Core::FlowfieldSystem::CalculatePathInDeadNode( Core::Entity rioter, int group )
{
	glm::vec3 toPolice = glm::vec3( 0.0f );

	Core::WorldPositionComponent* wpc = WGETC<Core::WorldPositionComponent>( rioter );
	glm::vec3 myPos = glm::vec3( wpc->position[0], wpc->position[1], wpc->position[2] );

	for( std::vector<Entity>::iterator it = m_entities.begin(); it != m_entities.end(); it++ )
	{
		UnitTypeComponent* utc = WGETC<UnitTypeComponent>(*it);
		if( utc->type != Core::UnitType::Police )
			continue;

		Core::WorldPositionComponent* policeWpc = WGETC<Core::WorldPositionComponent>( *it );
		toPolice += float(glm::distance2( glm::vec3( policeWpc->position[0], policeWpc->position[1], policeWpc->position[2] ), myPos ) < SQR_SEARCH_DISTANCE) 
			* (glm::vec3( policeWpc->position[0], policeWpc->position[1], policeWpc->position[2] ) - myPos);
	}

	if( glm::dot( toPolice, toPolice ) != 0.0f )
		toPolice = glm::normalize( toPolice );

	// find to which of the adjacent nodes you want to walk...
	Core::NavigationMesh* instance = Core::GetNavigationMesh();
	Core::FlowfieldComponent* ffc = WGETC<Core::FlowfieldComponent>(rioter);
	int edgesInNode = instance->nodes[ffc->node].corners[3].length < 0 ? 3 : 4;
	for( int i = 0; i < edgesInNode; i++ )
	{
		int nextNode = instance->nodes[ffc->node].corners[i].linksTo;
		if( !instance->flowfields[group].deadNodes[nextNode] )
		{
			glm::vec3 toNodeEdge = glm::normalize( instance->nodes[ffc->node].GetMidPoint( i ));
			if( glm::dot(toPolice, toNodeEdge ) < 0.3f )
			{
				Core::MovementComponent* mvmc = WGETC<Core::MovementComponent>(rioter);
				mvmc->SetDirection( mvmc, toNodeEdge.x, toNodeEdge.y, toNodeEdge.z );
				return;
			}			
		}
	}

	DRAW_POLICE_LINE( GFX::Debug::DrawLine( myPos, myPos + toPolice * 2.0f, GFXColor( 1.0f, 1.0f, 0.0f, 1.0f ), false ) );
}