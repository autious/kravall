#include "World.hpp"

#define WORLD_MEMORY_SIZE 1000000

#define WORLD_CONSTANT_MEMORY_START_OFFSET	0
#define WORLD_CONSTANT_MEMORY_SIZE			100000

#define WORLD_FRAME_MEMORY_START_OFFSET		WORLD_CONSTANT_MEMORY_SIZE
#define WORLD_FRAME_MEMORY_SIZE				(WORLD_MEMORY_SIZE - WORLD_CONSTANT_MEMORY_SIZE)


namespace Core
{
    World world;

	World::~World()
	{
		delete[] m_worldMemory;
	}


	void World::InitWorld()
	{
		m_worldMemory = new unsigned char[WORLD_MEMORY_SIZE]; 
		
		// constant memory
		m_constantAllocator = Core::LinearAllocator(Core::world.m_worldMemory + WORLD_CONSTANT_MEMORY_START_OFFSET, WORLD_CONSTANT_MEMORY_SIZE);
		m_constantHeap = Core::LinearHeap(Core::world.m_constantAllocator);

		// frame memory
		m_frameAllocator = Core::LinearAllocator(Core::world.m_worldMemory + WORLD_FRAME_MEMORY_START_OFFSET, WORLD_FRAME_MEMORY_SIZE);
		m_frameHeap = Core::LinearHeap(Core::world.m_frameAllocator);




	}

















}


