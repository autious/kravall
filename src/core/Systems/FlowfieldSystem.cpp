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
			if(	utc->type != Core::UnitType::Rioter )
				continue;

			Core::FlowfieldComponent* ffc = WGETC<Core::FlowfieldComponent>(*it);
			
			// entity is not assigned to a node...
			Core::MovementComponent* mvmc = WGETC<Core::MovementComponent>(*it);
			if( ffc->node < 0 )
			{
				*reinterpret_cast<glm::vec3*>(mvmc->newDirection) = glm::vec3(0.0f);
				continue;
			}

			Core::WorldPositionComponent* wpc = WGETC<Core::WorldPositionComponent>(*it);
			glm::vec3& position = *reinterpret_cast<glm::vec3*>(wpc->position);

			Core::AttributeComponent* attribc = WGETC<Core::AttributeComponent>(*it);

			glm::vec3 midOfEdgeLinkingToNextNode = instance->flowfields[attribc->rioter.groupID].list[ ffc->node ];
			if( glm::dot( midOfEdgeLinkingToNextNode, midOfEdgeLinkingToNextNode ) > 0.05f ) // goal node condition...
			//if( instance->flowfields[attribc->rioter.groupID].edges[ ffc->node] < 0.0f ) // goal node condition...
			{
				float* edgeNormal = instance->nodes[ ffc->node ].corners[ instance->flowfields[attribc->rioter.groupID].edges[ffc->node] ].normal;
				glm::vec3 normal = glm::vec3( edgeNormal[0], 0.0f, edgeNormal[1] );

				glm::vec3 dirctionToEdgeInNextNode = glm::normalize( instance->flowfields[attribc->rioter.groupID].list[ ffc->node ] - position );

				// calc distance from opposite edges...
				float squareDistanceToEntryLine;
				glm::vec3 lineMid;
				glm::vec3 otherMid;
				
				{
					int ii = instance->flowfields[attribc->rioter.groupID].edges[ ffc->node ] * 2;
					int oo = ( ii + 2 ) % 8;
			
					glm::vec3 lineStart = glm::vec3( instance->nodes[ ffc->node ].points[ ii ], 0.0f, instance->nodes[ ffc->node ].points[ ii + 1 ] );
					glm::vec3 lineEnd	= glm::vec3( instance->nodes[ ffc->node ].points[ oo ], 0.0f, instance->nodes[ ffc->node ].points[ oo + 1 ] );
					lineMid = lineStart + (( lineEnd - lineStart ) * 0.5f );

					glm::vec3 cross = glm::normalize( glm::cross( (lineEnd - lineStart), glm::vec3( 0.0f, 1.0f, 0.0f ) ) );
					squareDistanceToEntryLine = glm::dot( cross, position - lineStart );
					squareDistanceToEntryLine *= squareDistanceToEntryLine; // make it squared
				}

				if( instance->nodes[ ffc->node ].corners[3].length > 0 ) // if quad... 
				{
					int ii = ((instance->flowfields[attribc->rioter.groupID].edges[ ffc->node ] + 2) % 4) * 2;
					int oo = ( ii + 2 ) % 8;
					
					glm::vec3 lineStart = glm::vec3( instance->nodes[ ffc->node ].points[ ii ], 0.0f, instance->nodes[ ffc->node ].points[ ii + 1 ] );
					glm::vec3 lineEnd	= glm::vec3( instance->nodes[ ffc->node ].points[ oo ], 0.0f, instance->nodes[ ffc->node ].points[ oo + 1 ] );
					otherMid = lineStart + (( lineEnd - lineStart ) * 0.5f );

					//otherMid = instance->flowfields[attribc->rioter.groupID].list[ ffc->node ];

				}
				else // otherwise it's a triangle...
				{
					// get oppisite corner...
					int ii = ((instance->flowfields[attribc->rioter.groupID].edges[ ffc->node ] + 2) % 3) * 2;
					otherMid =  glm::vec3( instance->nodes[ ffc->node ].points[ ii ], 0.0f, instance->nodes[ ffc->node ].points[ ii + 1 ] );
					//otherMid = instance->flowfields[attribc->rioter.groupID].list[ ffc->node ];
				}


				// entity can be further from this point if a non quadratic node, wierd behaviour mighht result... keep your eyes vigilalt and open!
				float sqdistance = glm::dot( otherMid - lineMid, otherMid - lineMid ) + 0.001f;  

				//#define FF_NORMAL_INFLUENCE 0.5f
				float ratio = ( squareDistanceToEntryLine / sqdistance ) ; // + FF_NORMAL_INFLUENCE;
				
				//glm::vec3 testDirection = glm::normalize( (lineMid + ( ooo - lineMid ) * 0.5f) - position );
				
				ratio = ratio > 1.0f ? 1.0f : ratio;
				ratio = ratio < 0.5f ? 0.5f : ratio;

				//GFX::Debug::DrawSphere( lineMid, 5.0f, GFXColor(1), false );
				//GFX::Debug::DrawSphere( otherMid, 2.0f, GFXColor(1), false );
				
				GFX::Debug::DrawSphere( instance->flowfields[attribc->rioter.groupID].list[ ffc->node ], 2.0f, GFXColor( 1, 1, 0, 1 ), false );

				*reinterpret_cast<glm::vec3*>(mvmc->newDirection) = glm::normalize(
					-normal * (1 - ratio) + dirctionToEdgeInNextNode * ( ratio + 0.5f ));
				
				//*reinterpret_cast<glm::vec3*>(mvmc->newDirection) = testDirection;
				//*reinterpret_cast<glm::vec3*>(mvmc->newDirection) = dirctionToEdgeInNextNode;

				// left-overs...
				//*reinterpret_cast<glm::vec3*>(mvmc->newDirection) = glm::normalize( );
					//- *reinterpret_cast<glm::vec3*>( instance->nodes[ ffc->node ].corners[ instance->flowfields[attribc->rioter.groupID].edges[ffc->node] ].normal ) * FF_NORMAL_INFLUENCE ); 
					//- normal * FF_NORMAL_INFLUENCE );

				//GFX::Debug::DrawLine(position, position + *reinterpret_cast<glm::vec3*>(mvmc->newDirection), GFXColor(1.0f, 0.5f, 0.0f, 1.0f), false);

			}
			else
				*reinterpret_cast<glm::vec3*>(mvmc->newDirection) = glm::vec3(0.0f);


		}
	}
}
