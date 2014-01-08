#include "FlowfieldSystem.hpp"
#include "World.hpp"
#include <logger/Logger.hpp>

#include <gfx/GFXInterface.hpp>


Core::FlowfieldSystem::FlowfieldSystem()
	: BaseSystem( EntityHandler::GenerateAspect<
		WorldPositionComponent, BoundingVolumeComponent, UnitTypeComponent, AttributeComponent >(), 0ULL )
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
	Core::NavigationMesh* instance = Core::GetNavigationMesh();
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

				// check vs line
				for( int i = 0; i < 4; i++ )
				{
					if( instance->nodes[p].corners[i].length < 0 || instance->nodes[p].corners[i].linksTo >= 0 )
						continue;

					// fix indices for current line... 
					int ii = i * 2;
					int oo = (ii + 2) % 8;	

					// define lines...
					glm::vec3 lineStart = glm::vec3( points[ ii ], 0.0f, points[ ii + 1 ] );
					glm::vec3 lineEnd	= glm::vec3( points[ oo ], 0.0f, points[ oo + 1 ] );
					glm::vec3 fromStartToObject = position + *reinterpret_cast<glm::vec3*>( sphere.offset ) - lineStart;
					
					// check if outside line limits...
					float distanceAlongLine = glm::dot( (lineEnd - lineStart) * instance->nodes[p].corners[i].inverseLength, fromStartToObject );

					if( instance->nodes[p].corners[i].length < distanceAlongLine || distanceAlongLine < 0 )
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
					if( instance->nodes[p].corners[i].linksTo < -1.5f )
						continue;

					int ii = i * 2;
					glm::vec3 lineStart = glm::vec3( points[ ii ], 0.0f, points[ ii + 1 ] );
					float sqDistanceToCorner = 
						glm::dot( position + *reinterpret_cast<glm::vec3*>( sphere.offset ) 
						- lineStart, position + *reinterpret_cast<glm::vec3*>( sphere.offset ) - lineStart );

					if( sqDistanceToCorner < sphere.radius * sphere.radius )
					{
						position += glm::normalize( position + *reinterpret_cast<glm::vec3*>( sphere.offset ) - lineStart ) 
							* (sphere.radius - std::sqrt( sqDistanceToCorner ));
					}
				}

				#define FF_VS_PF_FACTOR 1.0f

				if( instance->CheckPointInsideNode( glm::vec3(position), p ) )
				{
					Core::MovementComponent* mvmc = WGETC<Core::MovementComponent>(*it);
					glm::vec3 temp = instance->flowfields[0].list[ p ];
					if( glm::dot( temp, temp ) > 0.05f ) // goal node condition...
					{
						Core::AttributeComponent* attribc = WGETC<Core::AttributeComponent>(*it);

						*reinterpret_cast<glm::vec3*>(mvmc->direction) = glm::normalize( glm::normalize( instance->flowfields[attribc->rioter.groupID].list[ p ] - position ) * FF_VS_PF_FACTOR + *reinterpret_cast<glm::vec3*>(mvmc->direction) );
						GFX::Debug::DrawLine( position, position + *reinterpret_cast<glm::vec3*>(mvmc->direction), GFXColor( 1.0f, 0.0f, 0.0f, 1.0f ), false );
					}
				}
			}
		}







		// collision only for current 'and adjacent' nodes
			// save current node in agents

		// calculate flowfield
			// save current group in agent
			// reitterate on flowfield vector behaviour...
			
		// assign vector of all agents to base flowfield...

	}
}
