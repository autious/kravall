#include "NavMeshBlockingSystem.hpp"
#include "World.hpp"
#include <logger/Logger.hpp>

#include <SystemDef.hpp>
#include <gfx/GFXInterface.hpp>


Core::NavMeshBlockingSystem::NavMeshBlockingSystem()
	: BaseSystem( EntityHandler::GenerateAspect<
		WorldPositionComponent, UnitTypeComponent, AttributeComponent, FlowfieldComponent >(), 0ULL )
{
}

void Core::NavMeshBlockingSystem::Update( float delta )
{
	Core::NavigationMesh* instance = Core::GetNavigationMesh();
	if( instance )
	{





	}


	return; // implementation incomplete
	/*
	Core::NavigationMesh* instance = Core::GetNavigationMesh();
	if( instance )
	{
		for( int i = 0; i < instance->nrNodes; i++ )
		{
			// count number of police in this node...
			int nrInThisNode = 0;
			for( std::vector<Entity>::iterator it = m_entities.begin(); it != m_entities.end(); it++ )
			{
				UnitTypeComponent* utc = WGETC<UnitTypeComponent>(*it);
				if( utc->type == Core::UnitType::Police )
				{
					Core::FlowfieldComponent* ffc = WGETC<Core::FlowfieldComponent>(*it);
					if( ffc->node == i )
						nrInThisNode++;
				}
			}

			// block the node
			bool isBlocked = false;
			if( nrInThisNode > 5 )
				isBlocked = true;

			int nrCorners = instance->nodes[ i ].corners[3].length < -0.5f ? 3 : 4;
			for( int p = 0; p < nrCorners; p++ )
				instance->nodes[i].blocked[p] = isBlocked;


			// debug
			float* pp = instance->nodes[i].points;
			glm::vec2 mid = glm::vec2( pp[0], pp[1] );

			for( int p = 2; p < nrCorners * 2 - 2; p++ )
				mid += glm::vec2( pp[ p * 2 ], pp[ p * 2 + 1 ] ) * 0.5f;

			if( !instance->nodes[ i ].blocked[0] )
				GFX::Debug::DrawSphere( glm::vec3( mid.x, 0.0f, mid.y ), 3.0f, GFXColor( 0, 1, 1, 1 ), false );

		}
	}
	*/
}
