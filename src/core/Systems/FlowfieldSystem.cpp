#include "FlowfieldSystem.hpp"
#include "World.hpp"
#include <logger/Logger.hpp>

#include <gfx/GFXInterface.hpp>

#define FF_NORMAL_INFLUENCE 1.6f

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
				//continue;
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
				glm::vec3 dirctionToEdgeInNextNode = glm::normalize( instance->flowfields[groupID].list[ ffc->node ] - position );

#ifdef SHOW_NAVMESH_NEXT_GOAL
				GFX::Debug::DrawSphere( instance->flowfields[groupID].list[ ffc->node ], 4.0f, GFXColor( 1, 1, 0, 1 ), false );
#endif

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

				ratio = ratio > 0.5f ? 0.5f : ratio;
				ratio = ratio < 0.0f ? 0.0f : ratio;				

				glm::vec3 flowfieldDirection = glm::normalize( -normal * (1 - ratio) + dirctionToEdgeInNextNode * (ratio + 0.5f) );



				//GFX::Debug::DrawLine( position, position + flowfieldDirection * 20.f, GFXColor( 1, 0, 1, 1 ), false );
				//GFX::Debug::DrawSphere( lineStart + ( lineEnd - lineStart ) * 0.5f, 4.0f, GFXColor( 1, 1, 0, 1 ), false );

				MovementComponent::SetDirection( mvmc, flowfieldDirection.x, 0, flowfieldDirection.z );

			}
			else
				MovementComponent::SetDirection( mvmc, 0.0f, 0.0f, 0.0f );
		}
	}
}
