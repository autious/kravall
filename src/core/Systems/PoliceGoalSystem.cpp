#include "PoliceGoalSystem.hpp"
#include "World.hpp"
#include <gfx/GFXInterface.hpp>
#include <logger/Logger.hpp>

#include <GameUtility/NavigationMeshAStar.hpp>

#include <DebugMacros.hpp>
#include <Systems/MovementSystem.hpp>

#include <GameUtility/GameData.hpp>



Core::PoliceGoalSystem::PoliceGoalSystem()
	: BaseSystem( EntityHandler::GenerateAspect< WorldPositionComponent, MovementComponent, 
		UnitTypeComponent, AttributeComponent, FlowfieldComponent >(), 0ULL )
{
}


void Core::PoliceGoalSystem::Update( float delta )
{
	Core::NavigationMesh* instance = Core::GetNavigationMesh();
	if( !instance )
		return;

	for( std::vector<Entity>::iterator it = m_entities.begin(); it != m_entities.end(); it++ )
	{
		UnitTypeComponent* utc = WGETC<UnitTypeComponent>(*it);
		if( utc->type != Core::UnitType::Police )
			continue;
		
		Core::AttributeComponent* attribc = WGETC<Core::AttributeComponent>(*it);
		int groupId = attribc->police.squadID;
		if( groupId < 0 )
			continue;

		if( instance->flowfields[ groupId ].goal[0] == FLT_MAX )
			continue;

		Core::WorldPositionComponent* wpc = WGETC<Core::WorldPositionComponent>(*it);
		Core::MovementComponent* mvmc = WGETC<Core::MovementComponent>(*it);
		Core::FlowfieldComponent* ffc = WGETC<Core::FlowfieldComponent>(*it);
		
		glm::vec3 position = glm::vec3( wpc->position[0], 0.0f, wpc->position[2] );
		glm::vec3 target = glm::vec3( instance->flowfields[ groupId ].goal[0], 0.0f, instance->flowfields[ groupId ].goal[1] );

		bool walk = true;

		if( PathFinder::CheckLineVsNavMesh( position, target, ffc->node ) ) 
		{
			glm::vec2 deltaVector = glm::vec2(target.x, target.z ) - glm::vec2( wpc->position[0], wpc->position[2] );
			float dot = glm::dot( deltaVector, deltaVector );
			if( dot > POLICE_GOAL_ARRIVAL_THRESHOLD )
			{
				// set goal for this agent...
				mvmc->goal[ 0 ] = target.x;
				mvmc->goal[ 1 ] = target.y;
				mvmc->goal[ 2 ] = target.z;	

				glm::vec3 direction = glm::normalize( target - position );
				MovementComponent::SetDirection( mvmc, direction.x, 0.0f, direction.z );

				#ifdef SHOW_POLICE_CAN_SEE_GOAL
				GFX::Debug::DrawSphere( wpc->GetVec3(*wpc), 5.0f, GFXColor( 1.0f, 1.0f, 0.0f, 1.0f ), false );
				#endif
			}
			else
			{
				walk = false;

				#ifdef SHOW_POLICE_CAN_SEE_GOAL 
				GFX::Debug::DrawSphere( wpc->GetVec3(*wpc), 5.0f, GFXColor( 0.0f, 1.0f, 0.0f, 1.0f ), false );
				#endif
			}
		}
		else
		{
			mvmc->goal[0] = FLT_MAX;

			#ifdef SHOW_POLICE_CAN_SEE_GOAL
			GFX::Debug::DrawSphere( wpc->GetVec3(*wpc), 5.0f, GFXColor( 1.0f, 0.0f, 0.0f, 1.0f ), false );
			#endif
		}

		if( walk )
		{
			// set speed according to state...
			mvmc->desiredSpeed = GameData::GetWalkingSpeed().speedToDesire;


		}
		else
		{
			mvmc->desiredSpeed = 0.0f;
		}
	}
}
