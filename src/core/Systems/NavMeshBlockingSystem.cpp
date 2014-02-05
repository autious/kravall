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
/*
void Core::NavMeshBlockingSystem::Update( float delta )
{
	Core::NavigationMesh* instance = Core::GetNavigationMesh();
	if( instance )
	{
		// reset data...
		for( int i = 0; i < instance->nrNodes; i++ )
		{
			instance->nodes[i].heat = 0.0f;
			for( int p = 0; p < 4; p++ )
			{
				instance->nodes[i].blocked[p] = false;
			}
		}

		for( std::vector<Entity>::iterator it = m_entities.begin(); it != m_entities.end(); it++ )
		{
			UnitTypeComponent* utc = WGETC<UnitTypeComponent>(*it);
			if( utc->type == Core::UnitType::Police )
			{
				Core::FlowfieldComponent* ffc = WGETC<Core::FlowfieldComponent>(*it);
				if( ffc->node < 0 )
					continue;

				instance->nodes[ffc->node].heat += 1.0f;
			}
		}




		for( int i = 0; i < instance->nrNodes; i++ )
		{
			if( instance->nodes[i].heat > 5.0f )
			{

				std::memset( instance->nodes[i].blocked, true, sizeof(bool) * 4 );

				//for( int t = 0; t < 4; t++ )
					//instance->nodes[i].blocked[t] = true;


				float* points = instance->nodes[i].points;
				glm::vec3 temp = glm::vec3(0.0f);
				for( int g = 0; g < 4; g++ )
					temp += glm::vec3( points[ g * 2 ], 0.0f, points[ g * 2 + 1] );
				temp *= 0.25f;
				GFX::Debug::DrawSphere( temp, instance->nodes[i].heat, GFXColor( 0.5f, 1.0f, 0.2f, 1.0f ), false );
			}
		}
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
	
}
*/

void Core::NavMeshBlockingSystem::CalculateBlockedNodes( int targetRioterGroup )
{
	Core::NavigationMesh* instance = Core::GetNavigationMesh();
	if( instance )
	{
		for( std::vector<Entity>::iterator it = m_entities.begin(); it != m_entities.end(); it++ )
		{
			UnitTypeComponent* utc = WGETC<UnitTypeComponent>(*it);
			if( utc->type == Core::UnitType::Police )
			{
				Core::FlowfieldComponent* ffc = WGETC<Core::FlowfieldComponent>(*it);
				if( ffc->node < 0 )
					continue;

				Core::WorldPositionComponent* wpc = WGETC<Core::WorldPositionComponent>(*it);
				glm::vec3 position = Core::WorldPositionComponent::GetVec3( *wpc );

				Core::AttributeComponent* attribc = WGETC<Core::AttributeComponent>(*it);

				float* points = instance->nodes[ ffc->node ].points;
				int nrCorners = instance->nodes[ ffc->node ].corners[3].length < 0.0f ? 3 : 4;
				for( int i = 0; i < nrCorners; i++ )
				{
					int ii = i * 2;
					int oo = (ii + 2) % 8;	

					// define lines...
					glm::vec3 lineStart = glm::vec3( points[ ii ], 0.0f, points[ ii + 1 ] );
					glm::vec3 lineEnd	= glm::vec3( points[ oo ], 0.0f, points[ oo + 1 ] );
					glm::vec3 fromStartToObject = position - lineStart;

					glm::vec3 cross = glm::normalize( glm::cross( (lineEnd - lineStart), glm::vec3( 0.0f, 1.0f, 0.0f ) ) );
					float distanceToLine = glm::dot( cross, fromStartToObject );

					instance->flowfields[ targetRioterGroup ].blocked[ ffc->node * 4 + i ] += 5.0f / ( distanceToLine * distanceToLine + 1 );
				}
			}
		}

		for( int i = 0; i < instance->nrNodes; i++ )
		{
			for( int pp = 0; pp < 4; pp++ )
			{
				if( instance->flowfields[ targetRioterGroup ].blocked[ i * 4 + pp ] > 5.0f )
				//if( instance->nodes[i].blocked[pp] > 5.0f )
				{
					int ii = pp * 2;
					int oo = (ii + 2) % 8;	
					float* points = instance->nodes[ i ].points;
					// define lines...
					glm::vec3 lineStart = glm::vec3( points[ ii ], 0.0f, points[ ii + 1 ] );
					glm::vec3 lineEnd	= glm::vec3( points[ oo ], 0.0f, points[ oo + 1 ] );
		
					GFX::Debug::DrawSphere( lineStart + ( lineEnd - lineStart ) * 0.5f, 
						instance->flowfields[ targetRioterGroup ].blocked[ i * 4 + pp ], GFXColor( 0.5f, 1.0f, 0.2f, 1.0f ), false );


					//float* points = instance->nodes[i].points;
					//glm::vec3 temp = glm::vec3(0.0f);
					//for( int g = 0; g < 4; g++ )
					//	temp += glm::vec3( points[ g * 2 ], 0.0f, points[ g * 2 + 1] );
					//temp *= 0.25f;
					//GFX::Debug::DrawSphere( temp, 5, GFXColor( 0.5f, 1.0f, 0.2f, 1.0f ), false );
				}
			}
		}


	}
}


