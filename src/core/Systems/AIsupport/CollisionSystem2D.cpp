#include "CollisionSystem2D.hpp"
#include "World.hpp"
#include <gfx/GFXInterface.hpp>
#include <logger/Logger.hpp>
#include <algorithm>
#include <DebugMacros.hpp>

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

static bool sortCullData( const cullData& me, const cullData& other )
{
	if( me.x == other.x )
		return me.z < other.z;
	return me.x < other.x;
}

void DrawCullingQuads( cullData* cullingList, int nrEntities, int gridNodeSize )
{
	for( int i = 0; i < nrEntities; i++ )
	{
		Core::WorldPositionComponent* wpc = WGETC<Core::WorldPositionComponent>(cullingList[i].ent);
		GFX::Debug::DrawSphere( glm::vec3(wpc->position[0], 0, wpc->position[2] ), 0.05f, GFXColor( 1, 0, 0, 1 ), false );

		int x = cullingList[i].x;
		int y = cullingList[i].z;
		GFX::Debug::DrawBox( 
			glm::vec3( x * gridNodeSize + gridNodeSize * 0.5f, 0, y * gridNodeSize + gridNodeSize * 0.5f ), glm::vec3( gridNodeSize, 0, gridNodeSize ), false, GFXColor( 1, 0, 0, 1 ), false );
	}
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

	float invMaxDoubleRadius = 1.0f / gridNodeSize;
	cullData* cullingList = Core::world.m_frameHeap.NewPODArray<cullData>( nrEntities );

	int cullIndex = 0;
	for( std::vector<Entity>::iterator it = m_entities.begin(); it != m_entities.end(); it++, cullIndex++ )
	{
		Core::WorldPositionComponent* wpc = WGETC<Core::WorldPositionComponent>(*it);

		cullingList[cullIndex].x = (int)(( wpc->position[0] / gridNodeSize ) + (wpc->position[0] > 0 ?  0 : -1));
		cullingList[cullIndex].z = (int)(( wpc->position[2] / gridNodeSize ) + (wpc->position[2] > 0 ?  0 : -1));

		cullingList[cullIndex].ent = *it;
	}

	std::sort( cullingList, cullingList + nrEntities, sortCullData );
	

	// debug draw culling quads....
	#ifdef SHOW_2D_COLLISION_CULLING_QUADS
	DrawCullingQuads( cullingList, nrEntities, gridNodeSize );
	#endif

	Core::Entity* toCheck			= Core::world.m_frameHeap.NewPODArray<Core::Entity>( nrEntities );
	Core::Entity* toCheckAgainst	= Core::world.m_frameHeap.NewPODArray<Core::Entity>( nrEntities );

	int currentX = cullingList[0].x;
	int currentZ = cullingList[0].z;

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
#ifdef SHOW_2D_COLLISION_LINE_BETWEEN_COLLISIONS
					GFX::Debug::DrawLine( myPosition, otherPosition, GFXColor( 1.0f, 1.0f, 0.0f, 1.0f ), false );
#endif

					// if objects are on top of eachother
					if( sqareDistance == 0 )
					{
						*reinterpret_cast<glm::vec3*>(wpc->position) += glm::vec3( 0.01f, 0.0f, 0.0f );
						myPosition = *reinterpret_cast<glm::vec3*>(wpc->position);
					}
					
					glm::vec3 collisionNormal = glm::normalize( myPosition - otherPosition );
					
					float deltaDist = ((otherSphere->radius + mySphere->radius) - std::sqrt( sqareDistance ));
					glm::vec3 movement;

					float mod = 0.5f;

					Core::MovementComponent* mvmc = WGETC<Core::MovementComponent>(it);
					Core::MovementComponent* mvmcOther = WGETC<Core::MovementComponent>(other);
					if( mvmc != nullptr || mvmcOther != nullptr )
					{
						float dot = glm::dot( glm::vec3( mvmc->direction[0], mvmc->direction[1], mvmc->direction[2] ), 
							glm::normalize( otherPosition - myPosition ) );
						if( dot > 0.3f )
						{	
							if( mvmc->speed > mvmc->desiredSpeed[ Core::MovementState::Movement_Walking ] * 0.8f )
								mvmc->speed -= 18.85f * delta;

							//mod = 0.75f;
						}
					}

					// move myself away from collision. If the other entity is static, move the entire overlap away from the entity,
					// otherwise move half the distance as to achieve mutual collision resolution
					bool otherIsAttacking = false;
					TargetingComponent* otherTc = WGETC<Core::TargetingComponent>(other);

					switch( bvcOther->collisionModel )
					{
					case Core::BoundingVolumeCollisionModel::DynamicResolution:

						{
							// Head of list will always bow for tail of list in perfect frontal collision.
							{
								movement = glm::vec3(0);
								movement += collisionNormal * ( deltaDist * mod );
								wpc->position[0] += movement.x;
								wpc->position[2] += movement.z;
							}

							// Head and tail is equal when in perfect frontal collision. Would theoretically results in 
							// less flow but potentially better crowd dynamics when in a closed environment. 
							{
								movement = glm::vec3(0);
								movement -= collisionNormal * ( deltaDist * ( 1.0f - mod ) );
								wpcOther->position[0] += movement.x;
								wpcOther->position[2] += movement.z;
							}

							// update own position for following tests...
							myPosition = glm::vec3( wpc->position[0], 0.0f, wpc->position[2] );
						}

						break;

					case Core::BoundingVolumeCollisionModel::StaticResolution:
						movement = glm::vec3(0);
						movement += collisionNormal * ( deltaDist * 1.0f );
						wpc->position[0] += movement.x;
						wpc->position[2] += movement.z;

						// update own position for following tests...
						myPosition = glm::vec3( wpc->position[0], 0.0f, wpc->position[2] );

						break;

					default:
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
