#include "CollisionSystem2D.hpp"
#include "World.hpp"
#include <gfx/GFXInterface.hpp>

Core::CollisionSystem2D::CollisionSystem2D()
	: BaseSystem( EntityHandler::GenerateAspect< WorldPositionComponent, BoundingVolumeComponent, MovementComponent, UnitTypeComponent >(), 0ULL )
{
}


void Core::CollisionSystem2D::Update( float delta )
{

	{ // loop once for all rioters...

		for( std::vector<Entity>::iterator it = m_entities.begin(); it != m_entities.end(); it++ )
		{		
			Core::UnitTypeComponent* myType = WGETC<Core::UnitTypeComponent>(*it);
			if( myType->type != Core::UnitType::Rioter )
				continue;

			Core::BoundingVolumeComponent* bvc = WGETC<Core::BoundingVolumeComponent>(*it);
			// only dynamic enteties checks for collisions...
			if( bvc->type != Core::BoundingVolumeType::SphereBoundingType 
				|| bvc->collisionModel == Core::BoundingVolumeCollisionModel::NoResolution
				|| bvc->collisionModel == Core::BoundingVolumeCollisionModel::StaticResolution )
				continue;

			// data...
			Core::WorldPositionComponent* wpc = WGETC<Core::WorldPositionComponent>(*it);
		
			// collision in XZ plane only
			glm::vec3 myPosition = *reinterpret_cast<glm::vec3*>(wpc->position);
			myPosition.y = 0.0f;

			Core::BoundingSphere* mySphere = reinterpret_cast<Core::BoundingSphere*>( bvc->data );

			for( std::vector<Entity>::iterator other = m_entities.begin(); other != m_entities.end(); other++ )
			{
				if( *it != *other )
				{
					Core::BoundingVolumeComponent* bvcOther = WGETC<Core::BoundingVolumeComponent>(*other);
					if ( bvcOther->collisionModel == Core::BoundingVolumeCollisionModel::NoResolution || bvcOther->type != Core::BoundingVolumeType::SphereBoundingType )
						continue;

					Core::WorldPositionComponent* wpcOther = WGETC<Core::WorldPositionComponent>(*other);

					glm::vec3 otherPosition = *reinterpret_cast<glm::vec3*>(wpcOther->position);
					otherPosition.y = 0.0f;

					Core::BoundingSphere* otherSphere = reinterpret_cast<Core::BoundingSphere*>( bvcOther->data );

					float sqareDistance = glm::dot( myPosition - otherPosition, myPosition - otherPosition );

					if( sqareDistance < (otherSphere->radius + mySphere->radius) * (otherSphere->radius + mySphere->radius) )
					{
						// Collision detected!
						// move myself away from collision. If the other entity is static, move the entire overlap away from the entity,
						// otherwise move half the distance as to achieve mutual collision resolution

						// if objects are on top of eachother
						if( sqareDistance == 0 )
						{
							*reinterpret_cast<glm::vec3*>(wpc->position) += glm::vec3( 0.01f, 0.0f, 0.0f );
							myPosition = *reinterpret_cast<glm::vec3*>(wpc->position);
						}

						glm::vec3 collisionNormal = glm::normalize( myPosition - otherPosition );
					
						// check if entity should step aside to get around object...
						Core::MovementComponent* mvmc = WGETC<Core::MovementComponent>(*it);
						if( glm::dot( collisionNormal, *reinterpret_cast<glm::vec3*>(mvmc->direction) ) < -0.999 )
						{							
								glm::vec3 moveDir = glm::normalize( glm::cross( collisionNormal, glm::vec3( 0.0f, 1.0f, 0.0f ) ));
								*reinterpret_cast<glm::vec3*>(wpc->position) += moveDir * 0.1f;
							
								// update data
								myPosition = *reinterpret_cast<glm::vec3*>(wpc->position);
								collisionNormal = glm::normalize( myPosition - otherPosition );
								sqareDistance = glm::dot( myPosition - otherPosition, myPosition - otherPosition );
						}
					
						float delta = ((otherSphere->radius + mySphere->radius) - std::sqrt( sqareDistance ));

						Core::UnitTypeComponent* otherType = WGETC<Core::UnitTypeComponent>(*other);

						switch( bvcOther->collisionModel )
						{
						case Core::BoundingVolumeCollisionModel::DynamicResolution:
						
							// Head of list will always bow for tail of list in perfect frontal collision.
							//*myPosition		+= collisionNormal * ( delta * 0.66666666f);

							if( otherType->type == Core::UnitType::Police )
							{
								*reinterpret_cast<glm::vec3*>(wpc->position)		+= collisionNormal * ( delta * 1.0f );
							}
							else 
							{
								// Head and tail is equal when in perfect frontal collision. Would theoretically results in 
								// less flow but potentially better crowd dynamics when in a closed environment. 
								*reinterpret_cast<glm::vec3*>(wpc->position)		+= collisionNormal * ( delta * 0.5f );
								*reinterpret_cast<glm::vec3*>(wpcOther->position)	-= collisionNormal * ( delta * 0.5f );
							}
							break;

						case Core::BoundingVolumeCollisionModel::StaticResolution:
							*reinterpret_cast<glm::vec3*>(wpc->position) += collisionNormal * delta;
							break;

						default:
							break;
						}
					}
				}
			}
		}

	} // end rioter collision check


	{ // loop once for all police

		for( std::vector<Entity>::iterator it = m_entities.begin(); it != m_entities.end(); it++ )
		{		
			Core::BoundingVolumeComponent* bvc = WGETC<Core::BoundingVolumeComponent>(*it);

			// only dynamic enteties checks for collisions...
			if( bvc->type != Core::BoundingVolumeType::SphereBoundingType 
				|| bvc->collisionModel == Core::BoundingVolumeCollisionModel::NoResolution
				|| bvc->collisionModel == Core::BoundingVolumeCollisionModel::StaticResolution )
				continue;

			Core::UnitTypeComponent* myType = WGETC<Core::UnitTypeComponent>(*it);
			if( myType->type != Core::UnitType::Police )
				continue;

			// data...
			Core::WorldPositionComponent* wpc = WGETC<Core::WorldPositionComponent>(*it);
		
			// collision in XZ plane only
			glm::vec3 myPosition = *reinterpret_cast<glm::vec3*>(wpc->position);
			myPosition.y = 0.0f;

			Core::BoundingSphere* mySphere = reinterpret_cast<Core::BoundingSphere*>( bvc->data );

			for( std::vector<Entity>::iterator other = m_entities.begin(); other != m_entities.end(); other++ )
			{
				if( *it != *other )
				{
					Core::BoundingVolumeComponent* bvcOther = WGETC<Core::BoundingVolumeComponent>(*other);
					if ( bvcOther->collisionModel == Core::BoundingVolumeCollisionModel::NoResolution || bvcOther->type != Core::BoundingVolumeType::SphereBoundingType )
						continue;

					Core::WorldPositionComponent* wpcOther = WGETC<Core::WorldPositionComponent>(*other);

					glm::vec3 otherPosition = *reinterpret_cast<glm::vec3*>(wpcOther->position);
					otherPosition.y = 0.0f;

					Core::BoundingSphere* otherSphere = reinterpret_cast<Core::BoundingSphere*>( bvcOther->data );

					float sqareDistance = glm::dot( myPosition - otherPosition, myPosition - otherPosition );

					if( sqareDistance < (otherSphere->radius + mySphere->radius) * (otherSphere->radius + mySphere->radius) )
					{
						// Collision detected!
						// move myself away from collision. If the other entity is static, move the entire overlap away from the entity,
						// otherwise move half the distance as to achieve mutual collision resolution

						// if objects are on top of eachother
						if( sqareDistance == 0 )
						{
							*reinterpret_cast<glm::vec3*>(wpc->position) += glm::vec3( 0.01f, 0.0f, 0.0f );
							myPosition = *reinterpret_cast<glm::vec3*>(wpc->position);
						}

						glm::vec3 collisionNormal = glm::normalize( myPosition - otherPosition );
					
						// check if entity should step aside to get around object...
						Core::MovementComponent* mvmc = WGETC<Core::MovementComponent>(*it);
						if( glm::dot( collisionNormal, *reinterpret_cast<glm::vec3*>(mvmc->direction) ) < -0.999 )
						{							
								glm::vec3 moveDir = glm::normalize( glm::cross( collisionNormal, glm::vec3( 0.0f, 1.0f, 0.0f ) ));
								*reinterpret_cast<glm::vec3*>(wpc->position) += moveDir * 0.1f;
							
								// update data
								myPosition = *reinterpret_cast<glm::vec3*>(wpc->position);
								collisionNormal = glm::normalize( myPosition - otherPosition );
								sqareDistance = glm::dot( myPosition - otherPosition, myPosition - otherPosition );
						}
					
						float delta = ((otherSphere->radius + mySphere->radius) - std::sqrt( sqareDistance ));

						Core::UnitTypeComponent* otherType = WGETC<Core::UnitTypeComponent>(*other);

						switch( bvcOther->collisionModel )
						{
						case Core::BoundingVolumeCollisionModel::DynamicResolution:
						
							// Head of list will always bow for tail of list in perfect frontal collision.
							//*myPosition		+= collisionNormal * ( delta * 0.66666666f);

							if( otherType->type == Core::UnitType::Rioter )
							{
								*reinterpret_cast<glm::vec3*>(wpc->position)		+= collisionNormal * ( delta * 0.2f );
								*reinterpret_cast<glm::vec3*>(wpcOther->position)	-= collisionNormal * ( delta * 0.8f );
							}
							else 
							{
								// Head and tail is equal when in perfect frontal collision. Would theoretically results in 
								// less flow but potentially better crowd dynamics when in a closed environment. 
								*reinterpret_cast<glm::vec3*>(wpc->position)		+= collisionNormal * ( delta * 0.5f );
								*reinterpret_cast<glm::vec3*>(wpcOther->position)	-= collisionNormal * ( delta * 0.5f );
							}
							break;

						case Core::BoundingVolumeCollisionModel::StaticResolution:
							*reinterpret_cast<glm::vec3*>(wpc->position) += collisionNormal * delta;
							break;

						default:
							break;
						}
					}
				}
			}
		}

	} // end police collision check

}
