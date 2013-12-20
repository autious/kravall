#include "FlowfieldSystem.hpp"
#include "World.hpp"
#include <logger/Logger.hpp>

#include <gfx/gfxinterface.hpp>


Core::FlowfieldSystem::FlowfieldSystem()
	: BaseSystem( EntityHandler::GenerateAspect< WorldPositionComponent, BoundingVolumeComponent, UnitTypeComponent >(), 0ULL )
{
}



void Drawem()
{
	const Core::NavigationMesh* instance = Core::GetNavigationMesh();

	for( int p = 0; p < instance->nrNodes; p++ )
	{
		float* nodes = instance->nodes[p].points;
		for( int i = 0; i < 3; i++ )
		{
			GFX::Debug::DrawLine(	glm::vec3( nodes[ i * 2 + 0], 0.0f, nodes[ i * 2 + 1 ] ), 
									glm::vec3( nodes[ (i+1) * 2 + 0], 0.0f, nodes[ (i+1) * 2 + 1 ] ),
									GFXColor( 1.0f, 1.0f, 0, 1.0f ), true );
		}

		GFX::Debug::DrawLine(	glm::vec3( nodes[6], 0.0f, nodes[7] ), 
									glm::vec3( nodes[0], 0.0f, nodes[1] ),
									GFXColor( 1.0f, 1.0f, 0, 1.0f ), true );
	}
}



void Core::FlowfieldSystem::Update( float delta )
{
	const Core::NavigationMesh* instance = Core::GetNavigationMesh();
	if( instance )
	{
		Drawem();

		// for all nodes
		for( int p = 0; p < instance->nrNodes; p++ )
		{
			float* points = instance->nodes[p].points;

			// for all entities
			for( std::vector<Entity>::iterator it = m_entities.begin(); it != m_entities.end(); it++ )
			{	
				UnitTypeComponent* utc = WGETC<UnitTypeComponent>(*it);
				Core::BoundingVolumeComponent* bvc = WGETC<Core::BoundingVolumeComponent>(*it);
				if( bvc->collisionModel != Core::BoundingVolumeCollisionModel::DynamicResolution ||
					bvc->type != Core::BoundingVolumeType::SphereBoundingType ||
					utc->type != Core::UnitType::Rioter )
					continue;

				Core::BoundingSphere& sphere = *reinterpret_cast<Core::BoundingSphere*>(bvc->data);
				Core::WorldPositionComponent* wpc = WGETC<Core::WorldPositionComponent>(*it);
				glm::vec3& position = *reinterpret_cast<glm::vec3*>(wpc->position);

				for( int i = 0; i < 4; i++ )
				{					
					if( instance->nodes->corners[i].length < 0 || instance->nodes->corners[i].linksTo >= 0 )
						continue;

					// fix indices for current line... 
					int ii = i * 2;
					int oo = i == 3 ? 0 : ii + 2;

					// define lines...
					glm::vec3 lineStart = glm::vec3( points[ ii ], 0.0f, points[ ii + 1 ] );
					glm::vec3 lineEnd	= glm::vec3( points[ oo ], 0.0f, points[ oo + 1 ] );
					glm::vec3 fromStartToObject = position + *reinterpret_cast<glm::vec3*>( sphere.offset ) - lineStart;
					
					// check if outside line limits...
					float distanceAlongLine = glm::dot( (lineEnd - lineStart) * instance->nodes->corners[i].inverseLength, fromStartToObject );
					if( instance->nodes->corners[i].length < distanceAlongLine || distanceAlongLine < 0 )
						continue;

					// check if collided with line...
					glm::vec3 cross = glm::normalize( glm::cross( glm::vec3( 0.0f, 1.0f, 0.0f ), (lineEnd - lineStart) ) );
					float distanceToLine = glm::dot( cross, fromStartToObject );
				
					if( distanceToLine < sphere.radius )
						position += cross * (sphere.radius - distanceToLine);
				}

				// check vs line
				
				// check vs corner

			}
		}		

		// collision only for current node
			// save current node in agents

		// calculate flowfield
			// save current group in agent
			// reitterate on flowfield vector behaviour...
			
		// assign vector of all agents to base flowfield...

	}
}