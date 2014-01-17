#include "FlowfieldSystem.hpp"
#include "World.hpp"
#include <logger/Logger.hpp>

#include <gfx/GFXInterface.hpp>

#define FF_NORMAL_INFLUENCE 1.6f

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
		// for all entities
		for( std::vector<Entity>::iterator it = m_entities.begin(); it != m_entities.end(); it++ )
		{
			UnitTypeComponent* utc = WGETC<UnitTypeComponent>(*it);
			if(	utc->type != Core::UnitType::Rioter )
				continue;

			Core::FlowfieldComponent* ffc = WGETC<Core::FlowfieldComponent>(*it);
			
			// entity is not assigned to a node...
			if( ffc->node < 0 )
				continue;

			Core::WorldPositionComponent* wpc = WGETC<Core::WorldPositionComponent>(*it);
			glm::vec3& position = *reinterpret_cast<glm::vec3*>(wpc->position);

			Core::MovementComponent* mvmc = WGETC<Core::MovementComponent>(*it);
			Core::AttributeComponent* attribc = WGETC<Core::AttributeComponent>(*it);
			#define FF_NORMAL_INFLUENCE 1.6f

			glm::vec3 midOfEdgeLinkingToNextNode = instance->flowfields[attribc->rioter.groupID].list[ ffc->node ];
			if( glm::dot( midOfEdgeLinkingToNextNode, midOfEdgeLinkingToNextNode ) > 0.05f ) // goal node condition...
			{
				*reinterpret_cast<glm::vec3*>(mvmc->newDirection) = glm::normalize( 
					glm::normalize( instance->flowfields[attribc->rioter.groupID].list[ ffc->node ] - position ) 
					- *reinterpret_cast<glm::vec3*>( instance->nodes[ ffc->node ].corners[ instance->flowfields[attribc->rioter.groupID].edges[ffc->node] ].normal ) * FF_NORMAL_INFLUENCE ); 
			}
			else
				*reinterpret_cast<glm::vec3*>(mvmc->newDirection) = glm::vec3(0.0f);


		}
	}
}
