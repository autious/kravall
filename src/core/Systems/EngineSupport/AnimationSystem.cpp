#include "AnimationSystem.hpp"

#include <World.hpp>

#include <logger/Logger.hpp>

namespace Core
{
    AnimationSystem::AnimationSystem() : BaseSystem(EntityHandler::GenerateAspect<
            Core::GraphicsComponent, Core::AnimationComponent>(), 0ULL)
    {
    }

    void AnimationSystem::Update(float delta)
    {
		unsigned int animationFramerate = GFX::Settings::GetAnimationFramerate();

        for(std::vector<Entity>::iterator it = m_entities.begin(); it != m_entities.end(); ++it)
        {
            Core::AnimationComponent* ac = WGETC<Core::AnimationComponent>(*it);
			Core::GraphicsComponent* gc = WGETC<Core::GraphicsComponent>(*it);
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
					if (ac->playing)
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
