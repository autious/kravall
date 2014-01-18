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
	int z;
	Core::Entity ent;
};

bool sortCullData( cullData& me, cullData& other )
{
	if( me.x == other.x )
		return me.z < other.z;
	return me.x < other.x;
}


void Core::CollisionSystem2D::Update( float delta )
{
	
	int nrEntities = m_entities.size();

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
	
	if( maxRadius == 0.0f )
		return;
	
	float gridNodeSize = 4.0f;

	//float invMaxDoubleRadius = 1.0f / (2.0f * maxRadius);
	float invMaxDoubleRadius = 1.0f / gridNodeSize;
	cullData* cullingList = Core::world.m_frameHeap.NewPODArray<cullData>( nrEntities );

	int cullIndex = 0;
	for( std::vector<Entity>::iterator it = m_entities.begin(); it != m_entities.end(); it++, cullIndex++ )
	{
		Core::WorldPositionComponent* wpc = WGETC<Core::WorldPositionComponent>(*it);

		cullingList[cullIndex].x = ( wpc->position[0] / gridNodeSize ) + (wpc->position[0] > 0 ?  0 : -1);
		cullingList[cullIndex].z = ( wpc->position[2] / gridNodeSize ) + (wpc->position[2] > 0 ?  0 : -1);

		cullingList[cullIndex].ent = *it;
	}

	std::sort( cullingList, cullingList + nrEntities, sortCullData );
	

	// debug draw culling quads....
	//for( int i = 0; i < nrEntities; i++)
	//{
	//	Core::WorldPositionComponent* wpc = WGETC<Core::WorldPositionComponent>(cullingList[i].ent);
	//	//GFX::Debug::DrawSphere( glm::vec3(wpc->position[0], 0, wpc->position[2] ), 0.1f, GFXColor( 1, 0, 0, 1 ), false );
	//
	//	int x = cullingList[i].x;
	//	int y = cullingList[i].z;
	//	GFX::Debug::DrawBox( 
	//		//glm::vec3( x * (2.0f * maxRadius) + maxRadius, 0, y * (2.0f * maxRadius) + maxRadius ), glm::vec3( 2.0f * maxRadius, 0, 2.0f * maxRadius ), false, GFXColor( 1, 0, 0, 1 ), false );
	//		glm::vec3( x * gridNodeSize + gridNodeSize * 0.5f, 0, y * gridNodeSize + gridNodeSize * 0.5f ), glm::vec3( gridNodeSize, 0, gridNodeSize ), false, GFXColor( 1, 0, 0, 1 ), false );
	//}

	Core::Entity* toCheck			= Core::world.m_frameHeap.NewPODArray<Core::Entity>( nrEntities );
	Core::Entity* toCheckAgainst	= Core::world.m_frameHeap.NewPODArray<Core::Entity>( nrEntities );

	int currentX = cullingList[0].x;
	int currentZ = cullingList[0].z;

	int nextIndex = 0;

	cullData data[500];
	std::memcpy( data, cullingList, nrEntities * sizeof(cullData) );

	while( true )
	{
		// rig current pass...
		int head = 0; // head for toCheck
		int head2 = 0; // head for toCheckAgainst

		int next = 0;

		for( int i = 0; i < nrEntities; i++ )
		{
			int xValue = currentX - cullingList[i].x + 2;
			int yValue = currentZ - cullingList[i].z + 2;

			if( xValue < 4 && xValue > 0 && yValue < 4 && yValue > 0 )
			{
				toCheckAgainst[head2] = cullingList[i].ent;
				head2++;

				if( cullingList[i].x == currentX && cullingList[i].z == currentZ ) // fuckedy bleh...
				{
					toCheck[head] = cullingList[i].ent;
					head++;
				}
			}

			// find next currentX value
			if( ((cullingList[i].x == currentX && cullingList[i].z > currentZ) || ( cullingList[i].x > currentX ) ) && !next )
				next = i;
		}


		// do current pass...
		for( int i = 0; i < head; i++ )
		{
			Core::Entity it = toCheck[i];
			Core::BoundingVolumeComponent* bvc = WGETC<Core::BoundingVolumeComponent>(it);
			
			// only dynamic enteties checks for collisions...
			if( bvc->type != Core::BoundingVolumeType::SphereBoundingType
				|| bvc->collisionModel == Core::BoundingVolumeCollisionModel::NoResolution
				|| bvc->collisionModel == Core::BoundingVolumeCollisionModel::StaticResolution )
				continue;

			// own data...
			Core::WorldPositionComponent* wpc = WGETC<Core::WorldPositionComponent>(it);
			glm::vec3 myPosition = *reinterpret_cast<glm::vec3*>(wpc->position);
			myPosition.y = 0.0f; // collision in XZ plane only
			Core::BoundingSphere* mySphere = reinterpret_cast<Core::BoundingSphere*>( bvc->data );
			

			for( int p = 0; p < head2; p++ )
			{
				Core::Entity other = toCheckAgainst[p];
				if( it == other )
					continue;
			
				Core::WorldPositionComponent* wpcOther = WGETC<Core::WorldPositionComponent>(other);
				glm::vec3 otherPosition = *reinterpret_cast<glm::vec3*>(wpcOther->position);
				otherPosition.y = 0.0f; // collision in XZ plane only

				float sqareDistance = glm::dot( myPosition - otherPosition, myPosition - otherPosition );

				Core::BoundingVolumeComponent* bvcOther = WGETC<Core::BoundingVolumeComponent>(other);			
				if ( bvcOther->collisionModel == Core::BoundingVolumeCollisionModel::NoResolution || bvcOther->type != Core::BoundingVolumeType::SphereBoundingType )
					continue;

				Core::BoundingSphere* otherSphere = reinterpret_cast<Core::BoundingSphere*>( bvcOther->data );
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
				
					//// check if entity should step aside to get around object...
					//if( glm::dot( collisionNormal, *reinterpret_cast<glm::vec3*>(mvmc->direction) ) < -0.999 )
					//{							
					//		glm::vec3 moveDir = glm::normalize( glm::cross( collisionNormal, glm::vec3( 0.0f, 1.0f, 0.0f ) ));
					//		*reinterpret_cast<glm::vec3*>(wpc->position) += moveDir * 0.1f;
					//		
					//		// update data
					//		myPosition = *reinterpret_cast<glm::vec3*>(wpc->position);
					//		collisionNormal = glm::normalize( myPosition - otherPosition );
					//		sqareDistance = glm::dot( myPosition - otherPosition, myPosition - otherPosition );
					//}
					
					float deltaDist = ((otherSphere->radius + mySphere->radius) - std::sqrt( sqareDistance ));
					glm::vec3 movement;

					switch( bvcOther->collisionModel )
					{
					case Core::BoundingVolumeCollisionModel::DynamicResolution:
						
						// Head of list will always bow for tail of list in perfect frontal collision.
						//*myPosition		+= collisionNormal * ( delta * 0.66666666f);

						// Head and tail is equal when in perfect frontal collision. Would theoretically results in 
						// less flow but potentially better crowd dynamics when in a closed environment. 
						*reinterpret_cast<glm::vec3*>(wpc->position)		+= collisionNormal * ( deltaDist * 0.5f );
						*reinterpret_cast<glm::vec3*>(wpcOther->position)	-= collisionNormal * ( deltaDist * 0.5f );

						// update own position for following tests...
						myPosition = *reinterpret_cast<glm::vec3*>(wpc->position);
						myPosition.y = 0.0f; // collision in XZ plane only

						//movement = glm::vec3(0);
						//movement += collisionNormal * ( delta * 0.5f );
						//wpc->position[0] += movement.x;
						//wpc->position[2] += movement.z;
						//
						//movement = glm::vec3(0);
						//movement -= collisionNormal * ( delta * 0.5f );
						//wpcOther->position[0] += movement.x;
						//wpcOther->position[2] += movement.z;

						break;

					case Core::BoundingVolumeCollisionModel::StaticResolution:
						//*reinterpret_cast<glm::vec3*>(wpc->position) += collisionNormal * deltaDist;
						movement = glm::vec3(0);
						movement += collisionNormal * ( deltaDist * 1.0f );
						wpc->position[0] += movement.x;
						wpc->position[2] += movement.z;


						break;

					default:
						GFX::Debug::DrawSphere( myPosition, 10.0f, GFXColor( 1.0f, 0.0f, 0.0f, 1.0f ), false );
						GFX::Debug::DrawSphere( otherPosition, 10.0f, GFXColor( 1.0f, 0.0f, 0.0f, 1.0f ), false );
						break;
					}
				}
			}
		}


		// exit condition
		if( !next )
			break;
		
		// prep next pass
		currentX = cullingList[next].x;
		currentZ = cullingList[next].z;
	}
}
