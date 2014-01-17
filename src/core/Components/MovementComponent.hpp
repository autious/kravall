#ifndef SRC_CORE_COMPONENTS_MOVEMENT_COMPONENT_HPP
#define SRC_CORE_COMPONENTS_MOVEMENT_COMPONENT_HPP

#include <float.h> // For FLT_MAX
#include <cassert>
#include <cmath>

namespace Core
{
	/*!
	Component holding a moving object's speed, maximum speed and direction of movement.
	*/
	struct MovementComponent
	{
		/*! The object's current speed. Represented by a floating point and should never exceed maxSpeed. */
		float speed;

		/*! The object's maximum speed. This is used with acceleration and should limit the range of speed. */
		float maxSpeed;

		/*! 
			Should NEVER be set directly - use the static function SetDirection instead. An array specifying the 
			object's direction of movement where index 0 = x, index 1 = y and index 2 = z. 
		*/
		float direction[3];

		/*! An array specifying the object's new direction of movement where index 0 = x, index 1 = y and index 2 = z. */
		float newDirection[3];

		/*!
			An array specifying the object's goal, ceasing movement when the goal is reached. The index 0 corresponds
			to the x component, 1 to y and 2 to z. An x-value of FLT_MAX disables the goal.
		*/
		float goal[3];

		/*! Default constructor. Initialising all members to 0. */
		MovementComponent() : speed(0.0f), maxSpeed(0.0f)
		{
			direction[0] = 0.0f;
			direction[1] = 0.0f;
			direction[2] = 0.0f;

			newDirection[0] = 0.0f;
			newDirection[1] = 0.0f;
			newDirection[2] = 0.0f;

			goal[0] = FLT_MAX;
			goal[1] = 0.0f;
			goal[2] = 0.0f;
		}

		/*!
			Constructor setting all members to starting values.
			\param dirX The x-component of the movement direction vector.
			\param dirY The y-component of the movement direction vector.
			\param dirZ The z-component of the movement direction vector.
			\param startSpeed The initial speed of the object.
			\param highestSpeed The maximum speed for the object.
		*/
		MovementComponent(const float& dirX, const float& dirY, const float& dirZ, const float& startSpeed,
			const float& highestSpeed)
			: speed(startSpeed), maxSpeed(highestSpeed)
		{
			direction[0] = dirX;
			direction[1] = dirY;
			direction[2] = dirZ;

			newDirection[0] = 0.0f;
			newDirection[1] = 0.0f;
			newDirection[2] = 0.0f;

			goal[0] = FLT_MAX;
			goal[1] = 0.0f;
			goal[2] = 0.0f;
		}

		inline static const char* GetName()
		{
			return "MovementComponent";
		}

		static void SetDirection(MovementComponent* mc, const float& newX, const float& newY, const float& newZ)
		{
			// Make sure vector is normalised.
			assert((newX == 0.0f && newY == 0.0f && newZ == 0.0f) || ((std::sqrt(newX * newX + newY * newY + newZ * newZ) - 1.0) < 0.01));

			mc->newDirection[0] = newX;
			mc->newDirection[1] = newY;
			mc->newDirection[2] = newZ;
		}
	};
}
#endif
