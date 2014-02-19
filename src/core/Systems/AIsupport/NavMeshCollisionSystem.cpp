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

			float* points = instance->nodes[ffc->node].points;

			Core::BoundingSphere& sphere = *reinterpret_cast<Core::BoundingSphere*>(bvc->data);
			Core::WorldPositionComponent* wpc = WGETC<Core::WorldPositionComponent>(*it);
			glm::vec3& position = *reinterpret_cast<glm::vec3*>(wpc->position);

			ffc->distance = std::numeric_limits<float>::max();
			ffc->wallDirX = 0;
			ffc->wallDirZ = 0;

			// check vs line
			for( int i = 0; i < 4; i++ )
			{
				if( instance->nodes[ffc->node].corners[i].length < 0 || instance->nodes[ffc->node].corners[i].linksTo >= 0 )
					continue;

				// fix indices for current line... 
				int ii = i * 2;
				int oo = (ii + 2) % 8;	

				// define lines...
				glm::vec3 lineStart = glm::vec3( points[ ii ], 0.0f, points[ ii + 1 ] );
				glm::vec3 lineEnd	= glm::vec3( points[ oo ], 0.0f, points[ oo + 1 ] );
				glm::vec3 fromStartToObject = position + *reinterpret_cast<glm::vec3*>( sphere.offset ) - lineStart;
					
				// check if outside line limits...
				float distanceAlongLine = glm::dot( (lineEnd - lineStart) * instance->nodes[ffc->node].corners[i].inverseLength, fromStartToObject );

				if( instance->nodes[ffc->node].corners[i].length < distanceAlongLine || distanceAlongLine < 0 )
					continue;

				// check if collided with line...
				glm::vec3 cross = glm::normalize( glm::cross( (lineEnd - lineStart), glm::vec3( 0.0f, 1.0f, 0.0f ) ) );
				float distanceToLine = glm::dot( cross, fromStartToObject );
				
				if( distanceToLine < sphere.radius )
					position += cross * (sphere.radius - distanceToLine);

				// pf utility...
				if( distanceToLine < ffc->distance )
				{
					ffc->distance = distanceToLine;
					ffc->wallDirX = abs( cross.z ) < 0.7853981633f ? cross.x > 0 ? 1.0f : -1.0f : 0.0f;
					ffc->wallDirZ = abs( cross.x ) < 0.7853981633f ? cross.z > 0 ? 1.0f : -1.0f : 0.0f;
				}				
			}

			// pf utility...
			#define WallCurDeclineVal 1.0f
			if( ffc->distance == std::numeric_limits<float>::max() )
				ffc->distance = 0.0f;
			else
				ffc->distance = WallCurDeclineVal / ( WallCurDeclineVal + ffc->distance * ffc->distance );
			

			//glm::vec3 tempVec = glm::vec3(0.0f);
			//tempVec = glm::vec3( 0, 0, 1.0f ) * (float)ffc->wallDirY + glm::vec3( 1.0f, 0, 0 ) * (float)ffc->wallDirX;
			//GFX::Debug::DrawLine( position, position + tempVec, GFXColor( 1, 1, 0.5, 1 ), false );





			// check vs corners
			for( int i = 0; i < 4; i++ )
			{
				// check for last corner of triangle
				if( instance->nodes[ffc->node].corners[i].linksTo < -1.5f )
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