void Core::NavMeshBlockingSystem::FreeBlockedNodes( int targetRioterGroup )
{
	Core::NavigationMesh* instance = Core::GetNavigationMesh();
	if( instance )
	{
		std::memset( instance->flowfields[ targetRioterGroup ].blocked, 0, instance->nrNodes * 4 * sizeof( float ) );
		//for( int i = 0; i < instance->nrNodes; i++ )
		//{
		//	std::memset( instance->nodes[i].blocked, 0, sizeof(float) * 4 );
		//
		//}
	}
}


void Core::NavMeshBlockingSystem::Update( float delta )
{
	Core::NavigationMesh* instance = Core::GetNavigationMesh();
	if( instance )
	{
		for( int i = 0; i < instance->nrUsedFlowfields; i++ )
		{			
			// currently, police squads will never recieve a ff goal...
			if( instance->flowfields[i].goal[0] != std::numeric_limits<float>::max() )
			{
				Core::NavigationMesh::Flowfield& meta = instance->flowfields[i];

				if( meta.timeSinceLastCheck > 4.0f )
				{
					meta.timeSinceLastCheck = 0.0f;

					glm::vec3 currentPosition = Core::world.m_systemHandler.GetSystem<Core::GroupDataSystem>()->GetMedianPosition(i);
					glm::vec3 lastPosition = glm::vec3( meta.recordedPosition[0], 0.0f, meta.recordedPosition[1] );

					GFX::Debug::DrawSphere( currentPosition, 4.0f, GFXColor( 0, 0, 1, 1 ), false );
					GFX::Debug::DrawLine( currentPosition, lastPosition, GFXColor( 1, 0, 1, 1 ), false );

					if( glm::distance( currentPosition, lastPosition ) < 3.0f )
					{
						// squad appears to be stuck...
						FreeBlockedNodes( i );
						CalculateBlockedNodes( i );
						instance->CalculateFlowfieldForGroup( glm::vec3( meta.goal[0], 0.0f, meta.goal[1] ), i  );					
					}

					meta.recordedPosition[0] = currentPosition.x;
					meta.recordedPosition[1] = currentPosition.z;
				}
				else if( meta.timeSinceLastCheck < 0 )
				{
					// init first recording...
					glm::vec3 currentPosition = Core::world.m_systemHandler.GetSystem<Core::GroupDataSystem>()->GetMedianPosition(i);
					meta.recordedPosition[0] = currentPosition.x;
					meta.recordedPosition[1] = currentPosition.z;
				}

				meta.timeSinceLastCheck += delta;
			}
		}
	}
	



}