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

			//glm::vec3 midOfEdgeLinkingToNextNode = instance->flowfields[groupID].list[ ffc->node ];
			float* navMeshGoal = instance->flowfields[ attribc->rioter.groupID ].goal;
			glm::vec3 navGoal = glm::vec3( navMeshGoal[0], 0.0f, navMeshGoal[2] );
			//if( glm::dot( navGoal, navGoal ) > 0.05f ) // goal node condition...
			//{
				
			if( !Core::PathFinder::CheckLineVsNavMesh( position, navGoal, 3.0f, ffc->node ) )
			{
				//GFX::Debug::DrawLine( position, navGoal, GFXColor( 0, 0.5, 1, 1 ), false );
				glm::vec3 newDir = glm::normalize( navGoal - position );
				glm::vec3 currentDir = glm::vec3( mvmc->newDirection[0], mvmc->newDirection[1], mvmc->newDirection[2] ); 
				
				Core::MovementComponent::SetDirection( mvmc, newDir.x, 0.0f, newDir.z );
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
						//targetPosition = lineStart + ( lineEnd - lineStart ) * 0.25f;
						targetPosition = lineStart + glm::normalize( lineEnd - lineStart ) * 1.25f;
					else 
						//targetPosition = lineEnd + ( lineStart - lineEnd ) * 0.25f;
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

			//}
			//else
			//	MovementComponent::SetDirection( mvmc, 0.0f, 0.0f, 0.0f );
		}
	}
}

/*
{
			if( !instance )
				continue;

			Core::AttributeComponent* attribc = WGETC<Core::AttributeComponent>(*it);
			Core::FlowfieldComponent* ffc = WGETC<Core::FlowfieldComponent>(*it);

			float* navMeshGoal = instance->flowfields[ attribc->rioter.groupID ].goal;
			if( !Core::PathFinder::CheckLineVsNavMesh( position, glm::vec3( navMeshGoal[0], 0.0f, navMeshGoal[2] ), 3.0f, ffc->node ) )
			{
				GFX::Debug::DrawLine( position, glm::vec3( navMeshGoal[0], 0.0f, navMeshGoal[2] ), GFXColor( 0, 0.5, 1, 1 ), false );
				glm::vec3 newDir = glm::normalize( glm::vec3( navMeshGoal[0], 0.0f, navMeshGoal[2] ) - position );
				glm::vec3 currentDir = glm::vec3( mvmc->newDirection[0], mvmc->newDirection[1], mvmc->newDirection[2] ); 
				
				if( glm::dot( currentDir, newDir ) > 0.0f )
					Core::MovementComponent::SetDirection( mvmc, newDir.x, 0.0f, newDir.z );
			}

			continue;
		}
		*/