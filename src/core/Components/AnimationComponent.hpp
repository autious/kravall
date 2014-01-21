#ifndef SRC_CORE_COMPONENTS_ANIMATIONCOMPONENT_H
#define SRC_CORE_COMPONENTS_ANIMATIONCOMPONENT_H

#include <iostream>

namespace Core
{
	struct AnimationComponent
	{
		int skeletonID;
		int animationID;
		float currentTime;
		bool loop;
		bool playing;

		AnimationComponent()
		{
			skeletonID	= -1;
			animationID = -1;
			currentTime = 0.0f;
			loop = false;
			playing = false;
		}
		inline static const char * GetName()
		{
			return "AnimationComponent";
		}
	};
}

#endif
