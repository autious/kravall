#ifndef SRC_CORE_WORLD_H
#define SRC_CORE_WORLD_H

#include <SystemDef.hpp>
#include <Lua/LuaState.hpp>
#include <Lua/ConfigurationHandler.hpp>
#include <Memory/LinearHeap.hpp>
#include <ContentManagement/ContentManager.hpp>

#define WGETC Core::world.m_entityHandler.GetComponentTmpPointer

namespace Core
{
    class World
    {
    public:
		World() : m_systemHandler(), m_entityHandler(&m_systemHandler), m_luaState(), m_config(&m_luaState), 
			m_constantAllocator(Core::LinearAllocator(nullptr, 0)), m_constantHeap(m_constantAllocator), 
			m_frameAllocator(Core::LinearAllocator(nullptr, 0)), m_frameHeap(Core::LinearHeap(m_frameAllocator)) {}

		~World();

		void InitWorld();

        SystemHandler m_systemHandler;
        EntityHandler m_entityHandler;             
        LuaState m_luaState;
        ConfigurationHandler m_config;
		
		Core::LinearAllocator m_constantAllocator;
		Core::LinearHeap m_constantHeap;

		Core::LinearAllocator m_frameAllocator;
		Core::LinearHeap m_frameHeap;
		unsigned char* m_worldMemory;
        ContentManager m_contentManager;
    };


    extern World world;
}


#endif
