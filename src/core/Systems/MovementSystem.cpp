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
		wpc->position[1] += mc->direction[1] * mc->speed * delta;;
		wpc->position[2] += mc->direction[2] * mc->speed * delta;;
	}
}