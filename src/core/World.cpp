#include "World.hpp"

namespace Core
{
    World world;

	World::~World()
	{
		delete[] m_worldMemory;
	}
}


