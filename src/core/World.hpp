#ifndef SRC_CORE_WORLD_H
#define SRC_CORE_WORLD_H

#include <SystemDef.hpp>
#include <Lua/LuaState.hpp>
#include <Lua/ConfigurationHandler.hpp>
#include <Memory/LinearHeap.hpp>
#include <ContentManagement/ContentManager.hpp>

#define WGETC Core::world.m_entityHandler.GetComponentTmpPointer
#define WORLD_MEMORY_SIZE 500000

namespace Core
{
    class World
    {
    public:
		World() : m_systemHandler(), m_entityHandler(&m_systemHandler), m_luaState(), m_config(&m_luaState), m_linearAllocator(Core::LinearAllocator(nullptr, 0)), m_linearHeap(Core::LinearHeap(m_linearAllocator)) {}
		~World();

        SystemHandler m_systemHandler;
        EntityHandler m_entityHandler;             
        LuaState m_luaState;
        ConfigurationHandler m_config;
		Core::LinearAllocator m_linearAllocator;
		Core::LinearHeap m_linearHeap;
		unsigned char* m_worldMemory;
        ContentManager m_contentManager;
    };



    extern World world;
}


#endif
