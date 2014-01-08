#include "FlowfieldSystem.hpp"
#include "World.hpp"
#include <logger/Logger.hpp>

#include <gfx/GFXInterface.hpp>


Core::FlowfieldSystem::FlowfieldSystem()
	: BaseSystem( EntityHandler::GenerateAspect<
		WorldPositionComponent, BoundingVolumeComponent, UnitTypeComponent, AttributeComponent >(), 0ULL )
{
}




void Core::FlowfieldSystem::Update( float delta )
{
	Core::NavigationMesh* instance = Core::GetNavigationMesh();
	if( instance )
	{
		// for all nodes
		for( int p = 0; p < instance->nrNodes; p++ )
		{
			// for all entities
			for( std::vector<Entity>::iterator it = m_entities.begin(); it != m_entities.end(); it++ )
			{	
				UnitTypeComponent* utc = WGETC<UnitTypeComponent>(*it);
				if(	utc->type != Core::UnitType::Rioter )
					continue;

				Core::WorldPositionComponent* wpc = WGETC<Core::WorldPositionComponent>(*it);
				glm::vec3& position = *reinterpret_cast<glm::vec3*>(wpc->position);

				#define FF_VS_PF_FACTOR 1.0f

				if( instance->CheckPointInsideNode( glm::vec3(position), p ) )
				{
					Core::MovementComponent* mvmc = WGETC<Core::MovementComponent>(*it);
					
					Core::AttributeComponent* attribc = WGETC<Core::AttributeComponent>(*it);
					glm::vec3 midOfEdgeLinkingToNextNode = instance->flowfields[attribc->rioter.groupID].list[ p ];

					if( glm::dot( midOfEdgeLinkingToNextNode, midOfEdgeLinkingToNextNode ) > 0.05f ) // goal node condition...
					{
						*reinterpret_cast<glm::vec3*>(mvmc->direction) = glm::normalize( glm::normalize( instance->flowfields[attribc->rioter.groupID].list[ p ] - position ) * FF_VS_PF_FACTOR + *reinterpret_cast<glm::vec3*>(mvmc->direction) );
						GFX::Debug::DrawLine( position, position + *reinterpret_cast<glm::vec3*>(mvmc->direction), GFXColor( 1.0f, 0.0f, 0.0f, 1.0f ), false );
					}
				}
			}
		}


		// collision only for current 'and adjacent' nodes
			// save current node in agents

		// calculate flowfield
			// save current group in agent
			// reitterate on flowfield vector behaviour...
			

	}
}
