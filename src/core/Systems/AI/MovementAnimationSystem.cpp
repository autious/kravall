#include "MovementAnimationSystem.hpp"

#include <World.hpp>
#include <logger/Logger.hpp>

#include <Animation/AnimationManager.hpp>
#include <GameUtility/GameData.hpp>

#define GRACE_THRESHOLD 0.2f

namespace Core
{
    MovementAnimationSystem::MovementAnimationSystem() : BaseSystem(EntityHandler::GenerateAspect<
		Core::MovementComponent, TargetingComponent, Core::GraphicsComponent, Core::AnimationComponent, 
		Core::AttributeComponent, Core::WorldPositionComponent, GraphicsComponent>(), 0ULL)
    {
    }



	void MovementAnimationSystem::Update(float delta)
	{

		for(std::vector<Entity>::iterator it = m_entities.begin(); it != m_entities.end(); ++it)
		{

			Core::WorldPositionComponent* wpc = WGETC<Core::WorldPositionComponent>(*it);
			Core::MovementComponent* mvmc = WGETC<Core::MovementComponent>(*it);
			Core::AnimationComponent* ac = WGETC<Core::AnimationComponent>(*it);
			Core::TargetingComponent* tc = WGETC<Core::TargetingComponent>(*it);
			Core::GraphicsComponent* grc = WGETC<Core::GraphicsComponent>(*it);


			// if prevPos has yet to be calculated, copy from wpc...
			if( mvmc->prevPos[0] == std::numeric_limits<float>::max() )
				for( char i = 0; i < 3; i++ )
					mvmc->prevPos[i] = wpc->position[i];
			float frameSpeed = 
				glm::length( glm::vec3( wpc->position[0], wpc->position[1], wpc->position[2] ) - 
				glm::vec3( mvmc->prevPos[0], mvmc->prevPos[1], mvmc->prevPos[2] )) / delta;
			// update prevPos...
			for( char i = 0; i < 3; i++ )
				mvmc->prevPos[i] = wpc->position[i];

			




			const Core::MovementData& walkingData = Core::GameData::GetMovementDataWithState( Core::MovementState::Movement_Walking );
			
			mvmc->movedThisFrame = false;
			if( frameSpeed > MOVEDTHISFRAME_THRESHOLD )
				mvmc->movedThisFrame = true;

			ac->loop = false;
			if ( !tc->isAttacking )
			{

				// if moving faster than walking but not already playing running animation or if not playing any animation...
				if( frameSpeed > walkingData.speedToDesire + GRACE_THRESHOLD &&
						( Core::AnimationManager::GetAnimationID( GFX::GetBitmaskValue( grc->bitmask, GFX::BITMASK::MESH_ID ), "walk-straight" ) != ac->animationID
						|| !ac->playing ))
					Core::AnimationManager::PlayAnimation( *it, "walk-straight" ); // running

				// if moving but not running and not already playing walk-animation or if not playing any animation
				else if( frameSpeed > 0.05f &&  
						( Core::AnimationManager::GetAnimationID( GFX::GetBitmaskValue( grc->bitmask, GFX::BITMASK::MESH_ID ), "walk-straight" ) != ac->animationID 
						|| !ac->playing ))
					Core::AnimationManager::PlayAnimation( *it, "walk-straight" ); // walking

				// if not moving and not already playing idle-animation or if not playing any animation
				else if( (Core::AnimationManager::GetAnimationID( GFX::GetBitmaskValue( grc->bitmask, GFX::BITMASK::MESH_ID ), "idle" ) != ac->animationID 
						|| !ac->playing) && 
						frameSpeed < 0.05f )
					Core::AnimationManager::PlayAnimation( *it, "idle" ); // still

			}



		}
	}
}
