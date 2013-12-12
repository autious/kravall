#ifndef MOVEMENT_COMPONENT_HPP
#define MOVEMENT_COMPONENT_HPP

namespace Core
{
	struct MovementComponent
	{
		float speed;
		float maxSpeed;
		float direction[3];

		MovementComponent()
		{
			speed = 0.0f;
			maxSpeed = 0.0f;
			direction[0] = 0.0f;
			direction[1] = 0.0f;
			direction[2] = 0.0f;
		}

		MovementComponent(float dirX, float dirY, float dirZ, float startSpeed, float highestSpeed)
		{
			speed = startSpeed;
			maxSpeed = highestSpeed;
			direction[0] = dirX;
			direction[1] = dirY;
			direction[2] = dirZ;
		}
	};
}
#endif