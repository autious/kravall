#include "BloodSystem.hpp"
#include <World.hpp>


namespace Core
{

	BloodSystem::BloodSystem() :BaseSystem(EntityHandler::GenerateAspect<BloodComponent, GraphicsComponent>(), 0ULL)
	{
	}


	void BloodSystem::Update(float delta)
	{
		for (std::vector<Entity>::iterator it = m_entities.begin(); it != m_entities.end(); it++)
		{
			BloodComponent* bc = WGETC<BloodComponent>(*it);
			GraphicsComponent* gc = WGETC<GraphicsComponent>(*it);

			if (!bc->expand)
			{
				continue;
			}

			gc->outlineColor[0] += bc->scaleSpeed[0] * delta;
			gc->outlineColor[2] += bc->scaleSpeed[1] * delta;

			if (gc->outlineColor[0] >= bc->maxSize)
				bc->expand = false;
		}
	}
}
