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
		
		static bool Attack(const Entity& attacker, TargetingComponent& target)
		{
			if (target.numberOfAttackers >= MAX_ATTACKERS)
				return false;

			for (int i = 0; i < target.numberOfAttackers; ++i)
			{
				if (target.attackers[i] == attacker)
					return false;
			}

			target.attackers[target.numberOfAttackers] = attacker;
			target.numberOfAttackers++;

			return true;
		}

		static void StopAttacking(const Entity& attacker, TargetingComponent& target)
		{
			bool found = false;

			for (int i = 0; i < target.numberOfAttackers; ++i)
			{
				// If not found, check to see if the current attacker is the sought attacker
				if (!found)
				{
					if (target.attackers[i] == attacker)
					{
						found = true;
						target.attackers[i] = INVALID_ENTITY;
					}
				}
				else
				{
					target.attackers[i - 1] = target.attackers[i];
					target.attackers[i] = INVALID_ENTITY;
				}
			}

			if (found)
			{
				target.numberOfAttackers--;
			}
		}
	};
}
#endif
