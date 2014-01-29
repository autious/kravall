#include "AnimationSystem.hpp"

#include <World.hpp>

#include <logger/Logger.hpp>

namespace Core
{
    AnimationSystem::AnimationSystem() : BaseSystem(EntityHandler::GenerateAspect<
            Core::AnimationComponent>(), 0ULL)
    {
    }

    void AnimationSystem::Update(float delta)
    {
		unsigned int animationFramerate = GFX::Settings::GetAnimationFramerate();

        for(std::vector<Entity>::iterator it = m_entities.begin(); it != m_entities.end(); ++it)
        {
            Core::AnimationComponent* ac = WGETC<Core::AnimationComponent>(*it);
			if (ac->playing)
			{

				// Get the number of frames in the current animation
				unsigned int animationFrameCount;
				GFX::Content::GetAnimationFrameCount(ac->skeletonID, ac->animationID, animationFrameCount);

				// Get the time animation duration in seconds
				float animationDuration = animationFrameCount / static_cast<float>(animationFramerate);

				// Step forward the animation
				ac->currentTime += delta * ac->speed;
				if (ac->currentTime > animationDuration)
				{
					if (ac->loop)
					{
						ac->currentTime = fmod(ac->currentTime, animationDuration);
					}
					else
					{
						ac->currentTime = 0.0f;
						ac->playing = false;
					}
					ac->currentFrame = static_cast<unsigned int>(ac->currentTime * static_cast<float>(animationFrameCount));
				}


			}
        }
    }

}
