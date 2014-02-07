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

		//for( int i = 0; i < instance->nrNodes; i++ )
		//{
		//	for( int pp = 0; pp < 4; pp++ )
		//	{
		//		if( instance->flowfields[ targetRioterGroup ].blocked[ i * 4 + pp ] > 5.0f )
		//		{
		//			int ii = pp * 2;
		//			int oo = (ii + 2) % 8;	
		//			float* points = instance->nodes[ i ].points;
		//			glm::vec3 lineStart = glm::vec3( points[ ii ], 0.0f, points[ ii + 1 ] );
		//			glm::vec3 lineEnd	= glm::vec3( points[ oo ], 0.0f, points[ oo + 1 ] );
		//
		//			GFX::Debug::DrawSphere( lineStart + ( lineEnd - lineStart ) * 0.5f, 
		//				instance->flowfields[ targetRioterGroup ].blocked[ i * 4 + pp ], GFXColor( 0.5f, 1.0f, 0.2f, 1.0f ), false );
		//
		//
		//			//float* points = instance->nodes[i].points;
		//			//glm::vec3 temp = glm::vec3(0.0f);
		//			//for( int g = 0; g < 4; g++ )
		//			//	temp += glm::vec3( points[ g * 2 ], 0.0f, points[ g * 2 + 1] );
		//			//temp *= 0.25f;
		//			//GFX::Debug::DrawSphere( temp, 5, GFXColor( 0.5f, 1.0f, 0.2f, 1.0f ), false );
		//		}
		//	}
		//}


	}
}


void Core::NavMeshBlockingSystem::FreeBlockedNodes( int targetRioterGroup )
{
	Core::NavigationMesh* instance = Core::GetNavigationMesh();
	if( instance )
	{
		std::memset( instance->flowfields[ targetRioterGroup ].blocked, 0, instance->nrNodes * 4 * sizeof( float ) );
	}
}

#define GROUP_CHECK_STUCK_TIMER 5.0f
#define GROUP_IS_STUCK_DISTANCE 5.0f
#define GROUP_FIND_NEW_PATH_TIMER 10.0f

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

				glm::vec3 currentPosition = Core::world.m_systemHandler.GetSystem<Core::GroupDataSystem>()->GetMedianPosition(i);
				//GFX::Debug::DrawSphere( currentPosition, meta.stuckTimer + 2.0f, GFXColor( 0, 0, 1, 1 ), false );

				if( meta.timeSinceLastCheck > GROUP_CHECK_STUCK_TIMER )
				{
					meta.timeSinceLastCheck = 0.0f;

					
					glm::vec3 lastPosition = glm::vec3( meta.recordedPosition[0], 0.0f, meta.recordedPosition[1] );

					
					//GFX::Debug::DrawLine( currentPosition, lastPosition, GFXColor( 1, 0, 1, 1 ), false );

					if( glm::distance( currentPosition, lastPosition ) < GROUP_IS_STUCK_DISTANCE )
					{
						meta.stuckTimer += meta.stuckTimer == 0.0f ? delta : GROUP_CHECK_STUCK_TIMER;
						//GFX::Debug::DrawSphere( currentPosition, 4.0f, GFXColor( 1, 1, 0, 1 ), false );
						if( meta.stuckTimer > GROUP_FIND_NEW_PATH_TIMER )
						{
							// squad appears to be stuck...
							FreeBlockedNodes( i );
							CalculateBlockedNodes( i );
							instance->CalculateFlowfieldForGroup( glm::vec3( meta.goal[0], 0.0f, meta.goal[1] ), i  );
						}
					}
					else
					{
						//GFX::Debug::DrawSphere( currentPosition, 4.0f, GFXColor( 1, 0, 0, 1 ), false );
						meta.stuckTimer = 0.0f;
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