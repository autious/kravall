#ifndef SRC_CORE_COMPONENTS_ANIMATIONCOMPONENT_H
#define SRC_CORE_COMPONENTS_ANIMATIONCOMPONENT_H

#include <iostream>

namespace Core
{
	struct AnimationComponent
	{
		int animationID;
		int queuedAnimationID;

		float currentTime;
		bool loop;
		bool playing;
		float speed;
		unsigned int currentFrame;
		float playTime;

		AnimationComponent()
		{
			animationID = -1;
			queuedAnimationID = -1;
			currentTime = 0.0f;
			loop = false;
			playing = false;
			speed = 1.0f;
			currentFrame = 0;
			playTime = 0.0f;
		}
		inline static const char * GetName()
		{
			return "AnimationComponent";
		}
	};
}

#endif
