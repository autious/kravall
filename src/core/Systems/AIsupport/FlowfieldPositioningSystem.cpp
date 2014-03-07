#include "FlowfieldPositioningSystem.hpp"
#include "World.hpp"
#include <logger/Logger.hpp>

#include <gfx/GFXInterface.hpp>

Core::FlowfieldPositioningSystem::FlowfieldPositioningSystem()
	: BaseSystem( EntityHandler::GenerateAspect<
		WorldPositionComponent, FlowfieldComponent >(), 0ULL )
{
}

//#define Draw_line_to_parent_node

#ifdef Draw_line_to_parent_node
#define DRAW_PARENT_NODE( x ) x
#else
#define DRAW_PARENT_NODE( x ) ;
#endif

void Core::FlowfieldPositioningSystem::Update( float delta )
{
	Core::NavigationMesh* instance = Core::GetNavigationMesh();
	if( instance )
	{
		for( std::vector<Entity>::iterator it = m_entities.begin(); it != m_entities.end(); it++ )
		{
			Core::WorldPositionComponent* wpc = WGETC<Core::WorldPositionComponent>(*it);
			Core::FlowfieldComponent* ffc = WGETC<Core::FlowfieldComponent>(*it);

			glm::vec3 position = *reinterpret_cast<glm::vec3*>(wpc->position);
			
			// has node assigned...
			if( ffc->node >= 0 )
			{
				// debug
				DRAW_PARENT_NODE(
				float* points = instance->nodes[ffc->node].points;
				glm::vec3 temp = glm::vec3(0.0f);
				int nrLines = instance->nodes[ ffc->node ].corners[3].length < 0 ? 3 : 4;
				for( int g = 0; g < nrLines; g++ )
					temp += glm::vec3( points[ g * 2 ], 0.0f, points[ g * 2 + 1] );
				temp /= nrLines;
				GFX::Debug::DrawLine( position, temp, GFXColor( 1, 1.0f, 0, 1 ), false ); )


				// check if still inside node...
				if( instance->CheckPointInsideNode( position, ffc->node ) )
					continue;

				// entity is not in assigned node anymore, check neighbours...
				else
				{
					int i;
					for( i = 0; i < 4; i++ )
					{
						if( instance->nodes[ffc->node].corners[i].linksTo < 0 )
							continue;

						if( instance->CheckPointInsideNode( position, instance->nodes[ffc->node].corners[i].linksTo ) )
						{
							// assign new node...
							ffc->node = instance->nodes[ffc->node].corners[i].linksTo;
							break;
						}
					}

					// check if we found a new node
					if( i < 4 )
						continue;
				}
			}

			// if not assigned to a node, loop through all of them.
			if( ffc->node < 0 )
			{
				for( int i = 0; i < instance->nrNodes; i++ )
				{
					if( instance->CheckPointInsideNode( position, i ) )
					{
						ffc->node = i;
						break;
					}
				}
			}
		}
	}

}
