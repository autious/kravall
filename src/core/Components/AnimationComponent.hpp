#ifndef SRC_CORE_COMPONENTS_ANIMATIONCOMPONENT_H
#define SRC_CORE_COMPONENTS_ANIMATIONCOMPONENT_H

#include <iostream>

namespace Core
{
	struct AnimationComponent
	{
		int animationID;

		float currentTime;
		bool loop;
		bool playing;
		float speed;
		unsigned int currentFrame;

		AnimationComponent()
		{
			animationID = -1;
			currentTime = 0.0f;
			loop = false;
			playing = false;
			speed = 0.0f;
			currentFrame = 0;
		}
		inline static const char * GetName()
		{
			return "AnimationComponent";
		}
	};
}

#endif
