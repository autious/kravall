#ifndef SRC_CORE_COMPONENTS_MOVEMENT_COMPONENT_HPP
#define SRC_CORE_COMPONENTS_MOVEMENT_COMPONENT_HPP

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

		/* An array specifying the object's direction of movement where index 0 = x, index 1 = y and index 2 = z. */
		float direction[3];

		/*! Default constructor. Initialising all members to 0. */
		MovementComponent() : speed(0.0f), maxSpeed(0.0f)
		{
			direction[0] = 0.0f;
			direction[1] = 0.0f;
			direction[2] = 0.0f;
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
		}

        inline static const char* GetName()
        {
            return "MovementComponent";
        }
	};
}
#endif
