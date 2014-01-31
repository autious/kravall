#include "FlowfieldSystem.hpp"
#include "World.hpp"
#include <logger/Logger.hpp>

#include <gfx/GFXInterface.hpp>

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
			if( utc->type == Core::UnitType::Police )
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

			Core::WorldPositionComponent* wpc = WGETC<Core::WorldPositionComponent>(*it);
			glm::vec3 position = wpc->GetVec3( *wpc );			

			glm::vec3 midOfEdgeLinkingToNextNode = instance->flowfields[groupID].list[ ffc->node ];
			if( glm::dot( midOfEdgeLinkingToNextNode, midOfEdgeLinkingToNextNode ) > 0.05f ) // goal node condition...
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

				//GFX::Debug::DrawLine( targetPosition, position, GFXColor( 0, 0, 1, 1 ), false );
				

				glm::vec3 flowfieldDirection = glm::normalize( targetPosition - position );

				MovementComponent::SetDirection( mvmc, flowfieldDirection.x, 0, flowfieldDirection.z );

			}
			else
				MovementComponent::SetDirection( mvmc, 0.0f, 0.0f, 0.0f );
		}
	}
}





/*
void Core::FlowfieldSystem::Update( float delta )
{
	Core::NavigationMesh* instance = Core::GetNavigationMesh();
	if( instance )
	{
		if( !instance->AllocateFrameMemoryForAstar() )
			return;

		// for all entities
		for( std::vector<Entity>::iterator it = m_entities.begin(); it != m_entities.end(); it++ )
		{
			// entity is not assigned to a node...
			Core::FlowfieldComponent* ffc = WGETC<Core::FlowfieldComponent>(*it);
			if( ffc->node < 0 )
				continue;

			// entity has separate goal...
			Core::MovementComponent* mvmc = WGETC<Core::MovementComponent>(*it);
			if( mvmc->goal[0] != std::numeric_limits<float>::max() )
				continue;


			int groupID;
			Core::AttributeComponent* attribc = WGETC<Core::AttributeComponent>(*it);
			UnitTypeComponent* utc = WGETC<UnitTypeComponent>(*it);
			if (utc->type == Core::UnitType::Rioter)
				groupID = attribc->rioter.groupID;
			else
				groupID = attribc->police.squadID;


			Core::WorldPositionComponent* wpc = WGETC<Core::WorldPositionComponent>(*it);
			glm::vec3 position = wpc->GetVec3( *wpc );			

			glm::vec3 ffGoal = glm::vec3( instance->flowfields[groupID].goal[0], 0.0f, instance->flowfields[groupID].goal[1] );

			// find out what node we want to go to...
			int goalNode = -1;
			for( int i = 0; i < instance->nrNodes; i++ )
			{
				if( instance->CheckPointInsideNode( ffGoal, i ) )
				{
					goalNode = i;
					break;
				}	
			}

			if( goalNode < 0 )
				continue;

			Core::PathData path = instance->CalculateShortPath( ffc->node, position, goalNode, ffGoal );
				
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
					//targetPosition = lineStart + ( lineEnd - lineStart ) * 0.25f;
					targetPosition = lineStart + glm::normalize( lineEnd - lineStart ) * 1.25f;
				else 
					//targetPosition = lineEnd + ( lineStart - lineEnd ) * 0.25f;
					targetPosition = lineEnd + glm::normalize( lineStart - lineEnd ) * 1.25f;
			}
			else 
			{
				// is inside edges...
				targetPosition = path.point;
			}

			//GFX::Debug::DrawLine( targetPosition, position, GFXColor( 0, 0, 1, 1 ), false );
				

			glm::vec3 flowfieldDirection = glm::normalize( targetPosition - position );

			MovementComponent::SetDirection( mvmc, flowfieldDirection.x, 0, flowfieldDirection.z );

		}
	}

}
*/








