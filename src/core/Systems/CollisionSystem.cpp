#include "CollisionSystem.hpp"
#include "World.hpp"
#include <gfx/GFXInterface.hpp>

Core::CollisionSystem::CollisionSystem()
	: BaseSystem( EntityHandler::GenerateAspect< WorldPositionComponent, BoundingVolumeComponent >(), 0ULL )
{
}


void Core::CollisionSystem::Update( float delta )
{


	for( std::vector<Entity>::iterator it = m_entities.begin(); it != m_entities.end(); it++ )
	{		
		Core::BoundingVolumeComponent* bvc = WGETC<Core::BoundingVolumeComponent>(*it);

		// only dynamic enteties checks for collisions...
		if( bvc->type != Core::BoundingVolumeType::SphereBoundingType 
			|| bvc->collisionModel == Core::BoundingVolumeCollisionModel::NoResolution
			|| bvc->collisionModel == Core::BoundingVolumeCollisionModel::StaticResolution )
			continue;

		// data...
		Core::WorldPositionComponent* wpc = WGETC<Core::WorldPositionComponent>(*it);
		glm::vec3* myPosition = reinterpret_cast<glm::vec3*>(wpc->position);
		Core::BoundingSphere* mySphere = reinterpret_cast<Core::BoundingSphere*>( bvc->data );

		for( std::vector<Entity>::iterator other = m_entities.begin(); other != m_entities.end(); other++ )
		{
			if( *it != *other )
			{
				Core::BoundingVolumeComponent* bvcOther = WGETC<Core::BoundingVolumeComponent>(*other);
				if ( bvcOther->collisionModel == Core::BoundingVolumeCollisionModel::NoResolution || bvcOther->type != Core::BoundingVolumeType::SphereBoundingType )
					continue;

				Core::WorldPositionComponent* wpcOther = WGETC<Core::WorldPositionComponent>(*other);
				
				glm::vec3* otherPosition = reinterpret_cast<glm::vec3*>(wpcOther->position);
				Core::BoundingSphere* otherSphere = reinterpret_cast<Core::BoundingSphere*>( bvcOther->data );

				float sqareDistance = glm::dot( *myPosition - *otherPosition, *myPosition - *otherPosition );

				if( sqareDistance < (otherSphere->radius + mySphere->radius) * (otherSphere->radius + mySphere->radius) )
				{
					// Collision detected!
					// move myself away from collision. If the other entity is static, move the entire overlap away from the entity,
					// otherwise move half the distance as to achieve mutual collision resolution

					float delta = ((otherSphere->radius + mySphere->radius) - std::sqrt( sqareDistance ));
					glm::vec3 collisionNormal = glm::normalize(*myPosition - *otherPosition);

					//GFX::Debug::DrawSphere( *myPosition, mySphere->radius, GFXColor( 1, 0, 0, 1 ) );

					switch( bvcOther->collisionModel )
					{
					case Core::BoundingVolumeCollisionModel::DynamicResolution:
						
						// Head of list will always bow for tail of list in perfect frontal collision.
						//*myPosition		+= collisionNormal * ( delta * 0.66666666f);

						// Head and tail is equal when in perfect frontal collision. Would theoretically results in 
						// less flow but potentially better crowd dynamics when in a closed environment. 
						*myPosition		+= collisionNormal * ( delta * 0.5f);
						*otherPosition	-= collisionNormal * ( delta * 0.5f);

						break;

					case Core::BoundingVolumeCollisionModel::StaticResolution:
						*myPosition += collisionNormal * delta;
						break;

					default:
						break;
					}
				}
			}
		}
	}






}
