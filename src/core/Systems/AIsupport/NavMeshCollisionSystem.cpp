#include "NavMeshCollisionSystem.hpp"
#include "World.hpp"
#include <logger/Logger.hpp>

#include <gfx/GFXInterface.hpp>


Core::NavMeshCollisionSystem::NavMeshCollisionSystem()
	: BaseSystem( EntityHandler::GenerateAspect<
		WorldPositionComponent, BoundingVolumeComponent, FlowfieldComponent >(), 0ULL )
{
}

//#define Draw_DEBUG_LINES_TO_SHOW_COLLIDING_EDGES
#ifdef Draw_DEBUG_LINES_TO_SHOW_COLLIDING_EDGES
#define DEBUG_ELIGABLE_EDGES_FOR_COLLISION( x ) x
#else
#define DEBUG_ELIGABLE_EDGES_FOR_COLLISION( x ) ;
#endif



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

			// check vs corners
			for( int i = 0; i < 4; i++ )
			{
				// check for last corner of triangle
				if( instance->nodes[ffc->node].corners[i].linksTo < -1.5f || instance->nodes[ ffc->node ].corners[i].cornerConnectsToNode[0] < 0 )
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

			// check vs line
			int nrEdges = instance->nodes[ffc->node].corners[3].length < 0 ? 3 : 4;

			// check vs. own node lines first...
			for( int i = 0; i < nrEdges; i++ )
			{
				DEBUG_ELIGABLE_EDGES_FOR_COLLISION( 
				for( int pp = 0; pp < 2; pp++ )
				{

					int collisionNode = instance->nodes[ffc->node].corners[i].cornerConnectsToNode[pp];
					int collisionCorner = instance->nodes[ffc->node].corners[i].cornerConnectsToCorner[pp];

					if( collisionNode < 0 || instance->nodes[collisionNode].corners[collisionCorner].linksTo >= 0 )
						continue;

					// debug
					glm::vec3 LineToCollide = instance->nodes[collisionNode].GetMidPoint( collisionCorner );
					
					// debug
					float* points = instance->nodes[ffc->node].points;
					glm::vec3 temp = glm::vec3(0.0f);
					int nrLines = instance->nodes[ ffc->node ].corners[3].length < 0 ? 3 : 4;
					for( int g = 0; g < nrLines; g++ )
						temp += glm::vec3( points[ g * 2 ], 0.0f, points[ g * 2 + 1] );
					temp /= nrLines;

					GFX::Debug::DrawLine( LineToCollide, temp, GFXColor( 1, 0.2f, 1, 1 ), false );
				} );

				
				
				int collisionNode = ffc->node;
				int collisionCorner = i;

				if( collisionNode < 0 )
					continue;

				int ii = collisionCorner * 2;
				int oo = (ii + 2) % 8;	

				// define lines...
				float* verticies = instance->nodes[collisionNode].points;
				glm::vec3 lineStart = glm::vec3( verticies[ ii ], 0.0f, verticies[ ii + 1 ] );
				glm::vec3 lineEnd	= glm::vec3( verticies[ oo ], 0.0f, verticies[ oo + 1 ] );
				glm::vec3 fromStartToObject = position - lineStart;

				float distanceAlongLine = glm::dot( (lineEnd - lineStart) * instance->nodes[collisionNode].corners[collisionCorner].inverseLength, fromStartToObject );
				if( instance->nodes[collisionNode].corners[collisionCorner].length < distanceAlongLine || distanceAlongLine < 0 )
					continue;

				glm::vec3 cross = glm::normalize( glm::cross( (lineEnd - lineStart), glm::vec3( 0.0f, 1.0f, 0.0f ) ) );
				float distanceToLine = glm::dot( cross, fromStartToObject );
				
				if( distanceToLine < sphere.radius && instance->nodes[collisionNode].corners[collisionCorner].linksTo < 0 )
				{
					position += cross * (sphere.radius - distanceToLine);
				}

				if( distanceToLine < ffc->distance )
				{
					ffc->distance = distanceToLine;
					ffc->wallDirX = abs( cross.z ) < 0.7853981633f ? cross.x > 0 ? -1.0f : 1.0f : 0.0f;
					ffc->wallDirZ = abs( cross.x ) < 0.7853981633f ? cross.z > 0 ? 1.0f : -1.0f : 0.0f;
				}
			}

			bool insideNodeOrNeighbours = instance->CheckPointInsideNode( position, ffc->node);
			for( int i = 0; i < 4 && !insideNodeOrNeighbours; i++ )
			{
				if( instance->nodes[ ffc->node ].corners[ i ].linksTo >= 0 && instance->nodes[ ffc->node ].corners[ i ].length > 0.0f )
					insideNodeOrNeighbours = instance->CheckPointInsideNode( position, instance->nodes[ ffc->node ].corners[ i ].linksTo );
			}


			// check vs. adjacent lines as well...
			if( !insideNodeOrNeighbours )
			{
				for( int i = 0; i < nrEdges; i++ )
				{				
					for( int pp = 0; pp < 2; pp++ )
					{
						int collisionNode = instance->nodes[ffc->node].corners[i].cornerConnectsToNode[pp];
						int collisionCorner = instance->nodes[ffc->node].corners[i].cornerConnectsToCorner[pp];

						if( collisionNode < 0 || instance->nodes[collisionNode].corners[collisionCorner].linksTo >= 0 )
							continue;

						int ii = collisionCorner * 2;
						int oo = (ii + 2) % 8;	

						// define lines...
						float* verticies = instance->nodes[collisionNode].points;
						glm::vec3 lineStart = glm::vec3( verticies[ ii ], 0.0f, verticies[ ii + 1 ] );
						glm::vec3 lineEnd	= glm::vec3( verticies[ oo ], 0.0f, verticies[ oo + 1 ] );
						glm::vec3 fromStartToObject = position - lineStart;

						float distanceAlongLine = glm::dot( (lineEnd - lineStart) * instance->nodes[collisionNode].corners[collisionCorner].inverseLength, fromStartToObject );
						if( instance->nodes[collisionNode].corners[collisionCorner].length < distanceAlongLine || distanceAlongLine < 0 )
							continue;

						glm::vec3 cross = glm::normalize( glm::cross( (lineEnd - lineStart), glm::vec3( 0.0f, 1.0f, 0.0f ) ) );
						float distanceToLine = glm::dot( cross, fromStartToObject );
				
						if( distanceToLine < sphere.radius )
							position += cross * (sphere.radius - distanceToLine);
					}
				}
			}

			// pf utility...
			#define WallCurDeclineVal 1.0f
			if( ffc->distance == std::numeric_limits<float>::max() )
				ffc->distance = 0.0f;
			else
				ffc->distance = WallCurDeclineVal / ( WallCurDeclineVal + ffc->distance * ffc->distance );
		}
	}
}
