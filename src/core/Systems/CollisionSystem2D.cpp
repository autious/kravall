#include "CollisionSystem2D.hpp"
#include "World.hpp"
#include <gfx/GFXInterface.hpp>
#include <logger/Logger.hpp>
#include <algorithm>


Core::CollisionSystem2D::CollisionSystem2D()
	: BaseSystem( EntityHandler::GenerateAspect< WorldPositionComponent, BoundingVolumeComponent, MovementComponent >(), 0ULL )
{
}

struct cullData
{
	int x;
	int y;
	Core::Entity ent;
};

bool sortCullData( cullData& me, cullData& other )
{
	if( me.x == other.x )
		return me.y < other.y;
	return me.x < other.x;
}


void Core::CollisionSystem2D::Update( float delta )
{
	
	float maxRadius = 0.0f;
	for( std::vector<Entity>::iterator it = m_entities.begin(); it != m_entities.end(); it++ )
	{
		Core::BoundingVolumeComponent* bvc = WGETC<Core::BoundingVolumeComponent>(*it);
		if( bvc->type == Core::BoundingVolumeType::SphereBoundingType )
		{
			if( reinterpret_cast<Core::BoundingSphere*>(bvc->data)->radius > maxRadius )
				maxRadius = reinterpret_cast<Core::BoundingSphere*>(bvc->data)->radius;
		}
	}
	/*
	if( maxRadius == 0.0f )
		return;

	//LOG_DEBUG << maxRadius << std::endl;
	float invMaxDoubleRadius = 1.0f / (2.0f * maxRadius);
	cullData* cullingList = Core::world.m_frameHeap.NewPODArray<cullData>( m_entities.size() );

	int cullIndex = 0;
	for( std::vector<Entity>::iterator it = m_entities.begin(); it != m_entities.end(); it++, cullIndex++ )
	{
		Core::WorldPositionComponent* wpc = WGETC<Core::WorldPositionComponent>(*it);
		cullingList[cullIndex].x = wpc->position[0] * invMaxDoubleRadius;
		cullingList[cullIndex].y = wpc->position[2] * invMaxDoubleRadius;
		cullingList[cullIndex].ent = *it;
	}

	std::sort( cullingList, cullingList + m_entities.size(), sortCullData );

	//cullData temp[500];
	//std::memcpy( temp, cullingList, 500 * sizeof(cullData) );

	LOG_DEBUG << cullingList[cullIndex - 1].x << std::endl;
	LOG_DEBUG << cullingList[cullIndex - 1].y << std::endl;

	Core::Entity* toCheck			= Core::world.m_frameHeap.NewPODArray<Core::Entity>( m_entities.size() );
	Core::Entity* toCheckAgainst	= Core::world.m_frameHeap.NewPODArray<Core::Entity>( m_entities.size() );

	*/
	
	
	int totalcounter = 0;
	float doubleMaxRadius = maxRadius * 2.0f;
	int counter = 0;
	for( std::vector<Entity>::iterator it = m_entities.begin(); it != m_entities.end(); it++, counter++ )
	{
		Core::BoundingVolumeComponent* bvc = WGETC<Core::BoundingVolumeComponent>(*it);
			
		// only dynamic enteties checks for collisions...
		if( bvc->type != Core::BoundingVolumeType::SphereBoundingType 
			|| bvc->collisionModel == Core::BoundingVolumeCollisionModel::NoResolution
			|| bvc->collisionModel == Core::BoundingVolumeCollisionModel::StaticResolution )
			continue;

		// own data...
		Core::WorldPositionComponent* wpc = WGETC<Core::WorldPositionComponent>(*it);
		glm::vec3 myPosition = *reinterpret_cast<glm::vec3*>(wpc->position);
		myPosition.y = 0.0f; // collision in XZ plane only
		Core::BoundingSphere* mySphere = reinterpret_cast<Core::BoundingSphere*>( bvc->data );

		//for( std::vector<Entity>::iterator other = m_entities.begin() + counter; other != m_entities.end(); other++ )
		for( std::vector<Entity>::iterator other = m_entities.begin(); other != m_entities.end(); other++ )
		{
			if( *it == *other )
				continue;
			
			Core::WorldPositionComponent* wpcOther = WGETC<Core::WorldPositionComponent>(*other);
			glm::vec3 otherPosition = *reinterpret_cast<glm::vec3*>(wpcOther->position);
			otherPosition.y = 0.0f; // collision in XZ plane only

			float sqareDistance = glm::dot( myPosition - otherPosition, myPosition - otherPosition );
			if( sqareDistance > doubleMaxRadius )
				continue;

			Core::BoundingVolumeComponent* bvcOther = WGETC<Core::BoundingVolumeComponent>(*other);			
			if ( bvcOther->collisionModel == Core::BoundingVolumeCollisionModel::NoResolution || bvcOther->type != Core::BoundingVolumeType::SphereBoundingType )
				continue;

			totalcounter++;

			Core::BoundingSphere* otherSphere = reinterpret_cast<Core::BoundingSphere*>( bvcOther->data );
			if( sqareDistance < (otherSphere->radius + mySphere->radius) * (otherSphere->radius + mySphere->radius) )
			{
				//GFX::Debug::DrawSphere( myPosition, 1.0f, GFXColor( 0.0f, 0.0f, 1.0f, 1.0f ), false );
				//GFX::Debug::DrawSphere( otherPosition, 1.0f, GFXColor( 0.0f, 1.0f, 1.0f, 1.0f ), false );

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

				switch( bvcOther->collisionModel )
				{
				case Core::BoundingVolumeCollisionModel::DynamicResolution:
						
					// Head of list will always bow for tail of list in perfect frontal collision.
					//*myPosition		+= collisionNormal * ( delta * 0.66666666f);

					// Head and tail is equal when in perfect frontal collision. Would theoretically results in 
					// less flow but potentially better crowd dynamics when in a closed environment. 
					*reinterpret_cast<glm::vec3*>(wpc->position)		+= collisionNormal * ( delta * 0.5f );
					*reinterpret_cast<glm::vec3*>(wpcOther->position)	-= collisionNormal * ( delta * 0.5f );

					break;

				case Core::BoundingVolumeCollisionModel::StaticResolution:
					*reinterpret_cast<glm::vec3*>(wpc->position) += collisionNormal * delta;
					break;

				default:
					GFX::Debug::DrawSphere( myPosition, 10.0f, GFXColor( 1.0f, 0.0f, 0.0f, 1.0f ), false );
					GFX::Debug::DrawSphere( otherPosition, 10.0f, GFXColor( 1.0f, 0.0f, 0.0f, 1.0f ), false );
					break;
				}
			}			
		}
	}

	//LOG_DEBUG << totalcounter << std::endl;

	//int counter = 0;
	//for( std::vector<Entity>::iterator it = m_entities.begin(); it != m_entities.end(); it++ )
	//{
	//	for( std::vector<Entity>::iterator other = m_entities.begin(); other != m_entities.end(); other++ )
	//	{
	//		counter++;
	//	}
	//}
	//
	//LOG_DEBUG << counter << std::endl;

	/*
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
	*/



	/*
	int ListHead = 0;
	Core::Entity* culledList = Core::world.m_frameHeap.NewPODArray<Core::Entity>( m_entities.size() );

	int searchHead = 0;
	for( std::vector<Entity>::iterator it = m_entities.begin(); it != m_entities.end(); it++, searchHead++ )
	{
		Core::WorldPositionComponent* wpc = WGETC<Core::WorldPositionComponent>(*it);
		glm::vec3 myPosition = *reinterpret_cast<glm::vec3*>(wpc->position);
		myPosition.y = 0.0f; // collision in XZ plane only

		for( std::vector<Entity>::iterator  other = m_entities.begin() + searchHead + 1; other != m_entities.end(); other++ )
		{
			Core::WorldPositionComponent* wpcOther = WGETC<Core::WorldPositionComponent>(*other);
			glm::vec3 otherPosition = *reinterpret_cast<glm::vec3*>(wpcOther->position);
			otherPosition.y = 0.0f; // collision in XZ plane only

			float sqareDistance = glm::dot( myPosition - otherPosition, myPosition - otherPosition );
			if( sqareDistance <= maxRadius )
			{
				culledList[ListHead] = *it;
				ListHead++;
			}
		}
	}
	LOG_DEBUG << ListHead << std::endl;
	*/


	/*
	int counter = 0;
	int tmp = 0;
	for( std::vector<Entity>::iterator it = m_entities.begin(); it != m_entities.end(); it++, counter++ )
	{
		Core::BoundingVolumeComponent* bvc = WGETC<Core::BoundingVolumeComponent>(*it);
		Core::WorldPositionComponent* wpc = WGETC<Core::WorldPositionComponent>(*it);
		Core::MovementComponent* mvmc = WGETC<Core::MovementComponent>(*it);

		//tmp = (int)bvc * 0.1f + wpc->position[0] * 0.1f + (int)mvmc * 0.1f;

		for( std::vector<Entity>::iterator  other = m_entities.begin() + counter; other != m_entities.end(); other++ )
		{
			Core::BoundingVolumeComponent* bvcOther = WGETC<Core::BoundingVolumeComponent>(*other);			
			Core::WorldPositionComponent* wpcOther = WGETC<Core::WorldPositionComponent>(*other);
			tmp = (int)bvc * 0.1f + wpc->position[0] * 0.1f + (int)bvcOther * 0.1f + (int)mvmc * 0.1f + (int)wpcOther * 0.1f;
		}
	}
	
	LOG_DEBUG << tmp << std::endl;
	*/
}
