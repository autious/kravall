#include "MovementSystem.hpp"
#include "World.hpp"

void Core::MovementSystem::Update(float delta)
{
	for (std::vector<Entity>::iterator it = m_entities.begin();
		it != m_entities.end();
		it++)
	{
		WorldPositionComponent* wpc = WGETC<WorldPositionComponent>(*it);
		MovementComponent* mc = WGETC<MovementComponent>(*it);

		wpc->position[0] += mc->direction[0] * mc->speed * delta;
		wpc->position[1] += mc->direction[1] * mc->speed * delta;
		wpc->position[2] += mc->direction[2] * mc->speed * delta;

		if (mc->goal[0] < FLT_MAX)
		{
			// Dot product between the direction and the vector from the current position to the goal.
			float dot = mc->direction[0] * (mc->goal[0] - wpc->position[0]) +
						mc->direction[1] * (mc->goal[1] - wpc->position[1]) +
						mc->direction[2] * (mc->goal[2] - wpc->position[2]);

			// Is negative if position is past the goal so stop.
			if (dot < 0)
			{
				mc->direction[0] = 0.0f;
				mc->direction[1] = 0.0f;
				mc->direction[2] = 0.0f;
				mc->goal[0] = FLT_MAX;
			}
		}
	}
}