/*
void Core::FlowfieldSystem::Update( float delta )
{
	Core::NavigationMesh* instance = Core::GetNavigationMesh();
	if( instance )
	{
		// for all entities
		for( std::vector<Entity>::iterator it = m_entities.begin(); it != m_entities.end(); it++ )
		{
			Core::FlowfieldComponent* ffc = WGETC<Core::FlowfieldComponent>(*it);
			
			// entity is not assigned to a node...
			if( ffc->node < 0 )
				continue;

			int groupID;
			Core::AttributeComponent* attribc = WGETC<Core::AttributeComponent>(*it);
			UnitTypeComponent* utc = WGETC<UnitTypeComponent>(*it);
			if (utc->type == Core::UnitType::Rioter)
				groupID = attribc->rioter.groupID;
			else
				groupID = attribc->police.squadID;

			Core::MovementComponent* mvmc = WGETC<Core::MovementComponent>(*it);

			if( mvmc->goal[0] != FLT_MAX )
				continue;


			Core::WorldPositionComponent* wpc = WGETC<Core::WorldPositionComponent>(*it);
			glm::vec3& position = *reinterpret_cast<glm::vec3*>(wpc->position);
			

			glm::vec3 midOfEdgeLinkingToNextNode = instance->flowfields[groupID].list[ ffc->node ];
			if( glm::dot( midOfEdgeLinkingToNextNode, midOfEdgeLinkingToNextNode ) > 0.05f ) // goal node condition...
			{

				float* edgeNormal = instance->nodes[ ffc->node ].corners[ instance->flowfields[groupID].edges[ffc->node] ].normal;
				glm::vec3 normal = glm::vec3( edgeNormal[0], 0.0f, edgeNormal[1] );
				//glm::vec3 dirctionToEdgeInNextNode = glm::normalize( instance->flowfields[groupID].list[ ffc->node ] - position );
				glm::vec3 dirToPointInNextNode = glm::normalize( instance->flowfields[groupID].list[ ffc->node ] - position );

				//GFX::Debug::DrawSphere( instance->flowfields[groupID].list[ ffc->node ], 4.0f, GFXColor( 1, 1, 0, 1 ), false ); 


				int ii = instance->flowfields[groupID].edges[ ffc->node ] * 2;
				int oo = ( ii + 2 ) % 8;
				glm::vec3 lineStart = glm::vec3( instance->nodes[ ffc->node ].points[ ii ], 0.0f, instance->nodes[ ffc->node ].points[ ii + 1 ] );
				glm::vec3 lineEnd	= glm::vec3( instance->nodes[ ffc->node ].points[ oo ], 0.0f, instance->nodes[ ffc->node ].points[ oo + 1 ] );
				glm::vec3 lineMid = lineStart + (( lineEnd - lineStart ) * 0.5f );


				glm::vec3 pointsToChoose[3];
				//pointsToChoose[0] = lineStart + glm::normalize( lineEnd - lineStart ) * 1.0f;
				//pointsToChoose[1] = lineMid;
				//pointsToChoose[2] = lineEnd + glm::normalize( lineStart - lineEnd ) * 1.0f;

				pointsToChoose[0] = lineStart + ( lineEnd - lineStart ) * 0.25f;
				pointsToChoose[1] = lineMid;
				pointsToChoose[2] = lineEnd + ( lineStart - lineEnd ) * 0.25f;



				for( int i = 0; i < 3; i++ )
					GFX::Debug::DrawSphere( pointsToChoose[i], 2.0f, GFXColor( 1, 0, 0, 1 ), false ); 

				// sort...
				std::sort( &pointsToChoose[0], &pointsToChoose[3], 
					[&dirToPointInNextNode, &position]( glm::vec3& A, glm::vec3& B )
				{ 
					//if( glm::distance(A, dirToPointInNextNode) + glm::distance( A, position ) < 
					//	glm::distance( B, dirToPointInNextNode ) + glm::distance( B, position ) ) 
					//	return true; 

					float distA = glm::dot( A - dirToPointInNextNode, A - dirToPointInNextNode ) + glm::dot( A - position, A - position ) * 0.5f;
					float distB = glm::dot( B - dirToPointInNextNode, B - dirToPointInNextNode ) + glm::dot( B - position, B - position ) * 0.5f;
					if( distA > distB )
						return true;
					return false; 
				} );
				
				GFX::Debug::DrawLine( instance->flowfields[groupID].list[ ffc->node ], position, GFXColor( 0, 1, 0, 1 ), false );
				GFX::Debug::DrawLine( pointsToChoose[0], position, GFXColor( 0, 0, 1, 1 ), false );

				/*
				// calc distance from opposite edges...
				float squareDistanceToEntryLine;
				glm::vec3 lineMid;
				glm::vec3 otherMid;
				
				{
					int ii = instance->flowfields[groupID].edges[ ffc->node ] * 2;
					int oo = ( ii + 2 ) % 8;
			
					glm::vec3 lineStart = glm::vec3( instance->nodes[ ffc->node ].points[ ii ], 0.0f, instance->nodes[ ffc->node ].points[ ii + 1 ] );
					glm::vec3 lineEnd	= glm::vec3( instance->nodes[ ffc->node ].points[ oo ], 0.0f, instance->nodes[ ffc->node ].points[ oo + 1 ] );
					lineMid = lineStart + (( lineEnd - lineStart ) * 0.5f );

					glm::vec3 cross = glm::normalize( glm::cross( (lineEnd - lineStart), glm::vec3( 0.0f, 1.0f, 0.0f ) ) );
					squareDistanceToEntryLine = glm::dot( cross, position - lineStart );
					squareDistanceToEntryLine *= squareDistanceToEntryLine; // make it squared
				}

				// if quad
				if( instance->nodes[ ffc->node ].corners[3].length > 0 )  
				{
					int ii = ((instance->flowfields[groupID].edges[ ffc->node ] + 2) % 4) * 2;
					int oo = ( ii + 2 ) % 8;
					
					glm::vec3 lineStart = glm::vec3( instance->nodes[ ffc->node ].points[ ii ], 0.0f, instance->nodes[ ffc->node ].points[ ii + 1 ] );
					glm::vec3 lineEnd	= glm::vec3( instance->nodes[ ffc->node ].points[ oo ], 0.0f, instance->nodes[ ffc->node ].points[ oo + 1 ] );
					otherMid = lineStart + (( lineEnd - lineStart ) * 0.5f );
				}
				// otherwise it's a triangle...
				else 
				{
					// get oppisite corner...
					int ii = ((instance->flowfields[groupID].edges[ ffc->node ] + 2) % 3) * 2;
					otherMid =  glm::vec3( instance->nodes[ ffc->node ].points[ ii ], 0.0f, instance->nodes[ ffc->node ].points[ ii + 1 ] );
				}


				// entity can be further from this point if a non quadratic node, wierd behaviour might result... keep your eyes vigilalt and open!
				float sqdistance = glm::dot( otherMid - lineMid, otherMid - lineMid ) + 0.001f;  
				float ratio = ( squareDistanceToEntryLine / sqdistance );

				ratio = ratio > 0.8f ? 0.8f : ratio;
				ratio = ratio < 0.0f ? 0.0f : ratio;				

				//glm::vec3 flowfieldDirection = glm::normalize( -normal * (1 - ratio) + dirctionToEdgeInNextNode * (ratio + 0.2f) );
				glm::vec3 flowfieldDirection = dirctionToEdgeInNextNode; // NOCOMMIT
				// end star here ...
				
				//glm::vec3 flowfieldDirection = glm::normalize( pointsToChoose[0] - position );
				glm::vec3 flowfieldDirection = dirToPointInNextNode; // NOCOMMIT

				MovementComponent::SetDirection( mvmc, flowfieldDirection.x, 0, flowfieldDirection.z );

			}
			else
				MovementComponent::SetDirection( mvmc, 0.0f, 0.0f, 0.0f );
		}
	}
}

*/