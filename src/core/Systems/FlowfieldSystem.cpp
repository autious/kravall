#include "FlowfieldSystem.hpp"
#include "World.hpp"
#include <logger/Logger.hpp>

#include <gfx/GFXInterface.hpp>

Core::FlowfieldSystem::FlowfieldSystem()
	: BaseSystem( EntityHandler::GenerateAspect<
		WorldPositionComponent, BoundingVolumeComponent, UnitTypeComponent, AttributeComponent, FlowfieldComponent >(), 0ULL )
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

				if( instance->CheckPointInsideNode( glm::vec3(position), p ) )
				{
					Core::MovementComponent* mvmc = WGETC<Core::MovementComponent>(*it);
					Core::AttributeComponent* attribc = WGETC<Core::AttributeComponent>(*it);
					#define FF_NORMAL_INFLUENCE 1.6f

					glm::vec3 midOfEdgeLinkingToNextNode = instance->flowfields[attribc->rioter.groupID].list[ p ];
					if( glm::dot( midOfEdgeLinkingToNextNode, midOfEdgeLinkingToNextNode ) > 0.05f ) // goal node condition...
					{
						*reinterpret_cast<glm::vec3*>(mvmc->direction) = glm::normalize( 
							glm::normalize( instance->flowfields[attribc->rioter.groupID].list[ p ] - position ) 
							- *reinterpret_cast<glm::vec3*>( instance->nodes[ p ].corners[ instance->flowfields[attribc->rioter.groupID].edges[p] ].normal ) * FF_NORMAL_INFLUENCE ); 
					}
					else
						*reinterpret_cast<glm::vec3*>(mvmc->direction) = glm::vec3(0.0f);
				}
			}
		}
	}
}
