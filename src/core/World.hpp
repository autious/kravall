#ifndef SRC_CORE_WORLD_H
#define SRC_CORE_WORLD_H

#include <SystemDef.hpp>
#include <Lua/LuaState.hpp>
#include <Lua/ConfigurationHandler.hpp>
#include <Memory/LinearHeap.hpp>
#include <ContentManagement/ContentManager.hpp>

#define WGETC Core::world.m_entityHandler.GetComponentTmpPointer
#define CONF Core::world.m_config

namespace Core
{
    class World
    {
    public:
		World() : m_systemHandler(), m_entityHandler(&m_systemHandler),  
			m_constantAllocator(Core::LinearAllocator(nullptr, 0)), m_constantHeap(Core::LinearHeap(m_constantAllocator)), 
			m_levelAllocator(Core::LinearAllocator(nullptr, 0)), m_levelHeap(Core::LinearHeap(m_levelAllocator)),
			m_frameAllocator(Core::LinearAllocator(nullptr, 0)), m_frameHeap(Core::LinearHeap(m_frameAllocator)),
            m_luaState(), m_config(&m_luaState){}

		~World();

		void InitWorld();

        SystemHandler m_systemHandler;
        EntityHandler m_entityHandler;             
		
		// program memory
		Core::LinearAllocator m_constantAllocator;
		Core::LinearHeap m_constantHeap;

		// level memory
		Core::LinearAllocator m_levelAllocator;
		Core::LinearHeap m_levelHeap;
		
		// frame memory...
		Core::LinearAllocator m_frameAllocator;
		Core::LinearHeap m_frameHeap;

		unsigned char* m_worldMemory;
        ContentManager m_contentManager;

        LuaState m_luaState;
        ConfigurationHandler m_config;
    };

    extern World world;
}


#endif
