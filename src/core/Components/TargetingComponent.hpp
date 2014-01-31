#ifndef SRC_CORE_COMPONENTS_TARGETING_COMPONENT_HPP
#define SRC_CORE_COMPONENTS_TARGETING_COMPONENT_HPP

#include <ComponentFramework/SystemTypes.hpp> // For Entity

#define MAX_ATTACKERS 5

namespace Core
{
	/*!
		Component holding a moving object's speed, maximum speed and direction of movement.
	*/
	struct TargetingComponent
	{
		/*! The number of entities currently attacking the entity. */
		int numberOfAttackers;

		/*! An array specifying entities currently attacking the entity. */
		Entity attackers[MAX_ATTACKERS];

		/*! The entity that is the current target. */
		Entity target;

		/*! Default constructor. Initialising all members to 0. */
		TargetingComponent() : numberOfAttackers(0), target(INVALID_ENTITY)
		{
			for (int i = 0; i < MAX_ATTACKERS; ++i)
			{
				attackers[i] = INVALID_ENTITY;
			}
		}

		inline static const char* GetName()
		{
			return "TargetingComponent";
		}
	};
}
#endif
