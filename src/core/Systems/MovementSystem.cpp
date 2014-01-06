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
			float dx = std::abs(wpc->position[0] - mc->goal[0]);
			float dy = std::abs(wpc->position[1] - mc->goal[1]);
			float dz = std::abs(wpc->position[2] - mc->goal[2]);

			if ((dx + dy + dz) < 1.0f)
			{
				mc->direction[0] = 0.0f;
				mc->direction[1] = 0.0f;
				mc->direction[2] = 0.0f;
				mc->goal[0] = FLT_MAX;
			}
		}
	}
}