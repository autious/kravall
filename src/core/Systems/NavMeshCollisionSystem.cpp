#include "NavMeshCollisionSystem.hpp"
#include "World.hpp"
#include <logger/Logger.hpp>

#include <gfx/GFXInterface.hpp>


Core::NavMeshCollisionSystem::NavMeshCollisionSystem()
	: BaseSystem( EntityHandler::GenerateAspect<
		WorldPositionComponent, BoundingVolumeComponent, FlowfieldComponent >(), 0ULL )
{
}



void Core::NavMeshCollisionSystem::Update( float delta )
{
	Core::NavigationMesh* instance = Core::GetNavigationMesh();
	if( instance )
	{
		// for all entities
		for( std::vector<Entity>::iterator it = m_entities.begin(); it != m_entities.end(); it++ )
		{
			Core::BoundingVolumeComponent* bvc = WGETC<Core::BoundingVolumeComponent>(*it);
			if( bvc->collisionModel != Core::BoundingVolumeCollisionModel::DynamicResolution ||
				bvc->type != Core::BoundingVolumeType::SphereBoundingType )
				continue;

			Core::FlowfieldComponent* ffc = WGETC<Core::FlowfieldComponent>(*it);
			
			// entity is not assigned to a node...
			if( ffc->node < 0 )
				continue;

			float* points = instance->m_nodes[ffc->node].points;

			Core::BoundingSphere& sphere = *reinterpret_cast<Core::BoundingSphere*>(bvc->data);
			Core::WorldPositionComponent* wpc = WGETC<Core::WorldPositionComponent>(*it);
			glm::vec3& position = *reinterpret_cast<glm::vec3*>(wpc->position);

			// check vs line
			for( int i = 0; i < 4; i++ )
			{
				if( instance->m_nodes[ffc->node].corners[i].length < 0 || instance->m_nodes[ffc->node].corners[i].linksTo >= 0 )
					continue;

				// fix indices for current line... 
				int ii = i * 2;
				int oo = (ii + 2) % 8;	

				// define lines...
				glm::vec3 lineStart = glm::vec3( points[ ii ], 0.0f, points[ ii + 1 ] );
				glm::vec3 lineEnd	= glm::vec3( points[ oo ], 0.0f, points[ oo + 1 ] );
				glm::vec3 fromStartToObject = position + *reinterpret_cast<glm::vec3*>( sphere.offset ) - lineStart;
					
				// check if outside line limits...
				float distanceAlongLine = glm::dot( (lineEnd - lineStart) * instance->m_nodes[ffc->node].corners[i].inverseLength, fromStartToObject );

				if( instance->m_nodes[ffc->node].corners[i].length < distanceAlongLine || distanceAlongLine < 0 )
					continue;

				// check if collided with line...
				glm::vec3 cross = glm::normalize( glm::cross( (lineEnd - lineStart), glm::vec3( 0.0f, 1.0f, 0.0f ) ) );
				float distanceToLine = glm::dot( cross, fromStartToObject );
				
				if( distanceToLine < sphere.radius && distanceToLine >= 0 )
					position += cross * (sphere.radius - distanceToLine);
			}

			// check vs corner
			for( int i = 0; i < 4; i++ )
			{
				// check for last corner of triangle
				if( instance->m_nodes[ffc->node].corners[i].linksTo < -1.5f )
					continue;

				int ii = i * 2;
				glm::vec3 lineStart = glm::vec3( points[ ii ], 0.0f, points[ ii + 1 ] );
				float sqDistanceToCorner = 
					glm::dot( position + *reinterpret_cast<glm::vec3*>( sphere.offset ) - lineStart, 
								position + *reinterpret_cast<glm::vec3*>( sphere.offset ) - lineStart );

				if( sqDistanceToCorner < sphere.radius * sphere.radius )
				{
					position += glm::normalize( position + *reinterpret_cast<glm::vec3*>( sphere.offset ) - lineStart ) 
						* (sphere.radius - std::sqrt( sqDistanceToCorner ));
				}
			}
		}
	}
}
