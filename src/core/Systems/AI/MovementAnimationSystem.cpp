#include "MovementAnimationSystem.hpp"

#include <World.hpp>
#include <logger/Logger.hpp>

#include <Animation/AnimationManager.hpp>
#include <GameUtility/GameData.hpp>

#define GRACE_THRESHOLD 0.3f

namespace Core
{
    MovementAnimationSystem::MovementAnimationSystem() : BaseSystem(EntityHandler::GenerateAspect<
		TargetingComponent, Core::GraphicsComponent, Core::AnimationComponent, Core::MovementComponent,
		Core::AttributeComponent, Core::MovementDataComponent, Core::WorldPositionComponent, GraphicsComponent>(), 0ULL)
    {
    }



	void MovementAnimationSystem::Update(float delta)
	{

		for(std::vector<Entity>::iterator it = m_entities.begin(); it != m_entities.end(); ++it)
		{

			Core::WorldPositionComponent* wpc	= WGETC<Core::WorldPositionComponent>(*it);
			Core::AnimationComponent* ac		= WGETC<Core::AnimationComponent>(*it);
			Core::TargetingComponent* tc		= WGETC<Core::TargetingComponent>(*it);
			Core::GraphicsComponent* grc		= WGETC<Core::GraphicsComponent>(*it);
			Core::MovementDataComponent *mdc	= WGETC<Core::MovementDataComponent>( *it );
			Core::MovementComponent *mvmc		= WGETC<Core::MovementComponent>( *it );

			// if prevPos has yet to be calculated, copy from wpc...
			if( mdc->prevPos[0] == std::numeric_limits<float>::max() )
				for( char i = 0; i < 3; i++ )
					mdc->prevPos[i] = wpc->position[i];
			float frameSpeed = 
				glm::distance( glm::vec3( wpc->position[0], wpc->position[1], wpc->position[2] ),
				glm::vec3( mdc->prevPos[0], mdc->prevPos[1], mdc->prevPos[2] )) / mdc->prevDt;

			// update prevPos...
			for( char i = 0; i < 3; i++ )
				mdc->prevPos[i] = wpc->position[i];
			mdc->prevDt = delta;

			float walkingSpeed = mvmc->desiredSpeed[ Core::MovementState::Movement_Walking ];
			
			mdc->movedThisFrame = false;
			if( frameSpeed > MOVEDTHISFRAME_THRESHOLD )
				mdc->movedThisFrame = true;


			bool hasAlteredAnimation = false;
			bool hasChangedAnimation = false;
			ac->loop = false;
			if ( !tc->isAttacking )
			{

				// if moving faster than walking but not already playing running animation or if not playing any animation...
				if( frameSpeed > walkingSpeed + GRACE_THRESHOLD &&
						( Core::AnimationManager::GetAnimationID( GFX::GetBitmaskValue( grc->bitmask, GFX::BITMASK::MESH_ID ), "idle" ) == ac->animationID
						|| !ac->playing ))
				{
					if( Core::AnimationManager::GetAnimationID( GFX::GetBitmaskValue( grc->bitmask, GFX::BITMASK::MESH_ID ), "agitated-run" ) != ac->animationID )
						hasChangedAnimation = true;

					ac->speed = 1.2f;
					hasAlteredAnimation = true;
					Core::AnimationManager::PlayAnimation( *it, "agitated-run" ); // running
				}

				// if moving but not running and not already playing walk-animation or if not playing any animation
				else if( frameSpeed > 0.05f &&  
						( Core::AnimationManager::GetAnimationID( GFX::GetBitmaskValue( grc->bitmask, GFX::BITMASK::MESH_ID ), "idle" ) == ac->animationID 
						|| !ac->playing ))
				{
					if( Core::AnimationManager::GetAnimationID( GFX::GetBitmaskValue( grc->bitmask, GFX::BITMASK::MESH_ID ), "walk-straight" ) != ac->animationID )
						hasChangedAnimation = true;

					ac->speed = 1.6f;
					hasAlteredAnimation = true;
					Core::AnimationManager::PlayAnimation( *it, "walk-straight" ); // walking
				}

				// if not moving and not already playing idle-animation or if not playing any animation
				else if( (Core::AnimationManager::GetAnimationID( GFX::GetBitmaskValue( grc->bitmask, GFX::BITMASK::MESH_ID ), "idle" ) != ac->animationID 
						|| !ac->playing) && 
						frameSpeed < 0.05f )
				{
					if( Core::AnimationManager::GetAnimationID( GFX::GetBitmaskValue( grc->bitmask, GFX::BITMASK::MESH_ID ), "idle" ) != ac->animationID )
						hasChangedAnimation = true;

					ac->speed = 1.0f;
					hasAlteredAnimation = true;
					Core::AnimationManager::PlayAnimation( *it, "idle" ); // still
				}

				// try to make animations less unified
				if( hasAlteredAnimation )
				{
					ac->speed = ac->speed + ((std::rand() % 1000) / 1000.0f) * 0.1f;
				}

				if( hasChangedAnimation )
				{
					ac->currentTime = ((std::rand() % 1000) / 1000.0f) * 1.0f;
				}
			}

		}
	}
}
