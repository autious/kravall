#include "MovementAnimationSystem.hpp"

#include <World.hpp>
#include <logger/Logger.hpp>

#include <Animation/AnimationManager.hpp>
#include <GameUtility/GameData.hpp>

// this value needs to be mirrored in the panicAnimationSystem, otherwise full recompile will be needed at change.
#define GRACE_THRESHOLD 0.3f
#define STILL_THRESHOLD 0.35f 

#define IDLE_MINIMUM_TIME 0.25f
#define MINIMUM_MOVEMENT_ANIMATION_TIME 0.35f

#include <vector>
std::vector<glm::vec3> derps;

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

			ac->playTime += delta;

			// if prevPos has yet to be calculated, copy from wpc...
			if( mdc->prevPos[0] == std::numeric_limits<float>::max() )
				for( char i = 0; i < 3; i++ )
					mdc->prevPos[i] = wpc->position[i];
			float frameSpeed = 
				glm::distance( glm::vec3( wpc->position[0], wpc->position[1], wpc->position[2] ),
				glm::vec3( mdc->prevPos[0], mdc->prevPos[1], mdc->prevPos[2] )) / mdc->prevDt;






			glm::vec3 cur = glm::vec3( mdc->prevPos[0], mdc->prevPos[1], mdc->prevPos[2] );
			glm::vec3 prev = glm::vec3( wpc->position[0], wpc->position[1], wpc->position[2] );
			Core::NavigationMesh* instance = Core::GetNavigationMesh();
			if( instance )
			{
				Core::FlowfieldComponent* ffc = WGETC<Core::FlowfieldComponent>(*it);
				float* points = instance->nodes[ffc->node].points;
				glm::vec3 temp = glm::vec3( 0.0f );
				for( int g = 0; g < 4; g++ )
					temp += glm::vec3( points[ g * 2 ], 0.0f, points[ g * 2 + 1] );
				temp *= 0.25f;
			
				if( glm::distance( cur, prev ) > 10.0f )
				{
					derps.push_back( cur );
					derps.push_back( prev );
					derps.push_back( temp );
				}

				for( int i = 0; i < derps.size(); i += 3 )
				{
					GFX::Debug::DrawLine( derps[i], derps[i+1], GFXColor( 1, 1, 0, 1 ), false );
					//GFX::Debug::DrawSphere( derps[i+2], 3.0f, GFXColor( 0, 1, 0, 1 ), false );
				}
			}














			// update prevPos...
			for( char i = 0; i < 3; i++ )
				mdc->prevPos[i] = wpc->position[i];
			mdc->prevDt = delta;

			float walkingSpeed = mvmc->desiredSpeed[ Core::MovementState::Movement_Walking ];
			float joggingSpeed = mvmc->desiredSpeed[ Core::MovementState::Movement_Jogging ];
			
			

			mdc->movedThisFrame = false;
			if( frameSpeed > MOVEDTHISFRAME_THRESHOLD )
				mdc->movedThisFrame = true;
			mdc->frameSpeed = frameSpeed;

			bool hasAlteredAnimation = false;
			bool hasChangedAnimation = false;
			ac->loop = false;
			if ( !tc->isAttacking )
			{
				// if moving faster than jogging but not already playing running animation or if not playing any animation...
				if( frameSpeed > joggingSpeed + GRACE_THRESHOLD &&
					(( Core::AnimationManager::GetAnimationID( GFX::GetBitmaskValue( grc->bitmask, GFX::BITMASK::MESH_ID ), "idle" )
					== ac->animationID && ac->playTime > IDLE_MINIMUM_TIME )
					|| !ac->playing ))
				{
					if( Core::AnimationManager::GetAnimationID( GFX::GetBitmaskValue( grc->bitmask, GFX::BITMASK::MESH_ID ), "agitated-run" ) != ac->animationID )
						hasChangedAnimation = true;

					ac->speed = 1.2f;
					hasAlteredAnimation = true;
					Core::AnimationManager::PlayAnimation( *it, "agitated-run" ); // running
				}
				
				// if moving faster than walking but not already playing joggin animation or if not playing any animation...
				else if( frameSpeed > walkingSpeed + GRACE_THRESHOLD &&
					(( Core::AnimationManager::GetAnimationID( GFX::GetBitmaskValue( grc->bitmask, GFX::BITMASK::MESH_ID ), "idle" ) 
					== ac->animationID && ac->playTime > IDLE_MINIMUM_TIME )
					|| !ac->playing ))
				{
					if( Core::AnimationManager::GetAnimationID( GFX::GetBitmaskValue( grc->bitmask, GFX::BITMASK::MESH_ID ), "jogging" ) != ac->animationID )
						hasChangedAnimation = true;

					ac->speed = 1.2f;
					hasAlteredAnimation = true;
					Core::AnimationManager::PlayAnimation( *it, "jogging" ); // running
				}

				// if moving but not running and not already playing walk-animation or if not playing any animation
				else if( frameSpeed > STILL_THRESHOLD &&  
					(( Core::AnimationManager::GetAnimationID( GFX::GetBitmaskValue( grc->bitmask, GFX::BITMASK::MESH_ID ), "idle" )
					== ac->animationID && ac->playTime > IDLE_MINIMUM_TIME )
					|| !ac->playing ))
				{
					if( Core::AnimationManager::GetAnimationID( GFX::GetBitmaskValue( grc->bitmask, GFX::BITMASK::MESH_ID ), "walk-straight" ) != ac->animationID )
						hasChangedAnimation = true;

					ac->speed = 1.6f;
					hasAlteredAnimation = true;
					Core::AnimationManager::PlayAnimation( *it, "walk-straight" ); // walking
				}

				// if not moving and not already playing idle-animation or if not playing any animation
				else if( ((Core::AnimationManager::GetAnimationID( GFX::GetBitmaskValue( grc->bitmask, GFX::BITMASK::MESH_ID ), "idle" )
					!= ac->animationID && ac->playTime > MINIMUM_MOVEMENT_ANIMATION_TIME )
					|| !ac->playing) && 
					frameSpeed < STILL_THRESHOLD )
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
