#include "RioterAnimationSystem.hpp"

#include <World.hpp>
#include <logger/Logger.hpp>

#include <Animation/AnimationManager.hpp>
#include <GameUtility/GameData.hpp>

namespace Core
{
    RioterAnimationSystem::RioterAnimationSystem() : BaseSystem(EntityHandler::GenerateAspect<
		Core::MovementComponent, Core::GraphicsComponent, Core::AnimationComponent, Core::UnitTypeComponent, Core::AttributeComponent, Core::WorldPositionComponent>(), 0ULL)
    {
    }



	void RioterAnimationSystem::Update(float delta)
	{
		return; // dont have any animations yet

		for(std::vector<Entity>::iterator it = m_entities.begin(); it != m_entities.end(); ++it)
		{
			Core::UnitTypeComponent* utc = WGETC<Core::UnitTypeComponent>(*it);
			if( utc->type == Core::UnitType::Police )
				continue;

			Core::WorldPositionComponent* wpc = WGETC<Core::WorldPositionComponent>(*it);
			Core::MovementComponent* mvmc = WGETC<Core::MovementComponent>(*it);
			Core::AnimationComponent* ac = WGETC<Core::AnimationComponent>(*it);

			// if prevPos has yet to be calculated, copy from wpc...
			if( mvmc->prevPos[0] == std::numeric_limits<float>::max() )
				for( char i = 0; i < 3; i++ )
					mvmc->prevPos[i] = wpc->position[i];

			float frameSpeed = 
				glm::length( glm::vec3( wpc->position[0], wpc->position[1], wpc->position[2] ) - 
				glm::vec3( mvmc->prevPos[0], mvmc->prevPos[1], mvmc->prevPos[2] )) / delta;

			const Core::MovementData& walkingData = Core::GameData::GetMovementDataWithState( Core::MovementState::Movement_Walking );
			

			if( frameSpeed < 0.05f )
				Core::AnimationManager::QueueAnimation( *it, "", true ); // still			
			else if( frameSpeed > walkingData.speedToDesire )
				Core::AnimationManager::QueueAnimation( *it, "", true ); // running			
			else 
				Core::AnimationManager::QueueAnimation( *it, "", true ); // walking

			// update prevPos...
			for( char i = 0; i < 3; i++ )
				mvmc->prevPos[i] = wpc->position[i];


			//switch( mvmc->state )
			//{
			//case Core::MovementState::Movement_Idle :
			//
			//	if( frameSpeed > 0.05 )
			//		Core::AnimationManager::QueueAnimation( *it, "", true ); // not moving
			//	else
			//		Core::AnimationManager::QueueAnimation( *it, "", true ); // moveing
			//
			//	break;
			//	
			//case Core::MovementState::Movement_Walking :
			//	
			//	if( frameSpeed < 0.05 )
			//		Core::AnimationManager::QueueAnimation( *it, "", true ); // not moving
			//	else
			//		Core::AnimationManager::QueueAnimation( *it, "", true );
			//	
			//
			//
			//	Core::AnimationManager::QueueAnimation( *it, "", true );
			//	break;
			//
			//case Core::MovementState::Movement_Sprinting :
			//	Core::AnimationManager::LoopAnimation( *it, "" );
			//	break;
			//
			//default :
			//	break;
			//}


			//glm::vec3 temp = glm::vec3( mvmc->direction[0], mvmc->direction[1], mvmc->direction[2] );
			//if( mvmc->speed < 0.01f || glm::dot( temp, temp ) == 0.0f )
			//{
			//	//ac->currentTime = 0.33f;
			//	ac->speed = 0.0f;
			//}
			//else
			//{
			//	ac->speed = 1.0f;
			//}
		}
	}



	/*
    void RioterAnimationSystem::Update(float delta)
    {
		unsigned int animationFramerate = GFX::Settings::GetAnimationFramerate();

        for(std::vector<Entity>::iterator it = m_entities.begin(); it != m_entities.end(); ++it)
        {
            Core::AnimationComponent* ac = WGETC<Core::AnimationComponent>(*it);
			Core::GraphicsComponent* gc = WGETC<Core::GraphicsComponent>(*it);
			if (ac->playing)
			{
				unsigned int meshID = GFX::GetBitmaskValue(gc->bitmask, GFX::BITMASK::MESH_ID);
				
				// Get the number of frames in the current animation
				unsigned int animationFrameCount;
				unsigned int bonesPerFrame;
				unsigned int offset;

				int skeletonID = GFX::Content::GetSkeletonID(meshID);
				if (GFX::Content::GetAnimationInfo(skeletonID, ac->animationID, animationFrameCount, bonesPerFrame, offset) == GFX_SUCCESS)
				{

					// Get the time animation duration in seconds
					float animationDuration = (animationFrameCount-1) / static_cast<float>(animationFramerate);

					// Step forward the animation
					ac->currentTime += delta * ac->speed;
					if (ac->currentTime > animationDuration)
					{
						
						if (ac->queuedAnimationID > 0 && 
							GFX::Content::GetAnimationInfo(skeletonID, ac->queuedAnimationID, animationFrameCount, bonesPerFrame, offset) == GFX_SUCCESS)
						{
							animationDuration = (animationFrameCount-1) / static_cast<float>(animationFramerate);
							ac->currentTime = fmod(ac->currentTime, animationDuration);
							ac->animationID = ac->queuedAnimationID;
							ac->queuedAnimationID = -1;
							ac->playing = true;
						}
						else
						{
							if (ac->loop)
							{
								ac->currentTime = fmod(ac->currentTime, animationDuration);
							}
							else
							{
								ac->currentTime = animationDuration;
								ac->playing = false;
							}
						}
					}
					ac->currentFrame = static_cast<unsigned int>(ac->currentTime * static_cast<float>(animationFramerate));
					//ac->animationID = ac->animationID * animationFrameCount * bonesPerFrame;
					//if (ac->currentFrame%10 == 0)
					//std::cout << "Frame: " << ac->currentFrame+1 << "/" << animationFrameCount << "\n";
					ac->currentFrame = offset + ac->currentFrame * bonesPerFrame;
				}
			}
        }
    }
	*/

}
