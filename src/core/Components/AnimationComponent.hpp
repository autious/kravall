#ifndef SRC_CORE_COMPONENTS_ANIMATIONCOMPONENT_H
#define SRC_CORE_COMPONENTS_ANIMATIONCOMPONENT_H

#include <iostream>

namespace Core
{
	struct AnimationComponent
	{
		int skeletonID;
		int animationID;
		float animationLengthSeconds;
		float currentTime;
		bool loop;
		bool playing;

		AnimationComponent()
		{
			skeletonID	= -1;
			animationID = -1;
			animationLengthSeconds = 0.0f;
			currentTime = 0.0f;
			loop = false;
			playing = false;
		}

		inline static void PlayAnimation(int animationID, AnimationComponent& ac)
		{
			ac.animationID = animationID;
			ac.currentTime = 0.0f;
			ac.playing = true;
		}

		inline static void LoopAnimation(int animationID, AnimationComponent& ac)
		{
			ac.animationID = animationID;
			ac.currentTime = 0.0f;
			ac.playing = true;
			ac.loop = true;
		}

		inline static const char * GetName()
		{
			return "AnimationComponent";
		}
	};
}

#endif
